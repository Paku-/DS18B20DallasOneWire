/*
 * dallas_paku_lib.c
 *
 * DS18B20(!) OneWire sensor
 * Copyright © 2015  Paku
 *
 *
 * Some utility functions used in 1wire demo
 *
 */
#include "dallas_paku_lib.h"



/*
 * Utility functions
 */

//Converts Dallas two byte temperature into real like structure
DALLAS_TEMPERATURE getDallasTemp(uint8_t msb, uint8_t lsb) {

	DALLAS_TEMPERATURE temp;
	temp.sign='+';

	uint16_t result;

	//put lsb and msb into int16
	result = msb;
	result = ((result << 8) | lsb);

	//test if temperature is negative then process data
	if (result & 0xF800) {
		//little magic: to convert negative temps you just have to make ...
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

	//Slaves list
	DALLAS_IDENTIFIER_LIST_t * ids;


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



