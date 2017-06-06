#include "idatatrans.h"
#include <QDebug>

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
        m_nextItem->SetPreDataTrans(0);
        m_nextItem->deleteLater();
    }

    if( m_preItem )
    {
        m_preItem->SetNextDataTrans(0);
    }
}

bool IDataTrans::InputDataDown(IDataTrans* pPreItem, const QByteArray &data)
{
    QByteArrayList dataOutForward, dataOutBack;
    bool bOk = this->TransDataDown(data, dataOutForward, dataOutBack);
    if( bOk )
    {        
        foreach (const QByteArray& d, dataOutForward) {
            if( m_nextItem )
            {
                bOk &= m_nextItem->InputDataDown( this, d );
            }
            else
            {
                qDebug() << "Next item is null! can't trans data forward!!!";
            }
        }

        foreach (const QByteArray& d, dataOutBack) {
            if( m_preItem )
            {
                bOk &= m_preItem->InputDataUp( this, d );
            }
            else
            {
                qDebug() << "Pre item is null! can't trans data back!";
            }
        }
    }
    return bOk;
}

bool IDataTrans::InputDataUp(IDataTrans* pNextItem, const QByteArray &data)
{
    QByteArrayList dataOutForward, dataOutBack;
    bool bOk = this->TransDataUp(data, dataOutForward, dataOutBack);
    if( bOk )
    {
        foreach (const QByteArray& d, dataOutForward) {
            if( m_nextItem )
            {
                bOk &= m_nextItem->InputDataDown( this, d );
            }
            else
            {
                qDebug() << "Next item is null! can't trans data forward!!!";
            }
        }

        foreach (const QByteArray& d, dataOutBack) {
            if( m_preItem )
            {
                bOk &= m_preItem->InputDataUp( this, d );
            }
            else
            {
                qDebug() << "Pre item is null! can't trans data back!";
            }
        }
    }
    return bOk;
}

bool IDataTrans::SetNextDataTrans( IDataTrans* pre, IDataTrans* next )
{
    m_preItem = pre;
    m_nextItem = next;
    return true;
}

void IDataTrans::CloseUp()
{
    if( m_preItem )
    {
        m_preItem->CloseUp();
    }
    else
    {
        this->deleteLater();
    }
}

IDataTrans *IDataTrans::NextDataTrans() const
{
    return m_nextItem;
}

