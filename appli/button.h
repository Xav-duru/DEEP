/*
 * button.h
 *
 *  Created on: 26 juin 2019
 *      Author: Nirgal
 */

#ifndef BUTTON_H_
#define BUTTON_H_
#include "macro_types.h"

typedef enum
{
	BUTTON_EVENT_NONE,
	BUTTON_EVENT_SHORT_PRESS,
	BUTTON_EVENT_LONG_PRESS
}button_event_e;

void BUTTON_init(void);

//button_event_e BUTTON_state_machine(void);


typedef enum
{
	BUTTON_ID_NONE = 0,
	BUTTON_ID_LEFT,
	BUTTON_ID_RIGHT,
	BUTTON_ID_VALID,
	BUTTON_ID_NB // Le nombre de boutons dans la liste...
}button_id_e;


button_id_e button_press_event(bool_e * release);

#endif /* BUTTON_H_ */
