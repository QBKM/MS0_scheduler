#include "window.h"



window_t window;

void init_window()
{
    window.flag_close   = false;
    window.flag_open    = false;
    window.flag_forced  = false;
}

void set_window_open_flag(bool flag)
{
    window.flag_open    = flag;
}
void set_window_close_IT_flag(bool flag)
{
    window.flag_close    = flag;
}

void set_window_forced_flag(bool flag)
{
    window.flag_forced    = flag;
}

bool get_window_open_flag()
{
    return window.flag_open;
}

bool get_window_forced_flag()
{
    return window.flag_forced;
}

bool get_window_close_IT_flag()
{
    return window.flag_close;
}

void IT_window_close()
{
    set_window_close_IT_flag(true);
}
