#include "mpu6050_optimized.h"
#include "MPU6050.h"
#include "Delay.h"
#include "error_handler.h"

// 全局MPU6050管理器实例
static MPU6050Manager_t g_mpu6050Manager;

// 快速atan2查找表（用于优化浮点运算）
static const float atan2_lut[64] = {
    0.0000f, 0.0491f, 0.0982f, 0.1473f, 0.1963f, 0.2454f, 0.2945f, 0.3436f,
    0.3927f, 0.4418f, 0.4909f, 0.5400f, 0.5891f, 0.6382f, 0.6873f, 0.7364f,
    0.7854f, 0.8345f, 0.8836f, 0.9327f, 0.9818f, 1.0309f, 1.0800f, 1.1291f,
    1.1782f, 1.2273f, 1.2764f, 1.3255f, 1.3746f, 1.4237f, 1.4728f, 1.5219f,
    1.5710f, 1.6201f, 1.6692f, 1.7183f, 1.7674f, 1.8165f, 1.8656f, 1.9147f,
    1.9638f, 2.0129f, 2.0620f, 2.1111f, 2.1602f, 2.2093f, 2.2584f, 2.3075f,
    2.3566f, 2.4057f, 2.4548f, 2.5039f, 2.5530f, 2.6021f, 2.6512f, 2.7003f,
    2.7494f, 2.7985f, 2.8476f, 2.8967f, 2.9458f, 2.9949f, 3.0440f, 3.0931f
};

/**
 * @brief 初始化MPU6050管理器
 */
void MPU6050Manager_Init(void)
{
    // 初始化MPU6050硬件
    MPU6050_Init();
    
    // 初始化管理器
    g_mpu6050Manager.currentData.accX = 0;
    g_mpu6050Manager.currentData.accY = 0;
    g_mpu6050Manager.currentData.accZ = 0;
    g_mpu6050Manager.currentData.gyroX = 0;
    g_mpu6050Manager.currentData.gyroY = 0;
    g_mpu6050Manager.currentData.gyroZ = 0;
    g_mpu6050Manager.currentData.roll = 0.0f;
    g_mpu6050Manager.currentData.pitch = 0.0f;
    g_mpu6050Manager.currentData.yaw = 0.0f;
    g_mpu6050Manager.currentData.timestamp = 0;
    g_mpu6050Manager.currentData.dataValid = 0;
    
    // 初始化滤波器配置
    g_mpu6050Manager.filterConfig.alpha = 0.9f;
    g_mpu6050Manager.filterConfig.deltaT = 0.005f;
    g_mpu6050Manager.filterConfig.enableFilter = 1;
    
    // 初始化其他参数
    g_mpu6050Manager.lastUpdateTime = 0;
    g_mpu6050Manager.updateInterval = 5;  // 5ms更新间隔
    g_mpu6050Manager.isInitialized = 1;
    g_mpu6050Manager.dataReady = 0;
}

/**
 * @brief 更新MPU6050数据
 */
void MPU6050Manager_Update(void)
{
    uint32_t currentTime = Delay_GetTick();
    
    // 检查是否到了更新时间
    if (!g_mpu6050Manager.isInitialized || 
        (currentTime - g_mpu6050Manager.lastUpdateTime) < g_mpu6050Manager.updateInterval) {
        return;
    }
    
    // 读取原始数据
    MPU6050Manager_ReadRawData();
    
    // 应用滤波器
    if (g_mpu6050Manager.filterConfig.enableFilter) {
        MPU6050Manager_ApplyFilter();
    }
    
    // 更新状态
    g_mpu6050Manager.currentData.timestamp = currentTime;
    g_mpu6050Manager.currentData.dataValid = 1;
    g_mpu6050Manager.dataReady = 1;
    g_mpu6050Manager.lastUpdateTime = currentTime;
}

/**
 * @brief 读取原始数据
 */
static void MPU6050Manager_ReadRawData(void)
{
    // 保存上一次的数据
    g_mpu6050Manager.previousData = g_mpu6050Manager.currentData;
    
    // 读取新的原始数据
    MPU6050_GetData(&g_mpu6050Manager.currentData.accX,
                    &g_mpu6050Manager.currentData.accY,
                    &g_mpu6050Manager.currentData.accZ,
                    &g_mpu6050Manager.currentData.gyroX,
                    &g_mpu6050Manager.currentData.gyroY,
                    &g_mpu6050Manager.currentData.gyroZ);
}

