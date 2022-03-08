#ifndef ALG_H
#define ALG_H
#include <vector>
#include <core/core.hpp>
#include<opencv.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
#include "hkmove.h"
#include <cmath>
#include <QDebug>
using namespace cv;
using namespace std;
using namespace dnn;

// 需改善
typedef struct
{
    int id;//结果类别id
    float confidence;//结果置信度
    cv::Rect box;//矩形框
} YoloOutput ;

typedef struct
{
    PTZPOS anomaly_position;
    int Id;
    int belong_img_mark;
    float confidence;
} AnomalyInfo;

// 用类静态变量 还是 全局变量作为 异常信息的容器 先用静态变量试一试


class Alg
{
public:
    Alg();

    // Yolo 算法模块
    bool YoloReadModel(string &NetPath, bool isCuda);
    bool YoloDetect(Mat& img, vector<YoloOutput>& DetectResult);
    void YoloDrawPred(Mat &img, vector<YoloOutput> result);
    void YoloDrawAnomaly(Mat &img, YoloOutput result);


    PTZPOS AnomalyLocalization(PTZInfo dome_position,WORD width, WORD height, Rect box);
    // 异常位置信息 异常类型 置信度 （来源于那一张图片 是否设计）
    void PushInformation(PTZPOS anomaly_position, int Id, float confidence, int belong_img_mark);
public:
     static vector<AnomalyInfo> contain_anomlay;

private:
     //Yolo 算法模块变量
     const float netAnchors[3][6] = { { 10.0, 13.0, 16.0, 30.0, 33.0, 23.0 },{ 30.0, 61.0, 62.0, 45.0, 59.0, 119.0 },{ 116.0, 90.0, 156.0, 198.0, 373.0, 326.0 } };
     const float netStride[3] = { 8, 16.0,32 };
     const int netWidth = 640;
     const int netHeight = 640;
     float nmsThreshold = 0.45;
     float boxThreshold = 0.25;
     float classThreshold = 0.25;
     vector<string> className = {"fire", "smoke"};
     dnn::Net YoloNet;
     vector<Scalar> DrawColor;

};

#endif // ALG_H
