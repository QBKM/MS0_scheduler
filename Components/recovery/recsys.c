/** ************************************************************* *
 * @file        recsys.c
 * @brief       
 * 
 * @date        2021-05-29
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#include "recsys.h"

#include "motor.h"
#include "tca6408a.h"

#include "gpio.h"

#include "config_file.h"

/* GPIO extender pins link */
#define PIN_LOCK_M1     TCA6408A_PIN0
#define PIN_UNLOCK_M1   TCA6408A_PIN1
#define PIN_LOCK_M2     TCA6408A_PIN2
#define PIN_UNLOCK_M2   TCA6408A_PIN3



RECSYS_t RECSYS;

/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void RECSYS_Init(void)
{
    PIN_STATUS_t temp_pin =
    {
        .CFDC_LOCK_M1   = LOW,
        .CFDC_UNLOCK_M1 = LOW,
        .CFDC_LOCK_M2   = LOW,
        .CFDC_UNLOCK_M2 = LOW
    };
    RECSYS.PIN = temp_pin;

    SYS_STATUS_t temp_sys =
    {
        .M1     = UNDEFINED,
        .M2     = UNDEFINED,
        .GLOBAL = UNDEFINED
    };
    RECSYS.SYS = temp_sys;

    ADC_STATUS_t temp_adc =
    {
        .M1     = false,
        .M2     = false,
        .GLOBAL = false
    };
    RECSYS.ADC = temp_adc;

    TCA6408A_Init();
    TCA6408A_Set_Mode((PIN_LOCK_M1 | PIN_UNLOCK_M1 | PIN_LOCK_M2 | PIN_UNLOCK_M2), TCA_INPUT);

    TCA6408A_Read_Pin_All();
    TCA6408A_t TCA = TCA6408A_Get_Struct();

    if(TCA.PIN_state & PIN_LOCK_M1)   
    {   
        RECSYS.PIN.CFDC_LOCK_M1 = HIGH; 
        RECSYS.SYS.M1           = LOCKED; 
    }

    if(TCA.PIN_state & PIN_UNLOCK_M1)
    {
        RECSYS.PIN.CFDC_UNLOCK_M1   = HIGH;
        RECSYS.SYS.M1               = UNLOCKED; 
    }

    if(TCA.PIN_state & PIN_LOCK_M2)
    {
        RECSYS.PIN.CFDC_LOCK_M2 = HIGH;
        RECSYS.SYS.M2           = LOCKED; 
    }     

    if(TCA.PIN_state & PIN_UNLOCK_M2)
    {
        RECSYS.PIN.CFDC_UNLOCK_M2   = HIGH;
        RECSYS.SYS.M2               = UNLOCKED; 
    }

    if(RECSYS.SYS.M1 == LOCKED && RECSYS.SYS.M2 == LOCKED) RECSYS.SYS.GLOBAL = LOCKED;
    if(RECSYS.SYS.M1 == UNLOCKED && RECSYS.SYS.M2 == UNLOCKED) RECSYS.SYS.GLOBAL = UNLOCKED;
}


/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
RECSYS_t RECSYS_Get_Struct(void)
{
    return RECSYS;
}

/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void RECSYS_Lock(uint8_t select)
{
    if(select & RECSYS_M1)
    { 
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
    }
    if(select & RECSYS_M2)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);
    }
}   

/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void RECSYS_Unlock(uint8_t select)
{
    if(select & RECSYS_M1)
    { 
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
    }
    if(select & RECSYS_M2)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);
    }
}

/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void  RECSYS_Start(uint8_t select)
{
    if(select & RECSYS_M1)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);
    }
    if(select & RECSYS_M2)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 1);
    }
}

/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void RECSYS_Stop(uint8_t select)
{
    if(select & RECSYS_M1)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);
    }
    if(select & RECSYS_M2)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 1);
    }
}

/** ************************************************************* *
 * @brief       
 * 
 * @param       select 
 * @param       status 
 * ************************************************************* **/
void RECSYS_set_Sys(uint8_t select, STATUS_t status)
{
    switch (select)
    {
    case RECSYS_M1:     RECSYS.SYS.M1       = status;   break;
    case RECSYS_M2:     RECSYS.SYS.M2       = status;   break;
    case RECSYS_GLOBAL: RECSYS.SYS.GLOBAL   = status;   break;
    default:                                            break;
    }
}

/** ************************************************************* *
 * @brief       
 * 
 * @param       select 
 * @param       status 
 * ************************************************************* **/
void RECSYS_set_Pin(uint8_t select, STATUS_t status)
{
    switch (select)
    {
    case RECSYS_LOCK_M1:    RECSYS.PIN.CFDC_LOCK_M1     = status;   break;
    case RECSYS_LOCK_M2:    RECSYS.PIN.CFDC_LOCK_M2     = status;   break;
    case RECSYS_UNLOCK_M1:  RECSYS.PIN.CFDC_UNLOCK_M1   = status;   break;
    case RECSYS_UNLOCK_M2:  RECSYS.PIN.CFDC_UNLOCK_M2   = status;   break;
    default:                                                        break;
    }
}

/** ************************************************************* *
 * @brief       
 * 
 * @param       select 
 * @param       status 
 * ************************************************************* **/
void RECSYS_set_Adc(uint8_t select, bool status)
{
    switch (select)
    {
    case RECSYS_M1:     RECSYS.ADC.M1       = status;   break;
    case RECSYS_M2:     RECSYS.ADC.M2       = status;   break;
    case RECSYS_GLOBAL: RECSYS.ADC.GLOBAL   = status;   break;
    default:                                            break;
    }
}

#ifndef RECSYS_NO_ADC
/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void IT_flag_Analog_WD(uint8_t select)
{
    switch (select)
    {
    case RECSYS_M1:
        RECSYS.ADC.M1 = true;   
        //hadc->Instance &= ~(1<<7);
        break;
    case RECSYS_M2:
        RECSYS.ADC.M2 = true;
        //hadc->Instance &= ~(1<<8);
        break;
    default:
        break;
    }

    RECSYS.ADC.GLOBAL   = true;
}
#endif