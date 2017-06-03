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
    virtual ~IDataTrans();

    // 传入数据.
    virtual bool InputDataDown(IDataTrans* pPreItem, const QByteArray& data);
    virtual bool InputDataUp(IDataTrans* pNextItem, const QByteArray& data);

    // 处理数据.
    virtual bool TransDataDown(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack) = 0;
    virtual bool TransDataUp(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack) = 0;

    // 设置下一环节数据处理对象.
    bool SetNextDataTrans( IDataTrans* pre, IDataTrans* next );

    // 向前关闭.
    virtual void CloseBack();

protected:
    IDataTrans* NextDataTrans() const;
private:
    IDataTrans* m_preItem;
    IDataTrans* m_nextItem;
};



#endif // IDATATRANS_H
