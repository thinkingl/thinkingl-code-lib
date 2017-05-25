#include "datamerge.h"

DataMerge::DataMerge(QObject *parent)
    : DataSplit(parent)
{

}

bool DataMerge::TransDataForward(const QByteArray &dataIn, QByteArrayList &dataOut)
{
    return DataSplit::TransDataBack( dataIn, dataOut );
}

bool DataMerge::TransDataBack(const QByteArray &dataIn, QByteArrayList &dataOut)
{
    return DataSplit::TransDataForward( dataIn, dataOut );
}
