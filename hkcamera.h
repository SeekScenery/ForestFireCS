#ifndef HKCAMERA_H
#define HKCAMERA_H

#include "HCNetSDK.h"
#include "hkmove.h"
#include "alg.h"
#include <string>
#include <vector>
#include <QDebug>
#include <QThread>
using namespace cv;
using namespace std;


class HKCamera: public QObject
{
    Q_OBJECT
public:
    explicit HKCamera(int index, HWND handle, QObject *parent=nullptr);
    explicit HKCamera(int index, HWND handle, int DomeDevice, QObject *parent=nullptr);
    ~HKCamera();

    // 设置相同的相机登录密码，IP 最后应该数不相同
    // 连接相机
    bool ConnectCamera();
    bool CloseCamera();
    bool RealPlay();
    Mat GrabImage();

    //报警回调函数
     bool SetDVRmessageCallBack();
    bool CALLBACK MessageCallBack(LONG lCommand , void *pUser);

public slots:
    void ForestDomePatrol();
    void test();


public:
    HKMove *DomeCameraMove;
    HKMove *Rechengxiang;

private:
    // 设备ID
    LONG UserId;
    // 视频流句柄
    LONG RealPlayHandle;
    int DeviceClass = 0;
    // 图片位置信息
//    PTZPOS GrabPosition;
    PTZInfo GrabPosition;
    int IPIndex;
    // 返回状态
    bool ReturnState;
    // 界面显示句柄
    HWND Handle;
    Alg *algorithm;
    // 图片大小 可设为结构体 or
    WORD width;
    WORD height;
    // 异常图片标号
    int anomlay_mark = 0;
};

#endif // HKCAMERA_H
