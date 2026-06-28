/*
 * Termo_Cntrl.c
 *
 * Created: 29.04.2025 6:33:22
 * Author : admin
 */ 


#include "main.h"

uint8_t EEMEM   e_t1_max ;//ïðåäåë òåìïåðàòóðû 1 â eeprom
uint8_t EEMEM   e_t2_max ;//ïðåäåë òåìïåðàòóðû 2 â eeprom
uint8_t EEMEM   e_t1_time ;//óñò-êà âðåìåí òåìï. ïàóçû 1 â eeprom
uint8_t EEMEM   e_t2_time ;//óñò-êà âðåìåí òåìï. ïàóçû 2 â eeprom
uint8_t EEMEM   e_t1_pause ;//óñò-êà òåìï. ïàóçû 1 â eeprom
uint8_t EEMEM   e_t2_pause ;//óñò-êà òåìï. ïàóçû 2 â eeprom

/**äëÿ äåëåíèÿ, ÷òîáû äîñòè÷ü 50 ìñ, ïîòîìó ÷òî òàéìåð ïåðåïîëíÿåòñÿ êàæäóþ 1 ìñ*/
static uint16_t divider = DIVIDER_RELOAD;
/**Äîïîëíèòåëüíîå çíà÷åíèå äëÿ ðåãèñòðà ñðàâíåíèÿ */
//#define COMPADD             225
uint8_t counter = 0;//

//RTOS Interrupt

 ISR(TIMER0_OVF_vect)//êàæäóþ 1ìñ
 {	  
	    TimerService();
		Enable_Interrupt;
 }


