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
#pragma comment(lib, "ICTCLAS50.lib") //ICTCLAS50.lib库加入到工程中
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
    testICTCLAS_ParagraphProcess();//分词用例	
	testImportUserDict();//导入用户字典
	getchar();
	return 1;
}


void testICTCLAS_ParagraphProcess()
{
	if(!ICTCLAS_Init()) //初始化分词组件。
	{
		printf("Init fails\n");  
		return ;
	}
	else
	{
		printf("Init ok\n");
	}

   //设置词性标注集(0 计算所二级标注集，1 计算所一级标注集，2 北大二级标注集，3 北大一级标注集)
	ICTCLAS_SetPOSmap(2);

	char* sSentence="随后温总理就离开了舟曲县城，预计温总理今天下午就回到北京。以上就是今天上午的最新动态"; // 需要分词的内容
	unsigned int nPaLen=strlen(sSentence); // 需要分词的长度
	char* sRst=0;   //用户自行分配空间，用于保存结果；
	sRst=(char *)malloc(nPaLen*6); //建议长度为字符串长度的倍。
	int nRstLen=0; //分词结果的长度

	nRstLen = ICTCLAS_ParagraphProcess(sSentence,nPaLen,sRst,CODE_TYPE_UNKNOWN,0);  //字符串处理
	printf("The result is:\n%s\n",sRst);
	free(sRst);
	//也可以用文件文本导入,调用文件分词接口，将分词结果写入“Test_reslult.txt”文件中
	ICTCLAS_FileProcess("Test.txt", "Test_result.txt",CODE_TYPE_GB,1);
	ICTCLAS_Exit();	//释放资源退出
	return ;  


}




void testImportUserDict()
{
	//初始化分词组件。
	//必须调用此接口后，才能调用其它接口！
	if(!ICTCLAS_Init())
	{ 
		printf("ICTCLAS INIT FAILED!\n");  //初始化失败，退出。
		return ;
	}

	char* sSentence="随后温总理就离开了舟曲县城，预计温总理今天下午就回到北京。以上就是今天上午的最新动态。";
	int nPaLen=strlen(sSentence);
	int nRstLen=0;
	char* sRst=0;//用户自行分配空间，用于保存结果；
	sRst=(char *)malloc(nPaLen*6);//建议长度为字符串长度的倍。


	//未导入用户字典前的处理
	nRstLen=ICTCLAS_ParagraphProcess(sSentence,nPaLen,sRst,CODE_TYPE_UNKNOWN,1); 
	printf("添加用户字典前分词结果:\n%s\n",sRst);
	free(sRst);


	const char* pszDictBuffer="舟曲县城@@nr;最新动态@@nr"; //字符串形式导入用户词典
	unsigned int nItems=ICTCLAS_ImportUserDict (pszDictBuffer, strlen(pszDictBuffer),CODE_TYPE_UNKNOWN);
	//也可以将用户词汇写入文件，词语之间用换行符隔开
	//unsigned int nItems=ICTCLAS_ImportUserDictFile("userdict.txt",CODE_TYPE_UNKNOWN);
	printf("添加了%d 个用户词\n",nItems); //文件形式导入用户字典

	char* sRst1=0;
	sRst1=(char *)malloc(nPaLen*6);
	nRstLen=ICTCLAS_ParagraphProcess(sSentence,nPaLen,sRst1,CODE_TYPE_UNKNOWN,1);//导入用户字典后分词处理
	printf("添加用户字典后分词结果:\n%s\n",sRst1);
	free(sRst1);

	ICTCLAS_SaveTheUsrDic(); //保存添加的用户词汇,下次使用依然有效
	//释放资源退出
	ICTCLAS_Exit();
	return ;
}