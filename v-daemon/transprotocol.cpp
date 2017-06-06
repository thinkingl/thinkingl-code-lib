#include "transprotocol.h"
#include <QtEndian>
#include <QDebug>
#include <QDateTime>


TransProtocol::TransProtocol(QObject *parent) : IDataTrans(parent)
  , m_curSentPackSN(0)
  , m_curConfirmedRecvPackSN(0)
  , m_curProcessedRecvPackSN(0)
  , m_lastSendPackDownTime( 0 )
  , m_lastRecvPackUpTime( 0 )
{
    connect( &m_timer, SIGNAL(timeout()), this, SLOT(OnTimer()) );
    m_timer.start( 500 );
}

TransProtocol::~TransProtocol()
{
    qDebug() << "TransProtocol destruct!";
    this->SendBye();
}

bool TransProtocol::TransDataDown(const QByteArray &dataIn, QByteArrayList& dataOutDown, QByteArrayList& dataOutUp)
{
    // 将dataIn中的原始数据加上包头, 发到下一环节.
    QByteArray dataOut;

    UDPPackHead head;
    head.cmd = CMD_DATA;
    head.sn = m_curSentPackSN;
    m_curSentPackSN++;

    this->WriteHead( dataOut, head );
    dataOut.push_back( dataIn );

    // 输出到下一环节.
    dataOutDown.push_back( dataOut );

    // 缓存.
    // to be continued.

    // 更新最近活动时间.
    m_lastSendPackDownTime = QDateTime::currentDateTime().toTime_t();

    // 激活接收时间检测.
    if( m_lastRecvPackUpTime == 0)
    {
        m_lastRecvPackUpTime = QDateTime::currentDateTime().toTime_t();
    }

    if( s_logRawData )
    {
        qDebug() << "TransProtocol trans data forward:[" << dataOut << "]";
    }

    return true;
}

bool TransProtocol::TransDataUp(const QByteArray &dataIn, QByteArrayList& dataOutDown, QByteArrayList& dataOutUp)
{
    UDPPackHead head;
    if( this->ReadHead( dataIn, head ) )
    {
        switch( head.cmd )
        {
        case CMD_DATA:
        {
            // 缓存数据包.
            this->CachePack( dataIn );

            // 确认.
            this->ConfirmPack( dataOutDown );

            // 处理数据.
            this->ProcessData( dataOutUp );
        }
            break;
        case CMD_CONFIRM:
        {
            qDebug() << "TransProtocol recv config pack!";
            this->OnConfirmPack( dataIn );
        }
            break;
        case CMD_HEART_BEAT:
        {
            qDebug() << "TransProtocol recv Heart Beat!";
        }
            break;
        case CMD_BYE:
        {
            qDebug() << "TransProtocol recv bye!";
            this->CloseUp();
        }
            break;
        default:
            break;
        }

        // 校验序号.
        // todo

        // 更新保活时间.
        m_lastRecvPackUpTime = QDateTime::currentDateTime().toTime_t();

    }
    else
    {
        qDebug() << "Error! TransProtocol read head fail! data:[" << dataIn << "]";
        return false;
    }

    return true;
}


void TransProtocol::OnTimer()
{
    // 检测重传.
    this->CheckSendRetrans();

    // 发送心跳.
    this->CheckSendHeartbeat();

    // 检测接收超时.
    this->CheckRecvHeartbeat();
}

void TransProtocol::CheckSendHeartbeat()
{
    int timeSpan = QDateTime::currentDateTime().toTime_t() - m_lastSendPackDownTime;
    if( timeSpan > 10 )
    {
        // 发送心跳包.
        this->SendHeartbeat();
        m_lastSendPackDownTime = QDateTime::currentDateTime().toTime_t();
    }
}

void TransProtocol::CheckRecvHeartbeat()
{
    if( 0 == m_lastRecvPackUpTime )
    {
        return;
    }

    int timeSpan = QDateTime::currentDateTime().toTime_t() - m_lastRecvPackUpTime;
    if( timeSpan > 30 )
    {
        // 超时.
        qDebug() << "TransProtocol timeout!";
        this->CloseUp();
    }
}

void TransProtocol::SendHeartbeat()
{
    UDPPackHead head;
    head.cmd = CMD_HEART_BEAT;

    QByteArray pack;
    WriteHead( pack, head );

    if( this->NextDataTrans() )
    {
        this->NextDataTrans()->InputDataDown( this, pack );

        qDebug() << "TransProtocol Send heart beat!";
    }
    else
    {
        qDebug() << "Error! TransProtocol has no down layer!";
    }

}

