/** ************************************************************* *
 * @file       routines.c
 * @brief      
 * 
 * @date       2021-04-24
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#include "stdbool.h"
#include "routines.h"
#include "i2c.h"
#include "mpu6050.h"
#include "ds3231.h"
#include "window.h"
#include "broadcast.h"

phase_t phase;

/** ************************************************************* *
 * @brief      
 * 
 * @param      NewPhase 
 * ************************************************************* **/
void phase_set(phase_t NewPhase)
{
    phase = NewPhase;
}

/** ************************************************************* *
 * @brief      
 * 
 * @return     phase_t 
 * ************************************************************* **/
phase_t phase_get(void)
{
    return phase;
}

/** ************************************************************* *
 * @brief      
 * 
 * ************************************************************* **/
void routine_wait(void)
{
    MPU6050_Read_All_Kalman();
}

/** ************************************************************* *
 * @brief      
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
 * @brief      
 * 
 * ************************************************************* **/
void routine_deploy(void)
{
    broadcast_uart(MSG_ID_phase_deploy);
    phase_set(PHASE_DESCEND);
}

/** ************************************************************* *
 * @brief      
 * 
 * ************************************************************* **/
void routine_descend(void)
{
    /* read data MPU6050 */
    MPU6050_Read_All();
}

/** ************************************************************* *
 * @brief      
 * 
 * ************************************************************* **/
void routine_landed(void)
{
	
}
