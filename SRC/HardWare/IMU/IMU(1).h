

#ifndef __IMU_H
#define __IMU_H

#include "Public_StdTypes.h"
#include "ICM20948.h"

#define M_PI  (float)3.1415926535

typedef struct imu_st_sensor_data_tag
{
	int16_t s16X;
	int16_t s16Y;
	int16_t s16Z;
}IMU_ST_SENSOR_DATA;

extern int16_t accel[3], gyro[3], magn[3];
extern float angles[3];
extern IMU_ST_SENSOR_DATA gstMagOffset;

extern void IMU_Init(void); 
extern void IMU_GetYawPitchRoll(float *Angles) ;



#endif



