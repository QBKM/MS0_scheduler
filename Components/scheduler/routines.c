/** ************************************************************* *
 * @file        routines.c
 * @brief      
 * 
 * @date        2021-04-24
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "routines.h"

#include "stdbool.h"

#include "window.h"

#include "i2c.h"
#include "mpu6050.h"
#include "ds3231.h"

#include "broadcast.h"

/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
phase_t phase;

/* ------------------------------------------------------------- --
   functions
-- ------------------------------------------------------------- */
/** ************************************************************* *
 * @brief       Change the phase for the scheduler
 * 
 * @param       NewPhase 
 * ************************************************************* **/
void phase_set(phase_t NewPhase)
{
    phase = NewPhase;
}

/** ************************************************************* *
 * @brief       return the current phase used by the scheduler
 * 
 * @return      phase_t 
 * ************************************************************* **/
phase_t phase_get(void)
{
    return phase;
}

/** ************************************************************* *
 * @brief       initial phase for the scheduler
 * 
 * ************************************************************* **/
void routine_wait(void)
{
    
}

/** ************************************************************* *
 * @brief       second phase for the scheduler. this phase is 
 *              set by removing the jack connector
 * 
 * ************************************************************* **/
void routine_ascend(void)
{
    /* read data */
    MPU6050_Read_All_Kalman();

    /* Temporal window [pooling mode]*/
    if(window_check_RTC_unlock() == true)
    {
        // if apogee, => phase_set(PHASE_DEPLY)
    }
}

/** ************************************************************* *
 * @brief       third phase for the scheduler. this phase is 
 *              set by reaching the apogee or by timeout the 
 *              temporal window
 * 
 * ************************************************************* **/
void routine_deploy(void)
{
    broadcast_uart(MSG_ID_phase_deploy);
    phase_set(PHASE_DESCEND);
}

/** ************************************************************* *
 * @brief       forth phase for the scheduler. this phase is 
 *              set just after leaving the apogee phase
 * 
 * ************************************************************* **/
void routine_descend(void)
{
    /* read data MPU6050 */
	MPU6050_Read_All_Kalman();
}

/** ************************************************************* *
 * @brief       fifth phase for the scheduler. this phase is 
 *              set by landing on the floor (final phase)
 * 
 * ************************************************************* **/
void routine_landed(void)
{
	
}

/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */
