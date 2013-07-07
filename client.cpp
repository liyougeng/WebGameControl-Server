#include "client.h"

client::client(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);
    host.setAddress("192.168.0.100");
    port = 8080;
    connect(socket,
            SIGNAL(connected()),
            this,
            SLOT(on_connected()));
    connect(socket,
            SIGNAL(disconnected()),
            this,
            SLOT(on_disconnect()));
}

void client::on_connected()
{
    qDebug()<<"client:connected.";
    char    buffer[1024]="hello ,i am client,now connected\n";
    connect(socket,
            SIGNAL(readyRead()),
            this,
            SLOT(on_readyread()));
    socket->write(buffer);
    socket->flush();
}

void client::on_disconnect()
{
    qDebug()<<"client:disconnect..";
    disconnect(socket,
               SIGNAL(readyRead()),
               this,
               SLOT(on_readyread()));
    disconnect(socket,
               SIGNAL(connected()),
               this,
               SLOT(on_connected()));
    //socket->close();
    socket->deleteLater();
}

void client::on_readyread(){
    QByteArray data;
    while(socket->canReadLine()){
        data.append(socket->readLine());
    }
    qDebug()<<"client:"<<QString(data);
    bufferedMessage.append(data);
    emit messageArrived(data);
}
void client::connectToServer(){
    if (socket == NULL) return;
    socket->connectToHost(host,port);
    qDebug()<<"client connecting to host.";
}

void client::setHostInfo(QHostAddress host, uint port){
    this->host = host;
    this->port = port;
}

client::~client(){
    if(socket != NULL)
        delete socket;
}

void client::sendToServer(QByteArray data){

    if(socket->isOpen()&&socket->isWritable()){
        socket->write(data);
        socket->flush();
    }else{
        qDebug()<<"client:sorry,socket not writable or not open!";
    }

}

QByteArray client::getBufferedMessage()
{
    return bufferedMessage;
}
