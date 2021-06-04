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

#include "recsys.h"

#include "config_file.h"

/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */


/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef struct 
{
    uint32_t time_ref;
    uint32_t time_stamp;
    uint8_t status;
}REVERSE_t;


/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
routines_t routines = {0};

uint32_t time_sync = 0;

REVERSE_t reverse_M1 ={0};
REVERSE_t reverse_M2 ={0};

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
        broadcast_uart_send(PHASE_WAIT);
    }

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
    if(window_check_RTC_unlock() == true)
    {
        // value to find
        MPU6050_t IMU = MPU6050_Get_Struct();
        if((IMU.KalmanAngleY >= 100) 
        || (IMU.KalmanAngleX >= 100) 
        || (IMU.Az <= 0))
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
    // first cycle tasks
    if(routines.cycles.deploy == 0)
    {
	    broadcast_uart_send(MSG_ID_phase_deploy);
        broadcast_uart_send(MSG_ID_recsys_unlocking); 
        broadcast_uart_send(MSG_ID_recsys_start_motor);

        RECSYS_Unlock(RECSYS_M1 | RECSYS_M2);
        RECSYS_Start(RECSYS_M1 | RECSYS_M2);
    }
    
    RECSYS_t RECOVERY = RECSYS_Get_Struct();

    /* reading the motor sensor */
    if(RECOVERY.PIN.CFDC_UNLOCK_M1 == HIGH)
    {
        RECSYS_set_Sys(RECSYS_M1, UNLOCKED);
        RECSYS_Stop(RECSYS_M1);
    }
    
    if(RECOVERY.PIN.CFDC_UNLOCK_M2 == HIGH)
    {
        RECSYS_set_Sys(RECSYS_M2, UNLOCKED);
        RECSYS_Stop(RECSYS_M2);
    }

    if((RECOVERY.SYS.M1 == UNLOCKED) && (RECOVERY.SYS.M2 == UNLOCKED))
    {
        RECSYS_set_Sys(RECSYS_GLOBAL, UNLOCKED);
        broadcast_uart_send(MSG_ID_recsys_unlocked);
        phase_set(PHASE_DESCEND);
    }

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

    //MPU6050_t IMU = MPU6050_Get_Struct();
    //if(IMU.Az >= ????????????)
    //{
    //    phase_set(PHASE_DEPLOY);
    //}

    routines.cycles.descend += 1;
    routines.cycles.total   +=1;
}

/** ************************************************************* *
 * @brief       fifth phase for the scheduler. this phase is 
 *              set by landing on the floor (final phase)
 * 
 * ************************************************************* **/
void routine_landed(void)
{
	routines.cycles.landed += 1;
    routines.cycles.total  +=1;
}


///* recovery without adc */
//#ifndef RECSYS_NO_ADC
//    /* test if ADC has trigged the IT flag */
//    if(RECOVERY.ADC.GLOBAL == true)
//    {
//        RECSYS_Stop(RECSYS_M1 | RECSYS_M2);
//
//        /* reading the motor sensor */
//        if(RECOVERY.PIN.CFDC_UNLOCK_M1 == HIGH)
//        {
//            RECSYS_set_Sys(RECSYS_M1, UNLOCKED);
//            RECSYS_Stop(RECSYS_M1);
//        }
//        else
//        {
//            RECSYS_set_Sys(RECSYS_M1, REVERSE);
//            
//            if(reverse_M1.status == false)
//            {
//                reverse_M1.status = true;
//                reverse_M1.time_ref = HAL_GetTick();
//                RECSYS_Lock(RECSYS_M1);
//            }
//
//            reverse_M1.time_stamp = HAL_GetTick();
//            if(reverse_M1.time_ref <= (reverse_M1.time_stamp + RECSYS_REVERSE_DELAY)) 
//            {
//                RECSYS_set_Sys(RECSYS_M1, UNLOCKING);
//                RECSYS_Unlock(RECSYS_M1);
//                reverse_M1.status = false;
//            }
//        }
//
//        if(RECOVERY.PIN.CFDC_UNLOCK_M2 == HIGH)
//        {
//            RECSYS_set_Sys(RECSYS_M2, UNLOCKED);
//            RECSYS_Stop(RECSYS_M2);
//        }
//        else
//        {
//            RECSYS_set_Sys(RECSYS_M2, REVERSE);
//            
//            if(reverse_M2.status == false)
//            {
//                reverse_M2.status = true;
//                reverse_M2.time_ref = HAL_GetTick();
//                RECSYS_Lock(RECSYS_M2);
//            }
//
//            reverse_M2.time_stamp = HAL_GetTick();
//            if(reverse_M2.time_ref <= (reverse_M2.time_stamp + RECSYS_REVERSE_DELAY))
//            {
//                RECSYS_set_Sys(RECSYS_M2, UNLOCKING);
//                RECSYS_Unlock(RECSYS_M2);
//                reverse_M2.status = false;
//            }
//        }
//
//        if((RECOVERY.SYS.M1 == UNLOCKED) && (RECOVERY.SYS.M2 == UNLOCKED))
//        {
//            RECSYS_set_Sys(RECSYS_GLOBAL, UNLOCKED);
//            broadcast_uart_send(MSG_ID_recsys_unlocked);
//            phase_set(PHASE_DESCEND);
//        }
//    }
//
//    if(RECOVERY.SYS.GLOBAL == UNLOCKED)
//    {
//        phase_set(PHASE_DESCEND);
//        broadcast_uart_send(MSG_ID_recsys_unlocked);
//    }
//#endif



/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */
