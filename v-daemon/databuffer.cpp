#include "databuffer.h"

int DataBuffer::s_timerInterval = 80;
int DataBuffer::s_transDataDownSuccessCount = 0;

DataBuffer::DataBuffer(QObject *parent) : IDataTrans(parent)
 // , m_timerInterval( 50 )
 // , m_transDataDownSuccessCount( 0 )
{
    m_timer.setInterval( s_timerInterval );

    connect( &m_timer, SIGNAL(timeout()), this, SLOT(OnTimer()) );
    m_timer.start();
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
            s_transDataDownSuccessCount++;

            if( s_transDataDownSuccessCount > 1000 )
            {
                s_transDataDownSuccessCount = 0;
                m_timer.setInterval( -- s_timerInterval );
            }
        }
        else
        {
            // fail.
            if( s_timerInterval < 100 )
            {
                m_timer.setInterval( ++s_timerInterval );
            }
            s_transDataDownSuccessCount = 0;
        }

    }
}
