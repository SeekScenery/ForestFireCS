#ifndef HKMOVE_H
#define HKMOVE_H
#include "HCNetSDK.h"
typedef struct
{
    float PanPos;  //水平参数
    float TiltPos; //垂直参数
    WORD ZoomPos; //变倍参数
}PTZPOS, *LPTZPOS;


typedef struct
{
    float PanPos;  //水平参数
    float TiltPos; //垂直参数
    float ZoomPos; //变倍参数
    float HorizontalAngle;
    float VerticalAngle;
}PTZInfo, *LPTZInfo;


#define MAX_PRESET_NUM 30


class HKMove
{
public:
    HKMove(LONG, LONG);
    void Up_start();        //控制镜头移动
    void Down_start();
    void Left_start();
    void Right_start();

    void Up_end();          //移动停止
    void Down_end();
    void Left_end();
    void Right_end();

    // 调节焦距
    void Zoom_IN_start();       // 焦距变大
    void Zoom_OUT_start();      // 焦距变小

    // 停止变焦
    void Zoom_IN_end();
    void Zoom_OUT_end();

    // 云台巡检
    PTZInfo Get_PTZPOS(); // 获取云台位置    参数
    void SetPreset(DWORD);             //设置预置点
    void ClearPreset(DWORD);          // 删除指定预置点
    void GotoPreset(DWORD);           // 转到预置点
    void GetPTZCruise(LONG, LONG);    // 查看巡航路线点

    void GetPresetName(); // 获取以设置的预置点？ 得测试一下？
    void FillPresetSeq(BYTE, BYTE, WORD); // 将预置点加入指定巡航路线的巡航点
    void ClealPresetSeq(BYTE, BYTE, WORD); // 将预置点从指定巡航路线删除
    void SetSeqDwell(BYTE, BYTE, WORD); // 设置巡航路线中巡航点的时间
    void SetSeqSpeed(BYTE, BYTE, WORD); // 设置巡航路线的速度

    void RunSeq(BYTE); // 开始巡检
    void StopSeq(BYTE); // 停止巡检

    void ClearPTZConfig();
private:
    LONG UserId;
    LONG RealPlayHandle;
};

#endif // HKMOVE_H
