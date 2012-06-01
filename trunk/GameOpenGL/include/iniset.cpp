//********************************************
//	IniSet  by thinkingl
//  �������ļ����еĲ���.

//  ������2006 .1 . 6 
//  ����� Sword2 ��Դ����.
//********************************************

#include "..\\stdafx.h"
#include "iniset.h"
#include <io.h>
#include <fcntl.h>

//#include <windows.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <malloc.h>


/////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////

//��ʼ��
CIniSet::CIniSet()
{
	DataLen=0;
	Data=NULL;
	IndexNum=0;
	IndexList=NULL;
}

//��ʼ��
CIniSet::CIniSet(char *filename)
{
	DataLen=0;
	Data=NULL;
	IndexNum=0;
	IndexList=NULL;
	Open(filename);
}

//�����ͷ�
CIniSet::~CIniSet()
{
	if( DataLen != 0 && Data != NULL )
	{
		_DELETE( Data );
	}

	if( IndexNum != 0 && IndexList != NULL )
	{
		_DELETE( IndexList );
	}
}

//�����ļ�
bool CIniSet::Open( char *filename)
{
	if (strcpy_s(FileName, filename))
		ShowMessage("�ַ������Ƴ���! strcpy_s");

	_DELETE( Data );

	DataLen=GetFileLength(filename);	//��ȡ�ļ�����
	if( DataLen > 0 )		//�ļ�����
	{
		Data=new char[DataLen+1];
		memset(Data,0,DataLen+1);		// ����������.
		FILE *fp;
		if(fopen_s ( &fp , filename , "r"))
			ShowMessage( "�ļ��򿪴���! �������:fopen_s" );;
		//fp=fopen(filename, "rb");
		fread(Data, DataLen, 1, fp);		//������
		fclose(fp);

		//��ʼ������
		InitIndex();
	}
	else
	{
		DataLen=1;
		Data=new char[DataLen];
		memset(Data, 0, 1);
		InitIndex();
	}

	return false;
}

//д���ļ�
bool CIniSet::Save(char *filename)
{
	if( filename==NULL )
		filename=FileName;

	FILE *fp;
	if(fopen_s ( &fp , filename , "w"))
		ShowMessage( "�ļ��򿪴���! �������:fopen_s" );

	if( fp==NULL )
	{
		ShowMessage("Can't save %s ",filename);
		return false;
	}

	fwrite(Data, DataLen, 1, fp);
	fclose(fp);

	return true;
}

//��������е�����λ��
void CIniSet::InitIndex()
{
	IndexNum=0;

	for(int i=0; i<DataLen; i++)
	{
		//�ҵ�,Ҫ��[��ͷ�����Ҷ����֮��Ҫ�ûس��ֿ�
		if( Data[i]=='[' && (i==0||(i>0&&Data[i-1]=='\n')  ) )
		{
			IndexNum++;
		}
	}

	//�����ڴ�
	_DELETE( IndexList );
	if( IndexNum>0 )
		IndexList=new int[IndexNum];

	int n=0;
	//��ʼ��
	for(int i=0; i<DataLen; i++)
	{
		if( Data[i]=='[' && ((i>0&&Data[i-1]=='\n' )|| i==0) )
		{
			IndexList[n]=i+1;		//���浽�������λ���� [ ����һ���ַ���λ��
			n++;
		}
	}
}

/////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////

//����ָ������λ��
int CIniSet::FindIndex(char *string)
{
	for(int i=0; i<IndexNum; i++)
	{
		char *str=ReadText( IndexList[i] );		//���������������Ե�����
		if( strcmp(string, str) == 0 )			//�Ƚϣ������ͬ�ͷ��ر����λ��
		{
			_FREE( str );
			return IndexList[i];				//�������ڵ�λ�ã��� [ ���ַ�����һ��λ��
		}
		_FREE( str );
	}
	return -1;
}

