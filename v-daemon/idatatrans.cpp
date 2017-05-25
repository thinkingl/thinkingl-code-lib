#include "idatatrans.h"

IDataTrans::IDataTrans(QObject* parent)
    :QObject( parent )
    ,m_preItem( 0 )
    ,m_nextItem( 0 )
{
}

IDataTrans::~IDataTrans()
{
    if( m_nextItem )
    {
        m_nextItem->deleteLater();
    }
}

bool IDataTrans::InputDataForward(IDataTrans* pPreItem, const QByteArray &data)
{
    QByteArrayList dataOut;
    bool bOk = this->TransDataForward(data, dataOut);
    if( m_nextItem && bOk && !dataOut.empty() )
    {
        foreach (const QByteArray& d, dataOut) {
            bOk &= m_nextItem->InputDataForward( this, d );
        }
        //bOk = m_nextItem->InputDataForward(dataOut);
    }
    return bOk;
}

bool IDataTrans::InputDataBack(IDataTrans* pNextItem, const QByteArray &data)
{
    QByteArrayList dataOut;
    bool bOk = this->TransDataBack(data, dataOut);
    if( m_preItem && bOk && !dataOut.empty() )
    {
        foreach (const QByteArray& d, dataOut) {
            bOk &= m_preItem->InputDataBack( this, d );
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

void IDataTrans::CloseBack()
{
    if( m_preItem )
    {
        m_preItem->CloseBack();
    }
}

