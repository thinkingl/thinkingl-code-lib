#include "datasplit.h"
#include <QDebug>
#include <QCryptographicHash>
#include <QtEndian>



DataSplit::DataSplit(QObject *parent) : IDataTrans(parent)
  , m_curSentPackSN(0)
  , m_curConfirmedRecvPackSN(0)
  , m_curProcessedRecvPackSN(0)
{

}

bool DataSplit::TransDataForward(const QByteArray &dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack)
{
    // 将数据拆成小包.( <1400byte )
    // 格式: cmd | sn | sub total | sub sn | data ( 确认和关闭命令没有data )
    // 格式: cmd 1byte 命令(UDP_CMD) | sn 4byte 当前总包序号 | 4byte 当前数据段的包总数 | 4byte 当前数据段的包序号 | data 数据
    // 数据格式: SHA-256 32byte | 原始数据
    const int MaxDataLen = 1400-UDP_PACK_HEAD_LEN;
    int totalNum = ( dataIn.size() + MaxDataLen - 1 ) / MaxDataLen;

    QByteArray sentData = QCryptographicHash::hash( dataIn, QCryptographicHash::Sha256 );

    sentData.push_back( dataIn );

    for( int i=0; i<totalNum; ++i )
    {
        QByteArray udpPack;
        UDPPackHead head;
        head.cmd = CMD_DATA;

        head.sn = m_curSentPackSN;
        head.curDataPackNum = totalNum;
        head.curDataSN = i;

        WriteHead( head, udpPack );

        udpPack.push_back( sentData.mid( i*MaxDataLen, MaxDataLen ) );

        dataOutForward.push_back( udpPack );

        // cache
        m_sendCache[ m_curSentPackSN ] = udpPack;
        ++m_curSentPackSN;
    }
    return true;
}

bool DataSplit::TransDataBack(const QByteArray &dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack)
{
    // 将小包数据合并.

    UDPPackHead head;
    if( ReadHead( dataIn, head ) )
    {
        switch( head.cmd )
        {
        case CMD_DATA:
        {
            if( int( head.sn - m_curProcessedRecvPackSN ) > 0 )
            {
                m_recvCache[ head.sn ] = dataIn;
                this->ProcessData( dataOutBack );
            }
            this->ConfirmPack( dataOutForward );

        }
            break;
        case CMD_CONFIRM:
            break;
        case CMD_BYE:
            break;
        default:
            qDebug() << "Invalid cmd! [" << head.cmd << "]";
            break;
        }
    }
    else
    {
        qDebug() << "DataSplit::TransDataBack ReadHead fail!";
        return false;
    }
    return true;
}

bool DataSplit::ReadHead(const QByteArray& data, UDPPackHead& head)
{
    if( data.size() < UDP_PACK_HEAD_LEN )
    {
        return false;
    }

    int pos = 0;
    head.cmd = data.at(pos);
    pos += 1;

    head.sn = qFromBigEndian( *(quint32*)(data.data()+pos) );
    pos += 4;

    head.curDataPackNum = qFromBigEndian( *(quint32*)(data.data()+pos) );
    pos += 4;

    head.curDataSN = qFromBigEndian( *(quint32*)(data.data()+pos) );
    pos += 4;

    return true;
}

bool DataSplit::WriteHead(const DataSplit::UDPPackHead &head, QByteArray &data)
{
    data.push_back( head.cmd );

    quint32 nlSN = qToBigEndian( head.sn );
    data.push_back( nlSN );

    quint32 nlTotal = qToBigEndian( head.curDataPackNum );
    data.push_back( nlTotal );

    quint32 nlSubSN = qToBigEndian( head.curDataSN );
    data.push_back( nlSubSN );
    return true;
}

void DataSplit::ConfirmPack(QByteArrayList& dataOutForward)
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
    head.curDataPackNum = 1;
    head.curDataSN = 0;
    WriteHead( head, udpConfirmPack );

    dataOutForward.push_back( udpConfirmPack );
    //this->InputDataForward( this, udpConfirmPack );
}

bool DataSplit::ProcessData(QByteArrayList& dataOutBack)
{
    if( !m_recvCache.empty() )
    {
        UDPPackHead head;
        if( ReadHead( *m_recvCache.begin(), head ) )
        {
            quint32 beginSN = 0;
            quint32 endSN = 0;
            if( head.curDataSN == 0 )
            {
                // 是一个数据的起始.
                beginSN = head.sn;
                endSN = beginSN + head.curDataPackNum;
            }
            else
            {
                qDebug() << "Invalid first item in rcv cache!";
                return false;
            }

            int dataPackNum = (endSN - beginSN);
            if( dataPackNum > 0 )   // 考虑翻转的情况, 用 - , 不用 >.
            {
                if( (int)(endSN - m_curConfirmedRecvPackSN) > 0 )
                {
                    // 数据已经够了,可以处理了.
                    QByteArray data;
                    for( quint32 i=beginSN; i!=endSN; ++i)
                    {
                        data.push_back( m_recvCache[i].mid( UDP_PACK_HEAD_LEN ) );

                        m_recvCache.erase( m_recvCache.find(i) );
                    }

                    // 校验数据有效性.
                    QByteArray shaData = data.mid(0,SHA_256_LEN);
                    QByteArray originalData = data.mid( SHA_256_LEN );
                    if( shaData == QCryptographicHash::hash( originalData, QCryptographicHash::Sha256 ) )
                    {
                        // 数据ok.
                        dataOutBack.push_back( originalData );
                    }
                    else
                    {
                        qDebug() << "Check the hash fail!!";
                        return false;
                    }
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
