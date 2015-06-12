![alt tag](https://travis-ci.org/Paku-/DS18B20DallasOneWire.svg?branch=master)
# DS18B20DallasOneWire

Dallas/Maxim OneWire Library implementation for ATMEL 8-bit AVR platform.

- Chip tested is DS18B20 digital temperature sensor.

- UART and LCD output.
- ATmega32 and ATmega328p (16MHz) ready.

For your platform modifications consult:
 - output pin configuration and 1wire bus frame timing in dallas_one_wire.* files
 - registers names and timing in UART files
 - LCD configuration in LCD*.h file

This is a free software, under MIT License.

Copyright © 2015  Paku

Have fun,

Paku


dallas_one_wire library	- GNU Copyright © 2010, Mike Roddewig

LCD library - GNU Scienceprog.com - Copyright (C) 2007

(both libs modified)



##Expected UART output 

(id code starting with 0x28h indicates DS18B20 chip)

```
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
```
 
