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

	// 是否正在解析网页。
	if ( m_pHtmlPageParser )
	{
		// 是否有需要下载的url？
		if ( !m_tUrlWaitingDownload.empty() )
		{
			// 是否需要下载。 

			// 下载。
		}
		else
		{
			// 已经没有需要下载的url了。
			// 这个网页已经处理完毕，可以保存了。

			// 保存到最终文件夹。

			// 删除缓存文件。
		}
	}
	else
	{
		// 获取一个需要处理的页面。 
		tstring strUrl, strLocalPath;

		// 尝试在缓存中获取页面以处理。
		if ( IWebpageManager::Instance()->GetCachedPage( strUrl, strLocalPath ) )
		{
			// 处理这个文件。

			// 判断是否是网页。
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
				// 保存这个文件到最终保存目录，并记录。

			}
			

			
		}

		// 获取主页。
	}

	

	return FALSE;
}

BOOL CPageFecher::HasPageWaiting()
{
	return TRUE;
}