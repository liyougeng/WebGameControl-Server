#include "server.h"

server::server(QObject *parent) :
    QObject(parent)
{
    logger = new LogToFile("G:/sensor.txt");
    if(logger->Err()>0)
    {
        qDebug()<<"file err occured!";
        delete logger;
        logger = NULL;
    }
    Server = new QTcpServer(this);
    connect(Server,
            SIGNAL(newConnection()),
            this,
            SLOT(on_newConnection()));
    connect(this,
            SIGNAL(ShouldReplay2Client(QByteArray)),
            this,
            SLOT(on_PrepareReply(QByteArray)));
    host.setAddress("192.168.0.100");
    port = 8080;
}
void server::listen()
{
    Server->listen(host,port);
}

void server::on_newConnection()
{
    socket = Server->nextPendingConnection();
    if(socket->state() == QTcpSocket::ConnectedState)
    {
        qDebug()<<"server:new connection established!\n";
    }
    socket->write("welcome to server.");socket->flush();
    connect(socket,
            SIGNAL(disconnected()),
            this,
            SLOT(on_disconnect()));
    connect(socket,
            SIGNAL(readyRead()),
            this,
            SLOT(on_readyRead()));
    //sockets.append(socket);
}

void server::on_readyRead()
{
    while(socket->canReadLine())
    {
        QByteArray bytes = socket->readLine();
        /*if(strcmp(bytes.constData(),"quit\n") ==0)
        {
            socket->disconnectFromHost();
            break;
        }*/
        //qDebug()<<"server:"<<bytes.constData();
        emit ShouldReplay2Client(bytes);
        logger->write(bytes.append("\n"));


    }
}
void server::on_PrepareReply(QByteArray data)
{
    sendToClient(data);
}
void server::on_disconnect()
{
    qDebug()<<"server:Disconnect..";
    disconnect(socket,SIGNAL(disconnected()));
    disconnect(socket,SIGNAL(readyRead()));
    socket->deleteLater();
}

void server::sendToClient(QByteArray data)
{
    if(socket->isWritable())
    {
        data.append("\n");
        socket->write(data);
        socket->flush();
    }
}
uint server::State()
{
    if (socket == NULL)
        return 0;
    if (!socket->isValid())
        return 1;
    if (socket->isOpen())
        return 2;
    return 3;
}
server::~server()
{
    if (socket == NULL && Server == NULL)
        return;
    if (socket->isOpen())
    {
        socket->close();
        socket->deleteLater();
    }
    if ( Server != NULL)
        Server->deleteLater();
    if(logger!=NULL){
        delete logger;
        logger = NULL;
    }
}
