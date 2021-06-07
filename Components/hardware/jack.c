/** ************************************************************* *
 * @file        jack.c
 * @brief       
 * 
 * @date        2021-04-24
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

/* ------------------------------------------------------------- --
   Includes
-- ------------------------------------------------------------- */
#include "jack.h"
#include "routines.h"
#include "ds3231.h"

#include "tim.h"

#include "broadcast.h"

/* ------------------------------------------------------------- --
   Types
-- ------------------------------------------------------------- */
jack_t jack;

/* ------------------------------------------------------------- --
   Public functions
-- ------------------------------------------------------------- */
/** ************************************************************* *
 * @brief       init the jack struct
 * 
 * ************************************************************* **/
void jack_init(void)
{
    jack_t temp =
    {
        .IT_flag        = false,
        .jack_status    = JACK_PLUGGED
    };
    jack = temp;
}

/** ************************************************************* *
 * @brief     Manage the jack IT flag  
 * 
 * ************************************************************* **/
void IT_flag_jack(void)
{
	if((jack.IT_flag == false) && (phase_get() == PHASE_WAIT))
	{
		jack.IT_flag = true;
	}
} 

/** ************************************************************* *
 * @brief       Construct a new it routine jack object
 * 
 * ************************************************************* **/
void IT_routine_jack(void)
{
    jack.IT_flag		= false;
    jack.jack_status 	= true;
    
    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_SR_UIF);
    HAL_TIM_Base_Start_IT(&htim2);

    __HAL_TIM_CLEAR_FLAG(&htim3, TIM_SR_UIF);
    HAL_TIM_Base_Start_IT(&htim3);

    DS3231_Init();
    
    broadcast_uart_send(MSG_ID_phase_ascend);
	phase_set(PHASE_ASCEND);
}

/** ************************************************************* *
 * @brief       Set the jack IT flag object
 * 
 * @param       IT_flag 
 * ************************************************************* **/
void set_jack_IT_flag(bool IT_flag)
{
    jack.IT_flag = IT_flag;
}

/** ************************************************************* *
 * @brief       Get the jack IT flag object
 * 
 * @return      true 
 * @return      false 
 * ************************************************************* **/
bool get_jack_IT_flag(void)
{
    return jack.IT_flag;
}

/** ************************************************************* *
 * @brief       Get the jack status object
 * 
 * @return      jack_status_t 
 * ************************************************************* **/
jack_status_t get_jack_status(void)
{
    return jack.jack_status;
}

/** ************************************************************* *
 * @brief       Set the jack status object
 * 
 * @param       jack_status 
 * ************************************************************* **/
void set_jack_status(jack_status_t jack_status)
{
    jack.jack_status = jack_status;
}

/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */
