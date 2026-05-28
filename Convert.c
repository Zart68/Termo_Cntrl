/*
 * Convert.c
 *
 * Created: 22.05.2025 8:35:43
 *  Author: admin
 */ 
#include "Convert.h"

void Power_level_conv(uint8_t level)
{
	
	switch (level)
	{
		case 0:_power_level=120;break;//minimum 148
		case 5:_power_level=127;break;
		case 10:_power_level=134;break;
		case 15:_power_level=141;break;
		case 20:_power_level=148;break;
		case 25:_power_level=155;break;
		case 30:_power_level=162;break;
		case 35:_power_level=169;break;
		case 40:_power_level=176;break;
		case 45:_power_level=183;break;
		case 50:_power_level=190;break;
		case 55:_power_level=197;break;
		case 60:_power_level=204;break;
		case 65:_power_level=211;break;
		case 70:_power_level=218;break;
		case 75:_power_level=225;break;
		case 80:_power_level=232;break;
		case 85:_power_level=239;break;
		case 90:_power_level=246;break;
		case 95:_power_level=250;break;
		case 100:_power_level=255;break;//maximum
	}
	
}
void Power_level_reconv()
{
	switch(_power_level)
	{
		case 120: power_level=0;break;//minimum
		case 127: power_level=5;break;//
		case 134: power_level=10;break;//
		case 141: power_level=15;break;//
		case 148: power_level=20;break;//
		case 155: power_level=25;break;//
		case 162: power_level=30;break;//
		case 169: power_level=35;break;//
		case 176: power_level=40;break;//
		case 183: power_level=45;break;//
		case 190: power_level=50;break;//
		case 197: power_level=55;break;//
		case 204: power_level=60;break;//
		case 211: power_level=65;break;//
		case 218: power_level=70;break;//
		case 225: power_level=75;break;//
		case 232: power_level=80;break;//
		case 239: power_level=85;break;//
		case 246: power_level=90;break;//
		case 250: power_level=95;break;//
		case 255: power_level=100;break;//maximum
		
	}
	SetTask(Power_level_reconv);
}

