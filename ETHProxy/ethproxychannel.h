#ifndef ETHPROXYCHANNEL_H
#define ETHPROXYCHANNEL_H

#include <QObject>

class ETHProxyChannel : public QObject
{
    Q_OBJECT
public:
    explicit ETHProxyChannel(QObject *parent = 0);

signals:

public slots:
};

#endif // ETHPROXYCHANNEL_H