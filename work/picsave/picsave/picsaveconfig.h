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
	const int GetElapseSec() const;
	void SetElapse(int timeElapseSecs);

	// 最后一次抓拍图片的路径.
	void SetPicPath( const QString& devId, int chnId, const QString& path );
	QString GetPicPath(const QString& devId, int chnId);

	// 抓拍时间段配置.
	void SetTimeRange( const QTime& start, const QTime& end );
	void GetTimeRange(QTime& start, QTime& end);

private:
	// 拼凑图片抓拍路径的Key.
	QString MakePicPathKey(const QString& devId, int chnId);
private:
	QSettings* m_pSetting;
};

