/******************************************************************************* 
@All Right Reserved (C), 2010-2100, golaxy.cn
Filepath   : E:\Project\IctClas\ictclas5
Filename   : ICTCLAS5.h
Version    : ver 5.0
Author     : x10n6y@gmail.com 
Date       : 2010/06/03  
Description:    
History    :
             1.2010/06/03 17:19 Created by x10n6y@gmail.com Version 5.0  

*******************************************************************************/
#ifndef ICTCLAS_LIB_
#define ICTCLAS_LIB_

#define ICTCLAS_EXPORTS

#ifdef OS_LINUX
	#define ICTCLAS_API 
#else
	#ifdef ICTCLAS_EXPORTS		
		#define ICTCLAS_API extern "C" __declspec(dllexport)	//export function
	#else
		#define ICTCLAS_API extern "C" //extern , c compiler
	#endif
#endif

//////////////////////////////////////////////////////////////////////////
//��ע��
//////////////////////////////////////////////////////////////////////////
#define ICT_POS_MAP_SECOND 0 //������������ע��
#define ICT_POS_MAP_FIRST 1  //������һ����ע��
#define  PKU_POS_MAP_SECOND 2 //���������ע��  
#define PKU_POS_MAP_FIRST 3	//����һ����ע��
#define POS_MAP_NUMBER 4 //��ע�� ���� 
#define  POS_SIZE 8 // ���Ա������ֽ��� 

//////////////////////////////////////////////////////////////////////////
// �ַ���������
//////////////////////////////////////////////////////////////////////////
enum eCodeType {
	CODE_TYPE_UNKNOWN,//type unknown
	CODE_TYPE_ASCII,//ASCII
	CODE_TYPE_GB,//GB2312,GBK,GB10380
	CODE_TYPE_UTF8,//UTF-8
	CODE_TYPE_BIG5//BIG5
};
//////////////////////////////////////////////////////////////////////////
//�ַ������ 
//////////////////////////////////////////////////////////////////////////
#pragma pack(1) 
struct tagICTCLAS_Result{
  int iStartPos; //��ʼλ��
  int iLength; //����
  char szPOS[POS_SIZE];//����
  int	iPOS; //����ID
  int iWordID; //��ID
  int iWordType; //�������ͣ��û��ʻ㣿(0-��,1-��)
  int iWeight;// ����Ȩ��
 };
#pragma pack() 
typedef tagICTCLAS_Result* LPICTCLAS_RESULT;
//////////////////////////////////////////////////////////////////////////
//�ӿ�
//////////////////////////////////////////////////////////////////////////
/************************************************************************
* Method:     ICTCLAS_Init<!��ȡ�����ļ������شʵ��>
* Parameter:  const char * pszInitDir<!�����ļ�����data�ļ�������·�� >
* Returns:      bool<!��ʼ���Ƿ�ɹ�>
* Description: ���������κνӿ�ǰ�����뱣֤���ӿڵ��óɹ���
* Remark: 1.2010/06/03 17 : 34 created by x10n6y version 5.0 
*************************************************************************/
ICTCLAS_API bool ICTCLAS_Init(const char* pszInitDir=NULL);

/************************************************************************
* Method:     ICTCLAS_Exit<! �˳����ͷ������Դ>
* Returns:     ICTCLAS_API bool<! �˳��Ƿ�ɹ�>
* Description:	���в�����ɺ�����ñ��ӿ��ͷ������Դ��
* Remark: 1.2010/06/04 9 : 42 created by x10n6y version 5.0 
*************************************************************************/
ICTCLAS_API bool ICTCLAS_Exit();

/************************************************************************
* Method:     ICTCLAS_SetPOSmap<!ָ�����Ա�ע��>
* Parameter:  int nPOSmap<! ��ע��ID>
					 ICT_POS_MAP_FIRST  ������һ����ע��
					 ICT_POS_MAP_SECOND  ������������ע��
					 PKU_POS_MAP_SECOND   ���������ע��
					 PKU_POS_MAP_FIRST 	  ����һ����ע��

* Returns:    ICTCLAS_API bool<! ָ���ɹ����>
* Description: 
* Remark: 1.2010/06/22 11 : 11 created by x10n6y version 5.0 
*************************************************************************/
ICTCLAS_API bool ICTCLAS_SetPOSmap(int nPOSmap);

