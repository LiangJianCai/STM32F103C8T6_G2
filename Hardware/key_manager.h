#ifndef __KEY_MANAGER_H__
#define __KEY_MANAGER_H__

#include "stm32f10x.h"

// 按键类型定义
typedef enum {
    KEY_NONE = 0,
    KEY_UP = 1,
    KEY_DOWN = 2,
    KEY_CONFIRM = 3,
    KEY_LONG_PRESS = 4
} KeyType_t;

// 按键状态
typedef enum {
    KEY_STATE_IDLE,      // 空闲状态
    KEY_STATE_PRESSED,   // 按下状态
    KEY_STATE_DEBOUNCE,  // 防抖状态
    KEY_STATE_LONG_PRESS // 长按状态
} KeyState_t;

// 按键配置
typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
    KeyType_t keyType;
    uint32_t debounceTime;    // 防抖时间(ms)
    uint32_t longPressTime;   // 长按时间(ms)
} KeyConfig_t;

// 按键实例
typedef struct {
    KeyConfig_t config;
    KeyState_t state;
    uint32_t pressStartTime;
    uint32_t lastEventTime;
    uint8_t isPressed;
    uint8_t longPressTriggered;
} KeyInstance_t;

// 按键管理器
typedef struct {
    KeyInstance_t keys[4];  // 支持4个按键
    uint8_t keyCount;
    uint32_t systemTick;
} KeyManager_t;

// 函数声明
void KeyManager_Init(void);
void KeyManager_AddKey(GPIO_TypeDef* port, uint16_t pin, KeyType_t keyType, 
                      uint32_t debounceTime, uint32_t longPressTime);
void KeyManager_Update(void);
KeyType_t KeyManager_GetKeyEvent(void);
uint8_t KeyManager_IsKeyPressed(KeyType_t keyType);
void KeyManager_ClearEvents(void);

#endif