//����ָ�����ݵ����ݵ�λ��
int CIniSet::FindData(int index, char *string)
{
	int p=index;	//ָ��

	while(1)
	{									//��������(����)���ڵ���
		p=GotoNextLine(p);				//��һ�е�λ��
		char *name=ReadDataName(p);		//��һ�����֣�ע��p�ǰ���ַ�ķ�ʽ���ε�
		if( strcmp(string, name)==0 )	//��ͬ�ͷ���λ�ã��������ݿ�ʼ��λ��
		{
			_FREE( name );
			return p;
		}

		_FREE( name );
		if( p>=DataLen ) return -1;		//û���ҵ�
	}
	return -1;
}

//����
int CIniSet::GotoNextLine(int p)
{
	int i = p;
	for(; i<DataLen; i++)
	{
		if( Data[i]=='\n' )				//�س�����
			return i+1;
	}
	return i;
}

//��ָ��λ�ö�һ��������
char *CIniSet::ReadDataName(int &p)			//���ò�������ͨ��p���أ�����ֵ��
{											//���ݵ����ݵ�λ��
	char chr;
	char *Ret;
	int m=0;

	Ret=(char *)malloc(256);
	memset(Ret, 0, 256);

	for(int i=p; i<DataLen; i++)
	{
		chr=Data[i];

		//����
		if( chr == '\r' )
		{
			p=i+1;			
			return Ret;
		}
		
		//����
		if( chr == '=' || chr == ';' )
		{
			p=i+1;						//ָ�����ݵ����ݣ������˷ָ���
			return Ret;
		}
		
		Ret[m]=chr;
		m++;
	}
	return Ret;
}

//��ָ��λ�ö�һ�ַ���
char *CIniSet::ReadText(int p)
{
	char chr;
	char *Ret;
	int n=p, m=0;

	int EndLine=GotoNextLine(p);		//��һ�е�λ�ã�ֻ��һ�е�����
	Ret=(char *)malloc(EndLine-p+1);	//�����ڴ�
	memset(Ret, 0, EndLine-p+1);		//��0

	for(int i=0; i<DataLen-p; i++)
	{
		chr=Data[n];

		//����			�÷ֺţ��س���TAB������]����
		if( chr == ';' || chr == '\r' || chr == '\t' || chr == ']' || chr == '\n')
		{
			return Ret;
		}
		
		Ret[m]=chr;		//���뵽������
		m++;
		n++;
	}
	return Ret;
}

/////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////

//����ͨ��ʽ��һ�ַ�������
char *CIniSet::ReadText(char *index, char *name)
{
	int n=FindIndex(index);				//ͨ�����������ҵ�λ��
	if( n == -1 )
	{
		return 0;
	}

	int m=FindData(n, name);			//ͨ��������λ�ú����������ҵ��������ݵ�λ��
	if( m==-1 )
	{
		return 0;
	}

	return ReadText(m);					//����������
}
	
//��ָ�����ж�һ�ַ��������������������ڶ���һ�����ݣ�
//���ҿ��Բ����ľ�������֣�������ѭ���ķ�ʽ��
char *CIniSet::ReadText(char *index, int lines)
{
	int n=FindIndex(index);				//ͨ�����������ҵ�λ��
	if( n == -1 )
	{
		ShowMessage("Can't find [%s] in file '%s'!", index, FileName);
		return "";
	}

	//����ָ������
	n=GotoNextLine(n);
	for(int i=0; i<lines; i++)
	{
		if( n<DataLen )
			n=GotoNextLine(n);
	}

	//������
	while( n<=DataLen )
	{
		if( Data[n] == '=' )		//�ҵ��ָ��� =
		{
			n++;					//�ƶ���=����һ���ַ�
			return ReadText(n);		//��������
		}
		if( Data[n] == '\r' )
		{
			return "";				//û���ҵ�
		}
		n++;
	}

	return "";
}

//����ͨ��ʽ��һ��������
int CIniSet::ReadInt(char *index, char *name)
{
	int n=FindIndex(index);			//��������
	if( n == -1 )
	{
		ShowMessage("Can't find [%s] in file <%s>",index, FileName);
		return ERROR_DATA;
	}

	int m=FindData(n, name);		//���������ע��
	if( m==-1 )
	{
		ShowMessage("Can't find [%s] '%s' in file <%s>",index, name, FileName);
		return ERROR_DATA;
	}

	char *str=ReadText(m);			//������������
	int ret=atoi(str);				//ת��������
	_FREE(str);
	return ret;
}



