/** ************************************************************* *
 * @file        tca6408a.c
 * @brief       
 * 
 * @date        2021-05-01
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

/* ------------------------------------------------------------- --
   include
-- ------------------------------------------------------------- */
#include "tca6408a.h"
#include "config_file.h"
#include "i2c.h"


/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
#ifndef TIMEOUT_I2C
#define TIMEOUT_I2C 10
#endif

/* TCA6408A address */
#ifndef TCA6408A_ADDR
#define TCA6408A_ADDR (0x20 << 1)
#endif

/*  register map */
#define TCA6408A_INPUT                  0x00
#define TCA6408A_OUTPUT                 0x01
#define TCA6408A_POLARITY               0x02
#define TCA6408A_CONFIG                 0x03


/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
TCA6408A_t TCA6408A;


/* ============================================================= ==
   public functions
== ============================================================= */
/** ************************************************************* *
 * @brief       init the TCA6408A
 * 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t TCA6408A_Init(void)
{
    uint8_t data[1];

    TCA6408A_t temp = 
    {
        .PIN_state  = 0xFF,     /* All pin to low */
        .MODE_state = 0x00      /* All mode to output */
    };
    TCA6408A = temp;
    
    /* send to i2c */
    data[0] = TCA6408A.MODE_state;
    if(HAL_I2C_Mem_Write(&hi2c1, TCA6408A_ADDR, TCA6408A_CONFIG, 1, data, sizeof(data), TIMEOUT_I2C)) return HAL_ERROR;

    data[0] = TCA6408A.PIN_state;
    if(HAL_I2C_Mem_Write(&hi2c1, (TCA6408A_ADDR), TCA6408A_OUTPUT, 1, data, sizeof(data), TIMEOUT_I2C)) return HAL_ERROR;

    return HAL_OK;
}


/** ************************************************************* *
 * @brief       set the mode for a pin
 * 
 * @param       PIN 
 * @param       MODE 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t TCA6408A_Set_Mode(uint8_t PIN, TCA6408A_MODE_STATE_t MODE)
{
    uint8_t data[1];

    /* apply the pin mask */
    if(MODE == TCA_INPUT)   TCA6408A.MODE_state |= PIN;
    if(MODE == TCA_OUTPUT)  TCA6408A.MODE_state &= ~PIN;

    /* send to i2c */
    if(HAL_I2C_Mem_Write(&hi2c1, TCA6408A_ADDR, TCA6408A_CONFIG, 1, data, sizeof(data), TIMEOUT_I2C)) return HAL_ERROR;

    return HAL_OK;
}


/** ************************************************************* *
 * @brief       write a pin value
 * 
 * @param       PIN 
 * @param       STATE 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t TCA6408A_Write_Pin(uint8_t PIN, TCA6408A_PIN_STATE_t STATE)
{
    uint8_t data[1];

    /* apply the pin mask */
    if(STATE == TCA_HIGH) TCA6408A.PIN_state |= PIN;
    if(STATE == TCA_LOW)  TCA6408A.PIN_state &= ~PIN;
    data[0] = TCA6408A.PIN_state;

    /* send to i2c */
    if(HAL_I2C_Mem_Write(&hi2c1, TCA6408A_ADDR, TCA6408A_OUTPUT, 1, data, sizeof(data), TIMEOUT_I2C)) return HAL_ERROR;

    return HAL_OK;
}


/** ************************************************************* *
 * @brief       write all pins values
 * 
 * @param       PINS 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t TCA6408A_Write_Pin_All(uint8_t PINS)
{
    uint8_t data[1];

    /* update the pins status with the new values */
    TCA6408A.PIN_state = PINS;
    data[0] = TCA6408A.PIN_state;

    /* send to i2c */
    if(HAL_I2C_Mem_Write(&hi2c1, TCA6408A_ADDR, TCA6408A_OUTPUT, 1, data, sizeof(data), TIMEOUT_I2C)) return HAL_ERROR;
    
    return HAL_OK;
}


/** ************************************************************* *
 * @brief       read a pin value
 * 
 * @param       PIN 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t TCA6408A_Read_Pin(uint8_t PIN)
{
    uint8_t data[1];

    /* read all the pin and select the good one */
    if(TCA6408A_Read_Pin_All()) return 0xFF;
    data[0] = TCA6408A.PIN_state &= ~PIN;

    return data[0];
}


/** ************************************************************* *
 * @brief       read all pins values
 * 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t TCA6408A_Read_Pin_All(void)
{
    uint8_t data[1];

    /* read the i2c */
    if(HAL_I2C_Mem_Read(&hi2c1, TCA6408A_ADDR, TCA6408A_INPUT, 1, data, sizeof(data), TIMEOUT_I2C)) return HAL_ERROR;
    TCA6408A.PIN_state = data[0];

    return HAL_OK;
}

/** ************************************************************* *
 * @brief       
 * 
 * @return      TCA6408A_t 
 * ************************************************************* **/
TCA6408A_t TCA6408A_Get_Struct(void)
{
	return TCA6408A;
}

/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */
