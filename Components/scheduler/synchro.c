/**
 * @file synchro.c
 * @author Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * @brief 
 * @version 0.1
 * @date 2021-04-11
 * 
 * Mines Space
 * 
 */

#include "synchro.h"
#include "stdint.h"

#define SYNC_DELAY  10 /* ms */

synchro_t synchro;
uint32_t duration;
uint32_t max_duration;

/**
 * @brief init the synchro
 * 
 */
void synchro_init(void)
{
    synchro_t temp =
    {
        .time_ref   = 0,
        .time_sync  = 0
    };
    synchro = temp;
}

/**
 * @brief update the ref time to prepare the synchro
 * 
 */
void synchro_update(void)
{
    synchro.time_ref    = HAL_GetTick();
    synchro.time_sync   = synchro.time_ref + SYNC_DELAY;
}

/**
 * @brief wait for synchro
 * 
 */
void synchro_wait(void)
{ 
    /* get the cycle duration */
    duration = (HAL_GetTick() - synchro.time_ref);

    /* record the max cycle duration */
    if(max_duration < duration) max_duration = duration;

    /* idle until the sync time */
    while(HAL_GetTick() < synchro.time_sync);
}