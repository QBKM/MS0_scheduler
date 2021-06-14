/** ************************************************************* *
 * @file        alim.c
 * @brief       
 * 
 * @date        2021-06-13
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#include "alim.h"
#include "broadcast.h"
#include "gpio.h"


void ALIM_check_all(void)
{
    ALIM_check_motor();
    ALIM_check_seq();
}

void ALIM_check_motor(void)
{
    if(HAL_GPIO_ReadPin(GPIOB, MONITORING_ALIM_MOTOR_Pin) == GPIO_PIN_SET)
    {
        broadcast_uart_send(MSG_ID_alim_motor_default);
    }
    else
    {
        broadcast_uart_send(MSG_ID_alim_motor_ok);
    }
}

void ALIM_check_seq(void)
{
    if(HAL_GPIO_ReadPin(GPIOB, MONITORING_ALIM_SEQ_Pin) == GPIO_PIN_SET)
    {
        broadcast_uart_send(MSG_ID_alim_seq_default);
    }
    else
    {
        broadcast_uart_send(MSG_ID_alim_seq_ok);
    }
}
