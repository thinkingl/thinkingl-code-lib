#include <windows.h>
#include <process.h>
#include "picsave.h"
#include <string>
#include <QDir>
#include <QFileDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <Qtimer>
#include <QDomDocument>
#include <QDebug>

using namespace std;

const QString DevicelistFileName = "/caps/wx/devicelist.xml";
const QString XmlFileLocalPath = "./devicelist.xml";


picsave::picsave(QWidget *parent)
	: QMainWindow(parent)
	, m_pDownloader(0)
	, m_pTimer(0)
	, m_curState( StateIdle )
{
	ui.setupUi(this);

	this->setWindowIcon(QIcon(":/picsave/pic.ico"));

	// 读取配置.
	ReadConfig();

	// 默认提示清空.
	ui.labelState->setText("");

	// 按钮的信号绑定.
	connect(ui.pushButtonOK, SIGNAL(clicked()), SLOT(OnBtnOk()));	// OK
	connect(ui.pushButtonCancel, SIGNAL(clicked()), SLOT(OnBtnCancel()));	// 取消.
	connect(ui.pushButtonChooseNewDir, SIGNAL(clicked()), SLOT(OnBtnChooseNewDir()));	// 选择新目录.
	connect(ui.pushButtonOpenDir, SIGNAL(clicked()), SLOT(OnBtnOpenDir()));	// 打开目录.

	// 系统托盘.
	QAction* pActionShow = new QAction(this);
	pActionShow->setText(tr("显示"));

	QAction* pActionExit = new QAction(this);
	pActionExit->setText( tr("关闭") );

	QMenu* pTrayMenu = new QMenu(this);
	pTrayMenu->addAction(pActionShow);
	pTrayMenu->addAction(pActionExit);



	// 创建托盘.
	QSystemTrayIcon* pTray = new QSystemTrayIcon(this);

	pTray->setToolTip(tr("图片抓拍助手"));
	pTray->setContextMenu(pTrayMenu);
	pTray->setIcon(QIcon(":/picsave/pic.ico"));

	pTray->show();
	pTray->showMessage(tr("图片抓拍助手"),tr("图片抓拍"));

	// 信号连接.
	connect(pActionShow, SIGNAL(triggered(bool)), this, SLOT(OnTrayShow()));
	connect(pActionExit, SIGNAL(triggered(bool)), this, SLOT(OnTrayExit()));

	//点击托盘执行的事件
	connect(pTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
	//connect(pTrayMenu, SIGNAL(showWidget()), this, SLOT(showNormal()));


	// 定时器.
	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(OnCheckPicTimer()));
	ResetAll();


	// 立马检查一次.
//	emit OnCheckPicTimer();
}

void picsave::iconIsActived(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
		//点击托盘显示窗口
	case QSystemTrayIcon::Trigger:
	{
		showNormal();
		break;
	}
		//双击托盘显示窗口
	case QSystemTrayIcon::DoubleClick:
	{
		showNormal();
		break;
	}
	default:
		break;
	}
}

picsave::~picsave()
{
	m_pTimer->stop();

//	SAFE_DELETE(m_pDownloader);
}

void picsave::OnBtnOk()
{
	// 保存配置.

	// 服务器地址.
	QString addr = ui.lineEditServerAddr->text();
	m_cfg.SetServerAddr(addr);

	// 图片保存目录.
	QString dir = ui.lineEditPicDir->text();
	m_cfg.SetPicSaveDir(dir);

	// 抓拍间隔
	int elapseMin = ui.lineEditElapse->text().toInt();
	m_cfg.SetElapse(elapseMin * 60);

	// 开机自动运行.
	m_cfg;

	// 更新定时器.
	ResetAll();

	// 关闭.
	this->hide();
}

void picsave::OnBtnCancel()
{
	this->hide();
}

void picsave::OnBtnChooseNewDir()
{
	QString oldDir = ui.lineEditPicDir->text();
	QString newDir = QFileDialog::getExistingDirectory(this, tr("选择新的图片目录"), oldDir);
	if ( !newDir.isEmpty() )
	{
		ui.lineEditPicDir->setText(newDir);
	}
}

void picsave::OnBtnOpenDir()
{
	QString dir = m_cfg.GetPicSaveDir();

	dir.replace('/', '\\');

	QDir().mkpath(dir);

	wstring cmd = L" /e, ";
	cmd += dir.toStdWString();
	ShellExecute(
		NULL,
		(L"open"),
		//NULL,
		(L"Explorer.exe"),
		cmd.c_str(), // (L"/e, d:\\pics"),
		NULL,
		SW_SHOWDEFAULT);
}

