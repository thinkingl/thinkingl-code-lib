
#include "urldownload.h"
#include <QObject>
#include <QEventLoop>
#include <QFileInfo>
#include <QTextCodec>
#include <QDebug>
#include <QStringList>


Download::Download(int index, QObject *pParent)
	: QObject(pParent), m_Index(index)
{

	m_HaveDoneBytes = 0;
	m_StartPoint = 0;
	m_EndPoint = 0;
	m_File = NULL;
	m_Reply = NULL;
}
void Download::StartDownload(const QUrl &url, QFile *file, qint64 startPoint/* =0 */, qint64 endPoint/* =-1 */)
{
	if( NULL == file )
		return;
	m_HaveDoneBytes = 0;
	m_StartPoint = startPoint;
	m_EndPoint = endPoint;
	m_File = file;
	//根据HTTP协议，写入RANGE头部，说明请求文件的范围
	// 如果不分段且必须 m_Qnam.get(QNetworkRequest(url)); 这样写，才会正常，所以现在就这样写了，不再支持多线程下载
	
	/*QNetworkRequest qheader(url);
	QString range;
	range.sprintf("bytes=%lld-%lld", m_StartPoint, m_EndPoint);	
	qheader.setRawHeader("Range", range.toAscii());	
	//开始下载
	qDebug() << "Part " << m_Index << " start download from "<<m_StartPoint<<" to "<<m_EndPoint;	
	*/
	m_Reply = m_Qnam.get(QNetworkRequest(url));
	QNetworkReply::NetworkError ne = m_Reply->error();
	if (m_Reply && QNetworkReply::NoError == ne)
	{
		connect(m_Reply, SIGNAL(readyRead()), this, SLOT(HttpReadyRead()));
		connect(m_Reply, SIGNAL(finished()), this, SLOT(FinishedSlot()));		
	}
	else
	{
		qDebug() << "m_Reply failed!, error ="<<ne;
	}

}
void Download::StopDownload()
{
	if (m_Reply)
	{
		m_Reply->abort();
	}
}
//下载结束
void Download::FinishedSlot()
{   
	if (m_File != NULL)
	{
		m_File->flush();
		m_File = NULL;//这个只是个缓存，所以直接清NULL即可	
	}
	if (m_Reply != NULL)
	{
		m_Reply->deleteLater();
		m_Reply = NULL;
	}
	
	//因为m_HaveDoneBytes实际为 (m_EndPoint-m_StartPoint+1)，所以比较时要减1
	bool bComplete =  (m_StartPoint+m_HaveDoneBytes-1==m_EndPoint);
	qDebug() << "Part " << m_Index << " download "<<m_HaveDoneBytes<<" bytes, complete = "<<bComplete;
	emit PartFinished(m_Index, bComplete);
}

void Download::HttpReadyRead()
{
	if (!m_File || !m_Reply)
	{
		emit PartFinished(m_Index, false);
		return;
	}
	if (QNetworkReply::NoError != m_Reply->error())
	{
		emit PartFinished(m_Index, false);
		return;
	}
	//将读到的信息写入文件
	QByteArray buffer = m_Reply->readAll();	
	m_File->seek( m_StartPoint + m_HaveDoneBytes );
	m_File->write(buffer);
	int nSize = buffer.size();
	m_HaveDoneBytes += nSize;
	emit PartProgress(m_Index, nSize);
}

DownloadControl::DownloadControl(QObject *pParent)
	: QObject(pParent)
{
	m_bFailed = false;
	m_ndlThreadCount = 0;
	m_nFinishedThreadNum = 0;
	m_nReqFileSize = 0;
	m_nDoneBytes = 0;
	m_pFileSave = NULL;
	m_strUrl = "";
	m_bStopDownload = false;

	qRegisterMetaType<emDownLoadErrorType>("emDownLoadErrorType");

}
DownloadControl::~DownloadControl()
{
	StopDownload();
}

