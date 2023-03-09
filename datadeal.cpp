#include "datadeal.h"


//字符串转16进制函数
QByteArray DataDeal::convertStringToHex(QString str)
{
    QByteArray senddata;
            int hexdata,lowhexdata;
            int hexdatalen = 0;
            int len = str.length();
            senddata.resize(len/2);
            char lstr,hstr;
            for(int i=0; i<len; )
            {
                hstr=str[i].toLatin1();   //字符型
                if(hstr == ' ')
                {
                    i++;
                    continue;
                }
                i++;
                if(i >= len)
                    break;
                lstr = str[i].toLatin1();
                hexdata = convertCharToHex(hstr);
                lowhexdata = convertCharToHex(lstr);
                if((hexdata == 16) || (lowhexdata == 16))
                    break;
                else
                    hexdata = hexdata*16+lowhexdata;
                i++;
                senddata[hexdatalen] = (char)hexdata;
                hexdatalen++;
            }
            senddata.resize(hexdatalen);
            return senddata;


}

char DataDeal::convertCharToHex(char ch)
{
    if((ch >= '0') && (ch <= '9'))
                 return ch-0x30;
             else if((ch >= 'A') && (ch <= 'F'))
                 return ch-'A'+10;
             else if((ch >= 'a') && (ch <= 'f'))
                 return ch-'a'+10;
             else return (-1);

}

 QString DataDeal::ShowHex(QByteArray str)
 {
     QDataStream out(&str,QIODevice::ReadWrite);   //将str的数据 读到out里面去
          QString buf;
          while(!out.atEnd())
          {
                qint8 outChar = 0;
                out >> outChar;   //每次一个字节的填充到 outchar
                QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0')).toUpper() + QString(" ");   //2 字符宽度
                buf += str;
          }
          return buf;


 }
 QString DataDeal::Convert4Hex(QByteArray str)
  {
      QDataStream out(&str,QIODevice::ReadWrite);   //将str的数据 读到out里面去
      QString buf;
      while(!out.atEnd())
      {
            qint16 outChar = 0;
            out>>outChar;   //每次一个字的填充到 outchar
            QString str = QString("%1").arg(outChar&0xFFFF,4,16,QLatin1Char('0')).toUpper() + QString(" ");   //4 字符宽度
            buf += str;
      }
      return buf;
  }

 uint8_t DataDeal::float_to_uint8(float num, uint8_t bit)
 {
     uint8_t* temp;

     temp = (uint8_t *) &num;
     return  *(temp + bit);
 }
 float DataDeal::uint8_to_float(uint8_t* p)
 {
     float temp;
     *((uint8_t*)&temp) = *p;
     *((uint8_t*)&temp+1) = *(p+1);
     *((uint8_t*)&temp+2) = *(p+2);
     *((uint8_t*)&temp+3) = *(p+3);
     return temp;
 }





