/*
 * DS18B20(!) OneWire sensor
 *
 * Copyright © 2015  Paku
 *
 * It may not work for other DS18x20 family chips!
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


#include <util/delay.h>
#include <stdio.h>
#include "USART.h"
#include "dallas_one_wire.h"



//Real number like representation of dallas two byte temp value
typedef struct {
	char sign;
	uint8_t integer;
	uint16_t fraction;
} DALLAS_TEMP;

char ticker_[8]={'|','/','-','\\','|','/','-','\\'};

//Slaves list
DALLAS_IDENTIFIER_LIST_t * ids;

//Scratchpad
uint8_t scratchpad[9];

//Converts Dallas two byte temperature into real like structure
DALLAS_TEMP getDallasTemp(uint8_t msb, uint8_t lsb) {

/*
 *chip precision definition
 *11 bits temp converter mode
 *4 LSBs value stands for 0.0625 deg Centigrades .
 *so 0010 means: 0.0625x2 on fractional part.
 */
#define PRECISION 625

	DALLAS_TEMP temp;
	temp.sign='+';

	uint16_t result;

	//put lsb and msb into int16
	result = msb;
	result = ((result << 8) | lsb);

	//test if temperature is negative then process data
	if (result & 0xF800) {
		result = ~result + 1;
		temp.sign='-';
	}

	//drop 4 lsb bits for integral part
	temp.integer = (result >> 4);
	//use 4 lsb bits for decimal part
	temp.fraction = (result & 0x000F)*PRECISION;

	return temp;
}


void search_bus() {

	uint8_t i;

	if (dallas_reset()) {

		switch (dallas_search_identifiers()) {
		case 0x00:
			printf("> Bus Test - OK\r\n");
			break;
		case 0x01:
			printf("> Error!!! Buss error\r\n");
			break;
		case 0x02:
			printf("> Error!!! More devices then specified\r\n");
			break;
		default:
			printf("> Error!!! Unknown INIT message\r\n");
			break;
		}

		ids = get_identifier_list();

		//Output IDs found.
		for (i = 0; i < DALLAS_NUM_IDENTIFIER_BITS / 8; i++) {
			printf("%X",ids->identifiers[0].identifier[i]);
			if (i == 7)
				printf("\r\n");
			else
				printf(":");
		}

		printf("> IDs collecting  finished\r\n");

	} else
		printf("BUS Error: No slaves found\r\n");

	return;
}


int main(void) {

	DALLAS_TEMP temp;

	//output ticker
	uint8_t ticker=0;

	USART_vInit();

	//set stdout to USART
	FILE stream = FDEV_SETUP_STREAM(USART_vSendByte,NULL, _FDEV_SETUP_WRITE);
	stdout=&stream;

	printf("\r\n* One-Wire Bus Master Monitor *\r\n");

	search_bus();

	while (1) {

		//DEMO: ONE SLAVE ONLY !!!
		if (dallas_command(SKIP_ROM_COMMAND, 1)) {

			dallas_command(CONVERT_TEMP__COMMAND, 0);
			_delay_ms(500); //wait for conversion result.

			if (dallas_command(SKIP_ROM_COMMAND, 1)) {
				dallas_command(READ_SCRATCHPAD_COMMAND, 0);
				dallas_read_buffer(scratchpad, 9);

				temp=getDallasTemp(scratchpad[1],scratchpad[0]);

				printf("%c Temp: 0x%02X%02Xh %c%d.%04d\r\n",ticker_[ticker%8],scratchpad[1],scratchpad[0],temp.sign,temp.integer,temp.fraction);
				//remove '\n' to get one line output


			}
		} else
			printf("BUS Error: No slave respond\r\n");

		_delay_ms(100);
		ticker++;

	}

	return 0;

}

