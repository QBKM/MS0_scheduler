/*
 * routines.c
 *
 *  Created on: Apr 10, 2021
 *      Author: Quent
 */

#include "routines.h"

phase_t phase;

void phase_set(phase_t NewPhase)
{
    phase = NewPhase;
}

phase_t phase_get(void)
{
    return phase;
}

void routine_wait(void)
{

}
void routine_ascend(void)
{
    /* read data */
    MPU6050_Read_All();
    DS3231_Read_All();

    /* get handler */
    DS3231_t RTC = DS3231_Get_Struct();

    /* Temporal window [pooling mode]*/
    if(RTC.Sec >= 20)
    {
        phase_set(PHASE_DEPLOY);
    }
    else if(RTC.Sec >= 17)
    {

    }
}
void routine_deploy(void)
{
    broadcast_uart(ID_phase_deploy);
    phase_set(PHASE_DESCEND);
}
void routine_descend(void)
{
    /* read data MPU6050 */
    MPU6050_Read_All();
}
void routine_landed(void)
{
	
}
