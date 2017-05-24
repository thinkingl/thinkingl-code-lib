#include "datasplit.h"
#include <QDebug>

DataSplit::DataSplit(QObject *parent) : IDataTrans(parent)
{

}

bool DataSplit::TransDataForward(const QByteArray &dataIn, QByteArrayList &dataOut)
{
    dataOut.clear();
    // 将数据拆成小包.( <1400byte )
    // 格式: 1byte 总包数 | 1byte 当前包序号 | 数据
    const int MaxDataLen = 1400-1-1;
    int totalNum = ( dataIn.size() + MaxDataLen - 1 ) / MaxDataLen;

    for( int i=0; i<totalNum; ++i )
    {
        QByteArray subData = dataIn.mid( i*MaxDataLen, MaxDataLen );
        subData.push_front( i );    // 当前包序号
        subData.push_front( totalNum );
        dataOut.push_back( subData );
    }
}

bool DataSplit::TransDataBack(const QByteArray &dataIn, QByteArrayList &dataOut)
{
    // 将小包数据合并.
    dataOut.clear();

    if( dataIn.size() > 2 )
    {
        int totalNum = dataIn.at(0);
        int curIndex = dataIn.at(1);
        int nextIndexInCache = m_spilitedDataCache.size();
        if( curIndex == nextIndexInCache )
        {
            m_spilitedDataCache.push_back( dataIn );
        }
        else
        {
            qDebug() << "DataSplit::TransDataBack wrong index!";
            return false;
        }
        if( totalNum == curIndex+1 )
        {
            m_spilitedDataCache.swap( dataOut );
        }
    }
    else
    {
        qDebug() << "DataSplit::TransDataBack invalid data len!";
        return false;
    }
}
