/*
 * WS2812S.h
 *
 *  Created on: 3 mai 2016
 *      Author: Nirgal
 */

#ifndef BSP_MATRIXLED_WS2812S_H_
#define BSP_MATRIXLED_WS2812S_H_

#define COLOR_BLUE	0x0000FF
#define COLOR_RED	0x00FF00
#define COLOR_GREEN	0xFF0000
#define COLOR_YELLOW	0xFFFF00
#define COLOR_BLACK	0x000000

#include "config.h"
#if USE_MATRIX_LED
#if !(defined LED_MATRIX_PORT_DATA) || !defined(LED_MATRIX_PIN_DATA)
	#define LED_MATRIX_PORT_DATA	GPIOB
	#define LED_MATRIX_PIN_DATA		GPIO_PIN_13
#endif
void LED_MATRIX_init(void);

void LED_MATRIX_demo(void);
void LED_MATRIX_avance(int8_t from, int8_t to);
void LED_MATRIX_teleportation(int8_t from, int8_t to);

//@pre pixels est un tableau de 64 cases maximum... (size est le nombre de cases)
void LED_MATRIX_display(uint32_t * pixels, uint8_t size);


void LED_MATRIX_display_only_one_pixel(uint32_t pixel, uint8_t rank, uint8_t size);
void LED_MATRIX_display_full(uint32_t pixel, uint8_t size);
void LED_MATRIX_send_pixel(uint32_t pixel);
void LED_MATRIX_reset(void);

#endif
#endif /* BSP_MATRIXLED_WS2812S_H_ */
