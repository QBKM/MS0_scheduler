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
#include "tim.h"

#include "config_file.h"

/* GPIO extender pins link */
#define PIN_LOCK_M1     TCA6408A_PIN1
#define PIN_UNLOCK_M1   TCA6408A_PIN2
#define PIN_LOCK_M2     TCA6408A_PIN3
#define PIN_UNLOCK_M2   TCA6408A_PIN4

#define PIN_LOCK_USER   TCA6408A_PIN5
#define PIN_UNLOCK_USER TCA6408A_PIN6



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
    TCA6408A_Set_Mode((PIN_LOCK_M1 | PIN_UNLOCK_M1 | PIN_LOCK_M2 | PIN_UNLOCK_M2 | PIN_LOCK_USER | PIN_UNLOCK_USER), TCA_INPUT);

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

    /* disable pwm channel 1 */
    htim1.Instance->CCER &= ~(1 << 2);

    /* disable pwm channel 2 */
    htim1.Instance->CCER &= ~(1 << 6);
    
    RECSYS_Update();
}

/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void RECSYS_Update(void)
{
	TCA6408A_Read_Pin_All();
    TCA6408A_t TCA = TCA6408A_Get_Struct();

    if(~TCA.PIN_state & PIN_LOCK_M1)
    {   
        RECSYS.PIN.CFDC_LOCK_M1 = LOW;
        RECSYS.SYS.M1           = LOCKED; 
    }

    if( ~TCA.PIN_state & PIN_UNLOCK_M1)
    {
        RECSYS.PIN.CFDC_UNLOCK_M1   = LOW;
        RECSYS.SYS.M1               = UNLOCKED; 
    }

    if(~TCA.PIN_state & PIN_LOCK_M2)
    {
        RECSYS.PIN.CFDC_LOCK_M2 = LOW;
        RECSYS.SYS.M2           = LOCKED; 
    }     

    if(~TCA.PIN_state & PIN_UNLOCK_M2)
    {
        RECSYS.PIN.CFDC_UNLOCK_M2   = LOW;
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
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, LOW);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, HIGH);
    }
    if(select & RECSYS_M2)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, LOW);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, HIGH);
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
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, HIGH);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, LOW);
    }
    if(select & RECSYS_M2)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, HIGH);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, LOW);
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
        //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, HIGH);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, HIGH);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, HIGH);
        htim1.Instance->CCR1 = 1999;
        htim1.Instance->CCER |= (1 << 2);
    }
    if(select & RECSYS_M2)
    {
        //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, HIGH);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, HIGH);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, HIGH);
        htim1.Instance->CCR2 = 1999;
        htim1.Instance->CCER |= (1 << 6);
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
        //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, LOW);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, LOW);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, LOW);
        htim1.Instance->CCR1 = 0;
        htim1.Instance->CCER &= ~(1 << 2);
        
    }
    if(select & RECSYS_M2)
    {
        //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, LOW);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, LOW);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, LOW);
        htim1.Instance->CCR2 = 0;
        htim1.Instance->CCER &= ~(1 << 6);
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
 * @return      PIN_STATUS_t 
 * ************************************************************* **/
PIN_STATUS_t RECSYS_get_Pin(void)
{
    return RECSYS.PIN;
}

/** ************************************************************* *
 * @brief       
 * 
 * @return      SYS_STATUS_t 
 * ************************************************************* **/
SYS_STATUS_t RECSYS_get_Sys(void)
{
    return RECSYS.SYS;
}

/** ************************************************************* *
 * @brief       open or close the recovery system with the buttons
 * 
 * ************************************************************* **/
void RECSYS_user_button(void)
{
    TCA6408A_Read_Pin_All();
    TCA6408A_t TCA = TCA6408A_Get_Struct();

    /* check button 1 */
    if((~TCA.PIN_state) & (TCA6408A_PIN5))
    {
        RECSYS_Unlock(RECSYS_M1 | RECSYS_M2);
        RECSYS_Start(RECSYS_M1 | RECSYS_M2);
        
        htim1.Instance->CCR1 = 666;
        htim1.Instance->CCR2 = 666;
        
        htim1.Instance->CCER |= (1 << 2);
        htim1.Instance->CCER |= (1 << 6);
    }
    else
    {
        RECSYS_Stop(RECSYS_M1 | RECSYS_M2);
        htim1.Instance->CCER &= ~(1 << 2);
        htim1.Instance->CCER &= ~(1 << 6);
    }

    /* check button 2 */
    if((~TCA.PIN_state) & (TCA6408A_PIN6))
    {
        RECSYS_Lock(RECSYS_M1 | RECSYS_M2);
        RECSYS_Start(RECSYS_M1 | RECSYS_M2);

        htim1.Instance->CCR1 = 666;
        htim1.Instance->CCR2 = 666;

        htim1.Instance->CCER |= (1 << 2);
        htim1.Instance->CCER |= (1 << 6);
    }
    else
    {
        RECSYS_Stop(RECSYS_M1 | RECSYS_M2);
        htim1.Instance->CCER &= ~(1 << 2);
        htim1.Instance->CCER &= ~(1 << 6);
    }
}

/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */
