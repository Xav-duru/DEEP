/*
 * WS2812S.c
 *
 *  Created on: 3 mai 2016
 *      Author: Samuel Poiraud
 *
 *
 *      les envois de pixels sont sous-traités à la fonction assembleur WS2812S_send_pixel
 *      	https://github.com/Daedaluz/stm32-ws2812/tree/master/src
 *
 */

 /*	Sur la matrice de 64 WS2812, les leds sont chainées ligne après ligne.
 */
 
 
#include "config.h"
#if USE_MATRIX_LED

#include "stm32f1xx_hal.h"
#include "WS2812S.h"
#include "config.h"
#include "macro_types.h"
#include "stm32f1_gpio.h"


#define NB_LED  16
#define NO_COLOR  0x000000
#define COLOR_BLUE	0x0000FF
#define COLOR_RED	0x00FF00
#define COLOR_GREEN	0xFF0000
#define COLOR_YELLOW	0xFFFF00
#define COLOR_BLACK	0x000000
#define T1H		1
#define T1L		1
#define T0H		1
#define T0L		1
#define RES     200

#define OUTPUT(x)	HAL_GPIO_WritePin(LED_MATRIX_PORT_DATA, LED_MATRIX_PIN_DATA, x)

void LED_MATRIX_init(void)
{
	BSP_GPIO_PinCfg(LED_MATRIX_PORT_DATA, LED_MATRIX_PIN_DATA, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);
}


#if MY_BLUEPILL_IS_COUNTERFEIT
	//fonction asm
	extern void WS2812S_send_pixel_for_chinese_clone_STM32(uint32_t pixel, uint32_t gpio_pin_x, uint32_t * gpiox_bsrr);
	#define WS2812S_send_pixel	WS2812S_send_pixel_for_chinese_clone_STM32
#else
	//fonction asm
	extern void WS2812S_send_pixel(uint32_t pixel, uint32_t gpio_pin_x, uint32_t * gpiox_bsrr);
#endif

static uint32_t pixels[14];

/*
extern void send_pixel(uint32_t pixel, uint32_t * gpiox_bsrr, uint32_t * gpiox_brr);

//Cette fonction est codée en assembleur et respecte les délais des bits imposés par le fabricant.
void send_pixel(uint32_t pixel, uint32_t * gpiox_bsrr, uint32_t * gpiox_brr)
{
	uint32_t bit;
	uint32_t i;
	i = 0;
	do
	{
		bit = (pixel >> i) & 1;
		if(bit)
			*gpiox_bsrr = 1;
		else
			*gpiox_brr = 1;
	}while(i<32);
}
*/


void LED_MATRIX_send_pixel(uint32_t pixel)
{
	WS2812S_send_pixel(pixel, LED_MATRIX_PIN_DATA, (uint32_t *)&LED_MATRIX_PORT_DATA->BSRR);
}

void LED_MATRIX_demo(void)
{
	static uint8_t j = 0;
	uint8_t i;
	LED_MATRIX_init();

	uint32_t pixels[14];
	for(i=0;i<14;i++)
	{
		if(i < 4)
			pixels[i] = COLOR_RED;
		else if(i < 8)
			pixels[i] = COLOR_YELLOW;
		else
			pixels[i] = COLOR_BLUE;
	}
	pixels[j] = COLOR_BLACK;
	j = (j+1)%14;
	LED_MATRIX_display(pixels, 14);
	HAL_Delay(100);
}

/*
 * @brief  Cette fonction avance le joueur courant suivi le nombre du dé.
 * @note   La case de départ est coloré en rouge, les autres sont en jaune.
 */


