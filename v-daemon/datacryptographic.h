#ifndef DATACRYPTOGRAPHIC_H
#define DATACRYPTOGRAPHIC_H

#include "idatatrans.h"

/**
*   数据加密/解密
*   +by lzx@2017-06-15
*/

class DataCryptographic : public IDataTrans
{
    Q_OBJECT
public:
    explicit DataCryptographic(QObject *parent = 0);

    // 处理数据.
    virtual bool TransDataDown(const QByteArray& dataIn, QByteArrayList& dataOutDown, QByteArrayList& dataOutUp);
    virtual bool TransDataUp(const QByteArray& dataIn, QByteArrayList &dataOutDown, QByteArrayList &dataOutUp);

signals:

public slots:
};

#endif // DATACRYPTOGRAPHIC_H