/**
 * @brief 应用滤波器
 */
static void MPU6050Manager_ApplyFilter(void)
{
    float alpha = g_mpu6050Manager.filterConfig.alpha;
    float deltaT = g_mpu6050Manager.filterConfig.deltaT;
    
    // 通过陀螺仪计算欧拉角
    float roll_g = g_mpu6050Manager.previousData.roll + 
                   (float)g_mpu6050Manager.currentData.gyroX * deltaT;
    float pitch_g = g_mpu6050Manager.previousData.pitch + 
                    (float)g_mpu6050Manager.currentData.gyroY * deltaT;
    float yaw_g = g_mpu6050Manager.previousData.yaw + 
                  (float)g_mpu6050Manager.currentData.gyroZ * deltaT;
    
    // 通过加速度计计算欧拉角（使用快速atan2）
    float pitch_a = MPU6050Manager_FastAtan2(-g_mpu6050Manager.currentData.accX, 
                                             g_mpu6050Manager.currentData.accZ) * 180.0f / 3.14159f;
    float roll_a = MPU6050Manager_FastAtan2(g_mpu6050Manager.currentData.accY, 
                                           g_mpu6050Manager.currentData.accZ) * 180.0f / 3.14159f;
    
    // 互补滤波融合
    g_mpu6050Manager.currentData.roll = alpha * roll_g + (1.0f - alpha) * roll_a;
    g_mpu6050Manager.currentData.pitch = alpha * pitch_g + (1.0f - alpha) * pitch_a;
    g_mpu6050Manager.currentData.yaw = alpha * yaw_g;
}

/**
 * @brief 快速atan2实现（使用查找表）
 */
static float MPU6050Manager_FastAtan2(float y, float x)
{
    if (x == 0.0f && y == 0.0f) {
        return 0.0f;
    }
    
    // 计算角度
    float angle = 0.0f;
    if (x > 0.0f) {
        angle = y / x;
    } else if (x < 0.0f && y >= 0.0f) {
        angle = y / x + 3.14159f;
    } else if (x < 0.0f && y < 0.0f) {
        angle = y / x - 3.14159f;
    } else if (x == 0.0f && y > 0.0f) {
        return 1.5708f;
    } else if (x == 0.0f && y < 0.0f) {
        return -1.5708f;
    }
    
    // 限制角度范围
    if (angle > 1.0f) angle = 1.0f;
    if (angle < -1.0f) angle = -1.0f;
    
    // 使用查找表
    int index = (int)((angle + 1.0f) * 32.0f);
    if (index < 0) index = 0;
    if (index >= 64) index = 63;
    
    return atan2_lut[index];
}

/**
 * @brief 获取MPU6050数据
 */
void MPU6050Manager_GetData(MPU6050Data_t* data)
{
    if (data == NULL) {
        REPORT_ERROR(ERROR_INVALID_PARAM, ERROR_LEVEL_ERROR, MODULE_ID_MPU6050, "Null pointer");
        return;
    }
    
    *data = g_mpu6050Manager.currentData;
}

/**
 * @brief 检查数据是否就绪
 */
uint8_t MPU6050Manager_IsDataReady(void)
{
    return g_mpu6050Manager.dataReady;
}

/**
 * @brief 设置更新间隔
 */
void MPU6050Manager_SetUpdateInterval(uint32_t interval)
{
    if (interval < 1 || interval > 1000) {
        REPORT_ERROR(ERROR_OUT_OF_RANGE, ERROR_LEVEL_WARNING, MODULE_ID_MPU6050, "Invalid update interval");
        return;
    }
    
    g_mpu6050Manager.updateInterval = interval;
}

/**
 * @brief 设置滤波器配置
 */
void MPU6050Manager_SetFilterConfig(float alpha, float deltaT)
{
    if (alpha < 0.0f || alpha > 1.0f) {
        REPORT_ERROR(ERROR_OUT_OF_RANGE, ERROR_LEVEL_WARNING, MODULE_ID_MPU6050, "Invalid alpha value");
        return;
    }
    
    if (deltaT <= 0.0f || deltaT > 0.1f) {
        REPORT_ERROR(ERROR_OUT_OF_RANGE, ERROR_LEVEL_WARNING, MODULE_ID_MPU6050, "Invalid deltaT value");
        return;
    }
    
    g_mpu6050Manager.filterConfig.alpha = alpha;
    g_mpu6050Manager.filterConfig.deltaT = deltaT;
}
