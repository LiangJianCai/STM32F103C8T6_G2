#include "sysconfig.h"
#include "modelconfig.h"
#include "boardconfig.h"
#include "model_test.h"


/**
  * �����ᶨ�壺
  * ���Ͻ�Ϊ(0, 0)��
  * ��������ΪX�ᣬȡֵ��Χ��0~127
  * ��������ΪY�ᣬȡֵ��Χ��0~63
  * 
  *       0             X��           127 
  *      .------------------------------->
  *    0 |
  *      |
  *      |
  *      |
  *  Y�� |
  *      |
  *      |
  *      |
  *   63 |
  *      v
  * 
  */

int main(void)
{
	/*OLED��ʼ��*/
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
		if(menu_flag==1){UI_Sliding_Menu();} //�˵�
		else if(menu_flag==2){UI_SettingPage();}  //����
	}

}


/* ÿ��1ms��һ���ж� */
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
