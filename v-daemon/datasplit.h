#ifndef DATASPLIT_H
#define DATASPLIT_H

#include "idatatrans.h"
#include <QMap>
#include <QTimer>

/**
*   数据的拆分和组合, 为了适合UDP的包大小用.
*   +by lzx@2017-05-25
*/
// 格式: cmd | sn | sub total | sub sn | data ( 重传和关闭命令没有data )
// 格式: cmd 1byte 命令(UDP_CMD) | sn 4byte 当前总包序号 | 4byte 当前数据段的包总数 | 4byte 当前数据段的包序号 | data 数据
// 数据格式: SHA-256 32byte | 原始数据
static const int UDP_PACK_HEAD_LEN = 1+4+4+4;
static const int SHA_256_LEN = 32;

class DataSplit : public IDataTrans
{
    Q_OBJECT
public:
    explicit DataSplit(QObject *parent = 0);



    // 处理数据.
    virtual bool TransDataForward(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack);
    virtual bool TransDataBack(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack);

    // 命令.
    enum UDP_CMD
    {
        CMD_DATA,   // 数据
        CMD_CONFIRM,// 确认
        CMD_BYE,    // 关闭
    };

    class UDPPackHead
    {
    public:
        quint8 cmd;
        quint32 sn;
        quint32 curDataPackNum;
        quint32 curDataSN;

        UDPPackHead() : cmd(CMD_DATA), sn(0), curDataPackNum(0), curDataSN(0)
        {
        }
    };

signals:

public slots:

private:
    bool ReadHead( const QByteArray& data, UDPPackHead& head );
    bool WriteHead( const UDPPackHead& head, QByteArray& data );

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

#endif // DATASPLIT_H
