/*
 * joueur.h
 *
 *  Created on: 29 nov. 2022
 *      Author: decuriar
 */

#ifndef JOUEUR_H_
#define JOUEUR_H_
#include <stdint.h>
#include "macro_types.h"


typedef enum
{
	JOUEUR_1,
	JOUEUR_2,
	JOUEUR_3,
	JOUEUR_4,
	JOUEUR_NB,
	JOUEUR_AUCUN
}joueur_e;

#include "plateau.h"


typedef struct
{
	uint32_t couleur;
	int32_t argent;
	carre_e position;
	bool_e enable;
}joueur_t;



void JOUEUR_init(uint8_t nb_joueurs);
joueur_t * get_pjoueurs(void);
uint32_t JOUEUR_get_couleur(joueur_e id);
void JOUEUR_add_argent(joueur_e id, int32_t money);
int32_t JOUEUR_get_argent(joueur_e id);
void JOUEUR_set_argent(joueur_e id, int32_t money);

carre_e JOUEUR_get_position(joueur_e id);
void JOUEUR_set_position(joueur_e id, carre_e carre);
bool_e JOUEUR_get_enable(joueur_e id);
void JOUEUR_verif_argent_positif(joueur_e id);
void JOUEUR_avance(joueur_e id, int8_t quantite);
#endif /* JOUEUR_H_ */
