#ifndef DATAHASHCHECK_H
#define DATAHASHCHECK_H

/**
*   对数据完整性进行校验.
*   目标:
*       1) 确保数据没有被篡改.
*       2) 确保数据没有丢失, 没有被插入.
*   包头结构:
*       Hash 32byte | Data
*       其中 Hash是对上一包的Hash + 本包data 的Hash.
*   +by lzx@2017-06-03
*/

#include "idatatrans.h"


class DataHashCheck : public IDataTrans
{
    Q_OBJECT
public:
    explicit DataHashCheck(QObject *parent = 0);

    static const int SHA_256_LEN = 32;

    // 处理数据.
    virtual bool TransDataDown(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack);
    virtual bool TransDataUp(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack);
signals:

public slots:

private:
    QByteArray m_lastSentPackHash;  // 最近一包的Hash, 用于计算本包的Hash.
    QByteArray m_lastRecvPackHash;  // 最近收到一包的Hash, 用于校验本包的数据.
};

#endif // DATAHASHCHECK_H
