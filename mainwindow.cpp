#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bInitedR =  bInitC = false;
    prevR    =  prevC  = 0;

    instHandler = new NativeHandle(this);
    connect(instHandler,
               SIGNAL(WindowGotten(QString)),
               this,
               SLOT(set_StatusBar(QString)));
    instServer = new server(this);
    instServer->listen();
    connect(instServer,
            SIGNAL(ShouldReplay2Client(QByteArray)),
            this,
            SLOT(ResponseClient(QByteArray)));
    instClient = new client(this);
    //instClient->connectToServer();
    connect(instClient,
            SIGNAL(messageArrived(QByteArray)),
            this,
            SLOT(updateServerMessage(QByteArray)));
    time    = 0;
    timer = new QTimer(this);
    timer->setInterval(2000);
    connect(timer,
            SIGNAL(timeout()),
            this,
            SLOT(sendClientMsg()));
    ui->statusBar->setToolTip("Ok.");
    //timer->start();
}
void MainWindow::ResponseClient(QByteArray bytes)
{
    QString value(bytes);
    uint now =0;
    //uint    now = abs(value.left(value.length()-1).toInt());
    QStringList info = value.split(",");
    if(info.count()<4) return;
    int     type = QString(info[0]).toInt();
    double  data[3]={0.0};
    data[0] = abs(50+QString(info[1]).toDouble());
    data[1] = abs(QString(info[2]).toDouble());
    data[2] = abs(QString(info[3]).toDouble());
    if(type == 1){
        int x = (int)(data[0]>100?100:data[0]);
        int y = (int)(data[1]>100?100:data[1]);
        if(!bInitedR){
            bInitedR=true;
            prevR = x;
        }
        if (x > 55 ){
            instHandler->ControlDirection(39);//prevR=now;
        }else if(x <45 ){
            instHandler->ControlDirection(37);//prevR=now;
        }else{
            instHandler->StopDirection(false);
        }
        if(!bInitC){
            bInitC = true;
            prevC = y;
        }
        if( y > 55){
            instHandler->ControlDirection(40);
        }else if( y < 45){
            instHandler->ControlDirection(38);
        }else{
            instHandler->StopDirection(true);
        }
    }
    return;
    if(now <10)
        return;
    if(!bInitedR){
        bInitedR=true;
        prevR = now;
    }
    qDebug()<<"Now:"<<now<<";prev:"<<prevR;
    if (now > 200){
        instHandler->ControlDirection(39);prevR=now;
    }else if(now <185 ){
        instHandler->ControlDirection(37);prevR=now;
    }else{
        instHandler->StopDirection(false);
    }
}
void MainWindow::set_StatusBar(QString tip)
{
    ui->lineString->setText(tip);
    ui->stateText->appendPlainText("New Window:"+tip);
    if(!this->isVisible())
        this->show();
}
MainWindow::~MainWindow()
{
    if(instClient !=NULL)
        delete instClient;
    if(instServer != NULL)
        delete instServer;
    if(timer != NULL)
        delete timer;
    if(instHandler != NULL){
        disconnect(instHandler,
                   SIGNAL(WindowGotten(QString)),
                   this,
                   SLOT(set_StatusBar(QString)));
        delete instHandler;
    }
    delete ui;
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}

void MainWindow::on_send_clicked()
{
    ui->lineString->setFocus();
    QString content = ui->lineString->text();
    if(content.isEmpty())
        return ;
    QByteArray data;
    data.append(content);data.append("\n");
    //connect instclient destroy signal!
    instClient->sendToServer(data);
    ui->serverContent->appendPlainText("client:\n"+content);
    ui->lineString->setText("");
}

void MainWindow::updateServerMessage(QByteArray data)
{
    qDebug()<<"widget: update.";
    ui->serverContent->appendPlainText("server:");
    ui->serverContent->appendPlainText(QString(data));
}
void MainWindow::sendClientMsg()
{
    if(instServer == NULL)
        return;
    instServer->sendToClient(QByteArray("this is from server."+time));
    //qDebug()<<"Timer emited!";
    time++;
}

void MainWindow::on_sld_row_valueChanged(int value)
{
    ui->row_value->setText(QString::number(value));
    if(value < 60 && value > 40){
        ui->stateText->appendPlainText(QString("Row reset middle"));
        instHandler->StopDirection(false);
    }else if(value <=40){
        ui->stateText->appendPlainText(QString("Row set   left"));
        instHandler->ControlDirection(37);
    }else{
        ui->stateText->appendPlainText(QString("Row set right"));
        instHandler->ControlDirection(39);
    }
    ui->KeyCodeState->appendPlainText(instHandler->debugState());
}

void MainWindow::on_sld_col_valueChanged(int value)
{
    ui->col_value->setText(QString::number(value));
    if(value < 60 && value > 40){
        ui->stateText->appendPlainText(QString("Col reset middle"));
        instHandler->StopDirection(true);
    }else if(value <=40){
        ui->stateText->appendPlainText(QString("Col set down"));
        instHandler->ControlDirection(40);
    }else{
        ui->stateText->appendPlainText(QString("Col set up"));
        instHandler->ControlDirection(38);
    }
    ui->KeyCodeState->appendPlainText(instHandler->debugState());
}

void MainWindow::on_btn_Shift_clicked()
{
    ui->stateText->appendPlainText(QString("Shift once"));
    instHandler->Shift();
}

void MainWindow::on_btn_Action_clicked()
{
    ui->stateText->appendPlainText(QString("Action once"));
    instHandler->Action();
}

void MainWindow::on_btn_resetWindow_clicked()
{
    this->hide();
    instHandler->getWindowStart();
}
