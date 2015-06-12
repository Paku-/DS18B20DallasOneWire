/*
 * DS18B20(!) OneWire sensor DEMO
 *
 * Copyright © 2015  Paku
 *
 * It may need some modifications for other DS18x20 family chips!
 * ATmega 32 and 328p (16MHz) ready
 * For other modifications:
 * - consult registers names in UART files
 * - USART_init(x) call in this file
 * - output pin configuration in dallas_one_wire.* files.).
 *
 * dallas_one_wire 	- Copyright © 2010, Mike Roddewig
 * with little modifications from me.
 *
 * Build on Atmel AVR8 GCC toolchain 3.4.1061
 *
 * This is a free software, under MIT License.
 *
 * Have fun,
 * Paku
 *
 *
 * Expected UART output (id code starting with 0x28h indicates DS18B20 chip)

    * One-Wire Bus Master Monitor *
	> Bus Test OK
	28:FF:B0:C7:xx:xx:xx:xx
	> IDs collecting Finished
 	DS18B20 Temp: 0xFFBEh -4.1250
	DS18B20 Temp: 0xFFD5h -2.6875
	DS18B20 Temp: 0xFFD5h -2.6875
	DS18B20 Temp: 0xFFE9h -1.4375
	DS18B20 Temp: 0xFFE9h -1.4375
	DS18B20 Temp: 0xFFF9h -0.4375
	DS18B20 Temp: 0xFFF9h -0.4375
	DS18B20 Temp: 0x0007h +0.4375
	DS18B20 Temp: 0x0007h +0.4375
	DS18B20 Temp: 0x0014h +1.2500
	DS18B20 Temp: 0x0014h +1.2500
	DS18B20 Temp: 0x001Eh +1.8750
	DS18B20 Temp: 0x001Eh +1.8750
	DS18B20 Temp: 0x0028h +2.5000
	DS18B20 Temp: 0x0028h +2.5000
	DS18B20 Temp: 0x0030h +3.0000
	DS18B20 Temp: 0x0030h +3.0000

 *
 */
#include "dallas_paku_lib.h"
#include "USART.h"
#include "lcd_lib.h"


const uint8_t LCDtitle[] PROGMEM="1Wire Bus Master\0";
const uint8_t LCDbusInit[] PROGMEM="  BUS INIT ...  \0";


int main(void) {

	/*
	 * UI variables
	 */

	char ticker_[8]={'|','/','-','\\','|','/','-','\\'};
	uint8_t ticker=0;
	uint16_t count=0;
	char line[30];


	/*
	 * OneWire Variables
	 */
	uint8_t chip_scratchpad[9];
	DALLAS_TEMPERATURE temp;


	LCDinit();
	CopyStringtoLCD(LCDtitle, 0, 0);
	CopyStringtoLCD(LCDbusInit, 0, 1);

	USART_vInit();


	//set stdout to USART
	FILE stream = FDEV_SETUP_STREAM(USART_vSendByte,NULL, _FDEV_SETUP_WRITE);
	stdout=&stream;


	printf("\r\n* One-Wire Bus Master Monitor *\r\n");

	//search bus and UART.printf slave ID
	search_bus();


	_delay_ms(1000);
	LCDclr();

	while (1) {

		//DEMO: ONE SLAVE ONLY !!!
		if (dallas_command(SKIP_ROM_COMMAND, 1)) {

			dallas_command(CONVERT_TEMP__COMMAND, 0);
			_delay_ms(500); //wait for conversion result.

			if (dallas_command(SKIP_ROM_COMMAND, 1)) {
				dallas_command(READ_SCRATCHPAD_COMMAND, 0);
				dallas_read_buffer(chip_scratchpad, 9);

				temp=getDallasTemp(chip_scratchpad[1],chip_scratchpad[0]);

				//create output string
				sprintf(line,"%c%5d Temp: 0x%02X%02Xh %c%d.%04d\r\n",ticker_[ticker++%8],count++,chip_scratchpad[1],chip_scratchpad[0],temp.sign,temp.integer,temp.fraction);

				//printf("%c Temp: 0x%02X%02Xh %c%d.%04d\r\n",ticker_[ticker%8],scratchpad[1],scratchpad[0],temp.sign,temp.integer,temp.fraction);
				//remove '\n' to get one line output

				//send it over USART/stdout
				printf("%s",line);

				//send to LCD as well

				//backslash as user defined char for non-european LCD ROMS.
				if (line[0]=='\\'){
					//put the user defined character 0 into first position = backslash
					//backslash is defined inside LCDinit() by default (under code 0x07)
					line[0]=0x07;
				}

				LCDgotoXY(10,0);
				LCDstring((uint8_t *)line,6);


				//show substring (x,x+16) on LCD
				LCDgotoXY(0,1);
				LCDstring((uint8_t *)line+13,14);

				//display °C
				LCDgotoXY(14,1);
				//degree sign = °
				LCDsendChar(0x06);
				LCDgotoXY(15,1);
				LCDsendChar('C');


			}else
				printf("BUS Error: READ_SCRATCHPAD\r\n");
		} else
			printf("BUS Error: No slaves found\r\n");

		_delay_ms(100);

	}

	return 0;

}

