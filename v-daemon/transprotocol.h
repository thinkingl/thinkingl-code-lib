#ifndef TRANSPROTOCOL_H
#define TRANSPROTOCOL_H

/**
 *  实现对包的可靠传输, 实现重传机制, 心跳, 超时检测.
 *  包头:
 *      CMD 1byte | sn 4byte | data
 *  +by lzx@2017-06-03
*/

#include "idatatrans.h"
#include <QMap>
#include <QTimer>

class TransProtocol : public IDataTrans
{
    Q_OBJECT
public:
    explicit TransProtocol(QObject *parent = 0);

    // 处理数据.
    virtual bool TransDataDown(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack);
    virtual bool TransDataUp(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack);

    // 命令.
    enum UDP_CMD
    {
        CMD_DATA,   // 数据
        CMD_CONFIRM,// 确认
        CMD_HEART_BEAT, // 心跳, 因为长时间没有数据, UDP的NAT通道会被路由器关闭.
        CMD_BYE,    // 关闭
    };

    class UDPPackHead
    {
    public:
        quint8 cmd;
        quint32 sn;

        UDPPackHead() : cmd(CMD_DATA), sn(0)
        {
        }
    };

    static const int TransProtocolHeadSize = 1+4;
signals:

public slots:

private slots:

    // 定时检测重传,断链.
    void OnTimer();
private:
    bool ReadHead( const QByteArray& data, UDPPackHead& head );
    bool WriteHead( QByteArray& data, const UDPPackHead& head );

    void ConfirmPack( QByteArrayList& dataOutForward );

    bool ProcessData(QByteArrayList& dataOutBack);

private:
    QByteArrayList m_recvSpilitedDataCache;

    typedef QMap<quint32, QByteArray> UDPPackCache;
    // 发送缓冲, 用于重传. 包序号->UDP包数据.
    UDPPackCache m_sendCache;

    // 接收缓冲.
    UDPPackCache m_recvCache;

    // 定时器, 用于检测重传及超时.
    QTimer m_timer;

    // 当前发送包序号.
    quint32 m_curSentPackSN;

    // 当前已经确认到的包序号.
    quint32 m_curConfirmedRecvPackSN;

    // 当前已经处理过的接收到的包序号, 再收到之前的序号就不用处理了.
    quint32 m_curProcessedRecvPackSN;
};

#endif // TRANSPROTOCOL_H
