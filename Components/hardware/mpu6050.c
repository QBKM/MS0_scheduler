/** ************************************************************* *
 * @file        mpu6050.c
 * @brief       
 * 
 * @date        2021-04-24
 * @author     Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/


/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "mpu6050.h"
#include "config_file.h"
#include <math.h>
#include "i2c.h"

#ifndef TIMEOUT_I2C
#define TIMEOUT_I2C 10
#endif


/* ------------------------------------------------------------- --
   defines
-- ------------------------------------------------------------- */
#define MPU6050_WHO_AM_I_REG 		0x75
#define MPU6050_PWR_MGMT_1_REG 		0x6B
#define MPU6050_SMPLRT_DIV_REG 		0x19
#define MPU6050_ACCEL_CONFIG_REG 	0x1C
#define MPU6050_ACCEL_XOUT_H_REG 	0x3B
#define MPU6050_TEMP_OUT_H_REG 		0x41
#define MPU6050_GYRO_CONFIG_REG 	0x1B
#define MPU6050_GYRO_XOUT_H_REG 	0x43

#ifndef MPU6050_ADDR
#define MPU6050_ADDR 				(0x69 << 1) 	/* ( << 1 because of the R/W bit */
#endif

#define RAD_TO_DEG 					57.295779513082320876798154814105


/* ------------------------------------------------------------- --
   types
-- ------------------------------------------------------------- */
/* Kalman structure */
typedef struct 
{
    double Q_angle;
    double Q_bias;
    double R_measure;
    double angle;
    double bias;
    double P[2][2];
} Kalman_t;


/* ------------------------------------------------------------- --
   private prototypes
-- ------------------------------------------------------------- */
double MPU6050_Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt);


/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
static uint32_t timer = 0;

/* MPU6050 struct */
static MPU6050_t MPU6050;
    
/* initialiaze the X axis Kalman */
static Kalman_t KalmanX = 
{
    .Q_angle   = 0.001f,
    .Q_bias    = 0.003f,
    .R_measure = 0.03f
};
/* initialiaze the Y axis Kalman */
static Kalman_t KalmanY = 
{
    .Q_angle   = 0.001f,
    .Q_bias    = 0.003f,
    .R_measure = 0.03f,
};

/* accel correctors */
const double Accel_X_Y_Z_corrector = 2048.0; 
/*  AFS_SEL = 2g ->  16384
    AFS_SEL = 4g ->  8192
    AFS_SEL = 8g ->  4096
    AFS_SEL = 16g->  2048
*/

/* gyro correctors */
const double Gyro_X_Y_Z_corrector = 16.4;
/*  FS_SEL = 250 ->  131
    FS_SEL = 500 ->  65.5
    FS_SEL = 1000->  32.8
    FS_SEL = 2000->  16.4
*/


/* ============================================================= ==
   private functions
== ============================================================= */
/** ************************************************************* *
 * @brief       apply the kalman filter with the input args and
 *              return the angle
 * 
 * @param       Kalman 
 * @param       newAngle 
 * @param       newRate 
 * @param       dt 
 * @return      double 
 * ************************************************************* **/
double MPU6050_Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt) 
{
    double rate = newRate - Kalman->bias;
    Kalman->angle += dt * rate;

    Kalman->P[0][0] += dt * (dt * Kalman->P[1][1] - Kalman->P[0][1] - Kalman->P[1][0] + Kalman->Q_angle);
    Kalman->P[0][1] -= dt * Kalman->P[1][1];
    Kalman->P[1][0] -= dt * Kalman->P[1][1];
    Kalman->P[1][1] += Kalman->Q_bias * dt;

    double S = Kalman->P[0][0] + Kalman->R_measure;
    double K[2];
    K[0] = Kalman->P[0][0] / S;
    K[1] = Kalman->P[1][0] / S;

    double y = newAngle - Kalman->angle;
    Kalman->angle += K[0] * y;
    Kalman->bias += K[1] * y;

    double P00_temp = Kalman->P[0][0];
    double P01_temp = Kalman->P[0][1];

    Kalman->P[0][0] -= K[0] * P00_temp;
    Kalman->P[0][1] -= K[0] * P01_temp;
    Kalman->P[1][0] -= K[1] * P00_temp;
    Kalman->P[1][1] -= K[1] * P01_temp;

    return Kalman->angle;
};


