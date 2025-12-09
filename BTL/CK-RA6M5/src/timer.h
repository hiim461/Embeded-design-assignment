/*
 * timer.h
 *
 *  Created on: 6 thg 12, 2025
 *      Author: NGUYEN - PC
 */

#ifndef TIMER_H_
#define TIMER_H_
#include <stdint.h>

void AGT0_Handler(void);
void timer_init(void);


uint32_t millis(void);


#endif /* TIMER_H_ */
