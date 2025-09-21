#include "sysconfig.h"
#include "modelconfig.h"
#include "boardconfig.h"
#include "model_test.h"


/**
  * 坐标轴定义：
  * 左上角为(0, 0)点
  * 横向向右为X轴，取值范围：0~127
  * 纵向向下为Y轴，取值范围：0~63
  * 
  *       0             X轴           127 
  *      .------------------------------->
  *    0 |
  *      |
  *      |
  *      |
  *  Y轴 |
  *      |
  *      |
  *      |
  *   63 |
  *      v
  * 
  */

int main(void)
{
	/*OLED初始化*/
	OLED_Init();
	OLED_Clear();
	menu_init();
	Timer_Init();
	int menu_flag=0;
	
	extern int press_time;;
	extern uint8_t Key_Num;
	while(1)
	{
	  UI_Show_Clock();
		OLED_Update();
		menu_flag=UI_First_Page();
		if(menu_flag==1){UI_Sliding_Menu();} //菜单
		else if(menu_flag==2){UI_SettingPage();}  //设置
	}

}


/* 每隔1ms进一次中断 */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_Tick();
		Key3_Tick();
		Stop_Watch_Tick();
		Dino_Tick();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
