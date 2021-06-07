/** ************************************************************* *
 * @file        recsys.h
 * @brief       
 * 
 * @date        2021-05-29
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef __RECSYS_H__
#define __RECSYS_H__

#include "stdbool.h"
#include "stdint.h"

/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
#define RECSYS_M1           0x1
#define RECSYS_M2           0x2
#define RECSYS_GLOBAL       0x4
#define RECSYS_LOCK_M1      0x8
#define RECSYS_LOCK_M2      0x16
#define RECSYS_UNLOCK_M1    0x32
#define RECSYS_UNLOCK_M2    0x64

/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef enum
{
	LOW,
	HIGH
}RECSYS_PIN_STATE_t;

typedef enum
{
    UNDEFINED,
    LOCKED,
    LOCKING,
    UNLOCKED,
    UNLOCKING,
    REVERSE,
}STATUS_t;

typedef struct
{
    STATUS_t M1;
    STATUS_t M2;
    STATUS_t GLOBAL;
}SYS_STATUS_t;

typedef struct 
{
    /* limit switch for motor 1 and 2 */
	RECSYS_PIN_STATE_t CFDC_LOCK_M1;
	RECSYS_PIN_STATE_t CFDC_UNLOCK_M1;
	RECSYS_PIN_STATE_t CFDC_LOCK_M2;
	RECSYS_PIN_STATE_t CFDC_UNLOCK_M2;
}PIN_STATUS_t;

typedef struct 
{
    bool M1;
    bool M2;
    bool GLOBAL;
}ADC_STATUS_t;


typedef struct 
{
    SYS_STATUS_t SYS;
    PIN_STATUS_t PIN;
    ADC_STATUS_t ADC;
}RECSYS_t;

/* ------------------------------------------------------------- --
   function prototypes
-- ------------------------------------------------------------- */
void RECSYS_Init(void);

void RECSYS_Lock(uint8_t select);
void RECSYS_Unlock(uint8_t select);

void RECSYS_Start(uint8_t select);
void RECSYS_Stop(uint8_t select);

void RECSYS_set_Pin(uint8_t select, STATUS_t status);
void RECSYS_set_Sys(uint8_t select, STATUS_t status);

PIN_STATUS_t RECSYS_get_Pin(void);
SYS_STATUS_t RECSYS_get_Sys(void);

RECSYS_t RECSYS_Get_Struct(void);

void RECSYS_button_mngr(void);

#endif
