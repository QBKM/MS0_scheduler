#ifndef INC_WINDOW_H_
#define INC_WINDOW_H_


#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"

#include "stm32f3xx_hal.h"
#include "tim.h"

#include "routines.h"

/* Temporal windows for the parachute */
/* +---------------------+--------------------------------+---------------------+ */
/* | T < T0*0.9 = LOCKED | T0*0.9 < T < T0*1.1 = UNLOCKED | T > T0*1.1 = FORCED | */
/* + --------------------+--------------------------------+---------------------+ */

#define WINDOW_TIMEREF      17.7
#define WINDOW_UNLOCK       (WINDOW_TIMEREF * 0.9)
#define WINDOW_RELOCK       (WINDOW_TIMEREF * 1.1)

typedef enum 
{
    w_flag_lock   = 1,
    w_flag_unlock = 3,
    w_flag_relock = 5,
    w_flag_forced = 7
}window_t;

void window_init(void);

void window_set_pool_flag(window_t flag);
void window_set_it_flag(window_t flag);

window_t window_get_pool_flag(void);
window_t window_get_it_flag(void);

void IT_routine_window_unlock(void);
void IT_routine_window_relock(void);
void IT_flag_window_unlock(void);
void IT_flag_window_relock(void);

bool get_winU_IT_flag(void);
bool get_winR_IT_flag(void);


#endif