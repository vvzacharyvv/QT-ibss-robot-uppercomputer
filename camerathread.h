#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <opencv2/opencv.hpp>
#include <QTimer>
#include<QThread>
#include<QImage>
#include<QString>
class CameraThread : public QThread
{
    Q_OBJECT
public:
   explicit  CameraThread(QObject *parent);
    ~CameraThread();
    QImage img1;
    QImage img2;
    void run() override;
    int flag=0;
    QImage img_process(const cv::Mat &frame);
    void geturl(QString url);
    void splitframe(cv::Mat frame);
     int m_Fps;
private:
    cv::VideoCapture cap;
    cv::Mat frame;
    cv::Mat frame1,frame2;
    QTimer *timer;
    QString m_url;

private slots:
    void img_update();
signals:
    //void sendimg(QImage img);
    void sendimg();
};

#endif // CAMERATHREAD_H
