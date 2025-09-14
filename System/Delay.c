#include "stm32f10x.h"

volatile uint32_t g_tick = 0;


/**
 * @brief 初始化 SysTick 定时器，1ms 触发一次中断
 */
void Delay_Init(void) {
    // SystemCoreClock 是系统时钟（默认72MHz）
    if (SysTick_Config(SystemCoreClock / 1000)) {
        while (1); // 配置失败则卡死
    }
}

/**
 * @brief 手动增加 tick（如果不用 SysTick_Handler，也可以在循环里调用）
 */
void Delay_Tick(void) {
    g_tick++;
}

/**
 * @brief 获取当前毫秒计数
 */
uint32_t Delay_GetTick(void) {
    return g_tick;
}


/**
  * @brief  微秒级延时
  * @param  xus 延时时长，范围：0~233015
  * @retval 无
  */
void Delay_us(uint32_t xus)
{
	SysTick->LOAD = 72 * xus;				//设置定时器重装值
	SysTick->VAL = 0x00;					//清空当前计数值
	SysTick->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器
	while(!(SysTick->CTRL & 0x00010000));	//等待计数到0
	SysTick->CTRL = 0x00000004;				//关闭定时器
}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
	while(xms--)
	{
		Delay_us(1000);
	}
}
 
/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);
	}
} 
