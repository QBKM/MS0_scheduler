/** ************************************************************* *
 * @file       window.h
 * @brief      this file provide a temporal window managment for 
 *             the rocket. 
 *             it has to way to do it :
 *             - IT
 *             - Pool (RTC)
 *             you can use both at the same time to make it safer.
 * 
 * @date       2021-04-21
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef INC_WINDOW_H_
#define INC_WINDOW_H_

/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"

#include "stm32f3xx_hal.h"
#include "tim.h"

#include "routines.h"
#include "ds3231.h"

/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
/* Temporal windows for the parachute */
/* +---------------------+--------------------------------+---------------------+ */
/* | T < T0*0.9 = LOCKED | T0*0.9 < T < T0*1.1 = UNLOCKED | T > T0*1.1 = FORCED | */
/* + --------------------+--------------------------------+---------------------+ */
#define WINDOW_TIMEREF      17.7
#define WINDOW_UNLOCK       15.93   /* - 10% */
#define WINDOW_RELOCK       19.47   /* + 10% */

#define WINDOW_UNLOCK_RTC   16      /* value for the double window safety with I2C RTC */
#define WINDOW_RELOCK_RTC   20      /* value for the double window safety with I2C RTC */

/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef struct 
{
    bool unlock;
    bool relock;
}window_t;

/* ------------------------------------------------------------- --
   public functions
-- ------------------------------------------------------------- */
/* init */
void window_init(void);

/* IT window */
void IT_routine_window_unlock(void);
void IT_routine_window_relock(void);
void IT_flag_window_unlock(void);
void IT_flag_window_relock(void);
bool get_winU_IT_flag(void);
bool get_winR_IT_flag(void);

/* RTC window */
bool window_check_RTC_unlock(void);


#endif
/* ------------------------------------------------------------- --
   End of file
-- ------------------------------------------------------------- */