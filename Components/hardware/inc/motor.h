/**
 * @file motor.h
 * @author Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * @brief 
 * @version 0.1
 * @date 2021-03-13
 * 
 * Mines Space
 * 
 */

/*
 * motor.h
 *
 *  Created on: Mar 12, 2021
 *      Author: Quent
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "stdbool.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% MOTOR
typedef enum
{
    DIAG_MOTOR_KO,
    DIAG_MOTOR_OK
}diag_motor_t;

typedef enum
{
	M1,
	M2
}select_motor_t;

typedef enum
{
    NO_DM_IT,
	DM_M1,
	DM_M2
}diag_motor_it_t;

typedef enum
{
	CURRENT_SENSING_OK,
	CURRENT_SENSING_KO
}current_sensing_t;

typedef enum
{
	IN,
	OUT
}H_bridge_t;

typedef struct
{
	/* IT */
	volatile diag_motor_it_t   	IT_flag;
    diag_motor_t    			DM_M1;
	diag_motor_t    			DM_M2;
    
    current_sensing_t CS_M1;		/* TO DO: IT par adc ???*/
	current_sensing_t CS_M2;

	H_bridge_t HB_A_M1;
	H_bridge_t HB_B_M1;
	H_bridge_t HB_A_M2;
	H_bridge_t HB_B_M2;

}motor_t;


void motor_init(void);

void IT_flag_diag_motor();
void IT_routine_diag_motor();

void set_motor_IT_flag(diag_motor_it_t IT_flag);diag_motor_it_t get_motor_IT_flag(void);

void set_diag_motor_status(select_motor_t motor_ID, diag_motor_t status); 	// modif avec M1 et M2
diag_motor_t get_diag_motor_status(select_motor_t motor_ID);		 		// idem


#endif /* INC_MOTOR_H_ */