/* ============================================================= ==
   public functions
== ============================================================= */
/** ************************************************************* *
 * @brief       init the MPU6050
 * 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t MPU6050_Init(void) {
    uint8_t check;
    uint8_t data;

    /* initialize the accel to 0 */
    MPU6050.Ax = 0.0;
    MPU6050.Ay = 0.0;
    MPU6050.Az = 0.0;

    /* initialize the gyro to 0 */
    MPU6050.Gx = 0.0;
    MPU6050.Gy = 0.0;
    MPU6050.Gz = 0.0;

    /* Structure to configure the MPU6050 */
    MPU6050_config_t config =
    {
        .AFS = 	MPU6050_AFS_16G,
        .GFS = 	MPU6050_GFS_2000_DEG_S,
        .SR  =	MPU6050_SR_1KHZ
    };
    MPU6050.config = config;

    /* check device ID WHO_AM_I */
    if(HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, MPU6050_WHO_AM_I_REG, 1, &check, sizeof(data), TIMEOUT_I2C)) return HAL_ERROR;

    /* 0x68 will be returned by the sensor if everything goes well */
    if (check != (0x68)) return HAL_ERROR;

	/* power management register 0X6B we should write all 0's to wake the sensor up */
	data = 0x0;
	if(HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, MPU6050_PWR_MGMT_1_REG, 1, &data, sizeof(data), TIMEOUT_I2C)) return HAL_ERROR;

	/* Set DATA RATE of 1KHz by writing SMPLRT_DIV register */
	data = (MPU6050.config.SR);
	if(HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, MPU6050_SMPLRT_DIV_REG, 1, &data, sizeof(data), TIMEOUT_I2C)) return HAL_ERROR;

	/* Set accelerometer configuration in ACCEL_CONFIG Register */
	data = (MPU6050.config.AFS <<3);
	if(HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, MPU6050_ACCEL_CONFIG_REG, 1, &data, sizeof(data), TIMEOUT_I2C)) return HAL_ERROR;

	/* Set Gyroscopic configuration in GYRO_CONFIG Register */
	data = (MPU6050.config.GFS <<3);
	if(HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, MPU6050_GYRO_CONFIG_REG, 1, &data, sizeof(data), TIMEOUT_I2C)) return HAL_ERROR;

	return HAL_OK;
}

/** ************************************************************* *
 * @brief       read the accel data
 * 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t MPU6050_Read_Accel(void)
{
    uint8_t data[6];

    /* Read 6 BYTES of data starting from ACCEL_XOUT_H register */
    if(HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H_REG, 1, data, sizeof(data), TIMEOUT_I2C)) return HAL_ERROR;

    MPU6050.Accel_X_RAW = (int16_t) (data[0] << 8 | data[1]);
    MPU6050.Accel_Y_RAW = (int16_t) (data[2] << 8 | data[3]);
    MPU6050.Accel_Z_RAW = (int16_t) (data[4] << 8 | data[5]);

    /* convert the RAW values into acceleration in 'g' */
    MPU6050.Ax = MPU6050.Accel_X_RAW / Accel_X_Y_Z_corrector;
    MPU6050.Ay = MPU6050.Accel_Y_RAW / Accel_X_Y_Z_corrector;
    MPU6050.Az = MPU6050.Accel_Z_RAW / Accel_X_Y_Z_corrector;

    return HAL_OK;
}

/** ************************************************************* *
 * @brief       read the gyro data
 * 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t MPU6050_Read_Gyro(void)
{
    uint8_t data[6];

    // Read 6 BYTES of data starting from GYRO_XOUT_H register
    if(HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, MPU6050_GYRO_XOUT_H_REG, 1, data, sizeof(data), TIMEOUT_I2C)) return HAL_ERROR;

    MPU6050.Gyro_X_RAW = (int16_t) (data[0] << 8 | data[1]);
    MPU6050.Gyro_Y_RAW = (int16_t) (data[2] << 8 | data[3]);
    MPU6050.Gyro_Z_RAW = (int16_t) (data[4] << 8 | data[5]);

    /* convert the RAW values into dps (deg/s) */
    MPU6050.Gx = MPU6050.Gyro_X_RAW / Gyro_X_Y_Z_corrector;
    MPU6050.Gy = MPU6050.Gyro_Y_RAW / Gyro_X_Y_Z_corrector;
    MPU6050.Gz = MPU6050.Gyro_Z_RAW / Gyro_X_Y_Z_corrector;

    return HAL_OK;
}

