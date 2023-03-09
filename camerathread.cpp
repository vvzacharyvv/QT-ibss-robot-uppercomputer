
#include "camerathread.h"

CameraThread::CameraThread(QObject *parent):QThread(parent)
{
timer =new QTimer(this);
}
CameraThread::~CameraThread()
{
cap.release();

}

void CameraThread::run()

{

//cap.open("rtsp://admin:password507@192.168.1.107:554/11");
cap.open(m_url.toStdString());

while(flag==1)
{
    img_update();
    this->msleep(30);
}


}

QImage CameraThread::img_process(const cv::Mat &frame)
{
    //CV_8UC1 8位无符号的单通道---灰度图片
        if(frame.type() == CV_8UC1)
        {
            //使用给定的大小和格式构造图像
            //QImage(int width, int height, Format format)
            QImage qImage(frame.cols,frame.rows,QImage::Format_Indexed8);
            //扩展颜色表的颜色数目
            qImage.setColorCount(256);

            //在给定的索引设置颜色
            for(int i = 0; i < 256; i ++)
            {
                //得到一个黑白图
                qImage.setColor(i,qRgb(i,i,i));
            }
            //复制输入图像,data数据段的首地址
            uchar *pframe = frame.data;
            //
            for(int row = 0; row < frame.rows; row ++)
            {
                //遍历像素指针
                uchar *pDest = qImage.scanLine(row);
                //从源frame所指的内存地址的起始位置开始拷贝n个
                //字节到目标dest所指的内存地址的起始位置中
                memcmp(pDest,pframe,frame.cols);
                //图像层像素地址
                pframe += frame.step;
            }
            return qImage;
        }
        //为3通道的彩色图片
        else if(frame.type() == CV_8UC3)
        {
            //得到图像的的首地址
            const uchar *pframe = (const uchar*)frame.data;
            //以frame构造图片
            QImage qImage(pframe,frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
            //在不改变实际图像数据的条件下，交换红蓝通道
            return qImage.rgbSwapped();
        }
        //四通道图片，带Alpha通道的RGB彩色图像
        else if(frame.type() == CV_8UC4)
        {
            const uchar *pframe = (const uchar*)frame.data;
            QImage qImage(pframe, frame.cols, frame.rows, frame.step, QImage::Format_ARGB32);
            //返回图像的子区域作为一个新图像
            return qImage.copy();
        }
        else
        {
            return QImage();
        }


}
void CameraThread::img_update()
{
    cap.read(frame);
    img=img_process(frame);
    emit sendimg(img);

}

void CameraThread::geturl(QString url)
{
    this->m_url=url;
}
