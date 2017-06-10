#include "databuffer.h"



DataBuffer::DataBuffer(QObject *parent) : IDataTrans(parent)
  , m_timerInterval( 50 )
  , m_transDataDownSuccessCount( 0 )
{
    m_timer.setInterval( m_timerInterval );

    connect( &m_timer, SIGNAL(timeout()), this, SLOT(OnTimer()) );
}

bool DataBuffer::TransDataDown(const QByteArray &dataIn, QByteArrayList &dataOutDown, QByteArrayList &dataOutUp)
{
    // 放入缓存, 等待定时发送.
    m_dataBuffer.push_back( dataIn );
    return true;
}

bool DataBuffer::TransDataUp(const QByteArray &dataIn, QByteArrayList& dataOutDown, QByteArrayList& dataOutUp)
{
    // 接收的数据透传.
    dataOutUp.push_back( dataIn );
    return true;
}

void DataBuffer::OnTimer()
{
    if( !m_dataBuffer.empty() )
    {
        QByteArray& data = m_dataBuffer.front();

        if( this->NextDataTrans()->InputDataDown( this, data ) )
        {
            m_dataBuffer.pop_front();
            m_transDataDownSuccessCount++;

            if( m_transDataDownSuccessCount > 100 )
            {
                m_transDataDownSuccessCount = 0;
                m_timer.setInterval( -- m_timerInterval );
            }
        }
        else
        {
            // fail.
            m_timer.setInterval( ++m_timerInterval );

            m_transDataDownSuccessCount = 0;
        }

    }
}