ISR(INT0_vect)//ìîìåíò ïðîõîæäåíèÿ ÷åðåç 0
{ 	
	TCNT2 =_power_level; //	ïðèñâàåâàåì ðåãèñòðó çíà÷åíèå ìîùíîñòè 120 min, 0xFF max	
	TCCR2B |= _BV(CS20)|_BV(CS22)|_BV(CS21); //çàïóñêàåì òàéìåð2 (in Normal Mode)/1024	
	sei();
}
//----------------------------------------
ISR(TIMER2_OVF_vect)//ïðåðûâàíèå ïî ïåðåïîëíåíèþ çà 0,016 ñåê
{
	if(divider>0)
	--divider;// /2
	else //0,032 ñåê
	{
		PORTC |=_BV(ScrPort);//îòêðûâàþùèé èìïóëüñ
		_delay_us(t_pulse_triac);	
		PORTC &=~ _BV(ScrPort);//
		TCCR2B &=~ _BV(CS20)|_BV(CS22)|_BV(CS21);//îñòàíîâêà òàéìåðà
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
	case 8:   PORTC &=~_BV(MixPin);mixer=0; break;//âûêëþ÷åíèå ìèêñåð
	case 81:  PORTC |= _BV(MixPin);mixer=1; break;//âêëþ÷åíèå ìèêñåð
	case 7:   {PORTC &=~_BV(PumpPin);pump=0; }break;//âûêëþ÷åíèå íàñîñ
	case 71:  {PORTC |= _BV(PumpPin);pump=1;} break;//âêëþ÷åíèå íàñîñ
	case 9:   {PORTD |= _BV(WaterPin);water=1;}break;//âêëþ÷åíèå âîäà
	case 91:  {PORTD &=~_BV(WaterPin);water=0;}break;//âûêëþ÷åíèå âîäà
	case 72:  {PORTD |=_BV(CoolerPin);cooler=1;}break;//âêëþ÷åíèå êóëåð ìèêñåðà
	case 73:  {PORTD &=~_BV(CoolerPin);cooler=0;}break;//âûêëþ÷åíèå êóëåð ìèêñåðà
	case 3:   mode = 3;break;//íàãðåâ
	case 1:   mode = 1;break;//äèñòèëëÿöèÿ
	//case 6:   {}break;//ñîåäèíåíèå ðàçîðâàíî
	//case 11:  PORTC |= _BV(Valve_pin); break;//êëàïàí âêëþ÷åí	
	//case 12:  PORTC &=~ _BV(Valve_pin); break;//êëàïàí îòêëþ÷åí	
	case 13:  timer1_start();break;//ïåðåñòàëüòè÷åñêèé íàñîñ âêëþ÷åí
	case 14:  timer1_stop();  break;//ïåðåñòàëüòè÷åñêèé íàñîñ îòêëþ÷åí
	case 15:  {PORTB |=_BV(PrstPumpStp);DDRB |=_BV(PrstPumpStp);} break;//ïåðåñòàëüòè÷åñêèé íàñîñ ïàóçà
	case 16:  {PORTB &=~ _BV(PrstPumpStp);DDRB &=~ _BV(PrstPumpStp);} break;//ïåðåñòàëüòè÷åñêèé íàñîñ îòìåíà ïàóçû 
	case 17:  {
				timer1_start();
				if (prst_work < 990)
				{
					prst_work = prst_work + 10 ;//èçìåíåíèå ñêâàæíîñòè ïåðåñòàëüòèêè
					prst_pause = period - prst_work;
					prst_spd_value = prst_work/10;//çíà÷åíèå â ïðîöåíòàõ
					rx_flag = 0;
				}
				
			  }break;
	case 18:  {
		       if(prst_work >10)
			   {
				   prst_work = prst_work - 10 ;//èçìåíåíèå ñêâàæíîñòè ïåðåñòàëüòèêè
		           prst_pause = period - prst_work;
		           prst_spd_value = prst_work/10;//çíà÷åíèå â ïðîöåíòàõ
				   rx_flag = 0;
				}
				else
				{
				  timer1_stop();
				}
	          }break;	
	case 19:	{
		            _prst_work = prst_work;//ñîõðàíÿåì ïðåäûäóùåå çíà÷åíèå
					prst_work = 999;//ðàçãîí
					prst_pause = period - prst_work;
					prst_spd_value = 100;//çíà÷åíèå â ïðîöåíòàõ
					rx_flag = 0;
				}break;	
	case 20:	{
		            prst_work = _prst_work;//âîçâðàùàåì ïðåäûäóùåå çíà÷åíèå
					prst_pause = period - prst_work;
					prst_spd_value = prst_work/10;//çíà÷åíèå â ïðîöåíòàõ
					_prst_work = 0;
					rx_flag = 0;
				}break;			  	   
	}
	
	SetTask(Main_task);
}
void Port_init()
{		
		DDRC |=_BV(MixPin)|_BV(PumpPin)/*|_BV(Valve_pin)*/|_BV(ScrPort);//íàñîñ, ìèêñåð,êëàïàí îòáîðà - íà âûõîä
		PORTC &=~ (_BV(MixPin)|_BV(PumpPin)|_BV(ScrPort))/*|_BV(Valve_pin)*/;//íèçêèé óðîâåíü
		PORTD |=_BV(LevelPin);//
		
		//PORTA |=_BV(ErrorPin);
		//DDRA |=_BV(ErrorPin);
		PORTB |=_BV(OnPin)|_BV(On_OffPin);//on/off
		DDRB |=_BV(OnPin)/*|_BV(PrstPumpPin)*//*|_BV(PrstPumpStp)*/; //on/off	íà âûõîä
		
		DDRD |=_BV(WaterPin)|_BV(CoolerPin)|_BV(ZummerPin);//íà âûõîä
		PORTD |=_BV(ZeroCross);//INT0 íà âõîä		
}

// void Ana_comp_init()
// {
// 	ACSR |= _BV(ACBG)/*|_BV(ACIS1)|_BV(ACIS0)*/;//ïîäêëþ÷åíèå âíóòðåííåãî èñòî÷íèêà îïîðíîãî íàïðÿæåíèÿ (ÈÎÍ) 
// 	                //íà 1.23 Â ê ïîëîæèòåëüíîìó âõîäó êîìïàðàòîðà (AIN0)		
// 	//ACSR |=_BV(ACIE);	//ðàçðåøåíèå ïðåðûâàíèé êîìïàðàòîðà
//}

