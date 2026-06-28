#ifndef LCDTWI_H_
#define LCDTWI_H_

#include "main.h"

//----------------------------------------
void LCD_ini(void);
void setpos(char x, unsigned y);
void str_lcd (char str1[]);
void clearlcd(void);
void sendcharlcd(unsigned char c);
void StrCopy(char* str ,char* target);
//----------------------------------------
//#define pcf8574_addr  0b01111110  //адрес расширителя
#define e1   I2C_SendByteByADDR(portlcd|=0x04,pcf8574_addr2_write) // установка линии E в 1
#define e0   I2C_SendByteByADDR(portlcd&=~0x04,pcf8574_addr2_write)  // установка линии E в 0
#define rs1    I2C_SendByteByADDR(portlcd|=0x01,pcf8574_addr2_write) // установка линии RS в 1
#define rs0   I2C_SendByteByADDR(portlcd&=~0x01,pcf8574_addr2_write)  // установка линии RS в 0
#define setled()    I2C_SendByteByADDR(portlcd|=0x08,pcf8574_addr2_write) // включение подсветки
#define setwrite()   I2C_SendByteByADDR(portlcd&=~0x02,pcf8574_addr2_write)  // установка записи в память дисплея
//----------------------------------------


#endif /* LCDTWI_H_ */