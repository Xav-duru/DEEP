/*
 * joueur.c
 *
 *  Created on: 29 nov. 2022
 *      Author: decuriar
 */
#include "joueur.h"
#include "plateau.h"
#include "MatrixLed/WS2812S.h"
#include "config.h"
#include "stm32f1_ili9341.h"


static joueur_t joueurs[JOUEUR_NB];
static uint8_t current_nb_joueurs = 0;
extern joueur_e current_player;

/*
 * @breif  Initialise les attributs des joueurs -> argent à 0, position à CaseDépart, couleur
 * @note   enable permet de définir si, parmi les 4 joueurs maximum, ils jouent tous ou non
 */


void JOUEUR_init(uint8_t nb_joueurs)
{
	if(nb_joueurs>JOUEUR_NB)
	{
		printf("trop de joueurs demandés !\n");
		return;
	}

	current_nb_joueurs = nb_joueurs;
	for(uint8_t i = 0; i< JOUEUR_NB; i++)
	{
		joueurs[i].argent = 0;
		joueurs[i].position = CARRE_0;
		joueurs[i].enable = (i>=nb_joueurs)?0:1;
	}
	joueurs[0].couleur = COLOR_RED;
	joueurs[1].couleur = COLOR_BLUE;
	joueurs[2].couleur = COLOR_GREEN;
	joueurs[3].couleur = COLOR_YELLOW;

}

joueur_t * get_pjoueurs(void)
{
	return joueurs;
}

uint32_t JOUEUR_get_couleur(joueur_e id)
{
	return joueurs[id].couleur;
}


int32_t JOUEUR_get_argent(joueur_e id)
{
	return joueurs[id].argent;
}

void JOUEUR_add_argent(joueur_e id, int32_t money)
{
	joueurs[id].argent += money;
}

void JOUEUR_set_argent(joueur_e id, int32_t money)
{
	joueurs[id].argent = money;
}
carre_e JOUEUR_get_position(joueur_e id)
{
	return joueurs[id].position;
}

void JOUEUR_set_position(joueur_e id, carre_e carre)
{
	joueurs[id].position = carre;
}


bool_e JOUEUR_get_enable(joueur_e id)
{
	return joueurs[id].enable;
}

void JOUEUR_avance(joueur_e id, int8_t quantite)
{
	carre_e previous = joueurs[id].position;
	joueurs[id].position = (joueurs[id].position+quantite)%CARRE_NB;
	if(joueurs[id].position < previous)
		JOUEUR_add_argent(id, 10);	//passage par la case départ !

	LED_MATRIX_avance((int8_t)previous, (int8_t)joueurs[id].position);
}

void JOUEUR_verif_argent_positif(joueur_e id){
	if(JOUEUR_get_enable(id)){
		if(JOUEUR_get_argent(id)<=0){
			joueurs[id].enable=false;
			ILI9341_Puts(100, 100,"GAME OVER" , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
			ILI9341_Puts(150, 50,"Tu n'as plus d'argent" , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
		}
	}
}

void JOUEUR_affiche_caracteristique(joueur_e id){
	ILI9341_Puts(100, 100,"Joueur n°"+1 , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);

	ILI9341_Puts(100, 100,"Argent" , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
	ILI9341_Puts(150, 50,"Tu n'as plus d'argent" , &Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
}
