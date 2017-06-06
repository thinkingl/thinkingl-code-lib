#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "idatatrans.h"
#include <QUdpSocket>
#include <QMap>
#include <QTimer>

class UDPServer : public IDataTrans
{
    Q_OBJECT
public:

    explicit UDPServer(QObject *parent);

    // 开始工作.
    bool Start(int localPort, QString socks5ServerAddr, quint16 socks5ServerPort);

    // 处理数据.
    virtual bool TransDataDown(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack);
    virtual bool TransDataUp(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack);

    // 回送数据.
    virtual bool InputDataDown(IDataTrans* pNextItem, const QByteArray& data);

    // 向下关闭, UDP Server不能销毁,  只销毁其中的记录.
    virtual void CloseDown( IDataTrans* pre );

signals:

private slots:
    void readPendingDatagrams();
    void CheckTableTimeout();
private:

    IDataTrans* CreateDataTrans( );
private:
    QUdpSocket* m_udpSocket;

    // UDP源地址同处理对象的双向映射.
    class UDPSenderAndDataTransTableItem
    {
    public:
        QHostAddress senderAddr;    // 发送者的地址.
        quint16 senderPort;         // 发送者的端口.
        IDataTrans* dataTrans;      // 数据处理对象.
        time_t lastActiveTime;      // 最后的活动时间.
    };
    typedef QList< UDPSenderAndDataTransTableItem > UDPSenderAndDataTransTable;
    UDPSenderAndDataTransTable m_udpSenderAndDataTransTable;

    // 检测超时的定时器.
    QTimer m_checkTimer;

    QString m_socks5ServerAddr;
    quint16 m_socks5ServerPort;
private:
    UDPSenderAndDataTransTableItem* findItem( const QHostAddress& senderAddr, quint16 senderPort );
    UDPSenderAndDataTransTableItem* findItem( IDataTrans* pTrans );
};

#endif // UDPSERVER_H
