/*
 * chance.c
 *
 *  Created on: 1 déc. 2022
 *      Author: xavie
 */

#include "chance.h"
#include <stdio.h>
#include <stdlib.h>
#include "systick.h"


volatile static uint32_t t = 0;

void CHANCE_process_ms(void)
{
	t++;
}

void CHANCE_init(void)
{
	Systick_add_callback_function(&CHANCE_process_ms);
}

/*
 * @brief  Pioche une carte pioche chance aléatoirement dans l'énumaration du chance.h
 */

carte_chance_e CHANCE_pioche(void){
	return (carte_chance_e)t%CHANCE_NB;
}


