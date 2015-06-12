//*****************************************************************************
//
// File Name	: 'lcd_lib.c'
// Title		: 8 and 4 bit LCd interface
// Author		: Scienceprog.com - Copyright (C) 2007
// Created		: 2007-03-29
// Revised		: 2007-08-08
// Version		: 1.0
// Target MCU	: Atmel AVR series
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************
// Modified (c) 2015 Paku
//*****************************************************************************
#include "lcd_lib.h"

const uint8_t LCDcustomChar[] PROGMEM = //define max. 8 custom LCD chars here
			  { 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, // 0. 0/5 full progress block
				0x00, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x00, // 1. 1/5 full progress block
				0x00, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x00, // 2. 2/5 full progress block
				0x00, 0x1F, 0x1C, 0x1C, 0x1C, 0x1C, 0x1F, 0x00, // 3. 3/5 full progress block
				0x00, 0x1F, 0x1E, 0x1E, 0x1E, 0x1E, 0x1F, 0x00, // 4. 4/5 full progress block
				0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, // 5. 5/5 full progress block
				//0x03, 0x07, 0x0F, 0x1F, 0x0F, 0x07, 0x03, 0x00, // 6. rewind arrow
				//0x18, 0x1C, 0x1E, 0x1F, 0x1E, 0x1C, 0x18, 0x00, // 7. fast-forward arrow
				0x04,0x0a,0x0a,0x04,0x00,0x00,0x00,0x00, // degree sign [°]
				0x00,0x10,0x08,0x04,0x02,0x01,0x00,0x00 // back slash

		};

void LCDsendChar(uint8_t ch)		//Sends Char to LCD
{

#ifdef LCD_4bit
	//4 bit part
	LCD_DATAPORT = (ch & 0b11110000);
	LCD_CONTROLPORT |= 1 << LCD_RS;
	LCD_CONTROLPORT |= 1 << LCD_ENABLE;
	_delay_us(ENABLE_TIME);
	LCD_CONTROLPORT &= ~(1 << LCD_ENABLE);
	LCD_CONTROLPORT &= ~(1 << LCD_RS);
	_delay_us(CHAR_DATA_TIME);
	LCD_DATAPORT = ((ch & 0b00001111) << 4);
	LCD_CONTROLPORT |= 1 << LCD_RS;
	LCD_CONTROLPORT |= 1 << LCD_ENABLE;
	_delay_us(ENABLE_TIME);
	LCD_CONTROLPORT &= ~(1 << LCD_ENABLE);
	LCD_CONTROLPORT &= ~(1 << LCD_RS);
	_delay_us(CHAR_DATA_TIME);
#else
	//8 bit part
	LCD_DATAPORT=ch;
	LCD_CONTROLPORT|=1<<LCD_RS;
	LCD_CONTROLPORT|=1<<LCD_ENABLE;
	_delay_us(ENABLE_TIME);
	LCD_CONTROLPORT&=~(1<<LCD_ENABLE);
	LCD_CONTROLPORT&=~(1<<LCD_RS);
	_delay_us(CHAR_DATA_TIME);
#endif
}

void LCDexecute(){
	LCD_CONTROLPORT |= 1 << LCD_ENABLE;
	//min here is 450ns
	_delay_us(ENABLE_TIME);
	LCD_CONTROLPORT &= ~(1 << LCD_ENABLE);
	//most commands need min100us delay to be executed
	//in case you need more add it manually
	_delay_us(COMMAND_TIME);
}

void LCDsendCommand(uint8_t cmd)	//Sends Command to LCD
{
#ifdef LCD_4bit	
	//4 bit part

	LCD_DATAPORT = (cmd & 0b11110000);

	LCDexecute();

	LCD_DATAPORT = ((cmd & 0b00001111) << 4);

	LCDexecute();

#else
	//8 bit part
	LCD_DATAPORT=cmd;

	LCDexecute();
#endif
}

