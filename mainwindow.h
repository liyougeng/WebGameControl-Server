#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QTimer>
#include "client.h"
#include "server.h"
#include "nativehandle.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // Note that this will only have an effect on Symbian and Fremantle.
    void setOrientation(ScreenOrientation orientation);

    void showExpanded();

private slots:
    void set_StatusBar(QString tip);
    void ResponseClient(QByteArray bytes);
private slots:
    void on_send_clicked();
    void updateServerMessage(QByteArray data);
    void sendClientMsg();
    void on_sld_row_valueChanged(int value);

    void on_sld_col_valueChanged(int value);

    void on_btn_Shift_clicked();

    void on_btn_Action_clicked();

    void on_btn_resetWindow_clicked();

private:
    Ui::MainWindow *ui;
    client  *instClient;
    server * instServer;
    NativeHandle * instHandler;
    QTimer * timer;
    uint     time;

   // NativeHandle instHandler;
    bool         bInitedR;
    bool         bInitC;
    uint         prevR;
    uint         prevC;
};

#endif // MAINWINDOW_H
