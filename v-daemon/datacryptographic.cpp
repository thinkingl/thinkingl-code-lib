#include "datacryptographic.h"


DataCryptographic::DataCryptographic(QObject *parent) : IDataTrans(parent)
{

}

bool DataCryptographic::TransDataDown(const QByteArray &dataIn, QByteArrayList &dataOutDown, QByteArrayList &dataOutUp)
{
    // 加密
    QByteArray dataOut;
    foreach (char d, dataIn) {
        d += dataIn.size();
        dataOut.push_back( d );
    }
    dataOutDown.push_back( dataOut );
    return true;
}

bool DataCryptographic::TransDataUp(const QByteArray &dataIn, QByteArrayList &dataOutDown, QByteArrayList &dataOutUp)
{
    // 解密.
    QByteArray dataOut;
    foreach (char d, dataIn) {
        d -= dataIn.size();
        dataOut.push_back( d );
    }
    dataOutUp.push_back( dataOut );
    return true;
}
