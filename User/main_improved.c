//#include "sysconfig.h"
//#include "app_state.h"
//#include "key_manager.h"
//#include "ui_manager.h"
//#include "error_handler.h"
//#include "mpu6050_optimized.h"
//#include "MyRTC.h"
//#include "Delay.h"

//AppEvent_t ConvertKeyToAppEvent(KeyType_t keyEvent);
///**
// * @brief 改进的主函数
// */
//int main(void)
//{
//    // 初始化系统
//    SystemInit();
//    
//    // 初始化错误处理器
//    ErrorHandler_Init();
//    
//    // 初始化硬件模块
//    OLED_Init();
//    OLED_Clear();
//    
//    // 初始化按键管理器
//    KeyManager_Init();
//    KeyManager_AddKey(GPIOB, GPIO_Pin_1, KEY_UP, 20, 1000);      // 按键1：上
//    KeyManager_AddKey(GPIOA, GPIO_Pin_6, KEY_DOWN, 20, 1000);    // 按键2：下
//    KeyManager_AddKey(GPIOA, GPIO_Pin_4, KEY_CONFIRM, 20, 1000); // 按键3：确认
//    
//    // 初始化UI管理器
//    UIManager_Init();
//    UIManager_SetUpdateInterval(50);  // 50ms更新间隔
//    
//    // 初始化MPU6050管理器
//    MPU6050Manager_Init();
//    MPU6050Manager_SetUpdateInterval(10);  // 10ms更新间隔
//    
//    // 初始化RTC
//    MyRTC_Init();
//    
//    // 初始化应用状态机
//    AppStateMachine_Init();
//    
//    // 初始化定时器
//    Timer_Init();
//    
//    // 主循环
//    while (1) {
//        // 更新按键管理器
//        KeyManager_Update();
//        
//        // 处理按键事件
//        KeyType_t keyEvent = KeyManager_GetKeyEvent();
//        if (keyEvent != KEY_NONE) {
//            AppEvent_t appEvent = ConvertKeyToAppEvent(keyEvent);
//            AppStateMachine_ProcessEvent(appEvent);
//        }
//        
//        // 更新MPU6050
//        MPU6050Manager_Update();
//        
//        // 更新UI
//        UIManager_Update();
//        
//        // 更新状态机
//        AppStateMachine_Update();
//        
//        // 处理错误
//        ErrorHandler_ProcessErrors();
//        
//        // 短暂延时，避免CPU占用过高
//        Delay_ms(1);
//    }
//}

///**
// * @brief 将按键事件转换为应用事件
// */
//AppEvent_t ConvertKeyToAppEvent(KeyType_t keyEvent)
//{
//    switch (keyEvent) {
//        case KEY_UP:
//            return EVENT_KEY_UP;
//        case KEY_DOWN:
//            return EVENT_KEY_DOWN;
//        case KEY_CONFIRM:
//            return EVENT_KEY_CONFIRM;
//        case KEY_LONG_PRESS:
//            return EVENT_KEY_LONG_PRESS;
//        default:
//            return EVENT_TIMEOUT;
//    }
//}

///**
// * @brief 系统初始化
// */
//static void SystemInit(void)
//{
//    // 配置系统时钟
//    SystemInit();
//    
//    // 配置NVIC优先级分组
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//    
//    // 初始化延时函数
//    Delay_Init();
//}

///**
// * @brief 定时器中断处理函数
// */
//void TIM2_IRQHandler(void)
//{
//    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
//        // 更新系统节拍
//        Delay_Tick();
//        
//        // 更新MPU6050节拍
//        MPU6050Manager_Update();
//        
//        // 清除中断标志
//        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//    }
//}
