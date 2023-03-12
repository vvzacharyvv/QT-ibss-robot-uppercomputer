#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"camerathread.h"
#include <QMainWindow>
#include <QSerialPort>			//访问串口的库
#include <QSerialPortInfo>		//查询串口信息的库
#include <QDebug>				//用于调试打印输出的库
#include <QTimer>				//定时器的库
#include <QTime>				//时间的库
#include <QDate>				//日期的库
#include <QMessageBox>			//一个小的弹窗库
#include"datadeal.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//    CameraThread *thread1=new CameraThread(this);
//    CameraThread *thread2=new CameraThread(this);
    int btn_on_off = 0;
    float m_fl;
    int btn_state = 0;
    bool video_flag=0;
    bool motion_state=0;
    bool hexshow_flag=0;
    bool hexsend_flag=0;
    QString m_pset;
    DataDeal *dd;
private slots:
    void on_btn_open_close_clicked();

    void serialPortReadyRead();

    void serialPortWrite();

    void on_btn_clear_rx_clicked();

    void on_btn_clear_tx_clicked();

    void on_btn_send_clicked();

    void on_btn_motion_clicked();





    void on_btn_vset_clicked();

    void on_btn_dset_clicked();

    void on_btn_gmset_clicked();

    void on_btn_tguset_clicked();

    void on_btn_pset_clicked();

    void on_checkBox_HexSend_stateChanged(int arg1);

    void on_checkBox_HexShow_stateChanged(int arg1);

    void on_btn_connect_clicked();

    void on_btn_refresh_clicked();

    //void on_PBT_Open_clicked();

    void on_btn_vsetZ_clicked();

private:
    Ui::MainWindow *ui;
    //定时器
    QTimer *timer;
    // 串口对象
    QSerialPort *serialport;
    //扫描串口
    void scanSerialPort();
    //初始化
    void serialPortInit();

    //窗口事件
//protected:
//       virtual bool event(QEvent *event);
//        void update_size();
//        double screen_height[2];
//        double screen_width[2];

};
#endif // MAINWINDOW_H
