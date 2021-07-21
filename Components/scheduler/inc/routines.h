/** ************************************************************* *
 * @file       routines.h
 * @brief      
 * 
 * @date       2021-04-23
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef INC_ROUTINES_H_
#define INC_ROUTINES_H_

#include "stdint.h"

/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef enum
{
  PHASE_WAIT,
  PHASE_ASCEND,
  PHASE_DEPLOY,
  PHASE_DESCEND,
  PHASE_LANDED
}phase_t;




/* ------------------------------------------------------------- --
   function protoypes
-- ------------------------------------------------------------- */
void phase_set(phase_t NewPhase);
phase_t phase_get(void);

void routine_wait(void);
void routine_ascend(void);
void routine_deploy(void);
void routine_descend(void);
void routine_landed(void);


#endif /* INC_ROUTINES_H_ */
/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */
