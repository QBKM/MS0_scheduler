/**
 * @file buzzer.h
 * @author Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * @brief 
 * @version 0.2
 * @date 2021-04-11
 * 
 * Mines Space
 * 
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

/* ------------------------------------------------------------- --
   Includes
-- ------------------------------------------------------------- */
#include "stdint.h"

/* ------------------------------------------------------------- --
   Types
-- ------------------------------------------------------------- */
typedef struct buzzer_t
{
	uint32_t    start_time;
    uint32_t    current_time;
    uint8_t     duty_cycle;
}buzzer_t;

/* ------------------------------------------------------------- --
   Public prototypes
-- ------------------------------------------------------------- */
void buzzer_init(void);
void buzzer_update(uint16_t delay, float duty_cycle);

#endif /* INC_BUZZER_H_ */
