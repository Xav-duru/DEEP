/*
 * button.c
 *
 *  Created on: 26 juin 2019
 *      Author: Nirgal
 */
#include "button.h"
#include "config.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"

#define LONG_PRESS_DURATION	1000	//unit� : [1ms] => 1 seconde.

static void process_ms(void);

static volatile bool_e flag_10ms;
static volatile uint32_t t = 0;
static bool_e initialized = FALSE;

void BUTTON_init(void)
{
	//Initialisation du port du bouton bleu en entr�e
	BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	Systick_add_callback_function(&process_ms);

	initialized = TRUE;
}

static void process_ms(void)
{
	static uint32_t t10ms = 0;
	t10ms = (t10ms + 1)%10;		//incr�mentation de la variable t10ms (modulo 10 !)
	if(!t10ms)
		flag_10ms = TRUE; //toutes les 10ms, on l�ve ce flag.
	if(t)
		t--;
}

/**
	Cette machine � �tats g�re la d�tection d'appuis sur le bouton bleu.
	Elle doit �tre appel�e en boucle tr�s r�guli�rement.
	Pr�condition : avoir appel� auparavant BUTTON_init();
	Si un appui vient d'�tre fait, elle renverra BUTTON_EVENT_SHORT_PRESS ou BUTTON_EVENT_LONG_PRESS
*/
button_event_e BUTTON_state_machine(void)
{
	typedef enum
	{
		INIT = 0,
		WAIT_BUTTON,	//En C, les nombres se suivent dans une enum.
		BUTTON_PRESSED,
		WAIT_RELEASE
	}state_e;

	static state_e state = INIT; //La variable d'�tat, = INIT au d�but du programme !
	/**	Le mot cl� static est INDISPENSABLE :
	* 	"state" DOIT GARDER SA VALEUR d'un appel � l'autre de la fonction.
	*	Une place lui est r�serv�e en m�moire de fa�on permanente
	*	(et non pas temporaire dans la pile !)
	*/

	button_event_e ret = BUTTON_EVENT_NONE;
	bool_e current_button;

	if(flag_10ms && initialized)	//le cadencement de cette portion de code � 10ms permet d'�liminer l'effet des rebonds sur le signal en provenance du bouton.
	{
		flag_10ms = FALSE;
		current_button = !HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN);
		switch(state)
		{
			case INIT:
				state = WAIT_BUTTON;	//Changement d'�tat
				break;
			case WAIT_BUTTON:
				if(current_button)
				{
					printf("[BUTTON      ] button pressed\n");
					t=LONG_PRESS_DURATION;	//Action r�alis�e sur la transition.
					state = BUTTON_PRESSED;	//Changement d'�tat conditionn� � "if(current_button)"
				}
				break;
			case BUTTON_PRESSED:
				if(t==0)
				{
					ret = BUTTON_EVENT_LONG_PRESS;
					printf("[BUTTON      ] long press event\n");
					state = WAIT_RELEASE;		//le temps est �coul�, c'�tait un appui long !
				}
				else if(!current_button)
				{
					ret = BUTTON_EVENT_SHORT_PRESS;
					printf("[BUTTON      ] short press event\n");
					state = WAIT_BUTTON;	//le bouton a �t� rel�ch� avant l'�coulement du temps, c'�tait un appui court !
				}
				break;

			case WAIT_RELEASE:
				if(!current_button)
				{
					printf("[BUTTON      ] release button after long press\n");
					state = WAIT_BUTTON;
				}
				break;
			default:
				state = INIT;	//N'est jamais sens� se produire.
				break;
		}
	}
	return ret;
}


button_id_e button_press_event(bool_e * release)
{
	static bool_e previous_state[BUTTON_ID_NB] = {FALSE};
	button_id_e ret = BUTTON_ID_NONE;
	button_id_e button_id;
	bool_e current_state;
	for(button_id = BUTTON_ID_LEFT; button_id<BUTTON_ID_NB; button_id++)
	{
		switch(button_id)
		{
			case BUTTON_ID_LEFT:
				current_state = !HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7);
				break;
			case BUTTON_ID_RIGHT:
				current_state = !HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8);
				break;
			case BUTTON_ID_VALID:
				current_state = !HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9);
				break;
			default:
				current_state = 0; break;
		}
		if(current_state && !previous_state[button_id])
		{
			ret = button_id;
			*release = FALSE;
		}
		else if(!current_state && previous_state[button_id])
		{
			ret = button_id;
			*release = TRUE;
		}
		previous_state[button_id] = current_state;
	}
	return ret;
}


