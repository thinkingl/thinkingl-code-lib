#ifndef IDATATRANS_H
#define IDATATRANS_H

/**
*   数据传输基类.
*   +by lzx@2017-05-23
*/
#include <QObject>
#include <QByteArray>

class IDataTrans : public QObject
{
    Q_OBJECT
public:
    IDataTrans(QObject* parent);

    // 传入数据.
    virtual bool InputDataForward(const QByteArray& data);
    virtual bool InputDataBack(const QByteArray& data);

    // 处理数据.
    virtual bool TransDataForward(const QByteArray& dataIn, QByteArrayList& dataOut) = 0;
    virtual bool TransDataBack(const QByteArray& dataIn, QByteArrayList& dataOut) = 0;

    // 设置下一环节数据处理对象.
    bool SetNextDataTrans( IDataTrans* pre, IDataTrans* next );

protected:
    IDataTrans* NextDataTrans() const;
private:
    IDataTrans* m_preItem;
    IDataTrans* m_nextItem;
};



#endif // IDATATRANS_H
