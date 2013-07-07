#ifndef NATIVEHANDLE_H
#define NATIVEHANDLE_H
#include <QObject>
#include <QTimer>
#include <QString>
#include <QDebug>
#include <windows.h>
#include <winuser.h>
typedef struct window{
    HWND id;
    char   caption[200];
    int	  captionSize;
}windowInfo;
#define RELEASED    true
#define PRESSED     false
#define ROW         false
class NativeHandle :public QObject
{
    Q_OBJECT
public:
    explicit NativeHandle(QObject *parent = 0);
    ~NativeHandle();

    void Shift();
    void SeeBack();
    void Action();
    void ControlDirection(int way = VK_UP);
    void StopDirection(bool bRowOrCol =ROW);
    void getWindowStart();
    QString debugState();
signals:
    void WindowGotten(QString wTitle);
private slots:
    void getWindowStop();
private:

private:
    int resetPrevWindowInfo(windowInfo & wi);
    bool SendHandleMessage(uint message,WPARAM r,LPARAM l);
    void initWindowInfoStruct(windowInfo &self)
    {
        self.id =NULL;
        self.id= NULL;
        memset(self.caption,0,200);
        self.captionSize = 200;
    }
private:
    QTimer     *timer;
    windowInfo  tmp;
    windowInfo self;
    windowInfo prev;
    //CString    CSWindowTitle;
    bool       keyState[128];
    /////
    bool ClickKey(char key);
    bool PressKey(char key);
    bool ReleaseKey(char key);
    void Forward();
    void Backward();
    void TurnLef();
    void TurnRight();
    void ControlFunction();
    void ControlExt();
};

#endif // NATIVEHANDLE_H
