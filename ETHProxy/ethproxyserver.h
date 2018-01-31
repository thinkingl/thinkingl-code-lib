#ifndef ETHPROXYSERVER_H
#define ETHPROXYSERVER_H

#include <QObject>

class ETHProxyServer : public QObject
{
    Q_OBJECT
public:
    explicit ETHProxyServer(QObject *parent = 0);

signals:

public slots:
};

#endif // ETHPROXYSERVER_H