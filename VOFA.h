/*
 * VOFA.h
 *
 *  Created on: 2020��8��14��
 *      Author: WHY
 */

#ifndef CODE_VOFA_H_
#define CODE_VOFA_H_
#include "sys.h"
#include "uart.h"
#include "24l01.h"
extern struct CreepMotionControl mc;
extern float my_data_buffer[6];
extern uint8_t Send_channels;

uint8_t float_to_uint8(float num, uint8_t bit);
float uint8_to_float(uint8_t* p);
void example_uart_callback(uint8_t userData);
void store_my_data_buffer(void);
void vofa_send_lines(void);

//u8 isprogramrun(u8* tmp_buf);
extern u8 flag_isrun;
#endif /* CODE_VOFA_H_ */
