#pragma once
/**
*	程序的配置文件.
*	+by lizhixing@2015-05-19
*/
#include <QString>
#include <QSettings>

class CPicSaveConfig
{
public:
	CPicSaveConfig();
	~CPicSaveConfig();

	// 服务器地址.
	const QString GetServerAddr() const;
	void SetServerAddr(const QString& addr);

	// 本地保存目录.
	const QString GetPicSaveDir() const;
	void SetPicSaveDir(const QString& dir);

	// 抓拍时间间隔.
	const int GetElapse() const;
	void SetElapse(int timeElapseSecs);

private:
	QSettings* m_pSetting;
};