/************************************************************************
* Method:     ICTCLAS_ImportUserDict<! �����û��ʵ��ļ�>
* Parameter:  const char * pszFileName<! �û��ʵ�·������>
* Parameter:  e_CodeType codeType<!�ʵ��������>
* Returns:      ICTCLAS_API unsigned int<! �ɹ�����Ĵʻ�����>
* Description: �û�����ʻ��ļ���ʽ���£�
                        1.����������á�@@����������磺���п�Ժ@@nr��
						2.һ��һ�ʣ�
						3.���Կ�ʡ��

* Remark: 1.2010/06/04 9 : 43 created by x10n6y version 5.0 
************************************************************************/
ICTCLAS_API unsigned int ICTCLAS_ImportUserDictFile(
	const char* pszFileName,
	eCodeType codeType=CODE_TYPE_UNKNOWN
	);

/************************************************************************
* Method:     ICTCLAS_ImportUserDict<! �����û��ʵ�>
* Parameter: char* pszDictBuffer<! �û��ʻ��ַ���>
					1.����������á�@@�������
					2.�����֮���� ��ǡ�;�������
					3.���Կ�ʡ��
					���磺���п�Ժ@@nr;�ִ� v;ϵͳ@@adj;����;����
					���ߣ����п�Ժ;�ִ�;ϵͳ;����;��
* Parameter:  e_CodeType codeType<!�ʵ��������>
* Returns:      ICTCLAS_API unsigned int<! �ɹ�����Ĵʻ�����>
* Description: 1.���ӿڽ������û�����Ĵʻ㣬������Ӧ���û��ʵ䡣
                      2.�ôʵ䣬�������ڴ���ԭ�е��û��ʵ䡣
* Remark: 1.2010/06/04 9 : 43 created by x10n6y version 5.0 
************************************************************************/
ICTCLAS_API unsigned int ICTCLAS_ImportUserDict(
								   const char* pszDictBuffer,
								   const int nLength,
								   eCodeType codeType
								   );
/************************************************************************
* Method:     ICTCLAS_SaveTheUsrDic<!�����û��ʵ�>
* Description:1.���ӿڽ��Ḳ��ԭ��/data/�ļ����û���شʵ䡣  
                     2.�û����������ļ��У�ָ���´��Ƿ�ʹ�øôʵ䡣
*
* Remark: 1.2010/07/05 16 : 16 created by x10n6y version 2.0 
*************************************************************************/
ICTCLAS_API int ICTCLAS_SaveTheUsrDic();

/************************************************************************
* Method:     ICTCLAS_ParagraphProcess<! �ִʣ����ؽ��Ϊ�ַ���>
* Parameter:  const char * pszText<!��Ҫ�ִʵ��ı�����>
* Parameter:  int iLength<!��Ҫ�ִʵ��ı�����>
* Parameter:  char*	pszResult [out]<!�ִʽ���ַ���>
* Parameter:  e_CodeType codeType<!�ַ���������>
* Parameter:  int bEnablePOS<���Ƿ���Ա�ע >
* Returns:      ICTCLAS_API int<! ����ַ�������>
* Description: ���ñ��ӿڣ����û������ڴ棬��������pszResult��
�������ڴ��Сstrlen(pszText)*6��
* Remark: 1.2010/06/22 11 : 11 created by x10n6y version 5.0 
*************************************************************************/
ICTCLAS_API int ICTCLAS_ParagraphProcess(
	const char*  pszText,
	int			    iLength,
	char*		    pszResult, //[out]
	eCodeType	codeType=CODE_TYPE_UNKNOWN,
	bool		        bEnablePOS=false
	);
