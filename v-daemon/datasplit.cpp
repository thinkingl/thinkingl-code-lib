#include "datasplit.h"
#include <QDebug>
#include <QCryptographicHash>
#include <QtEndian>



DataSplit::DataSplit(QObject *parent) : IDataTrans(parent)
{

}

bool DataSplit::TransDataDown(const QByteArray &dataIn, QByteArrayList& dataOutDown, QByteArrayList& dataOutUp)
{
    // 将数据拆成小包.( <1300byte )
    // 格式: total num 1byte | cur sn 1byte | data
    const int MaxDataLen = PACK_MAX_LEN;
    int totalNum = ( dataIn.size() + MaxDataLen - 1 ) / MaxDataLen;

    for( int i=0; i<totalNum; ++i )
    {
        DataSplitHead head;
        head.totalNum = totalNum;
        head.curSn = i;

        QByteArray pack;
        bool bOk = WriteHead( pack, head );
        if( !bOk )
        {
            qDebug() << "Write pack splited head fail!";
            return false;
        }

        pack.push_back( dataIn.mid( i*MaxDataLen, MaxDataLen ) );
        dataOutDown.push_back( pack );

        if( s_logRawData )
        {
            qDebug() << "Data split trans forward:[" << pack << "]";
        }
    }
    return true;
}

bool DataSplit::TransDataUp(const QByteArray &dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack)
{
    // 将小包数据合并.
    const int DataSplitHeadSize = 2;
    this->m_recvCache.push_back( dataIn.mid(DataSplitHeadSize) );

    DataSplitHead head;
    if( ReadHead( dataIn, head ) )
    {
        if( head.curSn == head.totalNum-1 )
        {
            dataOutBack.push_back( m_recvCache );

            if( s_logRawData )
            {
                qDebug() << "Data Spliter trans merged data up:[" << m_recvCache << "]";
            }

            m_recvCache.clear();
        }

        return true;
    }
    else
    {
        qDebug() << "Error! Data Spliter read head fail when trans data back!";
        return false;
    }
}

bool DataSplit::ReadHead(const QByteArray &dataIn, DataSplit::DataSplitHead &head)
{
    if( dataIn.size() >= 2 )
    {
        head.totalNum = dataIn.at(0);
        head.curSn = dataIn.at(1);
        return true;
    }
    else
    {
        qDebug() << "Error! Read DataSplit head fail! Data length is too short!";
        return false;
    }
}

bool DataSplit::WriteHead(QByteArray &dataOut, const DataSplit::DataSplitHead &head)
{
    dataOut.push_back( head.totalNum );
    dataOut.push_back( head.curSn );
    return true;
}
