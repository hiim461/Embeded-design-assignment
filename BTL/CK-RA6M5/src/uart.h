/*
 * uart.h
 *
 *  Created on: Sep 25, 2025
 *      Author: phu_h
 */

#ifndef UART_H_
#define UART_H_

typedef enum{
	TYPE_INIT,
	TYPE_SENSOR,
	TYPE_ID_0,
	TYPE_ID_1,
} ReadSensorState;
void uart_init (void);
void printUART(const char *str);
#endif /* UART_H_ */
