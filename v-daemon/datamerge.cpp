#include "datamerge.h"

DataMerge::DataMerge(QObject *parent)
    : DataSplit(parent)
{

}

bool DataMerge::TransDataForward(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack)
{
    return DataSplit::TransDataBack( dataIn, dataOutBack, dataOutForward );
}

bool DataMerge::TransDataBack(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack)
{
    return DataSplit::TransDataForward( dataIn, dataOutForward, dataOutBack );
}