/************************************************************************
* Method:     ICTCLAS_ParagraphProcessA<! �ִʣ����ؽ��Ϊ�ַ����ṹ����>
* Parameter:  const char * pszText<! ��Ҫ�ִʵ��ı�����>
* Parameter:  int iLength<! ��Ҫ�ִʵ��ı�����>
* Parameter:  int & nResultCount [out]<! ������鳤��>
* Parameter:  e_CodeType codeType<! �ַ���������>
* Parameter:  int bEnablePOS<! �Ƿ���Ա�ע>
* Returns:      ICTCLAS_API t_pRstVec<! �������>
* Description: 
                      ���ô˽ӿں�Ӧ����ICTCLAS_ResultFree() �ͷ�����ڴ档
* Remark: 1.2010/06/22 11 : 11 created by x10n6y version 5.0 
*************************************************************************/
ICTCLAS_API LPICTCLAS_RESULT  ICTCLAS_ParagraphProcessA(
	const char*  pszText,
	int			    iLength,
	int			    &nResultCount, //[out]
	eCodeType	codeType=CODE_TYPE_UNKNOWN,
	bool		        bEnablePOS=false
	);


/************************************************************************
* Method:       ICTCLAS_ResultFree<! �ͷŽ���ڴ�>
* Parameter:   t_pRstVec pRetVec<!Ҫ�ͷŵĽ������ >
* Returns:       ICTCLAS_API bool<! �ͷųɹ����>
* Description: ���ӿ������ͷ� ICTCLAS_ParagraphProcessA ���ɵĽ���ڴ�
* Remark: 1.2010/06/22 11 : 11 created by x10n6y version 5.0 
*************************************************************************/
ICTCLAS_API bool ICTCLAS_ResultFree(LPICTCLAS_RESULT pRetVec);



/************************************************************************
* Method:     ICTCLAS_FileProcess<!�ı��ļ��ִ�>
* Parameter:  const char * pszSrcFileName<!Ҫ�ִʵ��ļ�>
* Parameter:  const char * pszDstFileName<! ����ļ����λ��>
* Parameter:  e_CodeType srcCodeType<!Ҫ������ı���������>
* Parameter:  bool bEnablePOS<! �Ƿ���Ա�׼>
* Returns:      ICTCLAS_API bool<! �ִ��Ƿ�ɹ�>
* Description: 1.�û�����ָ���ִʽ������λ�ã�ϵͳ�����������
                        ��ǰĿ¼�£�test_result.txt �С�
					 2.pszDstFileName�����ļ������ڣ� ���Զ�����;
					    ����������������ݡ�
* Remark: 1.2010/06/22 11 : 11 created by x10n6y version 5.0 
*************************************************************************/
ICTCLAS_API bool ICTCLAS_FileProcess(
									 const char* pszSrcFileName,
									 const char* pszDstFileName, 
									 eCodeType	srcCodeType=CODE_TYPE_UNKNOWN,
									 bool		        bEnablePOS=false
									 );

/************************************************************************
* Method:      ICTCLAS_ParagraphProcessAW<!C# �ӿ�>
* Parameter:  const char * pszText<! Ҫ������ı�>
* Parameter:  LPICTCLAS_RESULT pResult<! �������>
* Parameter:  eCodeType codeType<! �ı���������>
* Parameter:  bool bEnablePOS<! �Ƿ���Ա�ע>
* Returns:      ICTCLAS_API int<! ������鳤��>
* Description:  ��������ڴ�ռ䣬�û��������κδ���
*
* Remark: 1.2010/07/05 16 : 16 created by x10n6y version 2.0 
*************************************************************************/
ICTCLAS_API int ICTCLAS_ParagraphProcessAW(
	const char*               pszText,
	LPICTCLAS_RESULT  pResult,
	eCodeType	             codeType=CODE_TYPE_UNKNOWN,
	bool	  	                     bEnablePOS=false
	);
#endif // ICTCLAS_LIB_

