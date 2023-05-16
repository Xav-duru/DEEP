/*
 * chance.c
 *
 *  Created on: 1 d�c. 2022
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
 * @brief  Pioche une carte pioche chance al�atoirement dans l'�numaration du chance.h
 */

carte_chance_e CHANCE_pioche(void){
	return (carte_chance_e)t%CHANCE_NB;
}


