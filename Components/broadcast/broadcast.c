/** ************************************************************* *
 * @file       broadcast.c
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
#include "broadcast.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "config_file.h"

/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
#ifndef TIMEOUT_UART
#define TIMEOUT_UART 10
#endif

/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
char MSG[10];

/* ------------------------------------------------------------- --
   Public functions
-- ------------------------------------------------------------- */
/** ************************************************************* *
 * @brief      send on uart a short ID for other boards
 * 
 * @param      message 
 * ************************************************************* **/
void broadcast_uart(const uint8_t message)
{
	sprintf(MSG, "%c", message);
    HAL_UART_Transmit(&huart1, (uint8_t*)MSG, strlen(MSG), TIMEOUT_UART);
}