void picsave::ReadConfig()
{
	// 服务器地址.
	QString addr = m_cfg.GetServerAddr();
	ui.lineEditServerAddr->setText(addr);

	// 图片保存目录.
	QString dir = m_cfg.GetPicSaveDir();
	ui.lineEditPicDir->setText(dir);

	// 抓拍间隔.
	int elapse = m_cfg.GetElapseSec();
	ui.lineEditElapse->setText(QString::number(elapse/60));

	// 开机自动运行.

}

void picsave::OnTrayShow()
{
	this->show();
}

void picsave::OnTrayExit()
{
	exit(0);
}

void picsave::closeEvent(QCloseEvent *event)
{
	this->hide();
	event->ignore();
}

void picsave::OnCheckPicTimer()
{
	switch (m_curState)
	{
	case StateIdle:
	{
		Q_ASSERT((m_pDownloader == 0)&&"Last work unfinished!");

		RecreateDownloader();


		QString serverAddr = m_cfg.GetServerAddr();
		QString xmlFileUrl = NormalizeUrl(serverAddr, DevicelistFileName);
		m_pDownloader->StartFileDownload(xmlFileUrl, XmlFileLocalPath);

		m_curState = StateDownloadXML;
	}
		break;

	case StateDownloadXML:
		break;

	case StateDownloadPic:
		break;
	}
}

void picsave::OnDownloadFinished(QString url, emDownLoadErrorType er, QDateTime fileLastModified)
{
	qDebug() << "Downlaod finish! url: " << url << " er: " << er << " lastmodified: " << fileLastModified.toString("yy/MM/dd hh:mm:ss");

	m_pDownloader->deleteLater();
	m_pDownloader = 0;

	if (url.indexOf(DevicelistFileName) != -1)
	{
		// 是xml文件完成了.
		qDebug() << "XML file download finished!";

		m_waittingPicList = ParsePicInfo(XmlFileLocalPath);

		// 切换到图片下载状态.
		m_curState = StateDownloadPic;

		// 判断图片最后抓拍时间,是否达到设置的抓拍间隔.
		emit OnCheckPic();
	}
	else
	{
		// 是图片文件完成了.
		QString state = fileLastModified.toLocalTime().toString(("最近抓拍时间 yyyy年MM月dd日  hh:mm:ss"));
		if ( !state.isEmpty() )
		{
			ui.labelState->setText(state);
		}

		// 下载下一个.
		emit OnCheckPic();
	}
}

void picsave::OnDownloadProgress(QString url, qint64 cur, qint64 total)
{
	qDebug() << "Download progress. url: " << url << " cur: " << cur << "/" << total;
}

QString picsave::NormalizeUrl(const QString& serverAddr, const QString urlDir)
{
	QString url = serverAddr;

	// 补上http://
	const QString httpHead = "http://";
	if (url.indexOf(httpHead) != 0)
	{
		url = httpHead + url;
	}

	if ( !urlDir.startsWith('/') && !url.endsWith('/'))
	{
		url += '/';
	}

	url += urlDir;

	return url;
}

CPicInfoList picsave::ParsePicInfo(QString xmlFileName)
{
	CPicInfoList picInfoList;

	// 解析XML文件.
	QFile xmlFile(xmlFileName);
	if (!xmlFile.open(QIODevice::ReadOnly))
	{
		qDebug() << "Open devicelist xml file fail! xml: " << XmlFileLocalPath;
		return picInfoList;
	}

	QDomDocument doc;
	if (!doc.setContent(&xmlFile))
	{
		qDebug() << "Parse xml fail! ";
		return picInfoList;
	}

	QDomNodeList devXmlList = doc.elementsByTagName("device");

	for (int i = 0; i < devXmlList.size(); ++i)
	{
		const QDomNode& devNode = devXmlList.at(i);

		CPicInfo pic;
		pic.m_deviceId = GetDomNodeValue(devNode, "id");			// 设备ID
		pic.m_deviceName = GetDomNodeValue(devNode, "name");	// 设备名称
		pic.m_picUrl = GetDomNodeValue(devNode, "photo");		// 图片路径(url)
		pic.m_chnId = GetDomNodeValue(devNode, "chn").toInt();	// 通道ID
		picInfoList.push_back(pic);
	}
	return picInfoList;
}

QString picsave::GetDomNodeValue(const QDomNode& node, const QString& tagName)
{
	QDomElement devIdEle = node.firstChildElement(tagName);
	if (!devIdEle.isNull())
	{
		QString text = devIdEle.text();
// 		if ( text.endsWith( '"' ) && text.startsWith('"') )
// 		{
// 			text = text.mid(1, text.length() - 1);
// 		}
		return text;
	}
	return "";
}

