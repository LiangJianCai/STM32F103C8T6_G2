#include "stm32f10x.h"                  // Device header
#include "Delay.h"

volatile uint8_t Key_Num;

/**
  * 函    数：按键初始化
  * 参    数：无
  * 返 回 值：无
  */
void Key_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		  //开启GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将PA6和PA4引脚初始化为下拉输入
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//将PB1引脚初始化为下拉输入
}

/**
  * 函    数：按键获取键码
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~3，返回0代表没有按键按下
  */
uint8_t Key_GetNum(void)
{
	uint8_t Temp;
	if(Key_Num)
	{
		Temp=Key_Num;
		Key_Num=0;
		return Temp;
	}
		return 0;
}
int press_time;
void Key3_Tick(void)
{
	 if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 1)
	 {
			press_time++;
	 }
	 else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)
	 {
			press_time=0;
	 }
}

/*
* brief 获取按键引脚电平状态
*/

uint8_t Key_GetState(void)
{	
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 1)			//读PB1输入寄存器的状态，如果为1，则代表按键1按下
		{
			return 1;
		}	
		else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 1)			//读PA6输入寄存器的状态，如果为1，则代表按键2按下
		{
			return 2;
		}	
		else if ((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 1) && press_time>1000)		
		{
		  return 4;
		}		
		else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 1)			//读PA4输入寄存器的状态，如果为1，则代表按键3按下
		{
			return 3;
		}			
		else
		{
			return 0;
		}
}

/*
* brief 判断按键是否按下
*/
void Key_Tick(void)
{
	    static uint8_t Count=0;
			static uint8_t CurrentState, PreState;
			Count++;
			if(Count>=20)
			{
			    Count=0;
					PreState=CurrentState;
					CurrentState=Key_GetState();
			    if(PreState!=0 && CurrentState==0)
					{
							Key_Num=PreState;
					}
			}
}