void LCDinit(void)	//Initializes LCD
{

	//power on delay
	_delay_ms(POWER_ON_DELAY);

	//setup data and control registers
	LCD_DDR |= 1 << LCD_D7 | 1 << LCD_D6 | 1 << LCD_D5 | 1 << LCD_D4;
	LCD_CDR |= 1 << LCD_ENABLE | 1 << LCD_RW | 1 << LCD_RS;

	//Issue MSB(0b0011xxxx) command 3 times. use proper timing
	LCD_DATAPORT = 0x00;
	LCD_CONTROLPORT = 0x00;
	LCD_DATAPORT = 0 << LCD_D7 | 0 << LCD_D6 | 1 << LCD_D5 | 1 << LCD_D4; //4 bit mode
	LCD_CONTROLPORT |= 0 << LCD_ENABLE | 0 << LCD_RW | 0 << LCD_RS;

	//1st pulse
	LCDexecute();
	_delay_ms(4); //wait more then 4.1ms

	//2nd pulse
	LCDexecute();

	//3rd pulse
	LCDexecute();


#ifdef LCD_4bit
	//4 bit mode

	//Function Set: change to 4bit mode
	//same call but without D4 bit
	LCD_DATAPORT &= ~(1 << LCD_D4); //4 bit mode
	LCDexecute();

	//4bit mode operation starts here
	//Function Set: 4 bit,dual line,5x8
	LCDsendCommand(
			(1 << LCD_FUNCTION) | (0 << LCD_FUNCTION_8BIT)
					| (1 << LCD_FUNCTION_2LINES) | (0 << LCD_FUNCTION_10DOTS));


#else
	//8-bit mode

	//Function Set: 8 bit,dual line,5x8
	LCDsendCommand(
			(1 << LCD_FUNCTION) | (1 << LCD_FUNCTION_8BIT)
			| (1 << LCD_FUNCTION_2LINES) | (0 << LCD_FUNCTION_10DOTS));

#endif

	//Display Control: disp Off,cursor off,blinking off
	LCDsendCommand(
			(1 << LCD_DISPLAY) | (0 << LCD_DISPLAY_ON) | (0 << LCD_CURSOR_ON)
					| (0 << LCD_BLINK_ON));

	//Clear Display
	LCDsendCommand(1 << LCD_CLR);
	_delay_ms(3); //special more then 3ms case

	//Entry Mode Set: I/D: On S:Off
	LCDsendCommand(
			(1 << LCD_ENTRY_MODE) | (1 << LCD_ENTRY_MODE_INC)
					| (0 << LCD_ENTRY_MODE_SHIFT));

	//End of Initialization , clear display now and switch it ON.

	//Display Control: disp ON,cursor off,blinking off
	LCDsendCommand(
			(1 << LCD_DISPLAY) | (1 << LCD_DISPLAY_ON) | (0 << LCD_CURSOR_ON)
					| (0 << LCD_BLINK_ON));


	//user defined chars definitions come here

	 //defines backslash char symbol in CGRAM memory
	 //LCDdefinechar(backslash,0);

	 //init 8 custom chars
	 uint8_t ch=0, chn=0;
	 while(ch<64)
	 {
	 LCDdefinechar((LCDcustomChar+ch),chn++);
	 ch=ch+8;
	 }



}
void LCDclr(void)				//Clears LCD
{
	LCDsendCommand(1 << LCD_CLR);
}
void LCDhome(void)			//LCD cursor home
{
	LCDsendCommand(1 << LCD_HOME);
}
void LCDstring(uint8_t* data, uint8_t nBytes)	//Outputs string to LCD
{
	register uint8_t i;

	// check to make sure we have a good pointer
	if (!data)
		return;

	// print data
	for (i = 0; i < nBytes; i++) {
		LCDsendChar(data[i]);
	}
}
void LCDgotoXY(uint8_t x, uint8_t y)	//Cursor to X Y position
{
	register uint8_t DDRAMAddr;
	// remap lines into proper order
	switch (y) {
	case 0:
		DDRAMAddr = LCD_LINE0_DDRAMADDR + x;
		break;
	case 1:
		DDRAMAddr = LCD_LINE1_DDRAMADDR + x;
		break;
	case 2:
		DDRAMAddr = LCD_LINE2_DDRAMADDR + x;
		break;
	case 3:
		DDRAMAddr = LCD_LINE3_DDRAMADDR + x;
		break;
	default:
		DDRAMAddr = LCD_LINE0_DDRAMADDR + x;
	}
	// set data address
	LCDsendCommand(1 << LCD_DDRAM | DDRAMAddr);

}
//Copies string from flash memory to LCD at x y position
//const uint8_t welcomeln1[] PROGMEM="AVR LCD DEMO\0";
//CopyStringtoLCD(welcomeln1, 3, 1);	
void CopyStringtoLCD(const uint8_t *FlashLoc, uint8_t x, uint8_t y) {
	uint8_t i;
	LCDgotoXY(x, y);
	for (i = 0; (uint8_t) pgm_read_byte(&FlashLoc[i]); i++) {
		LCDsendChar((uint8_t) pgm_read_byte(&FlashLoc[i]));
	}
}

