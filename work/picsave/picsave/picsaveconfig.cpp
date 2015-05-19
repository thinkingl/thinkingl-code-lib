#include "picsaveconfig.h"
#include <QSettings>

CPicSaveConfig::CPicSaveConfig()
	:m_pSetting(0)
{
	m_pSetting = new QSettings(QSettings::IniFormat, QSettings::UserScope, "LL", "PicSave");
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
	return m_pSetting->value("picsave/picdir", "./pics/").toString();
}

void CPicSaveConfig::SetPicSaveDir(const QString& dir)
{
	m_pSetting->setValue("picsave/picdir", dir);
}

const int CPicSaveConfig::GetElapse() const
{
	return m_pSetting->value("picsave/elapse", 60*60).toInt();
}

void CPicSaveConfig::SetElapse(int timeElapseSecs)
{
	m_pSetting->setValue("picsave/elapse", timeElapseSecs);
}


