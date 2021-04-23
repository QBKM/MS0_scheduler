/** ************************************************************* *
 * @file       config_file.h
 * @brief      
 * 
 * @date       2021-04-22
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

/* 	+-------------------+*/
/*	| LIST OF CONSTANTS |*/
/*	+-------------------+*/

/**************************************************************** *
 * 	PERIPHERALS
 **************************************************************** */
/* UART */
#define TIMEOUT_UART    	10				/* [ms] timeout value */

/* I2C */
#define TIMEOUT_I2C     	10				/* [ms] timeout value */


/**************************************************************** *
 * 	SCHEDULER
 **************************************************************** */
/* Synchro */
#define SYNC_DELAY  		10 				/* [ms] time the scheduler wait to get synchronized */

/* Temporal windows */
#define WINDOW_TIMEREF      17.7			/* [ms] reference to deploy the parachute */
#define WINDOW_UNLOCK       15.93   		/* [ms] - 10% */
#define WINDOW_RELOCK       19.47   		/* [ms] + 10% */

#define WINDOW_UNLOCK_RTC   16      		/* [ms] value for the double window safety with I2C RTC */
#define WINDOW_RELOCK_RTC   20      		/* [ms] value for the double window safety with I2C RTC */


/**************************************************************** *
 * 	HARDWARE
 **************************************************************** */
/* MPU6050 */
#define MPU6050_ADDR 		(0x69 << 1) 	/* ( << 1 because of the R/W bit */

/* DS3231 */
#define DS3231_ADDR     	(0x68 << 1)		/* ( << 1 because of the R/W bit */