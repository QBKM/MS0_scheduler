/** ************************************************************* *
 * @file        tca6408a.h
 * @brief       
 * 
 * @date        2021-05-01
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef __TCA6408A__
#define __TCA6408A__

/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "stdint.h"


/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
/* TCA6408A masks */
#define TCA6408A_PIN0                   0x00
#define TCA6408A_PIN1                   0x01
#define TCA6408A_PIN2                   0x02
#define TCA6408A_PIN3                   0x04
#define TCA6408A_PIN4                   0x08
#define TCA6408A_PIN5                   0x10
#define TCA6408A_PIN6                   0x20
#define TCA6408A_PIN7                   0x40


/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
typedef enum TCA6408A_PIN_STATE_t
{
    TCA_LOW,
    TCA_HIGH,
}TCA6408A_PIN_STATE_t;

typedef enum MODE_STATE_t
{
	TCA_OUTPUT,
	TCA_INPUT
}TCA6408A_MODE_STATE_t;

typedef struct TCA6408A_t
{
    uint8_t PIN_state;
    uint8_t MODE_state;
}TCA6408A_t;
/* ------------------------------------------------------------- --
   public prototypes
-- ------------------------------------------------------------- */
uint8_t TCA6408A_Init(void);

uint8_t TCA6408A_Set_Mode(uint8_t PIN, TCA6408A_MODE_STATE_t MODE);

uint8_t TCA6408A_Write_Pin(uint8_t PIN,  TCA6408A_PIN_STATE_t STATE);
uint8_t TCA6408A_Write_Pin_All(uint8_t PINS);

uint8_t TCA6408A_Read_Pin(uint8_t PIN);
uint8_t TCA6408A_Read_Pin_All(void);

TCA6408A_t TCA6408A_Get_Struct(void);

#endif
/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */
