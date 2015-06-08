/*
 * USART.h
 *
 *  Created on: 8 06 2015
 *      Author: paku
 *
 * This is a free software, under MIT License.
 *
 */

#ifndef USART_H_
#define USART_H_

#include <stdlib.h>
#include <avr/io.h>

void USART_vInit(void);
void USART_vSendByte(uint8_t u8Data);
uint8_t USART_vReceiveByte();



#endif /* USART_H_ */
