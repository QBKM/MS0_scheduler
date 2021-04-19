/*
 * broadcast.h
 *
 *  Created on: Apr 11, 2021
 *      Author: Quent
 */

#ifndef BROADCAST_INC_BROADCAST_H_
#define BROADCAST_INC_BROADCAST_H_

#include "stdint.h"

#define TIMEOUT_UART 10

static const uint8_t ID_phase_wait      = 0xA1;
static const uint8_t ID_phase_ascend    = 0xA2;
static const uint8_t ID_phase_deploy    = 0xA3;
static const uint8_t ID_phase_descend   = 0xA4;
static const uint8_t ID_phase_landed    = 0xA5;


static const uint8_t ID_TIMEOUT_PARACHUTE = 0xF1;


void broadcast_uart(const uint8_t message);

#endif /* BROADCAST_INC_BROADCAST_H_ */
