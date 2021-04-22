/** ************************************************************* *
 * @file       broadcast.h
 * @brief      
 * 
 * @date       2021-04-22
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/
#ifndef BROADCAST_INC_BROADCAST_H_
#define BROADCAST_INC_BROADCAST_H_

/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "stdint.h"

/* ------------------------------------------------------------- --
   constant
-- ------------------------------------------------------------- */
#define TIMEOUT_UART 10

/* phases ID */
const uint8_t MSG_ID_phase_wait      = 0xA1;
const uint8_t MSG_ID_phase_ascend    = 0xA2;
const uint8_t MSG_ID_phase_deploy    = 0xA3;
const uint8_t MSG_ID_phase_descend   = 0xA4;
const uint8_t MSG_ID_phase_landed    = 0xA5;

/* temporal window ID */
const uint8_t MSG_ID_unlock_window_IT    = 0xF1;
const uint8_t MSG_ID_unlock_window_POOL  = 0xF2;
const uint8_t MSG_ID_relock_window_IT    = 0xF3;
const uint8_t MSG_ID_relock_window_POOL  = 0xF4;

/* ------------------------------------------------------------- --
   Public protoypes
-- ------------------------------------------------------------- */
void broadcast_uart(const uint8_t message);


#endif /* BROADCAST_INC_BROADCAST_H_ */
/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */