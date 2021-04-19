/**
 * @file buzzer.c
 * @author Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * @brief 
 * @version 0.2
 * @date 2021-04-11
 * 
 * Mines Space
 * 
 */

/* ------------------------------------------------------------- --
   Includes
-- ------------------------------------------------------------- */
#include "buzzer.h"
#include "stm32f3xx_hal.h"
#include "tim.h"

/* ------------------------------------------------------------- --
   Private variables
-- ------------------------------------------------------------- */
buzzer_t buzzer;

/* ------------------------------------------------------------- --
   Public functions
-- ------------------------------------------------------------- */
/**
 * @brief Init the buzzer
 * 
 */
void buzzer_init(void)
{
    /* init the structure */
    buzzer_t temp =
    {
        .start_time     = 0,
        .current_time   = 0,
        .duty_cycle     = 0
    };
    buzzer = temp;

    /* start the PWM */
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
}


/**
 * @brief 	update the buzzer with the current time and settings. 
 * 
 * @param delay 
 * @param duty_cycle 
 */
void buzzer_update(uint16_t delay, float duty_cycle)
{
    /* update the values */
    buzzer.current_time = HAL_GetTick();
    buzzer.duty_cycle   = duty_cycle;

    /* reset the start time if the time is out of window */
    if(buzzer.current_time > (buzzer.start_time + delay))
    {
        buzzer.start_time = HAL_GetTick();
    }

    /* low part of the duty cycle */
    else if(buzzer.current_time < buzzer.start_time + (delay * duty_cycle))
    {
        htim1.Instance->CCER |= (1 << 10);
    }

    /* high part of the duty cycle */
    else if(buzzer.current_time > buzzer.start_time + (delay * duty_cycle))
    {
        htim1.Instance->CCER &= ~(1 << 10);
    }
}