//用阻塞的方式获取下载文件的长度
void DownloadControl::GetFileInfo(QUrl url, qint64& fileLen, QDateTime& lastmodifiedTime )
{
	QNetworkAccessManager manager;	
	qDebug() << "Getting the file size...";
	QEventLoop loop;
	//发出请求，获取目标地址的头部信息
	QNetworkReply *pReply = manager.head(QNetworkRequest(url));
	if ( pReply == 0 )
	{
		fileLen = 0;
		return;
	}
	QObject::connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
	loop.exec();
	QVariant var = pReply->header(QNetworkRequest::ContentLengthHeader);
	QVariant varLastmodified = pReply->header(QNetworkRequest::LastModifiedHeader);
	
	pReply->deleteLater();
	pReply = NULL;
	Q_UNUSED(pReply);
	fileLen = var.toLongLong();
	qDebug() << "The file size is: " << fileLen;

	if ( !varLastmodified.isNull() )
	{
		lastmodifiedTime = varLastmodified.toDateTime();
	}
	qDebug() << "File last modifyed: " << lastmodifiedTime.toLocalTime().toString("yyyy/MM/dd hh:mm:ss");

}
qint64 DownloadControl::GetReqFileSize()
{
	return m_nReqFileSize;
}
QDateTime DownloadControl::GetFileLastModified()
{
	return m_fileLastModified;
}
QString DownloadControl::GetDownloadFile()//返回下载的文件名
{
	if (m_pFileSave)
	{
		return m_pFileSave->fileName();
	}
	return "";
}
void DownloadControl::StopDownload()
{   
	m_bStopDownload = true;

	foreach (Download *pDLThread, m_lsDownloadThreads)
	{
		if (pDLThread)
		{
			pDLThread->StopDownload();
		}
	}
	m_lsDownloadThreads.clear();
	if (m_pFileSave)
	{
		m_pFileSave->close();
		m_pFileSave = NULL;
	}
}
bool DownloadControl::StartFileDownload(const QString &url, const QString &saveFile, int nCount)
{   
	m_bStopDownload = false;
	m_bFailed = false;
	m_ndlThreadCount = nCount;
	m_nFinishedThreadNum = 0;
	m_strUrl = url;
	//检测文件大小时去掉反向代理路径
	QStringList strl = url.split("/");
	QString tmpUrl = url;
	foreach(QString str, strl)
	{
		if(str == "proxy")
		{
			QString subStr = "/proxy/" + strl.value(strl.indexOf(str) + 1);
			tmpUrl.remove(subStr);
		}
	}
	m_Url = QUrl(tmpUrl);	
	m_nDoneBytes = 0;
	//m_nReqFileSize = static_cast<int>(GetFileSize(m_Url));
	GetFileInfo(m_Url, m_nReqFileSize, m_fileLastModified);
	if (m_nReqFileSize == 0)
	{
		m_bFailed = true;
		emit SignalDownloadFinished(m_strUrl, LOG_TYPE_NOTFOUNDFILE, m_fileLastModified);

		qDebug()<<"http file not found!";
		return false;
	}
	//先获得文件的名字
	//QFileInfo fileInfo(m_Url.path());
	QString fileName = saveFile;
	if (!m_pFileSave)
	{
		m_pFileSave = new QFile(this);
	}
	m_pFileSave->setFileName(fileName);
	//打开文件
	if (!m_pFileSave->open(QIODevice::WriteOnly))
	{
		m_bFailed = true;		        
		emit SignalDownloadFinished(m_strUrl, LOG_TYPE_OPENFILEFAILED, m_fileLastModified);

		qDebug()<<"open file "<<fileName<<" for write failed";
		return false;
	}
	Download *tempDownload = NULL;
	//将文件分成PointCount段，用异步的方式下载
	qDebug() << "Start download file from " << url;
	int nPartBytes = m_nReqFileSize/m_ndlThreadCount;
	int nCurPos = 0;
	m_lsDownloadThreads.clear();
	for(int i=0; i<m_ndlThreadCount; i++)
	{
		//先算出每段的开头和结尾（HTTP协议所需要的信息）
		int nStart = nCurPos;		
		int nEnd = nStart + nPartBytes;	
		nCurPos = nEnd + 1;
		if (i == m_ndlThreadCount-1)
		{
			nEnd = m_nReqFileSize-1;//end也是位置，不是长度，所以整个文件的最后一个位置是 m_nReqFileSize-1
		}
		//分段下载该文件
		tempDownload = new Download(i+1, this);
		connect(tempDownload, SIGNAL(PartProgress(int, qint64)), this, SLOT(PriSlotOnPartProgress(int, qint64)));
		connect(tempDownload, SIGNAL(PartFinished(int, bool)), this, SLOT(PriSlotOnPartFinished(int, bool)));
		//connect(tempDownload, SIGNAL(DownloadFinished(int, bool)), tempDownload, SLOT(deleteLater()));
		tempDownload->StartDownload(m_Url, m_pFileSave, nStart, nEnd);
		m_lsDownloadThreads.push_back(tempDownload);
	}
	return true;
}
void DownloadControl::PriSlotOnPartFinished(int nIndex, bool bSucessful)
{
	if (m_bFailed)
	{
		return;
	}
	if (!bSucessful)
	{		
		m_bFailed = true;		        
		emit SignalDownloadFinished(m_strUrl, LOG_TYPE_THREADSDOWNDFAILED, m_fileLastModified);

		qDebug() << "some threads download failed";	
		return;
	}

	m_nFinishedThreadNum++;
	//如果完成数等于文件段数，则说明文件下载完毕，关闭文件，发生信号
	if( m_nFinishedThreadNum == m_ndlThreadCount )
	{  
		if (m_pFileSave != NULL)
		{
			m_pFileSave->close();		
			SAFE_DELETE(m_pFileSave);
		}
		qDebug() << "Download finished";
		emit SignalDownloadFinished(m_strUrl, LOG_TYPE_TRUE, m_fileLastModified);
	}
}

