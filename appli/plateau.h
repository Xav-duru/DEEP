/*
 * plateau.h
 *
 *  Created on: 29 nov. 2022
 *      Author: xavie
 */

#ifndef PLATEAU_H_
#define PLATEAU_H_
#include <stdint.h>
#include "salle.h"

typedef enum
{
	CARRE_0,
	CARRE_1,
	CARRE_2,
	CARRE_3,
	CARRE_4,
	CARRE_5,
	CARRE_6,
	CARRE_7,
	CARRE_8,
	CARRE_9,
	CARRE_10,
	CARRE_11,
	CARRE_12,
	CARRE_13,
	CARRE_NB
}carre_e;

#include "joueur.h"


typedef enum
{
	CASE_SALLE,
	CASE_CHANCE,
	CASE_CAFET,
	CASE_DEPART
}case_e;

typedef struct
{
	case_e type_case;
	salle_t * salle;
	joueur_e proprietaire;
}case_t;

void PLATEAU_init(void);
case_t * PLATEAU_get_pplateau(void);
case_e PLATEAU_get_case(uint8_t id);


#endif /* PLATEAU_H_ */
