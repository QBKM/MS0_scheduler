/** ************************************************************* *
 * @file       synchro.c
 * @brief      
 * 
 * @date       2021-04-23
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "synchro.h"
#include "stdint.h"
#include "config_file.h"

/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
#ifndef SYNC_DELAY
#define SYNC_DELAY  10 /* ms */
#endif

/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
synchro_t   synchro;

uint32_t    duration;
uint32_t    max_duration;

/* ------------------------------------------------------------- --
   functions
-- ------------------------------------------------------------- */
/** ************************************************************* *
 * @brief      init the synchro
 * 
 * ************************************************************* **/
void synchro_init(void)
{
    synchro_t temp =
    {
        .time_ref   = 0,
        .time_sync  = 0
    };
    synchro = temp;
}

/** ************************************************************* *
 * @brief      update the ref time to prepare the synchro
 * 
 * ************************************************************* **/
void synchro_update(void)
{
    synchro.time_ref    = HAL_GetTick();
    synchro.time_sync   = synchro.time_ref + SYNC_DELAY;
}

/** ************************************************************* *
 * @brief      wait for synchro
 * 
 * ************************************************************* **/
void synchro_wait(void)
{ 
    /* get the cycle duration */
    duration = (HAL_GetTick() - synchro.time_ref);

    /* record the max cycle duration */
    if(max_duration < duration) max_duration = duration;

    /* idle until the sync time */
    while(HAL_GetTick() < synchro.time_sync);
}

/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */