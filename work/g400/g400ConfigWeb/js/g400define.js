// g400 常量定义

// 这里要与 g400_fcgi 的 g400define.h 中的定义的一致,否则会解析错误.

var G400_MESSAGE = {
    MSG_LOGIN: 'MSG_LOGIN',                 // 登录
    MSG_LOGINOUT: 'MSG_LOGINOUT',          // 退出登录.

    MSG_BASE_STATUS: 'MSG_BASE_STATUS',     // G400基本状态
    MSG_GK_STATUS : 'MSG_GK_STATUS',        // GK状态.
    MSG_KDM_STATUS : 'MSG_KDM_STATUS',      // 平台状态.

    MSG_ADD_KDM: 'MSG_ADD_KDM',         // 添加新平台.
    MSG_MODIFY_KDM: 'MSG_MODIFY_KDM',   // 修改平台信息.
    MSG_DEL_KDM: 'MSG_DEL_KDM',         // 删除平台.

    MSG_GET_NET_CONFIG : "MSG_GET_NET_CONFIG",  // 获取网络参数.
    MSG_SET_NET_CONFIG : "MSG_SET_NET_CONFIG",  // 设置网络参数.

    MSG_GET_GK_CONFIG : "MSG_GET_GK_CONFIG",    // 获取GK配置信息.
    MSG_SET_GK_CONFIG : "MSG_SET_GK_CONFIG",    // 设置GK配置信息.

    MSG_GET_MT_LIST : "MSG_MT_LIST",        // 获取MT列表.(分页).
    MSG_MT_CONFIG : "MSG_MT_CONFIG",        // 设置MT参数,当前只有优先级可配.

    MSG_LOG_URL_LIST : "MSG_LOG_URL_LIST",  // 日志文件列表.

    MSG_REBOOT : "MSG_REBOOT",              // 重启.
    MSG_UPDATE: "MSG_UPDATE",              // 系统升级.
    MSG_CHANGE_PASSWORD : "MSG_CHANGE_PASSWORD",    // 修改密码.
    
    // 客户端请求服务器要Notify消息.因为设计中服务器不主动向客户端发送消息.
    // 所以所有的通知消息都要由客户端主动请求获得.
    // 同时此消息也作为心跳包.
    MSG_GET_NOTIFY : "MSG_GET_NOTIFY",           
     
    MSG_NTY_MT_STATUS : "MSG_NTY_MT_STATUS",    // MT状态变化通知.(可能包含有多个).
    MSG_NTY_LOG : "MSG_NTY_LOG",                // 日志提醒.

    MSG_END : ""
};

var G400_ERROR = {
    ERR_UNKNOWN : 0,
    ERR_OK : 1
}