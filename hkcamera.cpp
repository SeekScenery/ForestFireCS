#include "hkcamera.h"

HKCamera::HKCamera(int index, HWND handle, QObject *parent)
    :QObject(parent), IPIndex(index), Handle(handle)
{
    algorithm = new Alg();
}

HKCamera::HKCamera(int index, HWND handle, int DomeDevice, QObject *parent)
    :QObject(parent), DeviceClass(DomeDevice), IPIndex(index), Handle(handle)
{
    algorithm = new Alg();
    string model_path = "./weight/smoke.onnx";
    algorithm->YoloReadModel(model_path, false);
    qDebug() << "the object of Dome camera move create success : " << DeviceClass << endl;
}

HKCamera::~HKCamera()
{
    delete algorithm;
    delete DomeCameraMove;
}


bool HKCamera::ConnectCamera()
{
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {};
    // 这个变量 是否 设为成员变量
    NET_DVR_DEVICEINFO_V40 struDeviceInfo = {};
    std::string IPStr = "192.168.1.6" + std::to_string(IPIndex);
    // 将 string to char*   IPStr.c_str()   const char* str = IPStr.c_str or IPStr.data + '\0'
    strcpy(struLoginInfo.sDeviceAddress, IPStr.c_str());  //设备 IP 地址
    strcpy(struLoginInfo.sUserName,"admin");        //设备登录用户名
    strcpy(struLoginInfo.sPassword,"changsha123");        //设备登录密码
    struLoginInfo.wPort = 8000;
    struLoginInfo.bUseAsynLogin = 0;             //登录，登录接口返回成功即登录成功
    UserId = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfo);
    if (UserId == -1)
    {
        qDebug()<<"NET_DVR_Login_V40 failed, error code: "<< NET_DVR_GetLastError() << endl;
        return false;
    }
    return true;
}

bool HKCamera::CloseCamera()
{
    ReturnState = NET_DVR_Logout(UserId);
    if (!ReturnState)
    {
        qDebug()<<"NET_DVR_Logout failed, error code: "<< NET_DVR_GetLastError() << endl;
        return false;
    }
    return true;
}

bool HKCamera::RealPlay()
{
    // 如果各相机参数不一样，可以单独写一个函数接口设置参数

    NET_DVR_PREVIEWINFO struPlayInfo;
    struPlayInfo = {};
    struPlayInfo.hPlayWnd = Handle;       //需要 SDK 解码时句柄设为有效值，仅取流不解码时可设为空
    struPlayInfo.lChannel = 1;          //预览通道号
    struPlayInfo.dwStreamType = 0;      //码流类型：0-主码流，1-子码流，2-三码流，3-虚拟码流，以此类推
    struPlayInfo.dwLinkMode = 7;        //0- TCP 方式，1- UDP 方式，2- 多播方式，3- RTP 方式，4-RTP/RTSP，5-RSTP/HTTP
    struPlayInfo.bBlocked = 1;         //0- 非阻塞取流，1- 阻塞取流
    struPlayInfo.bPassbackRecord = 0;

    RealPlayHandle = NET_DVR_RealPlay_V40( UserId, &struPlayInfo, nullptr, nullptr);

    if(RealPlayHandle <0)
    {
        qDebug() << "NET_DVR_RealPlay_V40 error;error number " <<NET_DVR_GetLastError();
        NET_DVR_Logout(UserId);
        return false;
    }
    if (DeviceClass != 0)
        DomeCameraMove = new HKMove(UserId, RealPlayHandle);
//    NET_DVR_STD_CONFIG StructConfig;
//    NET_DVR_GIS_INFO StructInfo;
//    StructConfig.lpOutBuffer = &StructInfo;

//    NET_DVR_GetSTDConfig(UserId,NET_DVR_GET_GISINFO, &StructConfig);
//    qDebug() << "HorizontalValue:" << StructInfo.fHorizontalValue << endl;
//    qDebug() << "VerticalValue:" << StructInfo.fVerticalValue << endl;

    NET_DVR_GIS_INFO struGISInfo;
    LONG m_lChannel = 1;
    NET_DVR_STD_CONFIG struCfg;
    struCfg.lpCondBuffer = &m_lChannel;
    struCfg.dwCondSize = sizeof(m_lChannel);
    struCfg.lpOutBuffer = &struGISInfo;
    struCfg.dwOutSize = sizeof(struGISInfo);
    char m_szStatusBuf[4096*4];
    memset(m_szStatusBuf, 0, 4096*4);
    struCfg.lpStatusBuffer = m_szStatusBuf;
    struCfg.dwStatusSize = 4096*4;
    if(!NET_DVR_GetSTDConfig(UserId, NET_DVR_GET_GISINFO, &struCfg))
   {
       qDebug() << "NET_DVR_GetSTDConfig error;error number " <<NET_DVR_GetLastError();
   }
        qDebug() << "HorizontalValue:" << struGISInfo.fHorizontalValue << endl;
        qDebug() << "VerticalValue:" << struGISInfo.fVerticalValue << endl;

//    ForestDomePatrol();

    return true;
}

