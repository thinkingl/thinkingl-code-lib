#include "datahashcheck.h"
#include <QCryptographicHash>
#include <QDebug>

DataHashCheck::DataHashCheck(QObject *parent) : IDataTrans(parent)
{
    // 默认填充0.
    m_lastSentPackHash.fill(0, SHA_256_LEN);
    m_lastRecvPackHash.fill(0, SHA_256_LEN );
}

DataHashCheck::~DataHashCheck()
{
    qDebug() << "DataHashCheck destruct!";
}

bool DataHashCheck::TransDataDown(const QByteArray &dataIn, QByteArrayList &dataOutForward, QByteArrayList &dataOutBack)
{
    QByteArray dataOut;
    dataOut.push_back( m_lastSentPackHash );
    dataOut.push_back( dataIn );

    m_lastSentPackHash = QCryptographicHash::hash( dataOut, QCryptographicHash::Sha256 );

    for( int i=0; i<SHA_256_LEN; ++i )
    {
        dataOut[i] = m_lastSentPackHash[i];
    }
    dataOutForward.push_back( dataOut );

    qDebug() << "Data hash check trans data forward:[" << dataOut << "]";

    return true;
}

bool DataHashCheck::TransDataUp(const QByteArray &dataIn, QByteArrayList &dataOutForward, QByteArrayList &dataOutBack)
{
    if( dataIn.size() < SHA_256_LEN )
    {
        qDebug() << "Data hash check fail! data is too short!!";
        return false;
    }

    QByteArray curHash = dataIn.mid( 0, SHA_256_LEN );
    QByteArray curData = dataIn.mid( SHA_256_LEN );

    QByteArray packCheck;
    packCheck.push_back( this->m_lastRecvPackHash );
    packCheck.push_back( curData );

    QByteArray hash = QCryptographicHash::hash( packCheck, QCryptographicHash::Sha256 );
    if( hash == curHash )
    {
        dataOutBack.push_back( curData );

        m_lastRecvPackHash = hash;

        qDebug() << "Data hash check ok, trans data back:[" << curData << "]";
        return true;
    }
    else
    {
        qDebug() << "Data hash check fail! data:[" << curData << "] cur hash:[" << curHash << "] Expected hash:[" << hash << "] last hash:[" << m_lastRecvPackHash << "]";

        this->CloseUp();

        return false;
    }
}
