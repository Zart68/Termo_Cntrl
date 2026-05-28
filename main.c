/*
 * Termo_Cntrl.c
 *
 * Created: 29.04.2025 6:33:22
 * Author : admin
 */ 


#include "main.h"

uint8_t EEMEM   e_t1_max ;//предел температуры 1 в eeprom
uint8_t EEMEM   e_t2_max ;//предел температуры 2 в eeprom
uint8_t EEMEM   e_t1_time ;//уст-ка времен темп. паузы 1 в eeprom
uint8_t EEMEM   e_t2_time ;//уст-ка времен темп. паузы 2 в eeprom
uint8_t EEMEM   e_t1_pause ;//уст-ка темп. паузы 1 в eeprom
uint8_t EEMEM   e_t2_pause ;//уст-ка темп. паузы 2 в eeprom

/**для деления, чтобы достичь 50 мс, потому что таймер переполняется каждую 1 мс*/
static uint16_t divider = DIVIDER_RELOAD;
/**Дополнительное значение для регистра сравнения */
//#define COMPADD             225
uint8_t counter = 0;//

//RTOS Interrupt

 ISR(TIMER0_OVF_vect)//каждую 1мс
 {	  
	    TimerService();
		Enable_Interrupt;
 }


ISR(INT0_vect)//момент прохождения через 0
{ 	
	TCNT2 =_power_level; //	присваеваем регистру значение мощности 120 min, 0xFF max	
	TCCR2B |= _BV(CS20)|_BV(CS22)|_BV(CS21); //запускаем таймер2 (in Normal Mode)/1024	
	sei();
}
//----------------------------------------
ISR(TIMER2_OVF_vect)//прерывание по переполнению за 0,016 сек
{
	if(divider>0)
	--divider;// /2
	else //0,032 сек
	{
		PORTC |=_BV(ScrPort);//открывающий импульс
		_delay_us(t_pulse_triac);	
		PORTC &=~ _BV(ScrPort);//
		TCCR2B &=~ _BV(CS20)|_BV(CS22)|_BV(CS21);//остановка таймера
	}
	sei();
}
//-----------------------------------------------
//---------------------------------
void Main_task()
{
	switch(rx_flag)
	{
 	//case 6: ;break;
// 	{
// 		_power_level=0xFF;
// 		power_level=0;
// 	}
	//case 5:   _power_level = power_level; break;
	case 8:   PORTC &=~_BV(MixPin);mixer=0; break;//выключение миксер
	case 81:  PORTC |= _BV(MixPin);mixer=1; break;//включение миксер
	case 7:   {PORTC &=~_BV(PumpPin);pump=0; }break;//выключение насос
	case 71:  {PORTC |= _BV(PumpPin);pump=1;} break;//включение насос
	case 9:   {PORTD |= _BV(WaterPin);water=1;}break;//включение вода
	case 91:  {PORTD &=~_BV(WaterPin);water=0;}break;//выключение вода
	case 72:  {PORTD |=_BV(CoolerPin);cooler=1;}break;//включение кулер миксера
	case 73:  {PORTD &=~_BV(CoolerPin);cooler=0;}break;//выключение кулер миксера
	case 3:   mode = 3;break;//нагрев
	case 1:   mode = 1;break;//дистилляция
	//case 6:   {}break;//соединение разорвано
	//case 11:  PORTC |= _BV(Valve_pin); break;//клапан включен	
	//case 12:  PORTC &=~ _BV(Valve_pin); break;//клапан отключен	
	case 13:  timer1_start();break;//перестальтический насос включен
	case 14:  timer1_stop();  break;//перестальтический насос отключен
	case 15:  {PORTB |=_BV(PrstPumpStp);DDRB |=_BV(PrstPumpStp);} break;//перестальтический насос пауза
	case 16:  {PORTB &=~ _BV(PrstPumpStp);DDRB &=~ _BV(PrstPumpStp);} break;//перестальтический насос отмена паузы 
	case 17:  {
				timer1_start();
				if (prst_work < 990)
				{
					prst_work = prst_work + 10 ;//изменение скважности перестальтики
					prst_pause = period - prst_work;
					prst_spd_value = prst_work/10;//значение в процентах
					rx_flag = 0;
				}
				
			  }break;
	case 18:  {
		       if(prst_work >10)
			   {
				   prst_work = prst_work - 10 ;//изменение скважности перестальтики
		           prst_pause = period - prst_work;
		           prst_spd_value = prst_work/10;//значение в процентах
				   rx_flag = 0;
				}
				else
				{
				  timer1_stop();
				}
	          }break;	
	case 19:	{
		            _prst_work = prst_work;//сохраняем предыдущее значение
					prst_work = 999;//разгон
					prst_pause = period - prst_work;
					prst_spd_value = 100;//значение в процентах
					rx_flag = 0;
				}break;	
	case 20:	{
		            prst_work = _prst_work;//возвращаем предыдущее значение
					prst_pause = period - prst_work;
					prst_spd_value = prst_work/10;//значение в процентах
					_prst_work = 0;
					rx_flag = 0;
				}break;			  	   
	}
	
	SetTask(Main_task);
}
void Port_init()
{		
		DDRC |=_BV(MixPin)|_BV(PumpPin)/*|_BV(Valve_pin)*/|_BV(ScrPort);//насос, миксер,клапан отбора - на выход
		PORTC &=~ (_BV(MixPin)|_BV(PumpPin)|_BV(ScrPort))/*|_BV(Valve_pin)*/;//низкий уровень
		PORTD |=_BV(LevelPin);//
		
		//PORTA |=_BV(ErrorPin);
		//DDRA |=_BV(ErrorPin);
		PORTB |=_BV(OnPin)|_BV(On_OffPin);//on/off
		DDRB |=_BV(OnPin)/*|_BV(PrstPumpPin)*//*|_BV(PrstPumpStp)*/; //on/off	на выход
		
		DDRD |=_BV(WaterPin)|_BV(CoolerPin)|_BV(ZummerPin);//на выход
		PORTD |=_BV(ZeroCross);//INT0 на вход		
}

