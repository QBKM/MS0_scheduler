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

/* phases ID */
#define MSG_ID_phase_wait           (uint8_t)0xA1
#define MSG_ID_phase_ascend         (uint8_t)0xA2
#define MSG_ID_phase_deploy         (uint8_t)0xA3
#define MSG_ID_phase_descend        (uint8_t)0xA4
#define MSG_ID_phase_landed         (uint8_t)0xA5

/* temporal window ID */
#define MSG_ID_unlock_window_IT     (uint8_t)0xF1
#define MSG_ID_unlock_window_POOL   (uint8_t)0xF2
#define MSG_ID_relock_window_IT     (uint8_t)0xF3
#define MSG_ID_relock_window_POOL   (uint8_t)0xF4

/* ------------------------------------------------------------- --
   Public protoypes
-- ------------------------------------------------------------- */
void     broadcast_uart_send(const uint8_t message);
uint8_t  broadcast_uart_receive(void);

#endif /* BROADCAST_INC_BROADCAST_H_ */
/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */
