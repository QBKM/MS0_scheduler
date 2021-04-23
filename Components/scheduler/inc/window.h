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

#include "config_file.h"

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