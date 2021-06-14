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
#include "msg_list.h"

/* ------------------------------------------------------------- --
   Public protoypes
-- ------------------------------------------------------------- */
void     broadcast_uart_send(const uint8_t message);


#endif /* BROADCAST_INC_BROADCAST_H_ */
/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */
