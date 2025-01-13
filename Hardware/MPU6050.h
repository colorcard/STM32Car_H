#ifndef __MPU6050_H
#define __MPU6050_H

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);

void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData(int16_t *GyroZ);

void dataGetERROR(void);

void getMPU6050Data(void);

void dataGetAndFilter(void);

typedef struct SensorMsg {
	float A[3];
	float G[3];
} SensorMsg;

typedef struct MPU6050Params {
	uint16_t MPU6050dt;
	uint64_t preMillis;
	float MPU6050ERROE[6];
	
	
} MPU6050Params;



#endif
