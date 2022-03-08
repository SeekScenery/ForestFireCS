#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //初始海康SDK
    ReturnState = NET_DVR_Init();
    if(!ReturnState)
        qDebug()<<"NET_DVR_Init failed, error code: "<< NET_DVR_GetLastError() << endl;
    //设置HK设备网络连接时间与重连时间 可以使用默认值
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);

    // 创建相机对象  这需要确定是否需传引用
    HK_Camsera_1 = new HKCamera(1, (HWND) ui->VideoStream1->winId(), nullptr); // 高清相机
    HK_Camsera_2 = new HKCamera(2, (HWND) ui->VideoStream2->winId(), nullptr); // 热成像
    HK_Camsera_3 = new HKCamera(3, (HWND) ui->VideoStream3->winId(), 1, nullptr); // 球机1
    HK_Camsera_4 = new HKCamera(4, (HWND) ui->VideoStream4->winId(), 2, nullptr); // 球机2
    // 创建线程 加入线程
    Thread_1 = new QThread();
    HK_Camsera_1->moveToThread(Thread_1);
    Thread_2 = new QThread();
    HK_Camsera_2->moveToThread(Thread_2);
    Thread_3 = new QThread();
    HK_Camsera_3->moveToThread(Thread_3);
    Thread_4 = new QThread();
    HK_Camsera_4->moveToThread(Thread_4);

    Thread_1->start();
    Thread_2->start();
    Thread_3->start();
    Thread_4->start();


//    QObject::connect(ui->pushButton, &QPushButton::clicked, HK_Camsera_4, &HKCamera::test);

    // 连接相机 可通过返回值判断是否 继续操作
    HK_Camsera_1->ConnectCamera();
    HK_Camsera_2->ConnectCamera();
    HK_Camsera_3->ConnectCamera();
    HK_Camsera_4->ConnectCamera();

    // 将视频流接到界面显示
    HK_Camsera_1->RealPlay();
    HK_Camsera_2->RealPlay();
    HK_Camsera_3->RealPlay();
    HK_Camsera_4->RealPlay();
//     connect(ui->SaveVideo, &QPushButton::pressed, this, &MainWindow::save_video);
    connect(Thread_1, &QThread::finished, HK_Camsera_1, &QObject::deleteLater);
    connect(Thread_2, &QThread::finished, HK_Camsera_2, &QObject::deleteLater);
    connect(Thread_3, &QThread::finished, HK_Camsera_3, &QObject::deleteLater);
    connect(Thread_4, &QThread::finished, HK_Camsera_4, &QObject::deleteLater);

    connect(this, &MainWindow::StartCamera, HK_Camsera_4, &HKCamera::ForestDomePatrol);





}



void MainWindow::initSocket()
{
    udpSocket = new QUdpSocket();
    udpSocket->bind(QHostAddress::LocalHost, 7755);

    connect(udpSocket, &QUdpSocket::readyRead,
            this, &MainWindow::readPendingDatagrams);
}

void MainWindow::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
//      datagram = udpSocket->receiveDatagram();
//        processTheDatagram(datagram);
    }
}


MainWindow::~MainWindow()
{
    //释放海康SDK
    ReturnState = NET_DVR_Cleanup();
    if(!ReturnState)
        qDebug()<<"NET_DVR_Cleanup failed, error code: "<< NET_DVR_GetLastError() << endl;

    HK_Camsera_1->CloseCamera();
    HK_Camsera_2->CloseCamera();
    HK_Camsera_3->CloseCamera();
    HK_Camsera_4->CloseCamera();

    Thread_1->quit();
    Thread_1->wait();
    Thread_2->quit();
    Thread_2->wait();
    Thread_3->quit();
    Thread_3->wait();
    Thread_4->quit();
    Thread_4->wait();

    delete Thread_1;
    delete Thread_2;
    delete Thread_3;
    delete Thread_4;
    
    delete HK_Camsera_1;
    delete HK_Camsera_2;
    delete HK_Camsera_3;
    delete HK_Camsera_4;
    delete ui;
}

void MainWindow::LaunchDomeCruise_1()
{
    HK_Camsera_3->DomeCameraMove->RunSeq(2);
    HK_Camsera_3->DomeCameraMove->RunSeq(2);
}

void MainWindow::StopDomeCruise_1()
{
    HK_Camsera_3->DomeCameraMove->StopSeq(2);
    HK_Camsera_3->DomeCameraMove->StopSeq(2);
}



void MainWindow::on_pushButton_clicked()
{

    emit StartCamera();
//  qDebug()<<"start Camera"<< endl;
//    QObject::connect(this, &MainWindow::StartCamera, &con, &connect_test::test_print);
}

void MainWindow::on_Zoom_In_Button_clicked()
{
    HK_Camsera_4->DomeCameraMove->Zoom_IN_start();
    HK_Camsera_4->DomeCameraMove->Zoom_IN_end();
}

void MainWindow::on_Zoom_Out_Button_clicked()
{
    HK_Camsera_4->DomeCameraMove->Zoom_OUT_start();
    HK_Camsera_4->DomeCameraMove->Zoom_OUT_end();
}