// void Ana_comp_init()
// {
// 	ACSR |= _BV(ACBG)/*|_BV(ACIS1)|_BV(ACIS0)*/;//подключение внутреннего источника опорного напряжения (ИОН) 
// 	                //на 1.23 В к положительному входу компаратора (AIN0)		
// 	//ACSR |=_BV(ACIE);	//разрешение прерываний компаратора
//}

void Perepherial_init()
{      						
		TIMSK2 |=_BV(TOIE2);//TIMER2_OVF_vect enable
		EIMSK |= _BV(INT0);//INT0 enable
		EICRA |= _BV(ISC00);//INT0 прерывание по по изменению уровня	
					
		//MCUCR		
		//TCCR0A &=~(_BV(WGM00)|_BV(WGM01));//Normal
		//MCUCR |=_BV(IVCE);//		
}

 void Change_mode()
{
 	if(PINC&_BV(StPin)){ I2C_SendByteByADDR(Blue_warn/*&Green_warn*/,pcf8574_addr);}//индикация подключения по Bluetooth
 	else {I2C_SendByteByADDR(Blue_warn|0x80,pcf8574_addr);}//гасим индикацию 
 	//if ((PINA&_BV(PA4)))
 	//{
		// I2C_SendByteByADDR(Green_warn,pcf8574_addr);
		// Read_pressure();// замер давления      
 	//}
	// else I2C_SendByteByADDR(Green_warn|0x02,pcf8574_addr);//гасим индикацию 
	 switch (mode)
 	{
  		case 1:break;// режим дистилляции
  		case 2:break;// режим пивоварения
  		case 3:// режим нагрева
  		{
          if(temp1>=(t1_max*10)) signal_flag = 3;
 		 if(signal_flag == 3)
		 {
 			 I2C_SendByteByADDR(Yell_warn,pcf8574_addr);//индикация события 		 
 			 _power_level =120;// отключение нагрева
 		 }
 		 else I2C_SendByteByADDR(Yell_warn|0x08,pcf8574_addr);//гасим индикацию
 		}break;
 	}
	//if(adc_measure_flag) Pressure_level_conv();
 	SetTimerTask(Change_mode,1000);
 }


int main(void)
{
	// k = 4;
	
	 InitRTOS();		//Инициализируем ядро
	 RunRTOS();		    //Старт ядра.
	 uart_init(MYUBRR); //Инициализируем uart 
	 ADC_init();        //Инициализируем ADC
	 Perepherial_init();//Инициализируем переферию
	 Port_init();       //Инициализируем порты
	 //Ana_comp_init();   //Инициализируем аналоговый компаратор
	 I2C_Init();        //Инициализируем I2C
	// SPI_init();        //Инициализируем SPI
    // ACSR |=_BV(ACD);   //запрещаем работу аналогового компаратора
  // //tx_data_write();    //запись данных в буфере передачи
	mode = 0; 
	x1 = x2 = 0;
	ptemp = 0;
	t1_max = eeprom_read_byte(&e_t1_max);
 	t2_max = eeprom_read_byte(&e_t2_max);
	t1_time = eeprom_read_byte(&e_t1_time);
	t2_time = eeprom_read_byte(&e_t2_time); 
	t1_pause = eeprom_read_byte(&e_t1_pause);
	t2_pause = eeprom_read_byte(&e_t2_pause);  
// 		cooler = 1;
     //
	 corr = 102;//коррекция значения температуры
	 _power_level = 120;//120 0% стартовое значение уровня мощности(120)
	 period = 1000;//период ШИМ перестальтики 0x3E8
	 prst_work = 100;//
	 prst_pause = period - prst_work;//
	 prst_spd_value = prst_work/10;//значение в процентах
	// adc_measure_flag = 0;
    
	//основные операции	
	//timer1_stop();
	
	Zummer_on();       //Звуковая индикация при включении
	
	rx_data_read();     //чтение данных из буфера приёма
	
	Power_level_reconv();
	
 	Read_temp();// замер температур   	           
	
	Read_pressure(); // замер давления
	
//	Read_signal_codes(); //чтение кода аварии
		
	Read_keys();//
	
	uart_send_msg();//
	
	Main_task();//
			
//	Change_mode();
	
	sei();
	
    while (1) 
    {    
			wdt_reset();	// Сброс собачьего таймера
			TaskManager();	// Вызов диспетчера					
    }
}

