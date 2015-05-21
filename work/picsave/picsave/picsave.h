#ifndef PICSAVE_H
#define PICSAVE_H

#include <QtWidgets/QMainWindow>
#include "ui_picsave.h"
#include "picsaveconfig.h"
#include <QSystemTrayIcon>
#include "urldownload.h"
#include <QString>
#include <QDomNode>

class CPicInfo
{
public:
	QString m_deviceId;
	int m_chnId;
	QString m_deviceName;
	QString m_picUrl;
};
typedef QList<CPicInfo> CPicInfoList;

class picsave : public QMainWindow
{
	Q_OBJECT

public:
	picsave(QWidget *parent = 0);
	~picsave();



public slots:
	// 确定按钮
	void OnBtnOk();

	// 取消按钮
	void OnBtnCancel();

	// 选择新的保存位置.
	void OnBtnChooseNewDir();
	// 打开图片保存目录.
	void OnBtnOpenDir();

	// 最小化.

	// 系统托盘处理.
	void iconIsActived(QSystemTrayIcon::ActivationReason reason);
	// 显示窗口.
	void OnTrayShow();
	// 退出程序.
	void OnTrayExit();

	// 重载关闭,改为隐藏窗口.
	void closeEvent(QCloseEvent *event);

	// 下载完成.
	void OnDownloadFinished(QString url, emDownLoadErrorType code );
	// 下载进度.
	void OnDownloadProgress(QString url, qint64 cur, qint64 total);

	// 定时监测服务器上图片的时间.
	void OnCheckPicTimer();

	// 检测图片时间,是否需要下载.
	void OnCheckPic();

	// 拼凑本地图片路径.

private:
	// 读取配置,显示到界面上.
	void ReadConfig();

	// 拼凑生成下载的完整URL.
	QString NormalizeUrl(const QString& serverAddr, const QString urlDir);


	// 解析出服务器上的图片完整信息.
	CPicInfoList ParsePicInfo( QString xmlFileName );

	// 获取一个XML节点下指定名称的子节点的值.
	QString GetDomNodeValue(const QDomNode& node, const QString& tagName);
	
private:
	Ui::picsaveClass ui;

	// 程序配置.
	CPicSaveConfig m_cfg;

	// 下载控件
	DownloadControl* m_pDownloader;

	// 定时器, 定时检测抓拍图片.
	QTimer* m_pTimer;

	enum EWorkState
	{
		StateIdle,			// 初始状态.
		StateDownloadXML,	// 正在下载XML文件.
		StateDownloadPic,	// 正在下载图片.
	};

	EWorkState m_curState;

	// 等待抓拍的图片
	CPicInfoList m_waittingPicList;
};

#endif // PICSAVE_H
