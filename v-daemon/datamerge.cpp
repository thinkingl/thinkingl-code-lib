#include "datamerge.h"

DataMerge::DataMerge(QObject *parent)
    : DataSplit(parent)
{

}

bool DataMerge::TransDataDown(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack)
{
    return DataSplit::TransDataUp( dataIn, dataOutBack, dataOutForward );
}

bool DataMerge::TransDataUp(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack)
{
    return DataSplit::TransDataDown( dataIn, dataOutForward, dataOutBack );
}