void TransProtocol::SendBye()
{
    UDPPackHead head;
    head.cmd = CMD_BYE;

    QByteArray pack;
    WriteHead( pack, head );

    if( this->NextDataTrans() )
    {
        this->NextDataTrans()->InputDataDown( this, pack );

        qDebug() << "TransProtocol Send Bye!";
    }
    else
    {
        qDebug() << "Error! TransProtocol has no down layer when send bye!";
    }
}

void TransProtocol::OnConfirmPack(const QByteArray &data)
{
    UDPPackHead head;
    if( ReadHead( data, head ) )
    {
        UDPPackCache::iterator it = m_sendCache.begin();
        while (it!=m_sendCache.end()) {
            UDPPackHead cacheHead;
            ReadHead( it.value(), cacheHead );
            if( cacheHead.sn <= head.sn )
            {
                it = m_sendCache.erase( it );
                qDebug() << "TransProtocol erase send cache:[" << cacheHead.sn << "]";
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        qDebug() << "Read confirm pack head fail! data:[" << data << "]";
    }
}

void TransProtocol::CheckSendRetrans()
{
    const int MaxRetransPack = 2;   // 一次最多重传的包数.

    int transPackCount = 0;
    UDPPackCache::iterator it = m_sendCache.begin();
    while (it!=m_sendCache.end()) {

        if( this->NextDataTrans() )
        {
            this->NextDataTrans()->InputDataDown( this, it.value() );

            qDebug() << "Retrans data:[" << it.value() << "]";

            transPackCount++;

            if(transPackCount>=MaxRetransPack)
            {
                break;
            }
        }

        ++it;
    }
}

bool TransProtocol::ReadHead(const QByteArray& data, UDPPackHead& head)
{
    if( data.size() < TransProtocolHeadSize )
    {
        return false;
    }

    int pos = 0;
    head.cmd = data.at(pos);
    pos += 1;

    head.sn = qFromBigEndian( *(quint32*)(data.data()+pos) );
    pos += 4;



    return true;
}

bool TransProtocol::WriteHead(QByteArray& data, const UDPPackHead& head)
{
    data.push_back( head.cmd );

    quint32 nlSN = qToBigEndian( head.sn );

    QByteArray bSN = QByteArray::fromRawData( (char*)&nlSN, sizeof( quint32 ) );

    data.push_back( bSN );

    return true;
}

void TransProtocol::CachePack(const QByteArray &data)
{
    UDPPackHead head;
    if( ReadHead( data, head ) )
    {
        if( head.sn > m_curConfirmedRecvPackSN
                || ( m_curConfirmedRecvPackSN == 0 && head.sn == m_curConfirmedRecvPackSN ) ) // 对第一个包特殊处理.
        {
            m_recvCache[ head.sn ] = data;
            qDebug() << "TransProtocol cache data:[" << head.sn << "]";
        }
        else
        {
            qDebug() << "TransProtocol Old sn not cache.[" << head.sn << "]";
        }
    }

}

void TransProtocol::ConfirmPack(QByteArrayList& dataOutDown)
{
    if( m_recvCache.empty() )
    {
        return;
    }

    quint32 sn = m_curConfirmedRecvPackSN;
    while (m_recvCache.find(sn+1) != m_recvCache.end() ) {
        ++sn;
    }

    m_curConfirmedRecvPackSN = sn;

    // 发送确认包.
    QByteArray udpConfirmPack;
    UDPPackHead head;
    head.cmd = CMD_CONFIRM;
    head.sn = sn;

    WriteHead( udpConfirmPack, head );

    dataOutDown.push_back( udpConfirmPack );
    qDebug() << "TransProtocol confirm recv pack:[" << sn << "]";
    //this->InputDataForward( this, udpConfirmPack );
}

bool TransProtocol::ProcessData(QByteArrayList& dataOutUp )
{
    UDPPackCache::iterator it= m_recvCache.begin();
    while(  it!=m_recvCache.end() )
    {
        const QByteArray& data = it.value();
        UDPPackHead head;
        ReadHead( data, head );
        if( head.sn <= m_curConfirmedRecvPackSN )
        {
            // 可以处理.
            dataOutUp.push_back( data.mid( TransProtocolHeadSize ) );

            it = m_recvCache.erase( it );

            qDebug() << "TransProtocol process data:[" << head.sn << "]";
        }
        else
        {
            // Out of range.
            break;
        }
    }
    return true;
}
