/** ************************************************************* *
 * @file       window.c
 * @brief      
 * 
 * @date       2021-04-22
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/
/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "window.h"

/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
window_t window_pool;
window_t window_it;

bool IT_flag_unlock;
bool IT_flag_relock;

/* ------------------------------------------------------------- --
   public functions
-- ------------------------------------------------------------- */
/** ************************************************************* *
 * @brief      init the window
 * 
 * ************************************************************* **/
void window_init(void)
{
    window_pool.unlock = false;
    window_pool.relock = false;

    window_it.unlock = false;
    window_it.relock = false;
}

/** ************************************************************* *
 * @brief       Set the unlock IT flag to true
 * 
 * ************************************************************* **/
void IT_flag_window_unlock(void)
{
    if(phase_get() == PHASE_ASCEND) IT_flag_unlock = true;
}

/** ************************************************************* *
 * @brief      Set the relock IT flag to true
 * 
 * ************************************************************* **/
void IT_flag_window_relock(void)
{
    if(phase_get() == PHASE_ASCEND) IT_flag_relock = true;
}

/** ************************************************************* *
 * @brief      routine for the unlock IT. set the unlock flag to
 *             true
 * 
 * ************************************************************* **/
void IT_routine_window_unlock(void)
{
	IT_flag_unlock = false;
    HAL_TIM_Base_Stop(&htim3);

    if(phase_get() == PHASE_ASCEND) window_it.unlock = true;
}

/** ************************************************************* *
 * @brief      routine for the unlock IT. set the relock flag to
 *             true
 * 
 * ************************************************************* **/
void IT_routine_window_relock(void)
{
	IT_flag_relock = false;
    HAL_TIM_Base_Stop(&htim2);

    if(phase_get() == PHASE_ASCEND)
    {
        window_it.relock = true;
        phase_set(PHASE_DEPLOY);
    }
}

/** ************************************************************* *
 * @brief      return the unlock IT flag
 * 
 * @return     true 
 * @return     false 
 * ************************************************************* **/
bool get_winU_IT_flag(void)
{
    return IT_flag_unlock;
}

/** ************************************************************* *
 * @brief      return the relock IT flag
 * 
 * @return     true 
 * @return     false 
 * ************************************************************* **/
bool get_winR_IT_flag(void)
{
    return IT_flag_relock;
}

/** ************************************************************* *
 * @brief      return true if the rtc is in unlock window
 * 
 * @return     true 
 * @return     false 
 * ************************************************************* **/
bool window_check_RTC_unlock(void)
{
    DS3231_Read_All();
    DS3231_t curernt_time = DS3231_Get_Struct();

    if(curernt_time.Sec >= WINDOW_RELOCK_RTC)
    {
        if(phase_get() == PHASE_ASCEND) 
        {
            window_pool.relock = true;
        }
        phase_set(PHASE_DEPLOY);
        return false;
    }
    else if(curernt_time.Sec >= WINDOW_UNLOCK_RTC)
    {
        window_pool.unlock = true;
        return true;
    }
    else
    {
    	return false;
    }
}
