/**
  ******************************************************************************
  * @file    main.c
  * @author  Nirgal
  * @date    03-July-2019
  * @brief   Default main function.
  ******************************************************************************

#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"
#include "stm32f1_ili9341.h"
#include "button.h"
#include "systick.h"


void nb_Joueur(void);


void writeLED(bool_e b)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, b);
}

bool_e readButton(void)
{
	return !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7);
	return !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);
	return !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);
}

static volatile uint8_t n = 2;
static volatile uint32_t random_value = 0;
static volatile uint32_t t = 0;
static volatile bool_e debut_du_jeu=1;
static void process_ms(void)
{
	if(t)
		t--;
	random_value = random_value>>1 | ( ((~(random_value&1 ^ (random_value>>5 & 1))&1)<<8));

}


uint8_t state_machine(void)
{
	typedef enum
	{
		INIT,
		CHOIX_NB_JOUEURS,
		ATTENTE_LANCEMENT_DE,
		LANCEMENT_DE,
		JOUEUR_AVANCE

	}state_e;
	static state_e state;

	if (debut_du_jeu==1)
		state = INIT;
	else
		state = ATTENTE_LANCEMENT_DE;

	static state_e previous_state = INIT;
	bool_e entrance = (state!=previous_state);
	previous_state = state;

	static uint8_t previous_displayed_n;
	static uint8_t valeur_de;

	bool_e release;
	button_id_e button_event;
	button_event = button_press_event(&release);

	switch(state)
	{
		case INIT:
			state = CHOIX_NB_JOUEURS;
			previous_displayed_n = 0;
			n = 2;
			break;
		case CHOIX_NB_JOUEURS:
			if(entrance)
			{
				ILI9341_Puts(5, 50,"Nombre de joueurs ?" , &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				ILI9341_Puts(110, 125, "<   >" , &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			}

			if(button_event == BUTTON_ID_RIGHT && release == FALSE && n<4)
				n++;
			else if(button_event == BUTTON_ID_LEFT && release == FALSE && n>2)
				n--;
			else if(button_event == BUTTON_ID_VALID)
				state = ATTENTE_LANCEMENT_DE;

			if(previous_displayed_n != n)
			{
				previous_displayed_n = n;
				ILI9341_printf(140, 125, &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE,"%d", n);
			}
			debut_du_jeu=0;
			state=JOUEUR_AVANCE;
			break;
		case ATTENTE_LANCEMENT_DE:
			if(entrance)
			{
				ILI9341_Puts(5, 50,"Nombre de joueurs ?" , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
				ILI9341_Puts(110, 125, "<   >" , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
				ILI9341_Puts(40, 10,"appuyez sur un " , &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				ILI9341_Puts(100, 50,"bouton" , &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				ILI9341_Puts(20, 90,"pour lancer le de" , &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				ILI9341_Puts(110, 140, "<   >" , &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			}
			if(button_event != BUTTON_ID_NONE && release == FALSE)
			{
				state = LANCEMENT_DE;
			}
			break;
		case LANCEMENT_DE:
			if(entrance)
			{
				t = 50;
			}

			if(button_event != BUTTON_ID_NONE && release == TRUE)
			{
				valeur_de = (uint8_t)((random_value%6)+1);
				state = JOUEUR_AVANCE;
			}

			if(!t || state != LANCEMENT_DE)
			{
				t = 50;
				ILI9341_printf(140, 140, &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE,"%ld", (random_value%6)+1);
			}

			break;
		case JOUEUR_AVANCE:
			t = 50;
			ILI9341_Puts(40, 10,"appuyez sur un " , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
			ILI9341_Puts(100, 50,"bouton" , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
			ILI9341_Puts(20, 90,"pour lancer le de" , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
			ILI9341_Puts(110, 140, "<   >" , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
			break;
		default:
			break;
	}
	return valeur_de;
}
*/