void Perepherial_init()
{      						
		TIMSK2 |=_BV(TOIE2);//TIMER2_OVF_vect enable
		EIMSK |= _BV(INT0);//INT0 enable
		EICRA |= _BV(ISC00);//INT0 ïðåðûâàíèå ïî ïî èçìåíåíèþ óðîâíÿ	
					
		//MCUCR		
		//TCCR0A &=~(_BV(WGM00)|_BV(WGM01));//Normal
		//MCUCR |=_BV(IVCE);//		
}

 void Change_mode()
{
 	if(PINC&_BV(StPin)){ I2C_SendByteByADDR(Blue_warn/*&Green_warn*/,pcf8574_addr);}//èíäèêàöèÿ ïîäêëþ÷åíèÿ ïî Bluetooth
 	else {I2C_SendByteByADDR(Blue_warn|0x80,pcf8574_addr);}//ãàñèì èíäèêàöèþ 
 	//if ((PINA&_BV(PA4)))
 	//{
		// I2C_SendByteByADDR(Green_warn,pcf8574_addr);
		// Read_pressure();// çàìåð äàâëåíèÿ      
 	//}
	// else I2C_SendByteByADDR(Green_warn|0x02,pcf8574_addr);//ãàñèì èíäèêàöèþ 
	 switch (mode)
 	{
  		case 1:break;// ðåæèì äèñòèëëÿöèè
  		case 2:break;// ðåæèì ïèâîâàðåíèÿ
  		case 3:// ðåæèì íàãðåâà
  		{
          if(temp1>=(t1_max*10)) signal_flag = 3;
 		 if(signal_flag == 3)
		 {
 			 I2C_SendByteByADDR(Yell_warn,pcf8574_addr);//èíäèêàöèÿ ñîáûòèÿ 		 
 			 _power_level =120;// îòêëþ÷åíèå íàãðåâà
 		 }
 		 else I2C_SendByteByADDR(Yell_warn|0x08,pcf8574_addr);//ãàñèì èíäèêàöèþ
 		}break;
 	}
	//if(adc_measure_flag) Pressure_level_conv();
 	SetTimerTask(Change_mode,1000);
 }


int main(void)
{
	// k = 4;
	
	 InitRTOS();		//Èíèöèàëèçèðóåì ÿäðî
	 RunRTOS();		    //Ñòàðò ÿäðà.
	 uart_init(MYUBRR); //Èíèöèàëèçèðóåì uart 
	 ADC_init();        //Èíèöèàëèçèðóåì ADC
	 Perepherial_init();//Èíèöèàëèçèðóåì ïåðåôåðèþ
	 Port_init();       //Èíèöèàëèçèðóåì ïîðòû
	 //Ana_comp_init();   //Èíèöèàëèçèðóåì àíàëîãîâûé êîìïàðàòîð
	 I2C_Init();        //Èíèöèàëèçèðóåì I2C
	 LCD_ini();          //Инициализируем дисплей
	 clearlcd();         //Очищаем дисплей
	// SPI_init();        //Èíèöèàëèçèðóåì SPI
    // ACSR |=_BV(ACD);   //çàïðåùàåì ðàáîòó àíàëîãîâîãî êîìïàðàòîðà
  // //tx_data_write();    //çàïèñü äàííûõ â áóôåðå ïåðåäà÷è
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
	 corr = 102;//êîððåêöèÿ çíà÷åíèÿ òåìïåðàòóðû
	 _power_level = 120;//120 0% ñòàðòîâîå çíà÷åíèå óðîâíÿ ìîùíîñòè(120)
	 period = 1000;//ïåðèîä ØÈÌ ïåðåñòàëüòèêè 0x3E8
	 prst_work = 100;//
	 prst_pause = period - prst_work;//
	 prst_spd_value = prst_work/10;//çíà÷åíèå â ïðîöåíòàõ
	// adc_measure_flag = 0;
    
	//îñíîâíûå îïåðàöèè	
	//timer1_stop();
	
	Zummer_on();       //Çâóêîâàÿ èíäèêàöèÿ ïðè âêëþ÷åíèè
	
	rx_data_read();     //÷òåíèå äàííûõ èç áóôåðà ïðè¸ìà
	
	Power_level_reconv();
	
 	Read_temp();// çàìåð òåìïåðàòóð   	           
	
	Read_pressure(); // çàìåð äàâëåíèÿ
	
//	Read_signal_codes(); //÷òåíèå êîäà àâàðèè
		
	Read_keys();//
	
	uart_send_msg();//
	
	Main_task();//
			
//	Change_mode();
	
	sei();
	
    while (1) 
    {    
			wdt_reset();	// Ñáðîñ ñîáà÷üåãî òàéìåðà
			TaskManager();	// Âûçîâ äèñïåò÷åðà					
    }
}

