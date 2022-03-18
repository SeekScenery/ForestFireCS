#include "hkmove.h"
#include <QDebug>
#include <time.h>
#include <unistd.h>



HKMove::HKMove(LONG userid, LONG realhandle)
    :UserId(userid), RealPlayHandle(realhandle)
{

}

//控制镜头移动
void HKMove::Up_start()
{
    bool control = NET_DVR_PTZControl(RealPlayHandle,TILT_UP,0);
    if(!control){
        qDebug()<<"NET_DVR_PTZControl failed, error code: %d\n"<< NET_DVR_GetLastError();
    }
    usleep(500000);                           //按键按一次,移动0.5s
}

void HKMove::Down_start()
{
    bool control = NET_DVR_PTZControl(RealPlayHandle,TILT_DOWN,0);
    if(!control){
        qDebug()<<"NET_DVR_PTZControl failed, error code: %d\n"<< NET_DVR_GetLastError();
    }
    usleep(500000);
}

void HKMove::Left_start()
{
    bool control = NET_DVR_PTZControl(RealPlayHandle,PAN_LEFT,0);
    if(!control){
        qDebug()<<"NET_DVR_PTZControl failed, error code: %d\n"<< NET_DVR_GetLastError();
    }
    usleep(500000);
}

void HKMove::Right_start()
{
    bool control = NET_DVR_PTZControl(RealPlayHandle,PAN_RIGHT,0);
    if(!control){
        qDebug()<<"NET_DVR_PTZControl failed, error code: %d\n"<< NET_DVR_GetLastError();
    }
    usleep(500000);
}

//镜头移动停止
void HKMove::Up_end()
{
    bool control = NET_DVR_PTZControl(RealPlayHandle,TILT_UP,1);
    if(!control){
        qDebug()<<"NET_DVR_PTZControl failed, error code: %d\n"<< NET_DVR_GetLastError();
    }
}

void HKMove::Down_end()
{
    bool control = NET_DVR_PTZControl(RealPlayHandle,TILT_DOWN,1);
    if(!control){
        qDebug()<<"NET_DVR_PTZControl failed, error code: %d\n"<< NET_DVR_GetLastError();
    }
}

void HKMove::Left_end()
{
    bool control = NET_DVR_PTZControl(RealPlayHandle,PAN_LEFT,1);
    if(!control){
        qDebug()<<"NET_DVR_PTZControl failed, error code: %d\n"<< NET_DVR_GetLastError();
    }
}

void HKMove::Right_end()
{
    bool control = NET_DVR_PTZControl(RealPlayHandle,PAN_RIGHT,1);
    if(!control){
        qDebug()<<"NET_DVR_PTZControl failed, error code: %d\n"<< NET_DVR_GetLastError();
    }
}

// 焦距调节

void HKMove::Zoom_IN_start()
{
    bool control = NET_DVR_PTZControl(RealPlayHandle,ZOOM_IN,0);
    if(!control){
        qDebug()<<"NET_DVR_PTZControl failed, error code: %d\n"<< NET_DVR_GetLastError();
    }
    usleep(500000);
}


void HKMove::Zoom_OUT_start()
{
    bool control = NET_DVR_PTZControl(RealPlayHandle,ZOOM_OUT,0);
    if(!control){
        qDebug()<<"NET_DVR_PTZControl failed, error code: %d\n"<< NET_DVR_GetLastError();
    }
    usleep(500000);
}

void HKMove::Zoom_IN_end()
{
    bool control = NET_DVR_PTZControl(RealPlayHandle,ZOOM_IN,1);
    if(!control){
        qDebug()<<"NET_DVR_PTZControl failed, error code: %d\n"<< NET_DVR_GetLastError();
    }
}
void HKMove::Zoom_OUT_end()
{
    bool control = NET_DVR_PTZControl(RealPlayHandle,ZOOM_OUT,0);
    if(!control){
        qDebug()<<"NET_DVR_PTZControl failed, error code: %d\n"<< NET_DVR_GetLastError();
    }
}


