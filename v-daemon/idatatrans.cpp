#include "idatatrans.h"

IDataTrans::IDataTrans(QObject* parent)
    :QObject( parent )
    ,m_preItem( 0 )
    ,m_nextItem( 0 )
{
}

bool IDataTrans::InputDataForward(const QByteArray &data)
{
    QByteArrayList dataOut;
    bool bOk = this->TransDataForward(data, dataOut);
    if( m_nextItem && bOk && !dataOut.empty() )
    {
        foreach (const QByteArray& d, dataOut) {
            bOk &= m_nextItem->InputDataForward( d );
        }
        //bOk = m_nextItem->InputDataForward(dataOut);
    }
    return bOk;
}

bool IDataTrans::InputDataBack(const QByteArray &data)
{
    QByteArrayList dataOut;
    bool bOk = this->TransDataBack(data, dataOut);
    if( m_preItem && bOk && !dataOut.empty() )
    {
        foreach (const QByteArray& d, dataOut) {
            bOk &= m_preItem->InputDataBack( d );
        }
       // bOk = m_preItem->InputDataBack(dataOut);
    }
    return bOk;
}

bool IDataTrans::SetNextDataTrans( IDataTrans* pre, IDataTrans* next )
{
    m_preItem = pre;
    m_nextItem = next;
    return true;
}

