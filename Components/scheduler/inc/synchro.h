/**
 * @file synchro.h
 * @author Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * @brief 
 * @version 0.1
 * @date 2021-04-11
 * 
 * Mines Space
 * 
 */

#ifndef SCHEDULER_INC_SYNCHRO_H_
#define SCHEDULER_INC_SYNCHRO_H_

#include "stdint.h"
#include "stm32f3xx_hal.h"

typedef struct synchro_t
{
    uint32_t time_ref;
    uint32_t time_sync;
}synchro_t;

void synchro_init(void);
void synchro_update(void);
void synchro_wait(void);

#endif /* SCHEDULER_INC_SYNCHRO_H_ */
