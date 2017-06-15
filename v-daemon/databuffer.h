#ifndef DATABUFFER_H
#define DATABUFFER_H

#include "idatatrans.h"
#include <QTimer>

/*
*   发送缓存, 用于平滑发送速度, 实现拥塞控制.
*       1) 将上层发下来的数据缓存下来, 定时发送出去.
*       2) 发送间隔根据下层的拥塞情况动态调整.
*   +by lzx@2017-06-06
*/

class DataBuffer : public IDataTrans
{
    Q_OBJECT
public:
    explicit DataBuffer(QObject *parent = 0);

    // 处理数据.
    virtual bool TransDataDown(const QByteArray& dataIn, QByteArrayList& dataOutDown, QByteArrayList& dataOutUp);
    virtual bool TransDataUp(const QByteArray& dataIn, QByteArrayList& dataOutDown, QByteArrayList& dataOutUp);
signals:

public slots:
    // 定时器.
    void OnTimer();
private:
    // 定时发包的定时器.
    // 当前有点问题, 因为这个定时器是属于这个对象的, 应该是全局控制发送频率. +by lzx@2017-06-15
    QTimer m_timer;

    //  发送缓存.
    QByteArrayList m_dataBuffer;

    // 定时器间隔.
    // 改为静态全局配置,因为网络状况是全局的. +by lzx@2017-06-15
    static int s_timerInterval;

    // 向下传输数据连续成功次数.
    // 改为静态全局配置, 因为网络状况是全局的. +by lzx@2017-06-15
    static int s_transDataDownSuccessCount;
};

#endif // DATABUFFER_H