void LCDdefinechar(const uint8_t *pc, uint8_t char_code) {
	uint8_t a, pcc;
	uint16_t i;
	a = (char_code << 3) | 0x40;
	for (i = 0; i < 8; i++) {
		pcc = pgm_read_byte(&pc[i]);
		LCDsendCommand(a++);
		LCDsendChar(pcc);
	}
}

void LCDshiftLeft(uint8_t n)	//Scrol n of characters Right
{
	for (uint8_t i = 0; i < n; i++) {
		LCDsendCommand(0x1E);
	}
}
void LCDshiftRight(uint8_t n)	//Scrol n of characters Left
{
	for (uint8_t i = 0; i < n; i++) {
		LCDsendCommand(0x18);
	}
}
void LCDcursorOn(void) //displays LCD cursor
{
	LCDsendCommand(0x0E);
}
void LCDcursorOnBlink(void)	//displays LCD blinking cursor
{
	LCDsendCommand(0x0F);
}
void LCDcursorOFF(void)	//turns OFF cursor
{
	LCDsendCommand(0x0C);
}
void LCDblank(void)		//blanks LCD
{
	LCDsendCommand(0x08);
}
void LCDvisible(void)		//Shows LCD
{
	LCDsendCommand(0x0C);
}
void LCDcursorLeft(uint8_t n)	//Moves cursor by n poisitions left
{
	for (uint8_t i = 0; i < n; i++) {
		LCDsendCommand(0x10);
	}
}
void LCDcursorRight(uint8_t n)	//Moves cursor by n poisitions left
{
	for (uint8_t i = 0; i < n; i++) {
		LCDsendCommand(0x14);
	}
}
//adapted fro mAVRLIB
void LCDprogressBar(uint8_t progress, uint8_t maxprogress, uint8_t length) {
	uint8_t i;
	uint16_t pixelprogress;
	uint8_t c;

	// draw a progress bar displaying (progress / maxprogress)
	// starting from the current cursor position
	// with a total length of "length" characters
	// ***note, LCD chars 0-5 must be programmed as the bar characters
	// char 0 = empty ... char 5 = full

	// total pixel length of bargraph equals length*PROGRESSPIXELS_PER_CHAR;
	// pixel length of bar itself is
	pixelprogress = ((progress * (length * PROGRESSPIXELS_PER_CHAR))
			/ maxprogress);

	// print exactly "length" characters
	for (i = 0; i < length; i++) {
		// check if this is a full block, or partial or empty
		// (u16) cast is needed to avoid sign comparison warning
		if (((i * (uint16_t) PROGRESSPIXELS_PER_CHAR) + 5) > pixelprogress) {
			// this is a partial or empty block
			if (((i * (uint16_t) PROGRESSPIXELS_PER_CHAR)) > pixelprogress) {
				// this is an empty block
				// use space character?
				c = 0;
			} else {
				// this is a partial block
				c = pixelprogress % PROGRESSPIXELS_PER_CHAR;
			}
		} else {
			// this is a full block
			c = 5;
		}

		// write character to display
		LCDsendChar(c);
	}

}
