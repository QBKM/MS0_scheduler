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
#include "math.h"

#include "window.h"

#include "i2c.h"
#include "mpu6050.h"
#include "ds3231.h"

#include "broadcast.h"

#include "recsys.h"
#include "tca6408a.h"

#include "config_file.h"
#include "math.h"

/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */


/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef struct cycles_t
{
   uint32_t total;
   uint32_t wait;
   uint32_t ascend;
   uint32_t deploy;
   uint32_t descend;
   uint32_t landed;
}cycles_t;

typedef struct routines_t
{
   phase_t phase;
   cycles_t cycles;
}routines_t;

/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
routines_t routines = {0};

uint32_t time_sync = 0;

uint32_t timeout_recsys = 0;

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
    routines.phase = NewPhase;
}

/** ************************************************************* *
 * @brief       return the current phase used by the scheduler
 * 
 * @return      phase_t 
 * ************************************************************* **/
phase_t phase_get(void)
{
    return routines.phase;
}

/** ************************************************************* *
 * @brief       initial phase for the scheduler
 * 
 * ************************************************************* **/
void routine_wait(void)
{
    /* send a wait message every second */
    if(HAL_GetTick() >= (time_sync + BROADCAST_WAIT_PHASE_DELAY))
    {
        time_sync = HAL_GetTick();
        broadcast_uart_send(MSG_ID_phase_wait);
    }

    RECSYS_user_button();

    routines.cycles.wait  += 1;
    routines.cycles.total += 1;
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
    if(window_check_RTC_unlock() == true || get_winU_IT_flag() == true)
    {
        // value to find
        MPU6050_t IMU = MPU6050_Get_Struct();
        if((fabs(IMU.KalmanAngleY) >= 70) 
        || (fabs(IMU.KalmanAngleX) >= 70))
        //if(sqrtf((IMU.Ax * IMU.Ax) + (IMU.Ay * IMU.Ay) + (IMU.Az * IMU.Az)) < 0.5) // if total acc < 0.5 then freefall (lim -> 0)
        //if("TODO")
        {
            phase_set(PHASE_DEPLOY);
        }
    }

    routines.cycles.ascend += 1;
    routines.cycles.total  += 1;
}

/** ************************************************************* *
 * @brief       third phase for the scheduler. this phase is 
 *              set by reaching the apogee or by timeout the 
 *              temporal window
 * 
 * ************************************************************* **/
void routine_deploy(void)
{
	broadcast_uart_send(MSG_ID_phase_deploy);
	broadcast_uart_send(MSG_ID_recsys_unlocking);
	broadcast_uart_send(MSG_ID_recsys_start_motor);

	RECSYS_Unlock(RECSYS_M1 | RECSYS_M2);
	RECSYS_Start(RECSYS_M1 | RECSYS_M2);

	phase_set(PHASE_DESCEND);
    broadcast_uart_send(MSG_ID_phase_descend);

    timeout_recsys = HAL_GetTick();

    routines.cycles.deploy += 1;
    routines.cycles.total  += 1;
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

    RECSYS_check_unlocked();

    if(HAL_GetTick() > (timeout_recsys + 5000)) RECSYS_Stop(RECSYS_M1 | RECSYS_M2); 

    routines.cycles.descend += 1;
    routines.cycles.total   += 1;
}

/** ************************************************************* *
 * @brief       fifth phase for the scheduler. this phase is 
 *              set by landing on the floor (final phase)
 * 
 * ************************************************************* **/
void routine_landed(void)
{
	routines.cycles.landed += 1;
    routines.cycles.total  += 1;
}



/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */
