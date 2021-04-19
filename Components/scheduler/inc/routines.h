/**
 * @file routines.h
 * @author Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * @brief 
 * @version 0.1
 * @date 2021-04-11
 * 
 * Mines Space
 * 
 */

#ifndef INC_ROUTINES_H_
#define INC_ROUTINES_H_

#include "i2c.h"
#include "mpu6050.h"
#include "ds3231.h"

typedef enum
{
  PHASE_WAIT,
  PHASE_ASCEND,
  PHASE_DEPLOY,
  PHASE_DESCEND,
  PHASE_LANDED
}phase_t;

void phase_set(phase_t NewPhase);
phase_t phase_get(void);

void routine_wait(void);
void routine_ascend(void);
void routine_deploy(void);
void routine_descend(void);
void routine_landed(void);

#endif /* INC_ROUTINES_H_ */