void LED_MATRIX_avance(int8_t from, int8_t to)
{
	LED_MATRIX_aucune_couleur();

	pixels[from] = COLOR_RED;
	HAL_Delay(400);
	for(uint8_t i=(from+1)%14; i!=to; i++)
	{
		pixels[i] = COLOR_YELLOW;
		LED_MATRIX_display(pixels, 14);
		HAL_Delay(400);
	}
	pixels[to] = COLOR_GREEN;
	LED_MATRIX_display(pixels, 14);

	HAL_Delay(1000);
	LED_MATRIX_aucune_couleur();
	pixels[to] = COLOR_GREEN;
	LED_MATRIX_display(pixels, 14);

	/*pixels[j] = COLOR_BLACK;
	j = (j+1)%64;
	LED_MATRIX_display(pixels, 16);
	HAL_Delay(100);*/
}

void LED_MATRIX_aucune_couleur(){
	LED_MATRIX_init();

	for(uint8_t k=0; k<NB_LED; k++)
		pixels[k] = NO_COLOR;
	LED_MATRIX_display(pixels, 14);
}


/*
 * @brief  Suite à une carte chance "allez à cette case", permet le déplacement instantané.
 * La case de départ est en rouge, la case d'arrivée est en vert
 */

void LED_MATRIX_teleportation(int8_t from, int8_t to){
	LED_MATRIX_aucune_couleur();
	HAL_Delay(500);
	pixels[from] = COLOR_RED;
	LED_MATRIX_display(pixels, 14);
	HAL_Delay(500);
	pixels[to] = COLOR_GREEN;
	LED_MATRIX_display(pixels, 14);
}


/*void LED_MATRIX_plateau(void){
	LED_MATRIX_init();
	for(uint8_t i=0; i<CARRE_NB;i++){

	}
}
*/

/*
 * @brief	Cette fonction envoie 64 pixels vers la matrice de leds.
 * @note	les envois de pixels sont sous-traités à la fonction assembleur WS2812S_send_pixel
 * 			Cette fonction est rédigée en assembleur pour respecter scrupuleusement les délais de production des signaux pour les leds de la matrice.
 * 			Remarque : les interruptions sont désactivées temporairement pendant l'exécution de cette fonction pour éviter qu'elles provoquent des 'pauses' lors de la production des signaux.
 * 			La durée d'exécution de cette fonction est de l'ordre de 2,5ms. Durée pendant laquelle aucune interruption ne peut survenir !!!
 * @param 	pixels est un tableau de 64 cases absolument...
 * @note	attention, le tableau de pixels correspond aux leds dans l'ordre où elles sont câblées. Sur la matrice 8x8, elles sont reliées en serpentin ! (et non en recommancant à gauche à chaque nouvelle ligne)...
 */
void LED_MATRIX_display(uint32_t * pixels, uint8_t size)
{
	uint8_t i;
	__disable_irq();
	LED_MATRIX_reset();
	for(i=0;i<size;i++)
		WS2812S_send_pixel(pixels[i], LED_MATRIX_PIN_DATA, (uint32_t *)&LED_MATRIX_PORT_DATA->BSRR);
	__enable_irq();
}

void LED_MATRIX_display_only_one_pixel(uint32_t pixel, uint8_t rank, uint8_t size)
{
	uint8_t i;
	__disable_irq();
	LED_MATRIX_reset();
	for(i=0;i<size;i++)
		WS2812S_send_pixel((i==rank)?pixel:COLOR_BLACK, LED_MATRIX_PIN_DATA, (uint32_t *)&LED_MATRIX_PORT_DATA->BSRR);
	__enable_irq();
}

void LED_MATRIX_display_full(uint32_t pixel, uint8_t size)
{
	uint8_t i;
	__disable_irq();
	LED_MATRIX_reset();
	for(i=0;i<size;i++)
		WS2812S_send_pixel(pixel, LED_MATRIX_PIN_DATA, (uint32_t *)&LED_MATRIX_PORT_DATA->BSRR);
	__enable_irq();
}

void LED_MATRIX_reset(void){

	int i;
	OUTPUT(0);
	Delay_us(100);
	//for(i = 0; i < RES; i++);	//Utilisez cette fonction et reglée RES si la fonction Delay_us n'est pas disponible.
}
#endif