//��ָ�����ж�һ������ͬ����Ϊ�˳����Ķ�ȡ�����ҿ��Բ����ľ��������
int CIniSet::ReadInt(char *index, int lines)
{
	int n=FindIndex(index);			//�����˵��
	if( n == -1 )
	{
		ShowMessage("Can't find [%s] in file <%s>",index, FileName);
		return ERROR_DATA;
	}

	//����ָ������
	n=GotoNextLine(n);
	for(int i=0; i<lines; i++)
	{
		if( n<DataLen )
			n=GotoNextLine(n);
	}

	//������
	while( n<DataLen )
	{
		if( Data[n] == '=' )		//Ӧ���������˰ɣ������׾Ϳ��������ע��
		{
			n++;
			char *str=ReadText(n);	//��������
			int ret=atoi(str);
			_FREE(str);
			return ret;
		}
		if( Data[n] == '\r' )
		{
			return ERROR_DATA;
		}
		n++;
	}

	return ERROR_DATA;
}

//��ָ�����ж�һ�������ƣ�Ŀ�ġ�������
char *CIniSet::ReadData(char *index, int lines)
{
	int n=FindIndex(index);			//û��������
	if( n == -1 )
	{
		ShowMessage("Can't find [%s] in file <%s>",index, FileName);
		return NULL;
	}

	//����ָ������
	n=GotoNextLine(n);
	for(int i=0; i<lines; i++)
	{
		if( n<DataLen )
			n=GotoNextLine(n);
	}

	return ReadDataName(n);			// �������ַ���
}

