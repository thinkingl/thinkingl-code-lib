#pragma once

#include "portabledefine.h"

class CConfig
{
//    static CConfig *Instance();
    // 获取根目录。
    static tstring GetRootFolder();

    static tstring GetRootUrl();

    static tstring GetProxyUserName();

    static tstring GetProxyPassword();

    static tstring GetProxyIp();

    static uint16 GetProxyPort();

protected:
    CConfig(void);
    virtual ~CConfig(void);
};
