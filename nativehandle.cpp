#include "nativehandle.h"

NativeHandle::NativeHandle(QObject *parent)
{
    initWindowInfoStruct(self);
    initWindowInfoStruct(prev);
    memset(keyState,RELEASED,128);
    resetPrevWindowInfo(self);
    timer = new QTimer(this);
}
NativeHandle::~NativeHandle()
{
    if(timer != NULL)
        delete timer;
}
int NativeHandle::resetPrevWindowInfo(windowInfo &wi)
{
    wi.id = GetForegroundWindow();
    int ret= GetWindowTextA(wi.id,wi.caption,200);
    qDebug()<<wi.caption;
    return ret;
}
bool NativeHandle::SendHandleMessage(uint message, WPARAM w, LPARAM l=1)
{
    //SendMessageA(prev.id,message,w,l);
    return PostMessageA(prev.id,message,w,l);
}

void NativeHandle::ControlDirection(int way)
{
    switch (way) {
    case VK_UP:
        Forward();
        break;
    case VK_DOWN:
        Backward();
        break;
    case VK_LEFT:
        TurnLef();
        break;
    case VK_RIGHT:
        TurnRight();
         break;
    default:;
    }
}
void NativeHandle::StopDirection(bool bRowOrCol)
{
    if(bRowOrCol == ROW){
        if(keyState[VK_LEFT]==PRESSED)
            SendHandleMessage(WM_KEYUP,VK_LEFT,1);
        if(keyState[VK_RIGHT]==PRESSED)
            SendHandleMessage(WM_KEYUP,VK_RIGHT,1);
        keyState[VK_LEFT]=keyState[VK_RIGHT]=RELEASED;
    }else{
        if(keyState[VK_UP]==PRESSED)
            SendHandleMessage(WM_KEYUP,VK_UP,1);
        if(keyState[VK_DOWN]==PRESSED)
            SendHandleMessage(WM_KEYUP,VK_DOWN,1);
        keyState[VK_UP]=keyState[VK_DOWN]=RELEASED;
    }
}
void NativeHandle::Forward()
{int way = VK_UP;
    if(!keyState[VK_DOWN]/* VK_DOWN == FALSE PRESSED*/){
        SendHandleMessage(WM_KEYUP,VK_DOWN,1);
        keyState[VK_DOWN] = RELEASED;
    }
    if(keyState[way] == RELEASED){
        SendHandleMessage(WM_KEYDOWN,way,1);
        keyState[way] = PRESSED;
    }
}
void NativeHandle::Backward()
{int way = VK_DOWN;
    if(!keyState[VK_UP]/* VK_UP == FALSE PRESSED*/){
        SendHandleMessage(WM_KEYUP,VK_UP,1);
        keyState[VK_UP] = RELEASED;
    }
    if(keyState[way] == RELEASED){
        SendHandleMessage(WM_KEYDOWN,way,1);
        keyState[way] = PRESSED;
    }
}
void NativeHandle::TurnLef()
{int way = VK_LEFT;
    if(!keyState[VK_RIGHT]/* VK_UP == FALSE PRESSED*/){
        SendHandleMessage(WM_KEYUP,VK_RIGHT,1);
        keyState[VK_RIGHT] = RELEASED;
    }
    if(keyState[way] == RELEASED){
        SendHandleMessage(WM_KEYDOWN,way,1);
        keyState[way] = PRESSED;
    }
}
void NativeHandle::TurnRight()
{int way = VK_RIGHT;
    if(!keyState[VK_LEFT]/* VK_UP == FALSE PRESSED*/){
        SendHandleMessage(WM_KEYUP,VK_LEFT,1);
        keyState[VK_LEFT] = RELEASED;
    }
    if(keyState[way] == RELEASED){
        SendHandleMessage(WM_KEYDOWN,way,1);
        keyState[way] = PRESSED;
    }
}
void NativeHandle::SeeBack()
{
    SendHandleMessage(WM_KEYDOWN,'A',1);
    Sleep(800);
    SendHandleMessage(WM_KEYUP,'A',1);
}
void NativeHandle::Action()
{
    SendHandleMessage(WM_KEYDOWN,'F',1);
    Sleep(800);
    SendHandleMessage(WM_KEYUP,'F',1);
}
void NativeHandle::Shift()
{
    SendHandleMessage(WM_KEYDOWN,'S',1);
    Sleep(800);
    SendHandleMessage(WM_KEYUP,'S',1);
}
void NativeHandle::getWindowStart()
{
    connect(timer,
            SIGNAL(timeout()),
            this,
            SLOT(getWindowStop()));
    timer->start(3000);
}
void NativeHandle::getWindowStop()
{
    initWindowInfoStruct(prev);
    resetPrevWindowInfo(prev);
    if(prev.id != self.id)
    {
        timer->stop();
        disconnect(timer,
                   SIGNAL(timeout()),
                   this,
                   SLOT(getWindowStop()));

        QString title(prev.caption);
        emit WindowGotten(title);
    }

}

QString NativeHandle::debugState()
{
    QString ret;
    for(int I=VK_LEFT;I<=VK_DOWN;I++){
        if(keyState[I] == RELEASED)
            ret.append("Key Code:"+QString::number(I)+";State:Released\n");
        else
            ret.append("Key Code:"+QString::number(I)+";State:Pressed\n");
    }
    qDebug()<<ret;
    return ret;
}
