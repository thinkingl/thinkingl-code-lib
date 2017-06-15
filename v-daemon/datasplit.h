#ifndef DATASPLIT_H
#define DATASPLIT_H

#include "idatatrans.h"
#include <QMap>
#include <QTimer>

/**
*   数据的拆分和组合, 为了适合UDP的包大小用.
*   +by lzx@2017-05-25
*/
// 格式: total num 1byte | cur sn 1byte | data
static const int PACK_MAX_LEN = 1000;

class DataSplit : public IDataTrans
{
    Q_OBJECT
public:
    explicit DataSplit(QObject *parent = 0);

    // 处理数据.
    virtual bool TransDataDown(const QByteArray& dataIn, QByteArrayList& dataOutDown, QByteArrayList& dataOutUp);
    virtual bool TransDataUp(const QByteArray& dataIn, QByteArrayList &dataOutDown, QByteArrayList &dataOutUp);

signals:

private:
    class DataSplitHead
    {
    public:
        quint8 totalNum;    // total splited pack num. >0
        quint8 curSn;       // cur pack sn. 0 -> totalNum-1
    };

    bool ReadHead( const QByteArray& dataIn, DataSplitHead& head );
    bool WriteHead( QByteArray& dataOut, const DataSplitHead& head );

private:
    QByteArray m_recvCache;

};

#endif // DATASPLIT_H
