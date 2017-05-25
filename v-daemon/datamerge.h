#ifndef UDPMERGE_H
#define UDPMERGE_H

#include "datasplit.h"
#include "udpserver.h"

class DataMerge : public DataSplit
{
    Q_OBJECT
public:
    explicit DataMerge(QObject *parent);

    // 处理数据.
    virtual bool TransDataForward(const QByteArray& dataIn, QByteArrayList& dataOut);
    virtual bool TransDataBack(const QByteArray& dataIn, QByteArrayList& dataOut);

signals:

public slots:
private:

};

#endif // UDPMERGE_H
