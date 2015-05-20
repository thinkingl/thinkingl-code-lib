/**
 **实际使用的时候只需要使用 DownloadControl 类即可，Download类只是一个下载线程，供DownloadControl内部使用
 **/
#ifndef urldownload_2013_2_27_h__
#define urldownload_2013_2_27_h__

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

enum emDownLoadErrorType
{
    LOG_TYPE_TRUE = 0,              //下载成功
    LOG_TYPE_NOTFOUNDFILE,          //没有找到文件而导致下载失败
    LOG_TYPE_OPENFILEFAILED,        //打开文件写入失败
    LOG_TYPE_THREADSDOWNDFAILED,    //多线程下载失败
	LOG_TYPE_TRANSCODECFAILED       //文件格式转码失败
};

//多线程下载的线程数，由于QT存在从公网下载文件会请求两次的bug（原因未知），所以目前只能用单线程下载，不要修改这个值 zsy 20141016
const int PointCount = 1;
//用于下载文件（或文件的一部分）
class Download : public QObject
{
	Q_OBJECT
private:
	QNetworkAccessManager m_Qnam;
	QNetworkReply *m_Reply;
	QFile *m_File;
	const int m_Index;
	qint64 m_HaveDoneBytes;
	qint64 m_StartPoint;
	qint64 m_EndPoint;
public:
	Download(int index, QObject *parent = 0);	
	void StartDownload(const QUrl &url, QFile *file, qint64 startPoint=0, qint64 endPoint=-1); 
	void StopDownload();
signals:
	void PartFinished(int nIndex, bool bSucessful);
	//上报本次收到的字节数
	void PartProgress(int nIndex, qint64 nCurRcvBytes);
public slots:
	void FinishedSlot();
	void HttpReadyRead();
};
//用于管理文件的下载
class DownloadControl : public QObject
{
	Q_OBJECT
protected:
	int m_ndlThreadCount;
	int m_nFinishedThreadNum;
	int m_nReqFileSize;
	int m_nDoneBytes;
	QUrl m_Url;
	QFile *m_pFileSave;
	bool m_bFailed;//是否已经发生错误的标记
	QString m_strUrl;//用于标示当前下载对象
	QList<Download *> m_lsDownloadThreads;//下载线程
	bool m_bStopDownload; //停止下载
public:
	DownloadControl(QObject *parent = 0);
	virtual ~DownloadControl();
	bool StartFileDownload(const QString &url, const QString &saveFile, int count = PointCount);
	qint64 GetReqFileSize();//返回所请求文件在服务器上的大小
	QString GetDownloadFile();//返回下载的文件名
	void StopDownload();//停止下载
private:
	qint64 GetFileSize(QUrl url);
signals:
	////下载成功通知（或失败通知）
    //@yu 2013.2.4 修改 （增加提示错误码）
	//void SignalDownloadFinished(bool bSucessful);
    void SignalDownloadFinished(QString strUrl, emDownLoadErrorType emErrorType);

	//进度通知，已经完成字节数/总字节数
	void SignalProgress(QString strUrl, qint64 nDoneBytes, qint64 nTotalBytes);
public slots:
	virtual void PriSlotOnPartFinished(int nIndex, bool bSucessful);
	virtual void PriSlotOnPartProgress(int nIndex, qint64 nCurRcvBytes);
};

//下载日志文件
class CLogFileDownloadControl : public DownloadControl
{
	Q_OBJECT
public:
	CLogFileDownloadControl(QObject* pParent = 0);
	virtual ~CLogFileDownloadControl();
public slots:
	virtual void PriSlotOnPartFinished(int nIndex, bool bSucessful);
private:
	bool TranslateFileToLocalCodec(QString strSrcFile,QString strDestFile,QString strSrcCodecName);
	bool TranslateLogFileCodec(QString strSrcLogFile,QString strSrcCodecName);
};

#endif // urldownload_2013_2_27_h__