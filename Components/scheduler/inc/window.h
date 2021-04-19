#ifndef INC_WINDOW_H_
#define INC_WINDOW_H_


#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"

#include "stm32f3xx_hal.h"

/* Temporal windows for the parachute */
/* +---------------------+--------------------------------+---------------------+ */
/* | T < T0*0.8 = LOCKED | T0*0.8 < T < T0*1.2 = UNLOCKED | T > T0*1.2 = FORCED | */
/* + --------------------+--------------------------------+---------------------+ */

typedef struct window_t
{
    bool flag_open;
    bool flag_close;
    bool flag_forced;
}window_t;

void init_window();

void set_window_open_flag(bool flag);
void set_window_close_IT_flag(bool flag);
void set_window_forced_flag(bool flag);


bool get_window_open_flag();
bool get_window_close_IT_flag();
bool get_window_forced_flag();

void IT_window_close();

#endif