/*
 * salle.c
 *
 *  Created on: 29 nov. 2022
 *      Author: xavie
 */

#include "salle.h"


/*
 * @brief  Permet d'initialiser chaque salle avec un prix et un nom. Elle initialise la salle comme disponible à acheter
 */

static salle_t salles[SALLE_NB]=
{
		(salle_t){.price = 5, .name="NEWTON", .room_enable=TRUE},	//SALLE_0
		(salle_t){.price = 10, .name="LANGEVIN", .room_enable=TRUE},	//SALLE_1
		(salle_t){.price = 15, .name="TESLA", .room_enable=TRUE},	//SALLE_2
		(salle_t){.price = 20, .name="EDISON", .room_enable=TRUE},	//SALLE_3
		(salle_t){.price = 25, .name="BODE", .room_enable=TRUE},	//SALLE_4
		(salle_t){.price = 30, .name="EINSTEIN", .room_enable=TRUE},	//SALLE_5
		(salle_t){.price = 35, .name="MONGES", .room_enable=TRUE},	//SALLE_6
		(salle_t){.price = 40, .name="MEITNER", .room_enable=TRUE},	//SALLE_7
		(salle_t){.price = 45, .name="ANJOU", .room_enable=TRUE},	//SALLE_8
		(salle_t){.price = 50, .name="JEANNETEAU", .room_enable=TRUE}	//SALLE_9
};


/*
 * @brief  Récupère l'addresse de la salle.
 */

salle_t * SALLE_get_p(salle_e id)
{
	if(id<SALLE_NB)
		return &salles[id];
	return NULL;
}

/*
uint16_t SALLE_get_price(salle_e id)
{
	return salles[id].price;
}

bool_e SALLE_get_room_enable(salle_e id)
{
	return salles[id].room_enable;
}
*/



