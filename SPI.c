/*
 * SPI.c
 *
 * Created: 17.05.2026 12:44:32
 *  Author: admin
 */ 

#include "SPI.h"
 
void SPI_init()
{
	DDRB |=_BV(MOSI)|_BV(SCK)|_BV(SS);//ножки SPI на выход
	PORTB |=_BV(SS);
	DDRC |=_BV(CS);//ножки SPI на выход
	//PORTB &=~(_BV(MOSI)|_BV(SCK));//низкий уровень
	CS_HIGH();//высокий уровень
	SPCR |= _BV(SPE)|_BV(MSTR)|_BV(SPR0)/*|_BV(SPR1)*/;//включим шину, объ§вим ведущим
}

 void SPI_SendByte (char byte)
 {
 	SPDR = byte;
 	while(!(SPSR & (1<<SPIF)));//подождем пока данные передадутс€
 }

uint8_t SPI_ChangeByte (uint8_t byte)
{
	SPDR = byte;
	while(!(SPSR & _BV(SPIF)));//подождем пока данные передадутс€ (обмен€ютс€)
	
	return SPDR;
}

// uint16_t Read_Ex_MCP3208 (uint8_t channel)
// {
// 	unsigned int b1,b2;
// 	PORTC &= ~ _BV(CS); //низкий уровень
// 	b1=SPI_ChangeByte(0);//первый байт
// 	b2=SPI_ChangeByte(0);//второй байт
// 	b1=(b1<<8)|b2;//собираем два байта в двухбайтовую величину
// 	b1<<=3;//убираем ненужные биты (3 слева и 1 справа
// 	b1>>=4;
// 	PORTC |= _BV(CS); //высокий уровень
// 	return b1;
// }

uint16_t Read_MCP3208(uint8_t ch)
{
	 uint8_t byte,data_high,data_low;

 	 byte=0b00000110;
 	 if(ch>3)
 	 byte|=0b00000001;
	//	 CS_HIGH(); 
   CS_LOW();//низкий уровень	
 	// _delay_ms(5);
      SPI_SendByte(byte);	
// 
   	 byte=ch<<6;
   	 data_high=SPI_ChangeByte(byte);
   	 data_high&=0b00001111;
   	 data_low=SPI_ChangeByte(0xFF);
// 
  	 CS_HIGH();//высокий уровень
	 return ((data_high<<8)|data_low);
}
