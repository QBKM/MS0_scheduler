/** ************************************************************* *
 * @file       mpu6050.h
 * @brief      
 * 
 * @date       2021-04-23
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

#ifndef __MPU6050_H__
#define __MPU6050_H__


/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include <stdint.h>

/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
/* Accel full scale range settings */
typedef enum
{
	MPU6050_AFS_2G 	= 0,
	MPU6050_AFS_4G 	= 1,
	MPU6050_AFS_8G 	= 2,
	MPU6050_AFS_16G   = 3
}MPU6050_AccelFullScale;

/* gyro full scale range settings */
typedef enum
{
	MPU6050_GFS_250_DEG_S 	= 0,
	MPU6050_GFS_500_DEG_S 	= 1,
	MPU6050_GFS_1000_DEG_S 	= 2,
	MPU6050_GFS_2000_DEG_S 	= 3
}MPU6050_GyroFullScale;

/* sample rate settings */
typedef enum
{						         /* Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV) */
	MPU6050_SR_8KHZ = 1,	   /* Gyroscope Output Rate = 8kHz when the DLPF is disabled (DLPF_CFG= 0 or 7), and 1kHz when the DLPF is enabled */
	MPU6050_SR_4KHZ = 3,
	MPU6050_SR_2KHZ = 5,
	MPU6050_SR_1KHZ = 7,
}MPU6050_SampleRate;

/* configuration structure */
typedef struct
{
	MPU6050_AccelFullScale  AFS;
	MPU6050_GyroFullScale   GFS;
	MPU6050_SampleRate      SR;
}MPU6050_config_t;

/* MPU6050 handle structure */
typedef struct 
{
    int16_t Accel_X_RAW;
    int16_t Accel_Y_RAW;
    int16_t Accel_Z_RAW;
    double Ax;
    double Ay;
    double Az;

    int16_t Gyro_X_RAW;
    int16_t Gyro_Y_RAW;
    int16_t Gyro_Z_RAW;
    double Gx;
    double Gy;
    double Gz;

    int16_t Temperature_RAW;
    float Temperature;

    double KalmanAngleX;
    double KalmanAngleY;

    MPU6050_config_t config;

} MPU6050_t;


/* ------------------------------------------------------------- --
   prototypes
-- ------------------------------------------------------------- */
uint8_t MPU6050_Init(void);
uint8_t MPU6050_Read_Accel(void);
uint8_t MPU6050_Read_Gyro(void);
uint8_t MPU6050_Read_Temp(void);
uint8_t MPU6050_Read_All(void);
uint8_t MPU6050_Read_All_Kalman(void);
MPU6050_t MPU6050_Get_Struct(void);


#endif /* INC_GY521_H_ */
/* ------------------------------------------------------------- --
   end of files
-- ------------------------------------------------------------- */