// PresetIndex 的范围是1 - 300
PTZInfo HKMove::Get_PTZPOS()
{
//    // 1.先获取参数数据 2.设置参数   这个设置参数是默认起始参数吗？ 还是当前参数？
//    bool flag;   // 获取参数 成果与否 的标志
//    DWORD dwReturnLen; // 返回数据的大小
//    NET_DVR_PTZPOS struParams = {};  // 承接数据的结构体 PTZ
//    //获取参数
//    flag = NET_DVR_GetDVRConfig(UserId, NET_DVR_GET_PTZPOS, 1,&struParams,sizeof(NET_DVR_PTZPOS), &dwReturnLen);
//    if (!flag){
//     qDebug()<<"NET_DVR_GET_PTZPOS GetDVRConfig error"<<NET_DVR_GetLastError();
//    }

//    // 自定义结构体
//    PTZPOS current_position = {};
//    current_position.PanPos = struParams.wPanPos; // 水平位置
//    current_position.TiltPos = struParams.wTiltPos; // 垂直位置
//    current_position.ZoomPos = struParams.wZoomPos; // 变倍大小
//    // 需进行转化
//    WORD P_H = struParams.wPanPos; // 水平位置
//    WORD P_V = struParams.wTiltPos; // 垂直位置
//    WORD P_Zoom = struParams.wZoomPos; // 变倍大小

//    // 打印参数
//    qDebug() << "P_H: " << P_H << endl;
//    qDebug() << "P_V: " << P_V << endl;
//    qDebug() << "P_Zoom: " << P_Zoom << endl;




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

    // 自定义结构体
    PTZInfo current_position = {};
    current_position.HorizontalAngle = struGISInfo.fHorizontalValue;
    current_position.VerticalAngle = struGISInfo.fVerticalValue;
    current_position.PanPos = struGISInfo.struPtzPos.fPanPos;
    current_position.TiltPos = struGISInfo.struPtzPos.fTiltPos;
    current_position.ZoomPos = struGISInfo.struPtzPos.fZoomPos;

    return current_position;

}

// 设置预置点
void HKMove::SetPreset(DWORD PresetIndex)
{
    bool flag;
    flag = NET_DVR_PTZPreset(RealPlayHandle, SET_PRESET, PresetIndex);
    if (!flag){
     qDebug()<<"NET_DVR_PTZPreset Set preset error"<<NET_DVR_GetLastError();
    }
}

// 删除指定预置点
void HKMove::ClearPreset(DWORD PresetIndex)
{
    bool flag;
    flag = NET_DVR_PTZPreset(RealPlayHandle, CLE_PRESET, PresetIndex);
    if (!flag){
     qDebug()<<"NET_DVR_PTZPreset clear preset error"<<NET_DVR_GetLastError();
    }
}

// 转至指定预置点
void HKMove::GotoPreset(DWORD PresetIndex)
{
    bool flag;
    flag = NET_DVR_PTZPreset(RealPlayHandle, GOTO_PRESET, PresetIndex);
    if (!flag){
     qDebug()<<"NET_DVR_PTZPreset goto preset error"<<NET_DVR_GetLastError();
    }
}

// 查看巡航路线中的预置点
void HKMove::GetPTZCruise(LONG CruiseRoute, LONG Channel=1)
{
    bool flag;
    NET_DVR_CRUISE_RET StruParams = {};
    flag = NET_DVR_GetPTZCruise(UserId, Channel, CruiseRoute, &StruParams);
    if (!flag){
     qDebug()<<"NET_DVR_GetPTZCruiset error"<<NET_DVR_GetLastError();
    }
    // 这个需调试需改
    qDebug() << "check the first cruise route point: " << StruParams.struCruisePoint[0].PresetNum << endl;
}

// 该函数还需好好测试
void HKMove::GetPresetName()
{

    bool flag;   // 获取参数 成果与否 的标志
    DWORD dwReturnLen; // 返回数据的大小
    NET_DVR_PRESET_NAME m_struPresetName[MAX_PRESET_NUM];  // 承接数据的结构体 PTZ
    memset(m_struPresetName, 0, sizeof(NET_DVR_PRESET_NAME) * MAX_PRESET_NUM/*256*/);
    //获取参数
    flag = NET_DVR_GetDVRConfig(UserId, NET_DVR_GET_PRESET_NAME, 1,m_struPresetName,sizeof(NET_DVR_PRESET_NAME)*MAX_PRESET_NUM, &dwReturnLen);
    if (!flag){
     qDebug()<<"NNET_DVR_GET_PRESET_NAME GetDVRConfig error"<<NET_DVR_GetLastError();
    }

    // 输出参数 测试一下
    qDebug()<<"print preset 1: "<< m_struPresetName[0].byName << endl;

}

// 将预置点加入指定巡航路线的巡航点
void HKMove::FillPresetSeq(BYTE CruiseRoute, BYTE CruisePoint, WORD PresetIndex)
{
    bool flag;
    flag = NET_DVR_PTZCruise(RealPlayHandle, FILL_PRE_SEQ, CruiseRoute, CruisePoint, PresetIndex);
    if (!flag){
     qDebug()<<"NET_DVR_PTZCruise FILL_PRE_SEQ error"<<NET_DVR_GetLastError();
    }
}

// 将预置点从指定巡航路线删除
void HKMove::ClealPresetSeq(BYTE CruiseRoute, BYTE CruisePoint, WORD PresetIndex)
{
    bool flag;
    flag = NET_DVR_PTZCruise(RealPlayHandle, CLE_PRE_SEQ, CruiseRoute, CruisePoint, PresetIndex);
    if (!flag){
     qDebug()<<"NET_DVR_PTZCruise CLE_PRE_SEQ error"<<NET_DVR_GetLastError();
    }
}

