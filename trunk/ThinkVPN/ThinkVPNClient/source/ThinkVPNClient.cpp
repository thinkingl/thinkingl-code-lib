// ThinkVPNClient.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ThinkVPNClient.h"
#include "ThinkVPNClientDlg.h"
#include "ThinkVPNCommonLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CThinkVPNClientApp

BEGIN_MESSAGE_MAP(CThinkVPNClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CThinkVPNClientApp ����

CThinkVPNClientApp::CThinkVPNClientApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CThinkVPNClientApp ����

CThinkVPNClientApp theApp;


// CThinkVPNClientApp ��ʼ��

BOOL CThinkVPNClientApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("ThinkVPNClient"));

    // E:\svn\thinkingl-code-lib\trunk\ThinkVPN\Debug
    //wofstream of;
    //of.open( _T( "E:\\svn\\thinkingl-code-lib\\trunk\\ThinkVPN\\Debug\\aaaaaa.log" )  );
    //of << "1111111111111111111111111" << endl;
    //of.flush();

    //of << _T( "2222222222222222222222" ) << endl;
    //of.close();

	CThinkVPNClientDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
