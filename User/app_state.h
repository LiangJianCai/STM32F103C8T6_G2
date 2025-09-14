#ifndef __APP_STATE_H__
#define __APP_STATE_H__

#include "stm32f10x.h"

// 应用状态枚举
typedef enum {
    APP_STATE_CLOCK,      // 时钟显示状态
    APP_STATE_MENU,       // 菜单状态
    APP_STATE_SETTING,    // 设置状态
    APP_STATE_STOPWATCH,  // 秒表状态
    APP_STATE_LED,        // LED控制状态
    APP_STATE_MPU6050,    // MPU6050状态
    APP_STATE_GAME,       // 游戏状态
    APP_STATE_EMOJI,      // 表情状态
    APP_STATE_GRADIENTER  // 水平仪状态
} AppState_t;

// 状态转换事件
typedef enum {
    EVENT_KEY_UP,         // 按键上
    EVENT_KEY_DOWN,       // 按键下
    EVENT_KEY_CONFIRM,    // 按键确认
    EVENT_KEY_LONG_PRESS, // 长按
    EVENT_TIMEOUT,        // 超时
    EVENT_RETURN          // 返回
} AppEvent_t;

// 状态处理函数类型
typedef AppState_t (*StateHandler_t)(AppEvent_t event);

// 状态机结构
typedef struct {
    AppState_t currentState;
    StateHandler_t handlers[9];  // 对应9个状态的处理函数
    uint32_t stateEnterTime;     // 状态进入时间
    uint32_t timeoutMs;         // 状态超时时间
} AppStateMachine_t;

// 函数声明
void AppStateMachine_Init(void);
void AppStateMachine_ProcessEvent(AppEvent_t event);
void AppStateMachine_Update(void);
AppState_t AppStateMachine_GetCurrentState(void);

#endif
