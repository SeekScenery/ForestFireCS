#include "alg.h"


vector<AnomalyInfo> Alg::contain_anomlay;

Alg::Alg()
{
    // yolo 检测框颜色设置 可根据类别添加
    DrawColor.push_back(Scalar(255, 0, 0)); // smoke 颜色
    DrawColor.push_back(Scalar(0, 255, 0)); // fire 颜色
}

bool Alg::YoloReadModel(string &NetPath, bool isCuda)
{
    try {
        YoloNet = dnn::readNet(NetPath);
    }
    catch (const std::exception&) {
        return false;
    }
    //cuda
    if (isCuda) {
        YoloNet.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
        YoloNet.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);

    }
    //cpu
    else {

        YoloNet.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
        YoloNet.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    }
    return true;
}

bool Alg::YoloDetect(Mat &SrcImg, vector<YoloOutput> &DetectResult)
{
    Mat blob;
    int col = SrcImg.cols;
    int row = SrcImg.rows;
    int maxLen = MAX(col, row);
    Mat netInputImg = SrcImg.clone();
    if (maxLen > 1.2*col || maxLen > 1.2*row) {
        Mat resizeImg = Mat::zeros(maxLen, maxLen, CV_8UC3);
        SrcImg.copyTo(resizeImg(Rect(0, 0, col, row)));
        netInputImg = resizeImg;
    }
//	blobFromImage(netInputImg, blob, 1 / 255.0, cv::Size(netWidth, netHeight), cv::Scalar(104, 117,123), true, false);
    //如果在其他设置没有问题的情况下但是结果偏差很大，可以尝试下用下面两句语句
    blobFromImage(netInputImg, blob, 1 / 255.0, cv::Size(netWidth, netHeight), cv::Scalar(0, 0,0), true, false);

    //blobFromImage(netInputImg, blob, 1 / 255.0, cv::Size(netWidth, netHeight), cv::Scalar(114, 114,114), true, false);
    YoloNet.setInput(blob);
    std::vector<cv::Mat> netOutputImg;
    //vector<string> outputLayerName{"345","403", "461","output" };
    //net.forward(netOutputImg, outputLayerName[3]); //获取output的输出
    YoloNet.forward(netOutputImg, YoloNet.getUnconnectedOutLayersNames());
    std::vector<int> classIds;//结果id数组
    std::vector<float> confidences;//结果每个id对应置信度数组
    std::vector<cv::Rect> boxes;//每个id矩形框
    float ratio_h = (float)netInputImg.rows / netHeight;
    float ratio_w = (float)netInputImg.cols / netWidth;
    int net_width = className.size() + 5;  //输出的网络宽度是类别数+5
    float* pdata = (float*)netOutputImg[0].data;
    for (int stride =0; stride < 3; stride++) {    //stride
        int grid_x = (int)(netWidth / netStride[stride]);
        int grid_y = (int)(netHeight / netStride[stride]);
        for (int anchor = 0; anchor < 3; anchor++) { //anchors
            // 未使用先注释了 以后研究
//            const float anchor_w = netAnchors[stride][anchor * 2];
//            const float anchor_h = netAnchors[stride][anchor * 2 + 1];
            for (int i = 0; i < grid_y; i++) {
                for (int j = 0; j < grid_x; j++) {
                    float box_score = pdata[4]; //Sigmoid(pdata[4]);//获取每一行的box框中含有某个物体的概率

                    if (box_score > boxThreshold) {
                        cv::Mat scores(1, className.size(), CV_32FC1, pdata + 5);
                        Point classIdPoint;
                        double max_class_socre;
                        minMaxLoc(scores, 0, &max_class_socre, 0, &classIdPoint);
                        max_class_socre = (float)max_class_socre; //Sigmoid((float)max_class_socre);
                        if (max_class_socre > classThreshold) {
                            //rect [x,y,w,h]
                            float x = pdata[0];// (Sigmoid(pdata[0]) * 2.f - 0.5f + j) * netStride[stride];  //x
                            float y = pdata[1];// (Sigmoid(pdata[1]) * 2.f - 0.5f + i) * netStride[stride];   //y
                            float w = pdata[2];// powf(Sigmoid(pdata[2]) * 2.f, 2.f) * anchor_w;   //w
                            float h = pdata[3];// powf(Sigmoid(pdata[3]) * 2.f, 2.f) * anchor_h;  //h
                            int left = (x - 0.5*w)*ratio_w;
                            int top = (y - 0.5*h)*ratio_h;

                            classIds.push_back(classIdPoint.x);
                            confidences.push_back(max_class_socre*box_score);
                            boxes.push_back(Rect(left, top, int(w*ratio_w), int(h*ratio_h)));

                        }
                    }
                    pdata += net_width;//下一行
                }
            }
        }
    }

    //执行非最大抑制以消除具有较低置信度的冗余重叠框（NMS）
    vector<int> nms_result;
    NMSBoxes(boxes, confidences, classThreshold, nmsThreshold, nms_result);
    for (int i = 0; i < int(nms_result.size()); i++) {
        int idx = nms_result[i];
        YoloOutput result;
        result.id = classIds[idx];
        result.confidence = confidences[idx];
        result.box = boxes[idx];
        DetectResult.push_back(result);
    }

    if (DetectResult.size())
        return true;
    else
        return false;
}

