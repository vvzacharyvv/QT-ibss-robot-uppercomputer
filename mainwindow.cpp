#include "mainwindow.h"
#include "ui_mainwindow.h"



#include"camerathread.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CameraThread *thread1=new CameraThread(this);
    CameraThread *thread2=new CameraThread(this);
    //init url
    thread1->geturl("http://192.168.50.245:8080/?action=stream?dummy=param.mjpg");
    thread2->geturl("rtsp://admin:password507@192.168.1.105:554/11");
    //启动子线程
    connect(ui->PBT_Open,&QPushButton::clicked,this,[=](){
        thread1->flag=1;
        thread1->start();
        thread2->flag=1;
        thread2->start();
    });
    //接受子线程的数据, 收到一张图更新一次画布
    connect(thread1,&CameraThread::sendimg,this,[=](){
        ui->label_1->setPixmap(QPixmap::fromImage(thread1->img));
        ui->label_2->setPixmap(QPixmap::fromImage(thread2->img));
    });
    //关闭子线程
    connect(ui->PBT_Close,&QPushButton::clicked,this,[=](){
        thread1->flag=0;
        thread1->exit();
        ui->label_1->clear();
        thread2->flag=0;
        thread2->exit();
        ui->label_2->clear();
    });


    //图标设置
    ui->btn_motion->setIcon(QIcon(":/run.png"));
    ui->btn_connect->setIcon(QIcon(":/lianjie.png"));
        ui->btn_refresh->setIcon(QIcon(":/shuaxin.png"));
        ui->PBT_Open->setIcon(QIcon(":/bofang.png"));
        ui->PBT_Open->setToolTip("播放");
        ui->PBT_Close->setIcon(QIcon(":/tingzhi.png"));
        ui->PBT_Close->setToolTip("停止");
   // ui->btn_motion->setIconSize(QSize(16,16));
    ui->btn_motion->setToolTip("启动");



    //init serial port and timer,scan available port.
    serialport=new QSerialPort(this);
    timer=new QTimer(this);
    scanSerialPort();
    connect(serialport,SIGNAL(readyRead()),this,
                              SLOT(serialPortReadyRead()));




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::scanSerialPort()
{
    //search available port info.
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {

        {
            //add port to combox
            ui->cbx_setPortName->addItem(info.portName());
        }
        qDebug()<<"串口名称:"<<ui->cbx_setPortName->currentText();
    }


}
void MainWindow::serialPortInit()
{
    //设置串口名称
    serialport->setPortName(ui->cbx_setPortName->currentText());
   qDebug()<<"串口名称:"<<serialport->portName();

   if (!serialport->open(QIODevice::ReadWrite))
   {
        qDebug()<<"cuowu";
        QMessageBox::about(this,"wrong","wrong");
        serialport->close();

        return ;
   }

   //波特率
   serialport->setBaudRate(ui->cbx_setBaudRate->currentText().toInt());
   qDebug()<<"波特率："<<ui->cbx_setBaudRate->currentText().toInt();
   //停止位
   switch (ui->cbx_setStopBits->currentText().toInt())
   {
   case 1 :  serialport->setStopBits(QSerialPort::OneStop); break;
   case 2 :  serialport->setStopBits(QSerialPort::TwoStop); break;
   default: break;
   }
   qDebug()<<"停止位:"<<serialport->stopBits();
   //数据位
   switch (ui->cbx_setDataBits->currentText().toInt())
   {
   case 5 :  serialport->setDataBits(QSerialPort::Data5); break;
   case 6 :  serialport->setDataBits(QSerialPort::Data6); break;
   case 7 :  serialport->setDataBits(QSerialPort::Data7); break;
   case 8 :  serialport->setDataBits(QSerialPort::Data8); break;
   default: break;
   }
   //奇偶位
   switch (ui->cbx_setParity->currentIndex())
   {
   case 0 :  serialport->setParity(QSerialPort::NoParity); break;
   case 1 :  serialport->setParity(QSerialPort::OddParity); break;
   case 2 :  serialport->setParity(QSerialPort::EvenParity); break;
   default: break;
   }
   qDebug()<<"奇偶位:"<<serialport->parity();

   serialport->setFlowControl(QSerialPort::NoFlowControl) ;


}