// 设置巡航路线中巡航点的时间
void HKMove::SetSeqDwell(BYTE CruiseRoute, BYTE CruisePoint, WORD PresetIndex)
{
    bool flag;
    flag = NET_DVR_PTZCruise(RealPlayHandle, SET_SEQ_DWELL, CruiseRoute, CruisePoint, PresetIndex);
    if (!flag){
     qDebug()<<"NET_DVR_PTZCruise SET_SEQ_DWELL error"<<NET_DVR_GetLastError();
    }
}

// 设置巡航路线的速度  应该不需要这么多参数
void HKMove::SetSeqSpeed(BYTE CruiseRoute, BYTE CruisePoint, WORD PresetIndex)
{
    bool flag;
    flag = NET_DVR_PTZCruise(RealPlayHandle, SET_SEQ_SPEED, CruiseRoute, CruisePoint, PresetIndex);
    if (!flag){
     qDebug()<<"NET_DVR_PTZCruise SET_SEQ_SPEED error"<<NET_DVR_GetLastError();
    }
}

// 巡检开始
void HKMove::RunSeq(BYTE CruiseRoute)
{
    bool flag;
    flag = NET_DVR_PTZCruise(RealPlayHandle, RUN_SEQ, CruiseRoute, 0, 0);
    if (!flag){
     qDebug()<<"NET_DVR_PTZCruise RUN_SEQ error"<<NET_DVR_GetLastError();
    }
}

// 停止开始
void HKMove::StopSeq(BYTE CruiseRoute)
{
    bool flag;
    flag = NET_DVR_PTZCruise(RealPlayHandle, STOP_SEQ, CruiseRoute, 0, 0);
    if (!flag){
     qDebug()<<"NET_DVR_PTZCruise STOP_SEQ error"<<NET_DVR_GetLastError();
    }
}

void HKMove::ClearPTZConfig()
{
    bool flag;
    NET_DVR_CLEARCTRL StruParams ={};
    // 清除所有的预置点 和 巡航路线
    StruParams.byAllPreset = 1;
    StruParams.byAllPatrols = 1;
    flag = NET_DVR_RemoteControl(UserId, NET_DVR_PTZ_CLEARCTRL, &StruParams, sizeof(StruParams));
    if (!flag){
     qDebug()<<"NET_DVR_RemoteControl clear PTD error"<<NET_DVR_GetLastError();
    }
}


//获取热成像火点检测参数
void HKMove::GetSTDAbility(LONG UserId)
{

    //火点检测参数配置结构体。
    NET_DVR_FIREDETECTION_CFG struFireCfg = {};


    NET_DVR_STD_ABILITY StruStdAbility={};
    StruStdAbility.dwOutSize = 1;
    StruStdAbility.dwOutSize =sizeof ( struFireCfg);

    //获取火点检测配置能力  是否支持配置的火点检测参数能力
    if(! NET_DVR_GetSTDAbility(UserId,NET_DVR_GET_FIREDETECTION_CAPABILITIES,&StruStdAbility))
    {
         qDebug()<<" NET_DVR_GetSTDAbility get error"<<NET_DVR_GetLastError();
    }
}


//设置火点检测参数
void HKMove::SetFireDetection(LONG UserId)
{
    NET_DVR_FIREDETECTION_CFG struFireCfg = {};
//     LONG m_lChannel = 1;
//    struFireCfg.dwSize= sizeof (m_lChannel); //结构体大小
    struFireCfg.byEnabled =1;
    struFireCfg.bySensitivity = 80;     //检测灵敏度
    struFireCfg.byFireComfirmTime = 3;  //发现火点等待时间
    struFireCfg.byDetectionMode  =0;   //0多帧  1单帧
    struFireCfg.byFireFocusMode = 0;   // 火点聚焦模式：0~自动模式,1~ 巡航模式。
    struFireCfg.byApplicationSceneMode = 1;  //森林防火
    struFireCfg.dwInstallationHeight =2;    //按照高度
    struFireCfg.byFireSourceDetection = 0;  //火点搜索模式：0- 动态火点 ，1- 吸烟模式
    struFireCfg.bySmokeAuxiliaryDetectionEnabled =1; //烟雾辅助判断启用使能，检测模式是二次判别时生效: 0-否， 1-是

    //配置 NET_DVR_SetSTDConfig 输入输出参数结构体。
    NET_DVR_STD_CONFIG struStdCfg = {};
     LONG m_lChannel = 1;
    struStdCfg.dwOutSize = sizeof (struFireCfg);
    struStdCfg.lpCondBuffer = &m_lChannel;
    struStdCfg.lpOutBuffer =  &struFireCfg;

    if(!NET_DVR_SetSTDConfig(UserId,NET_DVR_SET_FIREDETECTION,&struStdCfg))
    {

        qDebug()<<" NET_DVR_SetSTDConfig set fire_detection error"<<NET_DVR_GetLastError();
    }

}

// 球机的巡检流程
