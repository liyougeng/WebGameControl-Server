#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QObject *parent = 0);
    ~client();
    void connectToServer();
    void setHostInfo(QHostAddress host = QHostAddress::LocalHost,uint port=8080);
    void sendToServer(QByteArray data);
    QByteArray getBufferedMessage();
signals:
    void messageArrived(QByteArray data);
public slots:
    void on_connected();
    void on_disconnect();
    void on_readyread();
private:
    QHostAddress host;
    quint16      port;
    QTcpSocket *socket;
    QByteArray bufferedMessage;
};

#endif // CLIENT_H
