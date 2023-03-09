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
    QImage img;
    void run() override;
    int flag=0;
    QImage img_process(const cv::Mat &frame);
    void geturl(QString url);
private:
    cv::VideoCapture cap;
    cv::Mat frame;
    QTimer *timer;
    QString m_url;

private slots:
    void img_update();
signals:
    void sendimg(QImage img);

};

#endif // CAMERATHREAD_H
