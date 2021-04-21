#include "window.h"

window_t window_pool;
window_t window_it;

bool IT_flag_unlock;
bool IT_flag_unlock_enable;
bool IT_flag_relock;
bool IT_flag_relock_enable;

void window_init(void)
{
    window_pool = w_flag_lock;
    window_it   = w_flag_lock;

    IT_flag_unlock = false;
    IT_flag_relock = false;

    IT_flag_unlock_enable = true;
    IT_flag_relock_enable = true;
}

void window_set_pool_flag(window_t flag)
{
    window_pool = flag;
}

void window_set_it_flag(window_t flag)
{
    window_it = flag;
}

window_t window_get_pool_flag(void)
{
    return window_pool;
}

window_t window_get_it_flag(void)
{
   return window_it;
}

void IT_flag_window_unlock(void)
{
    if(phase_get() == PHASE_ASCEND)
    {
        IT_flag_unlock = true;
    }
}

void IT_flag_window_relock(void)
{
    if(phase_get() == PHASE_ASCEND)
    {
        IT_flag_relock = true;
    }
}

void IT_routine_window_unlock(void)
{
    if(IT_flag_unlock_enable == true)
    {
        if(phase_get() == PHASE_ASCEND)
        {
            window_it = w_flag_unlock;
            IT_flag_unlock_enable = false;
        }
    }
}

void IT_routine_window_relock(void)
{
    if(IT_flag_relock_enable == true)
    {
        if(phase_get() == PHASE_ASCEND)
        {
        	window_it = w_flag_forced;
            phase_set(PHASE_DEPLOY);
        }
        else
        {
        	window_it = w_flag_relock;
        }
    }
}

bool get_winU_IT_flag(void)
{
    return IT_flag_unlock;
}

bool get_winR_IT_flag(void)
{
    return IT_flag_relock;
}