void Alg::YoloDrawPred(Mat &img, vector<YoloOutput> result)
{

//    vector<Scalar> color;
//    srand(time(0));
//    for (int i = 0; i < 80; i++) {
//        int b = rand() % 256;
//        int g = rand() % 256;
//        int r = rand() % 256;
//        color.push_back(Scalar(b, g, r));
//    }

    for (int i = 0; i < int(result.size()); i++)
    {
        int left, top;
        left = result[i].box.x;
        top = result[i].box.y;
//		int color_num = i;
        rectangle(img, result[i].box, DrawColor[result[i].id], 2, 8);

        string label = className[result[i].id] +":" + to_string(result[i].confidence);

        int baseLine;
        Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        top = max(top, labelSize.height);
        //rectangle(frame, Point(left, top - int(1.5 * labelSize.height)), Point(left + int(1.5 * labelSize.width), top + baseLine), Scalar(0, 255, 0), FILLED);
        putText(img, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 1, DrawColor[result[i].id], 2);
    }
    //	imshow("1", img);
    //imwrite("out.bmp", img);
    //	waitKey();
    //destroyAllWindows();
}

void Alg::YoloDrawAnomaly(Mat &img, YoloOutput result)
{
    int left, top;
    left = result.box.x;
    top = result.box.y;

    cv::rectangle(img, result.box, DrawColor[result.id], 2, 8);

    string label = className[result.id] +":" + to_string(result.confidence);
    int baseLine;
    Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    top = max(top, labelSize.height);
    //rectangle(frame, Point(left, top - int(1.5 * labelSize.height)), Point(left + int(1.5 * labelSize.width), top + baseLine), Scalar(0, 255, 0), FILLED);
    putText(img, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 1, DrawColor[result.id], 2);
}

PTZPOS Alg::AnomalyLocalization(PTZInfo dome_position, WORD width, WORD height, Rect box)
{
//    WORD focus = dome_position.ZoomPos * 6; // 变倍 - 焦距
//    // 初步焦距转化为视场角
//    float H_temp = 3.6 / focus;
//    float V_temp = 2.7 /focus;
//    double H_angle = atan(H_temp);
//    double V_angle = atan(V_temp);

    // 定位异常坐标中心点
    float XAnomCenter = box.x + box.width / 2;
    float YAnomCenter = box.y + box.height / 2;

   qDebug() << "XAnomCenter:" << XAnomCenter << endl;
   qDebug() << "YAnomCenter:" << YAnomCenter << endl;

   qDebug() << "dome_position.HorizontalAngle:" << dome_position.HorizontalAngle << endl;
   qDebug() << "dome_position.VerticalAngle" << dome_position.VerticalAngle << endl;

    //将异常中心像素坐标点转化为角度坐标点 需考虑转角的正负问题
    float H_temp = XAnomCenter / width * dome_position.HorizontalAngle;
    float V_temp = YAnomCenter / height * dome_position.VerticalAngle;

    qDebug() << "H_temp:" << H_temp << endl;
    qDebug() << "H_temp:" << V_temp << endl;

    PTZPOS AnomalPostion;
    // 假设水平往右为正 垂直向下为正 转化为角度信息 定位角度信息
    AnomalPostion.PanPos = dome_position.PanPos - dome_position.HorizontalAngle / 2 + H_temp;
    AnomalPostion.TiltPos = dome_position.TiltPos - dome_position.VerticalAngle /2 + V_temp;

    // 计算高清相机和云台异常处理时合适的变倍Zoom      待写

    qDebug() << "dome_position.PanPos:" << dome_position.PanPos << endl;
    qDebug() << "dome_position.TiltPos:" << dome_position.TiltPos << endl;

    qDebug() << "AnomalPostion.PanPos:" << AnomalPostion.PanPos << endl;
    qDebug() << "AnomalPostion.TiltPos" << AnomalPostion.TiltPos << endl;


    AnomalPostion.ZoomPos = WORD(dome_position.ZoomPos);

    return AnomalPostion;
}

void Alg::PushInformation(PTZPOS anomaly_position, int Id, float confidence, int belong_img_mark)
{
    AnomalyInfo temporary_Anom;
    temporary_Anom.anomaly_position = anomaly_position;
    temporary_Anom.Id = Id;
    temporary_Anom.confidence = confidence;
    temporary_Anom.belong_img_mark = belong_img_mark;
    contain_anomlay.push_back(temporary_Anom);
}
