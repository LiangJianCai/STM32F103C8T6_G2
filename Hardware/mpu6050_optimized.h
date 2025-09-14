#ifndef __MPU6050_OPTIMIZED_H__
#define __MPU6050_OPTIMIZED_H__

#include "stm32f10x.h"

// MPU6050数据结构
typedef struct {
    int16_t accX, accY, accZ;
    int16_t gyroX, gyroY, gyroZ;
    float roll, pitch, yaw;
    uint32_t timestamp;
    uint8_t dataValid;
} MPU6050Data_t;

// 滤波器配置
typedef struct {
    float alpha;        // 互补滤波系数
    float deltaT;       // 采样时间间隔
    uint8_t enableFilter; // 是否启用滤波
} FilterConfig_t;

// MPU6050管理器
typedef struct {
    MPU6050Data_t currentData;
    MPU6050Data_t previousData;
    FilterConfig_t filterConfig;
    uint32_t lastUpdateTime;
    uint32_t updateInterval;  // 更新间隔(ms)
    uint8_t isInitialized;
    uint8_t dataReady;
} MPU6050Manager_t;

// 函数声明
void MPU6050Manager_Init(void);
void MPU6050Manager_Update(void);
void MPU6050Manager_GetData(MPU6050Data_t* data);
uint8_t MPU6050Manager_IsDataReady(void);
void MPU6050Manager_SetUpdateInterval(uint32_t interval);
void MPU6050Manager_SetFilterConfig(float alpha, float deltaT);

// 内部函数
static void MPU6050Manager_ReadRawData(void);
static void MPU6050Manager_ApplyFilter(void);
static float MPU6050Manager_FastAtan2(float y, float x);

#endif