void MainWindow::on_btn_open_close_clicked()
{
    if (btn_on_off == 0)
    {

        ui->cbx_setPortName->setEnabled(false);
        ui->cbx_setBaudRate->setEnabled(false);
        ui->cbx_setDataBits->setEnabled(false);
        ui->cbx_setStopBits->setEnabled(false);
        ui->cbx_setParity->setEnabled(false);
        ui->btn_open_close->setText("close serialport");
         ui->btn_connect->setIcon(QIcon(":/duankailianjie.png"));
            ui->btn_connect->setToolTip("断开连接");
        qDebug()<<"打开串口:";
        serialPortInit();
    }

   if (btn_on_off == 1)
   {

       serialport->close();
       ui->cbx_setPortName->setEnabled(true);
       ui->cbx_setBaudRate->setEnabled(true);
       ui->cbx_setDataBits->setEnabled(true);
       ui->cbx_setStopBits->setEnabled(true);
       ui->cbx_setParity->setEnabled(true);
       ui->btn_open_close->setText("open serialport");
        ui->btn_connect->setIcon(QIcon(":/lianjie.png"));
           ui->btn_connect->setToolTip("连接");
   }
   btn_on_off = !btn_on_off;
}

void MainWindow::serialPortReadyRead()
{
//    QByteArray temp = serialport->readAll();
    //     QString str = ui->textEdit_rx->toPlainText();
    //     str +=  QString::fromLocal8Bit(temp);//显示中文
    //      ui->textEdit_rx->append(str);
    QByteArray temp = serialport->readAll();
    //状态显示栏代码
    QString str;
    str=QString::fromLocal8Bit(temp);
    if(str=="run")
    {
        QString str2="启动成功";
        ui->textEdit_status->append(str2);
    }

    if(str=="STOP")
    {
        QString str2="停止运行";
        ui->textEdit_status->append(str2);
    }
    if(str=="change2")
    {
        QString str2=QString("速度设置成功！当前速度为：%1").arg(m_fl);
        ui->textEdit_status->append(str2);
    }
    if(str=="change3")
    {
        QString str2=QString("步态模式设置成功！当前步态模式为：%1").arg(m_fl);
        ui->textEdit_status->append(str2);
    }
    if(str=="change4")
    {
        QString str2=QString("时间步态更新设置成功！当前为：%1").arg(m_fl);
        ui->textEdit_status->append(str2);
    }
    if(str=="change5")
    {
        QString str2="下压设置成功！当前下压为:";
        QString str3=str2+m_pset;
        ui->textEdit_status->append(str3);
    }

    //串口助手部分代码
    //qDebug()<<"readdatais："<<QString::fromLocal8Bit(temp);
    if(ui->checkBox_HexShow->isChecked())
    {
        str=QString::fromLocal8Bit(temp.toHex());
        ui->textEdit_rx->append(str);
    }
    else
    {
    str =QString::fromLocal8Bit(temp);
    ui->textEdit_rx->append(str);
    }


}

void MainWindow::serialPortWrite()
{
    QByteArray buff;
    QString str;
    //判断是否非空
    if(!ui->textEdit_tx->toPlainText().isEmpty())
    {
        if(ui->checkBox_HexSend->isChecked())
        {
           str=ui->textEdit_tx->toPlainText();
           buff=dd->convertStringToHex(str);
          // qDebug()<<buff;
           serialport->write(buff);
        }
        else
        {
        buff = ui->textEdit_tx->toPlainText().toLocal8Bit();//可以写中文
        //buff+="\r\n";
        serialport->write(buff);
        //qDebug()<<"writedatais："<<serialport->write(buff);
        }
    }
}


void MainWindow::on_btn_clear_rx_clicked()
{
    ui->textEdit_rx->clear();
}

void MainWindow::on_btn_clear_tx_clicked()
{
    ui->textEdit_tx->clear();
}

void MainWindow::on_btn_send_clicked()
{
    serialPortWrite();
}
//机器人运动控制函数（先用点灯测试）
void MainWindow::on_btn_motion_clicked()
{
    QString str;
    QByteArray buff;
    if(motion_state==0)
    { str="23020600000031aa";
                buff=dd->convertStringToHex(str);
         //buff+="\r\n";
        motion_state=1;
    ui->btn_motion->setIcon(QIcon(":/stop.png"));
       ui->btn_motion->setToolTip("停止");
    serialport->write(buff);

}
    else
    {
//        buff.resize(4);
//        buff[0]='=';
//        buff[1]='2';
//        buff[2]='5';
//        buff[3]='4';
         //buff+="\r\n";
        str="23020600000030aa";
                        buff=dd->convertStringToHex(str);
                motion_state=0;
            ui->btn_motion->setIcon(QIcon(":/run.png"));
               ui->btn_motion->setToolTip("启动");
            serialport->write(buff);

    }

}






