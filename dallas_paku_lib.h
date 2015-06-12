/*
 * DS18B20(!) OneWire sensor
 * Copyright © 2015  Paku
 *
 * Dallas utility functions & defs
 */

#include <avr/pgmspace.h>
#include <stdio.h>
#include <util/delay.h>
#include "dallas_one_wire.h"

/*
 *chip precision definition
 *11 bits temp converter mode
 *4 LSBs value stands for 0.0625 deg Centigrades .
 *so 0010 means: 0.0625x2 on fractional part.
 */
#define PRECISION 625


//Real number like representation of dallas two byte temp value
typedef struct {
	char sign;
	uint8_t integer;
	uint16_t fraction;
} DALLAS_TEMPERATURE;


//Converts Dallas two byte temperature into real like structure
DALLAS_TEMPERATURE getDallasTemp(uint8_t msb, uint8_t lsb);
//search bus for slaves
void search_bus();