/** ************************************************************* *
 * @brief       read the temperature
 * 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t MPU6050_Read_Temp(void)
{
    uint8_t data[2];
    int16_t temp;

    // Read 2 BYTES of data starting from TEMP_OUT_H_REG register
    if(HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, MPU6050_TEMP_OUT_H_REG, 1, data, sizeof(data), TIMEOUT_I2C)) return HAL_ERROR;

    temp = (int16_t) (data[0] << 8 | data[1]);
    MPU6050.Temperature = (float) ((int16_t) temp / (float) 340.0 + (float) 36.53);

    return HAL_OK;
}

/** ************************************************************* *
 * @brief       read all
 * 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t MPU6050_Read_All(void)
{
    uint8_t data[14];

    // Read 14 BYTES of data starting from ACCEL_XOUT_H register
    if(HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H_REG, 1, data, sizeof(data), TIMEOUT_I2C)) return HAL_ERROR;

    /*< get accel >*/
    MPU6050.Accel_X_RAW = (int16_t) (data[0] << 8 | data[1]);
    MPU6050.Accel_Y_RAW = (int16_t) (data[2] << 8 | data[3]);
    MPU6050.Accel_Z_RAW = (int16_t) (data[4] << 8 | data[5]);

    /*< get temperature >*/
    MPU6050.Temperature_RAW = (int16_t) (data[6] << 8 | data[7]);

    /*< get gyro >*/
    MPU6050.Gyro_X_RAW = (int16_t) (data[8]  << 8 | data[9]);
    MPU6050.Gyro_Y_RAW = (int16_t) (data[10] << 8 | data[11]);
    MPU6050.Gyro_Z_RAW = (int16_t) (data[12] << 8 | data[13]);

    /*< get corrected accel >*/
    MPU6050.Ax = MPU6050.Accel_X_RAW / Accel_X_Y_Z_corrector;
    MPU6050.Ay = MPU6050.Accel_Y_RAW / Accel_X_Y_Z_corrector;
    MPU6050.Az = MPU6050.Accel_Z_RAW / Accel_X_Y_Z_corrector;

    /*< get corrected temperature >*/
    MPU6050.Temperature = (float) (MPU6050.Temperature_RAW / (float) 340.0 + (float) 36.53);

    /*< get corrected gyro >*/
    MPU6050.Gx = MPU6050.Gyro_X_RAW / Gyro_X_Y_Z_corrector;
    MPU6050.Gy = MPU6050.Gyro_Y_RAW / Gyro_X_Y_Z_corrector;
    MPU6050.Gz = MPU6050.Gyro_Z_RAW / Gyro_X_Y_Z_corrector;

    return HAL_OK;
}

/** ************************************************************* *
 * @brief       read all and apply the kalman filter to the result
 * 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t MPU6050_Read_All_Kalman(void)
{
    if(MPU6050_Read_All()) return HAL_ERROR;

    // Kalman angle solve
    double dt = (double) (HAL_GetTick() - timer) / 1000;
    timer = HAL_GetTick();

    double roll;
    double roll_sqrt = sqrt(MPU6050.Accel_X_RAW * MPU6050.Accel_X_RAW + MPU6050.Accel_Z_RAW * MPU6050.Accel_Z_RAW);
    if (roll_sqrt != 0.0) {
        roll = atan(MPU6050.Accel_Y_RAW / roll_sqrt) * RAD_TO_DEG;
    } 
	else 
	{
        roll = 0.0;
    }

    double pitch = atan2(-MPU6050.Accel_X_RAW, MPU6050.Accel_Z_RAW) * RAD_TO_DEG;
    if((pitch < -90 && MPU6050.KalmanAngleY > 90) 
	|| (pitch > 90 && MPU6050.KalmanAngleY < -90)) 
	{
        KalmanY.angle = pitch;
        MPU6050.KalmanAngleY = pitch;
    } 
	else 
	{
        MPU6050.KalmanAngleY = MPU6050_Kalman_getAngle(&KalmanY, pitch, MPU6050.Gy, dt);
    }

    if (fabs(MPU6050.KalmanAngleY) > 90) MPU6050.Gx = -MPU6050.Gx;
    MPU6050.KalmanAngleX = MPU6050_Kalman_getAngle(&KalmanX, roll, MPU6050.Gy, dt);

    return HAL_OK;
}

/** ************************************************************* *
 * @brief       
 * 
 * @return      MPU6050_t 
 * ************************************************************* **/
MPU6050_t MPU6050_Get_Struct(void)
{
	return MPU6050;
}
/* ------------------------------------------------------------- --
   end of file
-- ------------------------------------------------------------- */