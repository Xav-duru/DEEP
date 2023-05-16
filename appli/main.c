/**
  ******************************************************************************
  * @file    main.c
  * @author  Nirgal
  * @date    03-July-2019
  * @brief   Default main function.
  ******************************************************************************
*/
#include "stm32f1xx_hal.h"
#include "stm32f1_uart.h"
#include "stm32f1_sys.h"
#include "stm32f1_gpio.h"
#include "macro_types.h"
#include "systick.h"
#include "WS2812S.h"
#include "tft_ili9341/stm32f1_ili9341.h"
#include "joueur.h"
#include "chance.h"
#include "lanceDE.h"
#include "button.h"

static volatile uint32_t t = 0;
static volatile bool_e fin_du_jeu = 0;

void process_ms(void)
	{
		if(t)
			t--;
	}

void writeLED(bool_e b)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, b);
}


int main(void)
{
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la première étape de la fonction main().
	HAL_Init();

	//Initialisation de l'UART2 à la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
		//Attention, les pins PA2 et PA3 ne sont pas reliées jusqu'au connecteur de la Nucleo.
		//Ces broches sont redirigées vers la sonde de débogage, la liaison UART étant ensuite encapsulée sur l'USB vers le PC de développement.
	UART_init(UART2_ID,115200);

	//"Indique que les printf sortent vers le périphérique UART2."
	SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	//Initialisation du port de la led Verte (carte Nucleo)
	BSP_GPIO_PinCfg(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH);

	//Initialisation du port du bouton bleu (carte Nucleo)
	BSP_GPIO_PinCfg(BLUE_BUTTON_GPIO, BLUE_BUTTON_PIN, GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);

	//Initialisation des ports des boutons (boutons poussoirs)
	BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_7, GPIO_MODE_INPUT_PP,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_8, GPIO_MODE_INPUT_PP,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);
	BSP_GPIO_PinCfg(GPIOB, GPIO_PIN_9, GPIO_MODE_INPUT_PP,GPIO_PULLUP,GPIO_SPEED_FREQ_HIGH);


	//On ajoute la fonction process_ms à la liste des fonctions appelées automatiquement chaque ms par la routine d'interruption du périphérique SYSTICK
	Systick_add_callback_function(&process_ms);

	ILI9341_Init();

	// current_player=joueurs[0];

	while(fin_du_jeu==0)
	{
		//affiche_joueur**********
		state_machine();
	}


}



void state_machine(void){

	typedef enum
	{
		INIT,
		CHOIX_NB_JOUEURS,
		LANCER_DE,
		AVANCE,
		ACTION,
		PROCHAIN_JOUEUR

	}state_e;
	static state_e state = INIT;
	static state_e previous_state = INIT;
	bool_e entrance = (state!=previous_state);
	previous_state = state;
	static joueur_e current_player;
	static uint8_t de;
	static joueur_t * joueurs;
	static case_t * plateau;
	static uint8_t previous_displayed_n, n;
	bool_e release;
	button_id_e button_event;
	button_event = button_press_event(&release);

	switch(state)
	{
		case INIT:
			current_player = JOUEUR_NB-1;
			state = CHOIX_NB_JOUEURS;
			joueurs = get_pjoueurs();
			plateau = PLATEAU_get_pplateau();
			break;
		case CHOIX_NB_JOUEURS:
			if(entrance)
			{
				previous_displayed_n = 0;
				n = 2;
				ILI9341_Puts(5, 50,"Nombre de joueurs ?" , &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				ILI9341_Puts(110, 125, "<   >" , &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			}

			if(button_event == BUTTON_ID_RIGHT && release == FALSE && n<4)
				n++;
			else if(button_event == BUTTON_ID_LEFT && release == FALSE && n>2)
				n--;
			else if(button_event == BUTTON_ID_VALID)
				state = PROCHAIN_JOUEUR;

			if(previous_displayed_n != n)
			{
				previous_displayed_n = n;
				ILI9341_printf(140, 125, &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE,"%d", n);
			}
			break;
		case PROCHAIN_JOUEUR:
			do
			{
				current_player=(current_player+1)%JOUEUR_NB;
			}while(JOUEUR_get_enable(current_player) == 0);
			state = LANCER_DE;
			break;
		case LANCER_DE:
			de = lancement_de();
			if(de != 0)
				state = AVANCE;
			break;
		case AVANCE:
			JOUEUR_avance(current_player, (int8_t)de);
			state = ACTION;
			break;
		case ACTION:
			switch(plateau[joueurs[current_player].position].type_case)
			{
				case CASE_CHANCE:
					switch(CHANCE_pioche()){
						case CHANCE_CASE_CAFET:
							LED_MATRIX_teleportation(joueurs[current_player].position, CARRE_12);
							JOUEUR_set_position(current_player, CARRE_12);
							break;
						case CHANCE_AVANCE_1_CASE:
							JOUEUR_avance(current_player, 1);
							fin_du_jeu=1;
							break;
						case CHANCE_AVANCE_2_CASE:
							JOUEUR_avance(current_player, 2);
							break;
						case CHANCE_RECULE_1_CASE:
							JOUEUR_avance(current_player, -1);
							break;
						case CHANCE_CASE_DEPART:
							LED_MATRIX_teleportation(joueurs[current_player].position, CARRE_0);
							JOUEUR_set_position(current_player, CARRE_0);
							break;

						case CHANCE_PERD_10_EUROS:
							JOUEUR_add_argent(current_player, -10);
							break;

						case CHANCE_GAGNE_10_EUROS:
							JOUEUR_add_argent(current_player, 10);
							break;
						default:
							break;
					}
					break;
				case CASE_SALLE:

					break;

					/*
					 * Dans la case cafet, tu as 3 chances pour effectuer un 6 lors du lancer de dé, sinon tu paie 20 euros
					 */
				case CASE_CAFET:{
					uint8_t nb_lancer_de_restants;
					if(entrance)
					{
						nb_lancer_de_restants = 3;
						ILI9341_Fill(ILI9341_COLOR_WHITE);
						ILI9341_Puts(50, 30, "Bienvenue dans la cafet" , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
						ILI9341_Puts(100, 50,"Lancer le de !" , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
						ILI9341_Puts(140, 30,"Tentative numéro " , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
						ILI9341_printf(140, 150, &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE,"%d", 1+3-nb_lancer_de_restants);
					}

					de=lancement_de();
					if(de != 0)
					{
						if(de==6)
						{
							//gagné !
							ILI9341_Puts(100, 50,"GAGNE !" , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
							ILI9341_Puts(100, 50,"Tu as gagne 10 euros !" , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
							JOUEUR_add_argent(current_player, 10);
							state = PROCHAIN_JOUEUR;
						}
						else
						{
							nb_lancer_de_restants--;
							if(nb_lancer_de_restants == 0)
							{
								//perdu !
								state = PROCHAIN_JOUEUR;
								ILI9341_Puts(100, 50,"PERDU !" , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
								ILI9341_Puts(100, 50,"Tu as perdu 20 euros !" , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
								JOUEUR_set_argent(current_player, -20);
							}
							else
								ILI9341_printf(140, 150, &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE,"%d", 1+3-nb_lancer_de_restants);
						}

					}

					break;}

				case CASE_DEPART:
					JOUEUR_add_argent(current_player, 20);
					state = PROCHAIN_JOUEUR;

					break;
				default:
					break;

			}

			break;

		default:
			break;
	}
}