void picsave::OnCheckPic()
{
	// 取列表头上的.
	if ( m_waittingPicList.empty() )
	{
		qDebug() << "Waitting pic queue is empty!";
		m_curState = StateIdle;
		return;
	}

	CPicInfo picInfo = m_waittingPicList.front();
	m_waittingPicList.pop_front();

	// 找到最近的这个设备的图片.
	QString picPath = m_cfg.GetPicPath(picInfo.m_deviceId, picInfo.m_chnId);
	if ( IsPicExpired( picPath) )
	{
		// 下载这个图片.
		StartDownloadPic(picInfo);
	}
	else
	{
		qDebug() << "Pic is new, don't need update. name: " << picInfo.m_deviceName;
		// 这个图片不需要更新,继续检查下一张.
		emit OnCheckPic();
	}
}

bool picsave::IsPicExpired(const QString& picPath)
{
	if ( picPath.isEmpty() )
	{
		return true;
	}
	// 获取图片文件的时间.
	QFileInfo info(picPath);
	bool exist = info.exists();
	if ( !exist )
	{
		return true;	// 文件不存在,下载.
	}

	QDateTime lastModifiedTime = info.lastModified();
	QDateTime now = QDateTime::currentDateTime();
	uint diff = now.toTime_t() - lastModifiedTime.toTime_t();

	int elapseSec = m_cfg.GetElapseSec();
	return diff >= elapseSec;
}

void picsave::StartDownloadPic(const CPicInfo& picInfo)
{
	Q_ASSERT((m_pDownloader == 0) && "Last download unfinished!");
	//SAFE_DELETE(m_pDownloader);

	m_pDownloader->deleteLater();
	m_pDownloader = 0;

	RecreateDownloader();
	
	QString addr = m_cfg.GetServerAddr();
	// 图片所在路径url需要 http://ip/shm/图片相对路径.
	if ( !addr.endsWith('/'))
	{
		addr += '/';
	}
	if ( !picInfo.m_picUrl.startsWith("shm") )
	{
		addr += "shm";
	}

	QString picUrl = NormalizeUrl( addr, picInfo.m_picUrl );

	QString picPath = GetPicPath( picInfo );

	// 记录这个设备的图片路径.
	m_cfg.SetPicPath(picInfo.m_deviceId, picInfo.m_chnId, picPath);
	
	// 开始下载.
	m_pDownloader->StartFileDownload( picUrl, picPath );

	qDebug() << "Start download pic: " << picUrl << " path: " << picPath;
}

QString picsave::GetPicPath(const CPicInfo& picInfo)
{
	// 图片本地路径. 格式: 图片存放目录/日期/设备名_时间_设备ID_通道ID.文件后缀
	QString picPath = m_cfg.GetPicSaveDir();

	if (!(picPath.endsWith('\\') || picPath.endsWith("/")))
	{
		picPath += "/";
	}
	// 日期.
	QString dateStr = QDate::currentDate().toString("yyyyMMdd");
	picPath += dateStr;
	picPath += '/';

	// 创建目录.
	QDir().mkpath(picPath);

	picPath += picInfo.m_deviceName;
	picPath += '_';
	picPath += QDateTime::currentDateTime().toString("hhmmss");
	picPath += '_';
	picPath += picInfo.m_deviceId;
	picPath += '_';
	picPath += QString::number( picInfo.m_chnId );
	picPath += ".jpg";
	return picPath;
}

void picsave::ResetAll()
{
	m_pTimer->stop();

	int picExpiredTime = m_cfg.GetElapseSec();
	int minTimerElapse = 5 * 1000;
	m_pTimer->start(min(minTimerElapse, picExpiredTime * 1000 / 2));		// 定时器间隔, 设置为1分钟,或者图片超时时间的一半,保证图片会被更新.

	// 状态归位.
	m_curState = StateIdle;
	m_pDownloader->deleteLater();
	m_pDownloader = 0;
	m_waittingPicList.clear();
}

void picsave::RecreateDownloader()
{
	//SAFE_DELETE(m_pDownloader);
	m_pDownloader->deleteLater();
	m_pDownloader = 0;

	m_pDownloader = new DownloadControl(this);

	// 连接信号.
	connect(m_pDownloader, SIGNAL(SignalDownloadFinished(QString, emDownLoadErrorType,QDateTime)), this, SLOT(OnDownloadFinished(QString, emDownLoadErrorType,QDateTime)));
	connect(m_pDownloader, SIGNAL(SignalProgress(QString, qint64, qint64)), this, SLOT(OnDownloadProgress(QString, qint64, qint64)));
}
