#ifndef SERVER_H
#define SERVER_H
#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QDebug>
#include "logtofile.h"

#include "nativehandle.h"
class server : public QObject
{
    Q_OBJECT
public:
    explicit server(QObject *parent = 0);
    ~server();
    void listen();
    uint State();
    void sendToClient(QByteArray data);
signals:
    void ShouldReplay2Client(QByteArray data);
public slots:
    void on_newConnection();
    void on_readyRead();
    void on_disconnect();
    void on_PrepareReply(QByteArray data);
private:
    QTcpServer * Server ;
    QVector<QTcpSocket *> sockets;
    QTcpSocket * socket;
    QHostAddress host;
    quint16     port;

    LogToFile   *logger;
};

#endif // SERVER_H
