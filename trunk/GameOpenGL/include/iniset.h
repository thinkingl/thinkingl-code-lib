//********************************************
//	IniSet  by thinkingl
//  �������ļ����еĲ���.

//  ������2006 .1 . 6 
//  ����� Sword2 ��Դ����.
//********************************************
#ifndef _GP_INI_
#define _GP_INI_

#pragma once

#define ERROR_DATA -99999999

#define _RELEASE(A)			try{ if( (A) != NULL ) { (A) -> Release(); (A) = NULL; } } catch(...) { PutDebugString("error ->Release!"); }
#define _DELETE(X)			try{ if( (X) != NULL ) { delete (X); (X) = NULL; } } catch(...) { PutDebugString("error delete!"); }
#define _DELETE_ARRAY(X)	try{ if( (X) != NULL ) { delete[] (X); (X) = NULL; } } catch(...) { PutDebugString("error delete[]!"); }
#define _FREE(X)			try{ if( (X) != NULL ) { free(X); (X)=NULL; } } catch(...) { PutDebugString("error free()!"); }

//  ��������ַ�������������󳤶�.
int const MAX_LENGTH = 260;

//�����ļ���
class CIniSet{
private:
	char FileName[MAX_LENGTH];	//�ļ���
	int DataLen;			//�ļ�����
	char *Data;				//�ļ�����

	int IndexNum;			//������Ŀ��[]����Ŀ��
	int *IndexList;			//������λ���б�

	int Point;				//��ǰָ��
	int Line, Word;			//��ǰ����

public:
	CIniSet();
	CIniSet(char *);		//��ʼ���������ļ�
	~CIniSet();				//�ͷ��ڴ�
	char *GetData();		//�����ļ�����
	int GetLines(int);			//�����ļ�������

	bool Open( char *);		//�������ļ�
	bool Save(char *filename=0);		//���������ļ�

private:
	void InitIndex();			//��ʼ������
	int FindIndex(char *);		//���ر���λ��
	int FindData(int, char *);	//��������λ��
	int GotoNextLine(int); 		//����
	char *ReadDataName(int &);	//��ָ��λ�ö�һ��������
	char *ReadText(int);		//��ָ��λ�ö��ַ���

	bool AddIndex(char *);		//����һ������
	bool AddData(int, char *, char *);	//�ڵ�ǰλ�ü���һ������
	bool ModityData(int, char *, char *); //�ڵ�ǰλ���޸�һ�����ݵ�ֵ

	// ***************************
	// ��ȡ�ļ�����
	int GetFileLength(char *);
	// **************************
	// ��ʾ������Ϣ
	void PutDebugString(char *);
	// **************************
	// ��ʾ������Ϣ
	void ShowMessage(char *msg,...);

public:
	int ReadInt(char *, char *);	// ��һ������
	char *ReadText(char *, char *);	// ��һ���ַ���
	int ReadInt(char *, int );		// ��ָ�����ж�һ����
	char *ReadText(char *, int);	// ��ָ�����ж�һ�ַ���
	char *ReadData(char *, int);	// ��ָ���ж�һ�ַ�����

	bool WriteInt(char *, char *, int);		// дһ������
	bool WriteText(char *, char *, char *);	// дһ���ַ���

	int GetContinueDataNum(char *);			// ������������������INDEX����һ�����У�

};

#endif