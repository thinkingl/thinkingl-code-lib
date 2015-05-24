#include "picsaveconfig.h"
#include <QSettings>

CPicSaveConfig::CPicSaveConfig()
	:m_pSetting(0)
{
	m_pSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope, "LL", "PicSave");
	m_pSetting->setIniCodec("UTF-8");
}


CPicSaveConfig::~CPicSaveConfig()
{
}

const QString CPicSaveConfig::GetServerAddr() const
{
	return m_pSetting->value("picsave/server", "").toString();
}

void CPicSaveConfig::SetServerAddr(const QString& addr)
{
	m_pSetting->setValue("picsave/server", addr);
}

const QString CPicSaveConfig::GetPicSaveDir() const
{
	return m_pSetting->value("picsave/picdir", "d:/pics/").toString();
}

void CPicSaveConfig::SetPicSaveDir(const QString& dir)
{
	m_pSetting->setValue("picsave/picdir", dir);
}

const int CPicSaveConfig::GetElapseSec() const
{
	return m_pSetting->value("picsave/elapse", 60*60).toInt();
}

void CPicSaveConfig::SetElapse(int timeElapseSecs)
{
	m_pSetting->setValue("picsave/elapse", timeElapseSecs);
}

void CPicSaveConfig::SetPicPath(const QString& devId, int chnId, const QString& path)
{
	QString key = MakePicPathKey(devId, chnId);
	m_pSetting->setValue(key, path);
}

QString CPicSaveConfig::GetPicPath(const QString& devId, int chnId)
{
	QString key = MakePicPathKey(devId, chnId);
	return m_pSetting->value(key).toString();
}

QString CPicSaveConfig::MakePicPathKey(const QString& devId, int chnId)
{
	QString key = "picpath/";
	key = key + devId + "_" + QString::number(chnId);
	return key;
}