void DownloadControl::PriSlotOnPartProgress(int nIndex, qint64 nCurRcvBytes)
{
	qDebug()<<"thread"<<nIndex<<" recv "<<nCurRcvBytes<<" Bytes";
	m_nDoneBytes += static_cast<int>(nCurRcvBytes);
	emit SignalProgress(m_strUrl, m_nDoneBytes, m_nReqFileSize);
	if (m_nDoneBytes >= m_nReqFileSize)
	{        
        //emit SignalDownloadFinished(m_strUrl, LOG_TYPE_TRUE);
	}
}


CLogFileDownloadControl::CLogFileDownloadControl(QObject* pParent /* = 0 */):
DownloadControl(pParent)
{

}

CLogFileDownloadControl::~CLogFileDownloadControl()
{

}

void CLogFileDownloadControl::PriSlotOnPartFinished(int nIndex, bool bSucessful)
{   
	if (m_bFailed)
	{
		return;
	}
	if (!bSucessful)
	{		
		m_bFailed = true;		        
		emit SignalDownloadFinished(m_strUrl, LOG_TYPE_THREADSDOWNDFAILED, m_fileLastModified);

		qDebug() << "some threads download failed";	
		return;
	}

	m_nFinishedThreadNum++;

	//如果完成数等于文件段数，则说明文件下载完毕，关闭文件，发生信号
	if(m_nFinishedThreadNum == m_ndlThreadCount )
	{  
		if (m_pFileSave != NULL)
		{   
			QString strLogFile = m_pFileSave->fileName();
			m_pFileSave->close();		
			SAFE_DELETE(m_pFileSave);

			if (TranslateLogFileCodec(strLogFile,"utf8"))
			{
				qDebug() << "Download finished";
				emit SignalDownloadFinished(m_strUrl, LOG_TYPE_TRUE, m_fileLastModified);
			}
			else
			{
				emit SignalDownloadFinished(m_strUrl, LOG_TYPE_TRANSCODECFAILED, m_fileLastModified);
			}
		}
		else
		{
			emit SignalDownloadFinished(m_strUrl, LOG_TYPE_OPENFILEFAILED, m_fileLastModified);
		}
	}
}

bool CLogFileDownloadControl::TranslateLogFileCodec(QString strSrcLogFile,QString strSrcCodecName)
{
	QString strFileName = QFileInfo(strSrcLogFile).fileName(); //日志文件名
	QString strFileDir = QFileInfo(strSrcLogFile).absolutePath(); //日志文件所在目录
	QString strDestLogFile = strFileDir + "_" + strFileName; //转换后的日志文件名，在原文件名基础上加字符'_'

	bool bRst = false;
	if (TranslateFileToLocalCodec(strSrcLogFile,strDestLogFile,strSrcCodecName))
	{    
		//转换成功后，删除原日志文件
		QFile::remove(strSrcLogFile);
		bRst = QFile::rename(strDestLogFile,strSrcLogFile);
	}
	else
	{   
		//转换失败后，删除所有产生的临时文件
		QFile::remove(strSrcLogFile);
		QFile::remove(strDestLogFile);
		bRst = false;
	}

	return bRst;
}

bool CLogFileDownloadControl::TranslateFileToLocalCodec(QString strSrcFile,QString strDestFile,QString strSrcCodecName)
{   
	QFile srcFile(strSrcFile);
	if (!srcFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return false;
	}

	QFile destFile(strDestFile);
	if (!destFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
	{
		return false;
	}

	QTextCodec* pSrcCodec = QTextCodec::codecForName(strSrcCodecName.toLocal8Bit().constData());
	QTextCodec* pLocalCodec = QTextCodec::codecForLocale();

	//如果源文件编码与本地默认编码相同，则不做转换
	if (pSrcCodec != NULL &&
		pLocalCodec != NULL &&
		pSrcCodec->name() == pLocalCodec->name())
	{
		return true;
	}

	//源文件流,其字符编码由strSrcCodecName指定
	QTextStream srcTxtStream(&srcFile);
	srcTxtStream.setCodec(pSrcCodec);

	//转换后的目标文件流，默认使用当前系统编码
	QTextStream destTxtStream(&destFile);

	//每次进行编码转换的文件块大小
	const int nTransBlockSize = 1024;

	QString strContent = "";
	while (!srcTxtStream.atEnd() && !m_bStopDownload)
	{
		strContent = srcTxtStream.read(nTransBlockSize);
		destTxtStream << strContent;
	}

	return true;
}
