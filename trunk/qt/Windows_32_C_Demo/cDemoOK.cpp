/******************************************************************************* 
@All Right Reserved (C), 2010-2100, golaxy.cn
Filepath   : e:\Project\IctClas\ictclas_demo_c
Filename   : win_cDemo.cpp
Version    : ver 5.0
Author     : x10n6y@gmail.com 
Date       : 2010/07/12  
Description:   
History    :
1.2010/07/12 14:04 Created by x10n6y@gmail.com Version 5.0 
*******************************************************************************/
#ifndef OS_LINUX
#include <Windows.h>
#pragma comment(lib, "ICTCLAS50.lib") //ICTCLAS50.lib����뵽������
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include  <string>
#include "ICTCLAS50.h"

using namespace std;

#define POS_TAGGER_TEST
#ifdef POS_TAGGER_TEST
bool g_bPOSTagged=true;
#else
bool g_bPOSTagged=false;
#endif	
  


void testICTCLAS_ParagraphProcess();
void testImportUserDict();

int main()
{
    testICTCLAS_ParagraphProcess();//�ִ�����	
	testImportUserDict();//�����û��ֵ�
	getchar();
	return 1;
}


void testICTCLAS_ParagraphProcess()
{
	if(!ICTCLAS_Init()) //��ʼ���ִ������
	{
		printf("Init fails\n");  
		return ;
	}
	else
	{
		printf("Init ok\n");
	}

   //���ô��Ա�ע��(0 ������������ע����1 ������һ����ע����2 ���������ע����3 ����һ����ע��)
	ICTCLAS_SetPOSmap(2);

	char* sSentence="�����������뿪�������سǣ�Ԥ���������������ͻص����������Ͼ��ǽ�����������¶�̬"; // ��Ҫ�ִʵ�����
	unsigned int nPaLen=strlen(sSentence); // ��Ҫ�ִʵĳ���
	char* sRst=0;   //�û����з���ռ䣬���ڱ�������
	sRst=(char *)malloc(nPaLen*6); //���鳤��Ϊ�ַ������ȵı���
	int nRstLen=0; //�ִʽ���ĳ���

	nRstLen = ICTCLAS_ParagraphProcess(sSentence,nPaLen,sRst,CODE_TYPE_UNKNOWN,0);  //�ַ�������
	printf("The result is:\n%s\n",sRst);
	free(sRst);
	//Ҳ�������ļ��ı�����,�����ļ��ִʽӿڣ����ִʽ��д�롰Test_reslult.txt���ļ���
	ICTCLAS_FileProcess("Test.txt", "Test_result.txt",CODE_TYPE_GB,1);
	ICTCLAS_Exit();	//�ͷ���Դ�˳�
	return ;  


}




void testImportUserDict()
{
	//��ʼ���ִ������
	//������ô˽ӿں󣬲��ܵ��������ӿڣ�
	if(!ICTCLAS_Init())
	{ 
		printf("ICTCLAS INIT FAILED!\n");  //��ʼ��ʧ�ܣ��˳���
		return ;
	}

	char* sSentence="�����������뿪�������سǣ�Ԥ���������������ͻص����������Ͼ��ǽ�����������¶�̬��";
	int nPaLen=strlen(sSentence);
	int nRstLen=0;
	char* sRst=0;//�û����з���ռ䣬���ڱ�������
	sRst=(char *)malloc(nPaLen*6);//���鳤��Ϊ�ַ������ȵı���


	//δ�����û��ֵ�ǰ�Ĵ���
	nRstLen=ICTCLAS_ParagraphProcess(sSentence,nPaLen,sRst,CODE_TYPE_UNKNOWN,1); 
	printf("����û��ֵ�ǰ�ִʽ��:\n%s\n",sRst);
	free(sRst);


	const char* pszDictBuffer="�����س�@@nr;���¶�̬@@nr"; //�ַ�����ʽ�����û��ʵ�
	unsigned int nItems=ICTCLAS_ImportUserDict (pszDictBuffer, strlen(pszDictBuffer),CODE_TYPE_UNKNOWN);
	//Ҳ���Խ��û��ʻ�д���ļ�������֮���û��з�����
	//unsigned int nItems=ICTCLAS_ImportUserDictFile("userdict.txt",CODE_TYPE_UNKNOWN);
	printf("�����%d ���û���\n",nItems); //�ļ���ʽ�����û��ֵ�

	char* sRst1=0;
	sRst1=(char *)malloc(nPaLen*6);
	nRstLen=ICTCLAS_ParagraphProcess(sSentence,nPaLen,sRst1,CODE_TYPE_UNKNOWN,1);//�����û��ֵ��ִʴ���
	printf("����û��ֵ��ִʽ��:\n%s\n",sRst1);
	free(sRst1);

	ICTCLAS_SaveTheUsrDic(); //������ӵ��û��ʻ�,�´�ʹ����Ȼ��Ч
	//�ͷ���Դ�˳�
	ICTCLAS_Exit();
	return ;
}