//����ͨ��ʽдһ�ַ������ݡ����������֣�����
bool CIniSet::WriteText(char *index, char *name, char *string)
{
	int n=FindIndex(index);			// �Ҹ���������,����Ҳ���,���½�һ��.
	if( n == -1 )					// �½�����
	{
		AddIndex(index);			// ����һ��һ������
		n=FindIndex(index);			// �ҵ��ոս��õ�������λ��
		AddData(n, name, string);	// �ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	int m=FindData(n, name);
	if( m==-1 )		//�½�����
	{
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	ModityData(n, name, string);	//�޸�һ������

	return true;
}

//����ͨ��ʽдһ����
bool CIniSet::WriteInt(char *index, char *name, int num)
{
	char string[32];
	sprintf_s( string,32, "%d", num);
	int n=FindIndex(index);			//����
	if( n == -1 )	//�½�����
	{
		AddIndex(index);			//�������ע��
		n=FindIndex(index);
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	int m=FindData(n, name);
	if( m==-1 )		//�½�����
	{
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	ModityData(n, name, string);	//�޸�һ������

	return true;
}

/////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////

//����һ������
bool CIniSet::AddIndex(char *index)
{
	char str[256];
	memset(str, 0, 256);
	int n=FindIndex(index);			//���ҿ���û��һ����

	if( n == -1 )	//�½�����
	{
		sprintf_s(str,256,"\r\n[%s]\r\n",index);		//ע���ʽ������Ҫ�ֿ�
		Data=(char *)realloc(Data, DataLen+strlen(str));	//���·����ڴ�
		sprintf_s(&Data[DataLen],strlen(str), "%s", str);		//���ں���
		DataLen+=strlen(str);					//���³���

		InitIndex();							//���½���������
		return true;
	}
	
	return false;	//�Ѿ�����
}

//�ڵ�ǰλ�ü���һ������
bool CIniSet::AddData(int p, char *name, char *string)
{
	char *str;
	int len=strlen(string);
	str=new char[len+256];						//�����ռ�
	memset(str, 0, len+256);
	sprintf_s(str,len+256,"%s=%s\r\n",name,string);		//����tiamo=1�ĸ�ʽ
	len=strlen(str);							//�����㳤��

	p=GotoNextLine(p);	//����
	Data=(char *)realloc(Data, DataLen+len);	//���·����ڴ�

	char *temp=new char[DataLen-p];				//��������ݷŵ�temp����
	memcpy(temp, &Data[p], DataLen-p);
	memcpy(&Data[p+len], temp, DataLen-p);		//�Ѻ���İᵽĩβ
	memcpy(&Data[p], str, len);					//�����¼ӵ�����
	DataLen+=len;								//�������ݳ���

	_DELETE( temp );
	_DELETE( str );
	return true;
}

//�ڵ�ǰλ���޸�һ�����ݵ�ֵ
bool CIniSet::ModityData(int p, char *name, char *string)
{
	int n=FindData(p, name);					//�ҵ����ݡ�ע�⣺���ص�n�����ݵ�λ��

	char *t=ReadText(n);						//�������ݵ�����
	p=n+strlen(t);								//�ƶ������ݵ�ĩβ����һ��λ��
	_FREE(t);

	int newlen=strlen(string);					//�µ����ݳ���
	int oldlen=p-n;								//t�ĳ��ȣ�Ҳ����ԭ�������ݳ���

	Data=(char *)realloc(Data, DataLen+newlen-oldlen);	//���·����ڴ�

	char *temp=new char[DataLen-p];						//��ͬ�ķ���
	memcpy(temp, &Data[p], DataLen-p);
	memcpy(&Data[n+newlen], temp, DataLen-p);			//�Ѻ���İᵽĩβ
	memcpy(&Data[n], string, newlen);
	DataLen+=newlen-oldlen;								//�������ݳ���
	InitIndex();										// ��������λ���б�.

	_DELETE( temp );
	return true;
}

//�����ļ�����
char *CIniSet::GetData()
{
	return Data;
}

//����ļ�������
int CIniSet::GetLines(int cur)
{
	int n=1;
	for(int i=0; i<cur; i++)
	{
		if( Data[i]=='\n' )					//�ҵ�\n�������µ�һ��
			n++;
	}
	return n;
}

//�������������������Ǳ���(����)����ι��ж����У�һ��������ѭ��������
int CIniSet::GetContinueDataNum(char *index)
{
	int num=0;
	int n=FindIndex(index);				//����������
	n=GotoNextLine(n);					//��������(����)
	while(1)
	{									//Ҫ��һ�еĿ�ͷ����������ַ��Ļ����ͽ���
		if( Data[n] == '\r' || Data[n] == -3 || Data[n] == EOF 
			|| Data[n] == ' ' || Data[n] == '/' || Data[n] == '\t' || Data[n] == '\n' )
		{
			return num;
		}
		else
		{
			num++;						//��һ
			n=GotoNextLine(n);			//����һ�еĿ�ͷ
			if( n >= DataLen )	
				return num;			//�ļ�����
		}
	}
}

//*************************
//���������Ϣ
void CIniSet::PutDebugString(char *msg)
{
	static int num=0;
	char t[16];

	FILE *fp;
	if( (fopen_s(&fp , "debug.txt","a+")))
	{
		ShowMessage("Can't open Debug.txt (%s)", msg);
		return;
	}
	
	num++;
	sprintf_s(t,16, "\n%d:", num);
	fseek(fp, 0, SEEK_END);
	fwrite(t, strlen(t), 1, fp);
	fwrite(msg, strlen(msg), 1, fp);
	fclose(fp);
}

//***************************
//��ȡ�ļ�����
int CIniSet::GetFileLength(char *name)
{
	int fh, nbytes;

	fh = _open( name, _O_RDONLY );	//ֻ��ģʽ
	if( fh== -1 )
	{
		return -1;
	}
	
	nbytes=_filelength(fh);
	_close(fh);
	
	return nbytes;
}

//**************************
//��ʾ������Ϣ
void CIniSet::ShowMessage(char *msg,...)
{
	va_list va;
	char str[256];

	va_start(va,msg);
	vsprintf_s(str,256,msg,va);
	va_end(va);

	MessageBox(NULL, str, "Message",MB_OK);
}
