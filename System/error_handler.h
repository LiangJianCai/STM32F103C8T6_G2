#ifndef __ERROR_HANDLER_H__
#define __ERROR_HANDLER_H__

#include "stm32f10x.h"
#include "sysconfig.h"

// 错误代码定义
typedef enum {
    ERROR_NONE = 0,
    ERROR_INVALID_PARAM,
    ERROR_OUT_OF_RANGE,
    ERROR_TIMEOUT,
    ERROR_HARDWARE_FAIL,
    ERROR_MEMORY_ALLOC,
    ERROR_COMMUNICATION,
    ERROR_UNKNOWN,
	  ERROR_CRITICAL
} ErrorCode_t;

// 错误严重级别
typedef enum {
    ERROR_LEVEL_INFO,
    ERROR_LEVEL_WARNING,
    ERROR_LEVEL_ERROR,
    ERROR_LEVEL_CRITICAL
} ErrorLevel_t;

// 错误信息结构
typedef struct {
    ErrorCode_t code;
    ErrorLevel_t level;
    uint32_t timestamp;
    char description[64];
    uint8_t moduleId;
} ErrorInfo_t;

// 错误处理函数类型
typedef void (*ErrorHandler_t)(ErrorInfo_t* error);

// 函数声明
void ErrorHandler_Init(void);
void ErrorHandler_RegisterHandler(ErrorCode_t code, ErrorHandler_t handler);
void ErrorHandler_ReportError(ErrorCode_t code, ErrorLevel_t level, 
                             uint8_t moduleId, const char* description);
void ErrorHandler_ProcessErrors(void);
uint8_t ErrorHandler_HasError(ErrorCode_t code);
void ErrorHandler_ClearError(ErrorCode_t code);
void ErrorHandler_ClearAllErrors(void);
void ErrorHandler_DefaultCriticalHandler(ErrorInfo_t* error);

// 便捷宏定义
#define REPORT_ERROR(code, level, module, desc) \
    ErrorHandler_ReportError(code, level, module, desc)

#define CHECK_PARAM(param, min, max) \
    do { \
        if ((param) < (min) || (param) > (max)) { \
            REPORT_ERROR(ERROR_OUT_OF_RANGE, ERROR_LEVEL_ERROR, 0, "Parameter out of range"); \
            return ERROR_OUT_OF_RANGE; \
        } \
    } while(0)

#define CHECK_NULL(ptr) \
    do { \
        if ((ptr) == NULL) { \
            REPORT_ERROR(ERROR_INVALID_PARAM, ERROR_LEVEL_ERROR, 0, "Null pointer"); \
            return ERROR_INVALID_PARAM; \
        } \
    } while(0)

// 模块ID定义
#define MODULE_ID_MAIN        1
#define MODULE_ID_KEY         2
#define MODULE_ID_OLED        3
#define MODULE_ID_RTC         4
#define MODULE_ID_MPU6050     5
#define MODULE_ID_TIMER       6
#define MODULE_ID_UI          7

#endif
