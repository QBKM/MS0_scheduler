/** ************************************************************* *
 * @file       jack.h
 * @brief      
 * 
 * @date       2021-04-23
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef INC_JACK_H_
#define INC_JACK_H_

/* ------------------------------------------------------------- --
   Includes
-- ------------------------------------------------------------- */
#include "stdint.h"
#include "stdbool.h"

#include "routines.h"
#include "tim.h"

/* ------------------------------------------------------------- --
   Types
-- ------------------------------------------------------------- */
typedef enum
{
	JACK_UNPLUGGED,
	JACK_PLUGGED
}jack_status_t;

typedef struct 
{
	volatile bool	IT_flag;
	jack_status_t 	jack_status;
}jack_t;


/* ------------------------------------------------------------- --
   Public prototypes
-- ------------------------------------------------------------- */
void            jack_init(void);

void 			IT_flag_jack(void);
void 			IT_routine_jack(void);

void            set_jack_IT_flag(bool IT_flag);
bool            get_jack_IT_flag(void);

void            set_jack_status(jack_status_t jack_status);
jack_status_t   get_jack_status(void);




#endif /* INC_JACK_H_ */
