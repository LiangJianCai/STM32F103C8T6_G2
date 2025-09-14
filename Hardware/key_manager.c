#include "key_manager.h"
#include "Delay.h"

// 全局按键管理器实例
static KeyManager_t g_keyManager;

// 按键事件队列
#define KEY_EVENT_QUEUE_SIZE 8
static KeyType_t g_keyEventQueue[KEY_EVENT_QUEUE_SIZE];
static uint8_t g_queueHead = 0;
static uint8_t g_queueTail = 0;
static uint8_t g_queueCount = 0;

/**
 * @brief 初始化按键管理器
 */
void KeyManager_Init(void)
{
    uint8_t i;
    
    // 初始化管理器
    g_keyManager.keyCount = 0;
    g_keyManager.systemTick = 0;
    
    // 初始化按键实例
    for (i = 0; i < 4; i++) {
        g_keyManager.keys[i].state = KEY_STATE_IDLE;
        g_keyManager.keys[i].pressStartTime = 0;
        g_keyManager.keys[i].lastEventTime = 0;
        g_keyManager.keys[i].isPressed = 0;
        g_keyManager.keys[i].longPressTriggered = 0;
    }
    
    // 初始化事件队列
    g_queueHead = 0;
    g_queueTail = 0;
    g_queueCount = 0;
}

/**
 * @brief 添加按键
 */
void KeyManager_AddKey(GPIO_TypeDef* port, uint16_t pin, KeyType_t keyType, 
                      uint32_t debounceTime, uint32_t longPressTime)
{
    if (g_keyManager.keyCount >= 4) {
        return;  // 按键数量已达上限
    }
    
    KeyInstance_t* key = &g_keyManager.keys[g_keyManager.keyCount];
    
    key->config.port = port;
    key->config.pin = pin;
    key->config.keyType = keyType;
    key->config.debounceTime = debounceTime;
    key->config.longPressTime = longPressTime;
    
    g_keyManager.keyCount++;
}

/**
 * @brief 读取按键状态
 */
static uint8_t KeyManager_ReadKeyState(KeyInstance_t* key)
{
    return GPIO_ReadInputDataBit(key->config.port, key->config.pin);
}

/**
 * @brief 添加按键事件到队列
 */
static void KeyManager_AddEvent(KeyType_t event)
{
    if (g_queueCount >= KEY_EVENT_QUEUE_SIZE) {
        return;  // 队列已满
    }
    
    g_keyEventQueue[g_queueTail] = event;
    g_queueTail = (g_queueTail + 1) % KEY_EVENT_QUEUE_SIZE;
    g_queueCount++;
}

/**
 * @brief 更新按键管理器
 */
void KeyManager_Update(void)
{
    uint8_t i;
    uint32_t currentTick = Delay_GetTick();
    uint8_t keyState;
    
    for (i = 0; i < g_keyManager.keyCount; i++) {
        KeyInstance_t* key = &g_keyManager.keys[i];
        keyState = KeyManager_ReadKeyState(key);
        
        switch (key->state) {
            case KEY_STATE_IDLE:
                if (keyState == 1) {  // 按键按下
                    key->state = KEY_STATE_DEBOUNCE;
                    key->pressStartTime = currentTick;
                    key->lastEventTime = currentTick;
                }
                break;
                
            case KEY_STATE_DEBOUNCE:
                if (keyState == 0) {  // 按键释放
                    key->state = KEY_STATE_IDLE;
                } else if ((currentTick - key->lastEventTime) >= key->config.debounceTime) {
                    // 防抖时间到，确认按下
                    key->state = KEY_STATE_PRESSED;
                    key->isPressed = 1;
                    key->longPressTriggered = 0;
                    KeyManager_AddEvent(key->config.keyType);
                }
                break;
                
            case KEY_STATE_PRESSED:
                if (keyState == 0) {  // 按键释放
                    key->state = KEY_STATE_IDLE;
                    key->isPressed = 0;
                } else if (!key->longPressTriggered && 
                          (currentTick - key->pressStartTime) >= key->config.longPressTime) {
                    // 长按触发
                    key->state = KEY_STATE_LONG_PRESS;
                    key->longPressTriggered = 1;
                    KeyManager_AddEvent(KEY_LONG_PRESS);
                }
                break;
                
            case KEY_STATE_LONG_PRESS:
                if (keyState == 0) {  // 按键释放
                    key->state = KEY_STATE_IDLE;
                    key->isPressed = 0;
                }
                break;
        }
    }
}

/**
 * @brief 获取按键事件
 */
KeyType_t KeyManager_GetKeyEvent(void)
{
    KeyType_t event = KEY_NONE;
    
    if (g_queueCount > 0) {
        event = g_keyEventQueue[g_queueHead];
        g_queueHead = (g_queueHead + 1) % KEY_EVENT_QUEUE_SIZE;
        g_queueCount--;
    }
    
    return event;
}

/**
 * @brief 检查按键是否按下
 */
uint8_t KeyManager_IsKeyPressed(KeyType_t keyType)
{
    uint8_t i;
    
    for (i = 0; i < g_keyManager.keyCount; i++) {
        if (g_keyManager.keys[i].config.keyType == keyType) {
            return g_keyManager.keys[i].isPressed;
        }
    }
    
    return 0;
}

/**
 * @brief 清除所有按键事件
 */
void KeyManager_ClearEvents(void)
{
    g_queueHead = 0;
    g_queueTail = 0;
    g_queueCount = 0;
}
