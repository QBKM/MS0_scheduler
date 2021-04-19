/*
 * broadcast.c
 *
 *  Created on: Apr 11, 2021
 *      Author: Quent
 */

#include "broadcast.h"
#include "usart.h"

char MSG[10];

void broadcast_uart(const uint8_t message)
{
	sprintf(MSG, "%c", message);
    HAL_UART_Transmit(&huart1, (uint8_t*)MSG, strlen(MSG), TIMEOUT_UART);
}
