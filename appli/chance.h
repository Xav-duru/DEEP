/*
 * chance.h
 *
 *  Created on: 1 déc. 2022
 *      Author: xavie
 */

#ifndef CHANCE_H_
#define CHANCE_H_
#include <stdint.h>


typedef enum
{
 CHANCE_CASE_CAFET = 0,
 CHANCE_AVANCE_1_CASE,
 CHANCE_AVANCE_2_CASE,
 CHANCE_RECULE_1_CASE,
 CHANCE_CASE_DEPART,
 CHANCE_PERD_10_EUROS,
 CHANCE_GAGNE_10_EUROS,
 CHANCE_NB
}carte_chance_e;


typedef struct
{
	uint8_t type;
}chance_t;


void CHANCE_init(void);
carte_chance_e CHANCE_pioche(void);

#endif /* CHANCE_H_ */
