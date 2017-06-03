#include "transprotocol.h"
#include <QtEndian>
#include <QDebug>


TransProtocol::TransProtocol(QObject *parent) : IDataTrans(parent)
  , m_curSentPackSN(0)
  , m_curConfirmedRecvPackSN(0)
  , m_curProcessedRecvPackSN(0)
{

}

bool TransProtocol::TransDataDown(const QByteArray &dataIn, QByteArrayList &dataOutForward, QByteArrayList &dataOutBack)
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
    dataOutForward.push_back( dataOut );

    // 缓存.
    // to be continued.

    qDebug() << "TransProtocol trans data forward:[" << dataOut << "]";

    return true;
}

bool TransProtocol::TransDataUp(const QByteArray &dataIn, QByteArrayList &dataOutForward, QByteArrayList &dataOutBack)
{
    UDPPackHead head;
    if( this->ReadHead( dataIn, head ) )
    {
        switch( head.cmd )
        {
        case CMD_DATA:
        {
            // 读取数据.
            QByteArray data = dataIn.mid( TransProtocolHeadSize );

            // 输出.
            dataOutBack.push_back( data );
            qDebug() << "TransProtocol trans data back:[" << data << "]";
        }
            break;
        case CMD_CONFIRM:
            break;
        case CMD_HEART_BEAT:
            break;
        case CMD_BYE:
            break;
        default:
            break;
        }

        // 校验序号.
        // todo


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

void TransProtocol::ConfirmPack(QByteArrayList& dataOutForward)
{
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

    dataOutForward.push_back( udpConfirmPack );
    //this->InputDataForward( this, udpConfirmPack );
}

bool TransProtocol::ProcessData(QByteArrayList& dataOutBack)
{
    if( !m_recvCache.empty() )
    {
        UDPPackHead head;
        if( ReadHead( *m_recvCache.begin(), head ) )
        {
            quint32 beginSN = 0;
            quint32 endSN = 0;
//            if( head.curDataSN == 0 )
//            {
//                // 是一个数据的起始.
//                beginSN = head.sn;
//                endSN = beginSN + head.curDataPackNum;
//            }
//            else
//            {
//                qDebug() << "Invalid first item in rcv cache!";
//                return false;
//            }

            int dataPackNum = (endSN - beginSN);
            if( dataPackNum > 0 )   // 考虑翻转的情况, 用 - , 不用 >.
            {
                if( (int)(endSN - m_curConfirmedRecvPackSN) > 0 )
                {
                    // 数据已经够了,可以处理了.
                    QByteArray data;
                    for( quint32 i=beginSN; i!=endSN; ++i)
                    {
                        data.push_back( m_recvCache[i].mid( TransProtocolHeadSize ) );

                        m_recvCache.erase( m_recvCache.find(i) );
                    }

                    // 校验数据有效性.
//                    QByteArray shaData = data.mid(0,SHA_256_LEN);
//                    QByteArray originalData = data.mid( SHA_256_LEN );
//                    if( shaData == QCryptographicHash::hash( originalData, QCryptographicHash::Sha256 ) )
//                    {
//                        // 数据ok.
//                        dataOutBack.push_back( originalData );
//                    }
//                    else
//                    {
//                        qDebug() << "Check the hash fail!!";
//                        return false;
//                    }
                }
            }
            else
            {
                qDebug() << "end sn is not > begin sn.";
                return false;
            }
        }
        else
        {
            qDebug() << "Process data error ! Read head fail!!";
            return false;
        }
    }
    return true;
}
