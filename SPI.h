/*
 * SPI.h
 *
 * Created: 17.05.2026 12:44:53
 *  Author: admin
 */ 


#ifndef SPI_H_
#define SPI_H_

#include "main.h"

void SPI_init();
uint16_t Read_MCP3208(uint8_t ch);

#endif /* SPI_H_ */