Mat HKCamera::GrabImage()
{
    // 设置图片大小 和 质量
    NET_DVR_JPEGPARA JpegPara;
    JpegPara.wPicSize = 0xff;
    JpegPara.wPicQuality = 0;
    // PicSize 可设为成员变量 or if 判断 or 设置全局数组。。。
    width = 3840;
    height = 2160;
    DWORD PicSize = 3*3840*2160;
    char *PicBuffer = new char[PicSize];
    DWORD SizeReturned = 0;
    bool FlagError;
    FlagError = NET_DVR_CaptureJPEGPicture_NEW(UserId, 1, &JpegPara, PicBuffer, PicSize, &SizeReturned);
    if(!FlagError)
    {
        qDebug() << "NET_DVR_CaptureJPEGPicture_NEW, the error number: " << NET_DVR_GetLastError();
    }

    // char* buffer 转 Mat 可以再尝试改善
    vector<char> data(SizeReturned);
    for (DWORD i=0; i < SizeReturned; i++)
        data[i] = PicBuffer[i];
    Mat img = imdecode(Mat(data), 1);

//    Mat img = Mat(2160, 3840, CV_8UC3, (unsigned char*)PicBuffer);
//    namedWindow("Display window", WINDOW_NORMAL);
//    imshow("Display window", img);
//    imwrite("BufferToMat.jpg", img);
//    waitKey(0);

    delete[] PicBuffer;
    return img;
}

void HKCamera::ForestDomePatrol()
{
    // 先用 while 来 控制实时抓图
    bool GrapImageFlag = true;
    qDebug()<<"Forest DomePatrol"<< endl;
    while(GrapImageFlag)
    {
        // 设置抓取间隙 1.用时间 或 2.角度间隔
        // 位置判断  焦距技术 角度间隔 或 时间间隔
        PTZPOS anomaly_;
        QThread::msleep(1000);

        Mat img = GrabImage();

//        imwrite("C:\\Users\\18387\\Desktop\\image_pic\\img22"+to_string(anomlay_mark) + ".jpg", img);
        // 获取云台位置信息 和 焦距信息
        GrabPosition = this->DomeCameraMove->Get_PTZPOS();
        vector<YoloOutput> YoloResult;
        if(algorithm->YoloDetect(img, YoloResult))
        {
            anomlay_mark += 1;

            // 可保存异常图片

            for (auto result_elem:YoloResult)
            {

                algorithm->YoloDrawAnomaly(img, result_elem);
                anomaly_ = algorithm->AnomalyLocalization(GrabPosition, width, height,result_elem.box);
                string img_name = "H_" + to_string(anomaly_.PanPos) + "V_" + to_string(anomaly_.TiltPos);
                imwrite("C:\\Users\\18387\\Desktop\\image_pic\\" + img_name + to_string(anomlay_mark) + ".jpg", img);
                qDebug() << "anomaly position:" << anomaly_.PanPos << " " << anomaly_.TiltPos << endl;
                algorithm->PushInformation(anomaly_, result_elem.id, result_elem.confidence, anomlay_mark);
            }

        }
    }

}

void HKCamera::test()
{
    qDebug() << "enter test" << endl;
}
