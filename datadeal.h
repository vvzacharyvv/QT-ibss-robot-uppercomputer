#ifndef DATADEAL_H
#define DATADEAL_H
#include<QString>
#include<QDataStream>
#include<QByteArray>

class DataDeal
{
public:
    QByteArray convertStringToHex(QString str);//字符串转16进制
    char convertCharToHex(char ch);//字符转16进制
    QString ShowHex(QByteArray str);//数据显示16进制
    QString Convert4Hex(QByteArray str);//将串口中的16进制数每两个字节合并显示出来
    uint8_t float_to_uint8(float num, uint8_t bit);
     float uint8_to_float(uint8_t* p);

};


#endif // DATADEAL_H
