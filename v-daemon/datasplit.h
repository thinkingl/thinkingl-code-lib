#ifndef DATASPLIT_H
#define DATASPLIT_H

#include "idatatrans.h"

/**
*   数据的拆分和组合, 为了适合UDP的包大小用.
*/

class DataSplit : public IDataTrans
{
    Q_OBJECT
public:
    explicit DataSplit(QObject *parent = 0);

    // 处理数据.
    virtual bool TransDataForward(const QByteArray& dataIn, QByteArrayList& dataOut);
    virtual bool TransDataBack(const QByteArray& dataIn, QByteArrayList& dataOut);
signals:

public slots:

private:
    QByteArrayList m_spilitedDataCache;
};

#endif // DATASPLIT_H
