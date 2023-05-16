/*
 * salle.h
 *
 *  Created on: 29 nov. 2022
 *      Author: xavie
 */

#ifndef SALLE_H_
#define SALLE_H_
#include <stdbool.h>
#include <stdint.h>
#include "macro_types.h"


typedef enum
{
	SALLE_0,
	SALLE_1,
	SALLE_2,
	SALLE_3,
	SALLE_4,
	SALLE_5,
	SALLE_6,
	SALLE_7,
	SALLE_8,
	SALLE_9,
	SALLE_10,
	SALLE_NB
}salle_e;

typedef struct
{
	uint16_t price;
	char name[20];
	bool_e room_enable;
}salle_t;


salle_t * SALLE_get_p(salle_e id);
uint16_t SALLE_get_price(salle_e id);
bool_e SALLE_get_room_enable(salle_e id);


#endif /* SALLE_H_ */
