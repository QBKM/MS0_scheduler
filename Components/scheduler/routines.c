/**
 * @file routines.c
 * @author Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * @brief 
 * @version 0.1
 * @date 2021-04-21
 * 
 * Mines Space
 * 
 */

#include "stdbool.h"
#include "routines.h"
#include "i2c.h"
#include "mpu6050.h"
#include "ds3231.h"
#include "window.h"
#include "broadcast.h"

phase_t phase;

/* ************************************************************* *
 * @name		
 * @brief		
 *
 * @args
 * ************************************************************* */
void phase_set(phase_t NewPhase)
{
    phase = NewPhase;
}

/* ************************************************************* *
 * @name		
 * @brief		
 *
 * @args
 * ************************************************************* */
phase_t phase_get(void)
{
    return phase;
}

/* ************************************************************* *
 * @name		
 * @brief		
 *
 * @args
 * ************************************************************* */
void routine_wait(void)
{
    MPU6050_Read_All_Kalman();
}

/* ************************************************************* *
 * @name		
 * @brief		
 *
 * @args
 * ************************************************************* */
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

/* ************************************************************* *
 * @name		
 * @brief		
 *
 * @args
 * ************************************************************* */
void routine_deploy(void)
{
    broadcast_uart(MSG_ID_phase_deploy);
    phase_set(PHASE_DESCEND);
}

/* ************************************************************* *
 * @name		
 * @brief		
 *
 * @args
 * ************************************************************* */
void routine_descend(void)
{
    /* read data MPU6050 */
    MPU6050_Read_All();
}

/* ************************************************************* *
 * @name		
 * @brief		
 *
 * @args
 * ************************************************************* */
void routine_landed(void)
{
	
}
