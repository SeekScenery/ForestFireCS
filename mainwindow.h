#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include "hkcamera.h"
#include <QDebug>
#include "hkcamera.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // launch cruise of the dome camera 1
    void LaunchDomeCruise_1();
    // launch Cruise of the dome camera 2
    void LaunchDomeCruise_2();

    // stop cruise of the dome camera 1
    void StopDomeCruise_1();
    // Stop cruise of the dome camera 2
    void StopDomeCruise_2();


    void initSocket();
    void readPendingDatagrams();

private slots:
    void on_pushButton_clicked();

    void on_Zoom_In_Button_clicked();

    void on_Zoom_Out_Button_clicked();

private:
    Ui::MainWindow *ui;
    bool ReturnState;

    //定义相机指针
    HKCamera *HK_Camsera_1 = nullptr;
    HKCamera *HK_Camsera_2 = nullptr;
    HKCamera *HK_Camsera_3 = nullptr;
    HKCamera *HK_Camsera_4 = nullptr;

    QThread *Thread_1;
    QThread *Thread_2;
    QThread *Thread_3;
    QThread *Thread_4;

    // 定义HWND 视频流句柄
    HWND hWnd1;


signals:
    void StartCamera();

};
#endif // MAINWINDOW_H
