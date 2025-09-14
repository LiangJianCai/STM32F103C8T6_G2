#include "app_state.h"
#include "menu.h"
#include "SetTime.h"
#include "Delay.h"

// 全局状态机实例
static AppStateMachine_t g_stateMachine;

// 状态处理函数声明
static AppState_t ClockStateHandler(AppEvent_t event);
static AppState_t MenuStateHandler(AppEvent_t event);
static AppState_t SettingStateHandler(AppEvent_t event);
static AppState_t StopWatchStateHandler(AppEvent_t event);
static AppState_t LEDStateHandler(AppEvent_t event);
static AppState_t MPU6050StateHandler(AppEvent_t event);
static AppState_t GameStateHandler(AppEvent_t event);
static AppState_t EmojiStateHandler(AppEvent_t event);
static AppState_t GradienterStateHandler(AppEvent_t event);

/**
 * @brief 初始化应用状态机
 */
void AppStateMachine_Init(void)
{
    // 初始化状态机
    g_stateMachine.currentState = APP_STATE_CLOCK;
    g_stateMachine.stateEnterTime = 0;
    g_stateMachine.timeoutMs = 0;
    
    // 注册状态处理函数
    g_stateMachine.handlers[APP_STATE_CLOCK] = ClockStateHandler;
    g_stateMachine.handlers[APP_STATE_MENU] = MenuStateHandler;
    g_stateMachine.handlers[APP_STATE_SETTING] = SettingStateHandler;
    g_stateMachine.handlers[APP_STATE_STOPWATCH] = StopWatchStateHandler;
    g_stateMachine.handlers[APP_STATE_LED] = LEDStateHandler;
    g_stateMachine.handlers[APP_STATE_MPU6050] = MPU6050StateHandler;
    g_stateMachine.handlers[APP_STATE_GAME] = GameStateHandler;
    g_stateMachine.handlers[APP_STATE_EMOJI] = EmojiStateHandler;
    g_stateMachine.handlers[APP_STATE_GRADIENTER] = GradienterStateHandler;
}

/**
 * @brief 处理状态机事件
 */
void AppStateMachine_ProcessEvent(AppEvent_t event)
{
    if (g_stateMachine.currentState < 9 && g_stateMachine.handlers[g_stateMachine.currentState]) {
        AppState_t newState = g_stateMachine.handlers[g_stateMachine.currentState](event);
        
        if (newState != g_stateMachine.currentState) {
            // 状态转换
            g_stateMachine.currentState = newState;
            g_stateMachine.stateEnterTime = 0;  // 重置进入时间
        }
    }
}

/**
 * @brief 更新状态机
 */
void AppStateMachine_Update(void)
{
    // 检查超时
    if (g_stateMachine.timeoutMs > 0 && 
        g_stateMachine.stateEnterTime > 0 &&
        (Delay_GetTick() - g_stateMachine.stateEnterTime) > g_stateMachine.timeoutMs) {
        AppStateMachine_ProcessEvent(EVENT_TIMEOUT);
    }
}

/**
 * @brief 获取当前状态
 */
AppState_t AppStateMachine_GetCurrentState(void)
{
    return g_stateMachine.currentState;
}

/**
 * @brief 时钟状态处理函数
 */
static AppState_t ClockStateHandler(AppEvent_t event)
{
    static uint8_t menuFlag = 0;
    
    switch (event) {
        case EVENT_KEY_UP:
            menuFlag = (menuFlag == 0) ? 1 : 0;
            break;
        case EVENT_KEY_DOWN:
            menuFlag = (menuFlag == 0) ? 1 : 0;
            break;
        case EVENT_KEY_CONFIRM:
            if (menuFlag == 0) {
                return APP_STATE_MENU;
            } else {
                return APP_STATE_SETTING;
            }
        default:
            break;
    }
    
    // 显示时钟界面
    UI_Show_Clock();
    OLED_Update();
    
    return APP_STATE_CLOCK;
}

/**
 * @brief 菜单状态处理函数
 */
static AppState_t MenuStateHandler(AppEvent_t event)
{
    switch (event) {
        case EVENT_KEY_CONFIRM:
            return APP_STATE_CLOCK;
        case EVENT_RETURN:
            return APP_STATE_CLOCK;
        default:
            break;
    }
    
    // 处理菜单逻辑
    UI_Sliding_Menu();
    return APP_STATE_MENU;
}

/**
 * @brief 设置状态处理函数
 */
static AppState_t SettingStateHandler(AppEvent_t event)
{
    switch (event) {
        case EVENT_KEY_CONFIRM:
            return APP_STATE_CLOCK;
        case EVENT_RETURN:
            return APP_STATE_CLOCK;
        default:
            break;
    }
    
    // 处理设置逻辑
    UI_SettingPage();
    return APP_STATE_SETTING;
}

/**
 * @brief 秒表状态处理函数
 */
static AppState_t StopWatchStateHandler(AppEvent_t event)
{
    switch (event) {
        case EVENT_RETURN:
            return APP_STATE_MENU;
        default:
            break;
    }
    
    Stop_Watch();
    return APP_STATE_STOPWATCH;
}

/**
 * @brief LED状态处理函数
 */
static AppState_t LEDStateHandler(AppEvent_t event)
{
    switch (event) {
        case EVENT_RETURN:
            return APP_STATE_MENU;
        default:
            break;
    }
    
    LED();
    return APP_STATE_LED;
}

/**
 * @brief MPU6050状态处理函数
 */
static AppState_t MPU6050StateHandler(AppEvent_t event)
{
    switch (event) {
        case EVENT_RETURN:
            return APP_STATE_MENU;
        default:
            break;
    }
    
    MPU6050();
    return APP_STATE_MPU6050;
}

/**
 * @brief 游戏状态处理函数
 */
static AppState_t GameStateHandler(AppEvent_t event)
{
    switch (event) {
        case EVENT_RETURN:
            return APP_STATE_MENU;
        default:
            break;
    }
    
    Game();
    return APP_STATE_GAME;
}

/**
 * @brief 表情状态处理函数
 */
static AppState_t EmojiStateHandler(AppEvent_t event)
{
    switch (event) {
        case EVENT_RETURN:
            return APP_STATE_MENU;
        default:
            break;
    }
    
    Emoji();
    return APP_STATE_EMOJI;
}

/**
 * @brief 水平仪状态处理函数
 */
static AppState_t GradienterStateHandler(AppEvent_t event)
{
    switch (event) {
        case EVENT_RETURN:
            return APP_STATE_MENU;
        default:
            break;
    }
    
    Gradienter();
    return APP_STATE_GRADIENTER;
}
