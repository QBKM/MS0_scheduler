/** ************************************************************* *
 * @file       synchro.h
 * @brief      
 * 
 * @date       2021-04-23
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef SCHEDULER_INC_SYNCHRO_H_
#define SCHEDULER_INC_SYNCHRO_H_

/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "stdint.h"

/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef struct synchro_t
{
    uint32_t time_ref;
    uint32_t time_sync;
}synchro_t;

/* ------------------------------------------------------------- --
   function prototypes
-- ------------------------------------------------------------- */
void synchro_init(void);
void synchro_update(void);
void synchro_wait(void);



#endif /* SCHEDULER_INC_SYNCHRO_H_ */
/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */
