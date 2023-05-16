/*
 * plateau.c
 *
 *  Created on: 29 nov. 2022
 *      Author: xavie
 */

#include "plateau.h"
#include "salle.h"
#include "joueur.h"

static case_t plateau[CARRE_NB];

/*
 * @brief  Initialise les carres du plateau en temps que sall, chance, cafet ou case départ.
 */

void PLATEAU_init(void){
	for(uint8_t i = 0; i<CARRE_NB; i++)
	{
		plateau[i].type_case = CASE_SALLE;
		plateau[i].proprietaire = JOUEUR_AUCUN;
	}
	plateau[0].type_case = CASE_DEPART;
	plateau[3].type_case = CASE_CHANCE;
	plateau[8].type_case = CASE_CHANCE;
	plateau[11].type_case = CASE_CAFET;

	uint8_t s = 0;
	for(uint8_t i = 0; i<CARRE_NB; i++)
	{
		if(plateau[i].type_case == CASE_SALLE)
		{
			plateau[i].salle = SALLE_get_p(s);
			s++;
		}
		else
			plateau[i].salle = NULL;
	}
}

case_t * PLATEAU_get_pplateau(void)
{
	return plateau;
}
/*
 * @brief  Retourne le type de case parmi salle, chance, cafet et depart
 */

case_e PLATEAU_get_case(uint8_t id){
	return plateau[id].type_case;
}



