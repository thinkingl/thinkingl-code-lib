#include "..\include\PageFecher.h"
#include "Log.h"
#include "IWebpageManager.h"
#include "ClassFactory.h"
#include "Common.h"

CPageFecher::CPageFecher(void)
{
	this->m_pHtmlPageParser = NULL;
}

CPageFecher::~CPageFecher(void)
{
}

BOOL CPageFecher::FetchOnePage()
{
	CLog() << _T( "CPageFecher Fetch One Page!" ) << endl;
	Sleep( 1000 );

	// �Ƿ����ڽ�����ҳ��
	if ( m_pHtmlPageParser )
	{
		// �Ƿ�����Ҫ���ص�url��
		if ( !m_tUrlWaitingDownload.empty() )
		{
			// �Ƿ���Ҫ���ء� 

			// ���ء�
		}
		else
		{
			// �Ѿ�û����Ҫ���ص�url�ˡ�
			// �����ҳ�Ѿ�������ϣ����Ա����ˡ�

			// ���浽�����ļ��С�

			// ɾ�������ļ���
		}
	}
	else
	{
		// ��ȡһ����Ҫ�����ҳ�档 
		tstring strUrl, strLocalPath;

		// �����ڻ����л�ȡҳ���Դ���
		if ( IWebpageManager::Instance()->GetCachedPage( strUrl, strLocalPath ) )
		{
			// ��������ļ���

			// �ж��Ƿ�����ҳ��
			if ( CCommon::IsWebpage( strLocalPath.c_str() ) )
			{
				this->m_pHtmlPageParser = CClassFactory::CreateHtmlPageParser();
				BOOL bRet = this->m_pHtmlPageParser->Parse( strLocalPath.c_str(), strUrl.c_str() );
				ASSERT( bRet );

				bRet = this->m_pHtmlPageParser->GetAllUrl( this->m_tUrlWaitingDownload );
				ASSERT( bRet );
			}
			else
			{
				// ��������ļ������ձ���Ŀ¼������¼��

			}
			

			
		}

		// ��ȡ��ҳ��
	}

	

	return FALSE;
}

BOOL CPageFecher::HasPageWaiting()
{
	return TRUE;
}