void MainWindow::on_btn_vset_clicked()
{

    QString str,str_set;
    QByteArray buff;

    str="230202";
    str_set=ui->v_edit->text();
    m_fl=str_set.toFloat();
    //qDebug()<<"fl="<<fl;
    uint f_uint = *(uint*)&m_fl;
    QString f_hex = QString("%1").arg(f_uint, 4, 16, QLatin1Char('0'));	// 4是生成字符串的长度
   // qDebug() << f_hex;

    QString temp;
    temp.resize(8);
    temp[0]=f_hex[6];
    temp[1]=f_hex[7];
    temp[2]=f_hex[4];
    temp[3]=f_hex[5];
    temp[4]=f_hex[2];
    temp[5]=f_hex[3];
    temp[6]=f_hex[0];
    temp[7]=f_hex[1];
    str+=temp;
    qDebug()<<temp;
    str+="aa";
   // qDebug()<<"str="<<str;
    buff=dd->convertStringToHex(str);
    serialport->write(buff);


}

void MainWindow::on_btn_dset_clicked()
{
    QString str,str_set;
    QByteArray buff;
//    float fl;
    str="230201";
    str_set=ui->d_edit->text();
    //buff=

    m_fl=str_set.toFloat();
    //qDebug()<<"fl="<<fl;
    uint f_uint = *(uint*)&m_fl;
    QString f_hex = QString("%1").arg(f_uint, 4, 16, QLatin1Char('0'));	// 4是生成字符串的长度
   // qDebug() << f_hex;

    QString temp;
    temp.resize(8);
    temp[0]=f_hex[6];
    temp[1]=f_hex[7];
    temp[2]=f_hex[4];
    temp[3]=f_hex[5];
    temp[4]=f_hex[2];
    temp[5]=f_hex[3];
    temp[6]=f_hex[0];
    temp[7]=f_hex[1];
    str+=temp;
    str+="aa";
    qDebug()<<str;
    buff=dd->convertStringToHex(str);
    serialport->write(buff);

}

void MainWindow::on_btn_gmset_clicked()
{
    QString str,str_set;
    QByteArray buff;
//    float fl;
    str="230203";
    str+="000000";
    str_set=ui->gm_edit->text();
    str+=str_set;
    m_fl=str_set.toFloat();
    //buff=
//    float fl;
//    fl=str_set.toFloat();
//    //qDebug()<<"fl="<<fl;
//    uint f_uint = *(uint*)&fl;
//    QString f_hex = QString("%1").arg(f_uint, 4, 16, QLatin1Char('0'));	// 4是生成字符串的长度
//   // qDebug() << f_hex;

//    QString temp;
//    temp.resize(8);
//    temp[0]=f_hex[6];
//    temp[1]=f_hex[7];
//    temp[2]=f_hex[4];
//    temp[3]=f_hex[5];
//    temp[4]=f_hex[2];
//    temp[5]=f_hex[3];
//    temp[6]=f_hex[0];
//    temp[7]=f_hex[1];
//    str+=temp;
    str+="aa";
    qDebug()<<str;
    buff=dd->convertStringToHex(str);
    serialport->write(buff);
}


void MainWindow::on_btn_tguset_clicked()
{
    QString str,str_set;
    QByteArray buff;
//    float fl;
    str="230204";
    str_set=ui->tgu_edit->text();
    //buff=

    m_fl=str_set.toFloat();
    //qDebug()<<"fl="<<fl;
    uint f_uint = *(uint*)&m_fl;
    QString f_hex = QString("%1").arg(f_uint, 4, 16, QLatin1Char('0'));	// 4是生成字符串的长度
   // qDebug() << f_hex;

    QString temp;
    temp.resize(8);
    temp[0]=f_hex[6];
    temp[1]=f_hex[7];
    temp[2]=f_hex[4];
    temp[3]=f_hex[5];
    temp[4]=f_hex[2];
    temp[5]=f_hex[3];
    temp[6]=f_hex[0];
    temp[7]=f_hex[1];
    str+=temp;
    str+="aa";
    qDebug()<<str;
    buff=dd->convertStringToHex(str);
    serialport->write(buff);
}

void MainWindow::on_btn_pset_clicked()
{
    QString str,str_set;
    QByteArray buff;
//    float fl;
    str="230205";
    str_set=ui->p_edit->text();
    str+=str_set;
    m_pset=str_set;
    //buff=
//    float fl;
//    fl=str_set.toFloat();

//    uint f_uint = *(uint*)&fl;
//    QString f_hex = QString("%1").arg(f_uint, 4, 16, QLatin1Char('0'));	// 4是生成字符串的长度


//    QString temp;
//    temp.resize(8);
//    temp[0]=f_hex[6];
//    temp[1]=f_hex[7];
//    temp[2]=f_hex[4];
//    temp[3]=f_hex[5];
//    temp[4]=f_hex[2];
//    temp[5]=f_hex[3];
//    temp[6]=f_hex[0];
//    temp[7]=f_hex[1];
//    str+=temp;
    str+="aa";
    qDebug()<<str;
    buff=dd->convertStringToHex(str);
    serialport->write(buff);
}

