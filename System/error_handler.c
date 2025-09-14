#include "error_handler.h"

// 最大错误数量
#define MAX_ERRORS 16

// 错误处理函数表
static ErrorHandler_t g_errorHandlers[ERROR_UNKNOWN + 1];

// 错误队列
static ErrorInfo_t g_errorQueue[MAX_ERRORS];
static uint8_t g_errorCount = 0;
static uint8_t g_errorHead = 0;

/**
 * @brief 初始化错误处理器
 */
void ErrorHandler_Init(void)
{
    uint8_t i;
    
    // 初始化错误处理函数表
    for (i = 0; i <= ERROR_UNKNOWN; i++) {
        g_errorHandlers[i] = NULL;
    }
    
    // 初始化错误队列
    g_errorCount = 0;
    g_errorHead = 0;
    
    // 注册默认错误处理函数
    ErrorHandler_RegisterHandler(ERROR_CRITICAL, ErrorHandler_DefaultCriticalHandler);
}

/**
 * @brief 注册错误处理函数
 */
void ErrorHandler_RegisterHandler(ErrorCode_t code, ErrorHandler_t handler)
{
    if (code <= ERROR_UNKNOWN) {
        g_errorHandlers[code] = handler;
    }
}


/**
 * @brief 报告错误
 */
void ErrorHandler_ReportError(ErrorCode_t code, ErrorLevel_t level, 
                             uint8_t moduleId, const char* description)
{
    if (g_errorCount >= MAX_ERRORS) {
        return;  // 错误队列已满
    }
    
    ErrorInfo_t* error = &g_errorQueue[g_errorCount];
    
    error->code = code;
    error->level = level;
    error->timestamp = MyRTC_ReadTime();
    error->moduleId = moduleId;
    
    // 复制描述字符串
    strncpy(error->description, description, sizeof(error->description) - 1);
    error->description[sizeof(error->description) - 1] = '\0';
    
    g_errorCount++;
    
    // 如果是严重错误，立即处理
    if (level == ERROR_LEVEL_CRITICAL) {
        ErrorHandler_ProcessErrors();
    }
}

/**
 * @brief 处理错误
 */
void ErrorHandler_ProcessErrors(void)
{
    uint8_t i;
    
    for (i = 0; i < g_errorCount; i++) {
        ErrorInfo_t* error = &g_errorQueue[i];
        
        if (error->code <= ERROR_UNKNOWN && g_errorHandlers[error->code]) {
            g_errorHandlers[error->code](error);
        }
    }
    
    // 清除已处理的错误
    ErrorHandler_ClearAllErrors();
}

/**
 * @brief 检查是否有指定错误
 */
uint8_t ErrorHandler_HasError(ErrorCode_t code)
{
    uint8_t i;
    
    for (i = 0; i < g_errorCount; i++) {
        if (g_errorQueue[i].code == code) {
            return 1;
        }
    }
    
    return 0;
}

/**
 * @brief 清除指定错误
 */
void ErrorHandler_ClearError(ErrorCode_t code)
{
    uint8_t i, j;
    
    for (i = 0; i < g_errorCount; i++) {
        if (g_errorQueue[i].code == code) {
            // 移除错误，移动后续元素
            for (j = i; j < g_errorCount - 1; j++) {
                g_errorQueue[j] = g_errorQueue[j + 1];
            }
            g_errorCount--;
            i--;  // 重新检查当前位置
        }
    }
}

/**
 * @brief 清除所有错误
 */
void ErrorHandler_ClearAllErrors(void)
{
    g_errorCount = 0;
    g_errorHead = 0;
}

/**
 * @brief 默认严重错误处理函数
 */
void ErrorHandler_DefaultCriticalHandler(ErrorInfo_t* error)
{
    // 严重错误处理：停止系统或重启
    // 这里可以根据需要实现具体的处理逻辑
    
    // 示例：进入死循环，等待看门狗复位
    while (1) {
        // 可以在这里闪烁LED指示错误
    }
}

/**
 * @brief 默认错误处理函数
 */
void ErrorHandler_DefaultHandler(ErrorInfo_t* error)
{
    // 默认错误处理：记录错误信息
    // 这里可以根据需要实现具体的处理逻辑
    
    // 示例：在OLED上显示错误信息
    // OLED_ShowString(0, 0, "ERROR", OLED_8X16);
    // OLED_ShowNum(0, 16, error->code, 2, OLED_6X8);
}