void MainWindow::on_checkBox_HexSend_stateChanged(int arg1)
{
    if(ui->checkBox_HexSend->isChecked())
    {
        QString str = ui->textEdit_tx->toPlainText().toLocal8Bit().toHex();
        ui->textEdit_tx->clear();
        ui->textEdit_tx->append(str);

    }
    else
    {
        QString str =ui->textEdit_tx->toPlainText().toLocal8Bit();
        QByteArray buff = dd->convertStringToHex(str);
        str.clear();
        str.prepend(buff);
        ui->textEdit_tx->clear();
        ui->textEdit_tx->append(str);


    }
}

void MainWindow::on_checkBox_HexShow_stateChanged(int arg1)
{
    if(ui->checkBox_HexShow->isChecked())
    {
        QString str = ui->textEdit_rx->toPlainText().toLocal8Bit().toHex();
        ui->textEdit_rx->clear();
        ui->textEdit_rx->append(str);

    }
    else
    {
        QString str =ui->textEdit_rx->toPlainText().toLocal8Bit();
        QByteArray buff = dd->convertStringToHex(str);
        str.clear();
        str.prepend(buff);
        ui->textEdit_rx->clear();
        ui->textEdit_rx->append(str);


    }
}


//bool MainWindow::event(QEvent *event)
//{
//    if(event->type() == QEvent::NonClientAreaMouseButtonRelease)
//    {
//        update_size(); //调用绘制窗口大小的函数
//        return true;  //返回true该事件在此处被处理后，不用传到父类
//    }
//    else if(event->type() == QEvent::Resize)
//    {

//        /*更新此时的页面宽度和长度*/
//        screen_width[0] = this->geometry().width();
//        screen_height[0] = this->geometry().height();
//        return true;
//    }
//    else
//    {

//        return QWidget::event(event);
//    }

//}


//void MainWindow::update_size()
//{
//     //设置一个大小为2的数组，一个用来存新值，一个用来存旧值，通过比较判断为页面放大操作还是缩小操作
//     if(screen_height[0]>screen_height[1] || screen_width[0]>screen_width[1])
//     {
//        this->resize((screen_height[0]>screen_width[0]?screen_height[0]:screen_width[0])-10,(screen_height[0]>screen_width[0]?screen_height[0]:screen_width[0])-10);

//     }
//     if(screen_height[0]<screen_height[1] || screen_width[0]<screen_width[1])
//     {
//         this->resize((screen_height[0]<screen_width[0]?screen_height[0]:screen_width[0])-10,(screen_height[0]<screen_width[0]?screen_height[0]:screen_width[0])-10);

//     }
//     screen_width[1] = screen_width[0];
//     screen_height[1] = screen_height[0];

//}

void MainWindow::on_btn_connect_clicked()
{
    if (btn_on_off == 0)
    {

        ui->cbx_setPortName->setEnabled(false);
        ui->cbx_setBaudRate->setEnabled(false);
        ui->cbx_setDataBits->setEnabled(false);
        ui->cbx_setStopBits->setEnabled(false);
        ui->cbx_setParity->setEnabled(false);
        ui->btn_open_close->setText("close serialport");
        ui->btn_connect->setIcon(QIcon(":/duankailianjie.png"));
             ui->btn_connect->setToolTip("断开连接");
        qDebug()<<"打开串口:";
        serialPortInit();
    }

   if (btn_on_off == 1)
   {

       serialport->close();
       ui->cbx_setPortName->setEnabled(true);
       ui->cbx_setBaudRate->setEnabled(true);
       ui->cbx_setDataBits->setEnabled(true);
       ui->cbx_setStopBits->setEnabled(true);
       ui->cbx_setParity->setEnabled(true);
       ui->btn_open_close->setText("open serialport");
         ui->btn_connect->setIcon(QIcon(":/lianjie.png"));
              ui->btn_connect->setToolTip("连接");
   }
   btn_on_off = !btn_on_off;
}

void MainWindow::on_btn_refresh_clicked()
{
     scanSerialPort();
}

//void MainWindow::on_PBT_Open_clicked()
//{
//    if(video_flag==0)
//    {
//            thread1->flag=1;
//            thread1->start();
//            thread2->flag=1;
//            thread2->start();
//            ui->PBT_Open->setIcon(QIcon(":/tingzhi.png"));
//             ui->PBT_Open->setToolTip("停止");
//             video_flag=1;
//    }
//    if(video_flag==1)
//    {
//        thread1->flag=0;
//        thread1->exit();
//        ui->label_1->clear();
//        thread2->flag=0;
//        thread2->exit();
//        ui->label_2->clear();
//        ui->PBT_Open->setIcon(QIcon(":/bofang.png"));
//        ui->PBT_Open->setToolTip("播放");
//        video_flag=0;
//    }
//}
