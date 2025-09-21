#include "menu.h"

void menu_init(void)
{
		MyRTC_Init();
		Key_Init();
		LED_Init();
		MPU6050_Init();
		AD_Init();
	  OLED_Init();   // 如果驱动里还没做
    OLED_Clear();  // 缓冲区清零
    OLED_Update(); // 把黑屏刷出去
}

/*----------------------------------------------首页时钟---------------------------------------------*/

uint8_t KeyNum;//记录选项键值
int clkflag=1;
uint16_t ADValue;
float VBAT;
int Battery_Capacity;
void Show_Battery(void)
{
  uint16_t sum;
	for(int i=0;i<3000;i++)
	{
		 ADValue=AD_GetValue();
		 sum += ADValue;
	}
	ADValue=sum/3000;
	VBAT=(float)ADValue/4095*3.3;
	Battery_Capacity=(ADValue-328)/819*100;
	if(Battery_Capacity<0)Battery_Capacity=0;
//	OLED_ShowNum(64,0,ADValue,4,OLED_6X8);
//	OLED_Printf(64,8,OLED_6X8,"VBAT:.2%f",VBAT);
	OLED_ShowNum(85,4,Battery_Capacity,3,OLED_6X8);
	OLED_ShowChar(103,4,'%',OLED_6X8);
	if(Battery_Capacity==100)OLED_ShowImage(110,0,16,16,Battery);
	else if(Battery_Capacity>=10&&Battery_Capacity<100)
	{
		OLED_ShowImage(110,0,16,16,Battery);
		OLED_ClearArea((112+Battery_Capacity/10),5,(10-Battery_Capacity/10),6);
		OLED_ClearArea(85,4,6,8);
	}
	
	else
	{
		OLED_ShowImage(110,0,16,16,Battery);
		OLED_ClearArea(112,5,10,6);
		OLED_ClearArea(85,4,12,8);
	}
}

/**
 * @brief 首页时钟显示
 */
void UI_Show_Clock(void)
{
		 MyRTC_ReadTime();
		 Show_Battery();
	   OLED_Printf(0,0,OLED_6X8,"%d-%d-%d",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2]);
		 OLED_Printf(16,16,OLED_12X24,"%02d:%02d:%02d",MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
     OLED_ShowString(0,48,"菜单",OLED_8X16);
		 OLED_ShowString(96,48,"设置",OLED_8X16);	
}

 /**
 * @brief 记录所选项
 */
int UI_First_Page(void)
{
	 while(1)
	 {
		 KeyNum=Key_GetNum();
	   if(KeyNum==1)         //上一项
		 {
			  clkflag--;
				if(clkflag<=0)clkflag=2;
		 }
		 else if(KeyNum==2)    //下一项
		 {
		    clkflag++;
			 if(clkflag>=3)clkflag=1;
		 }
		 else if(KeyNum==3)    //确定,进入所选项
		 {
		    OLED_Clear();
				OLED_Update();
				return clkflag;
		 }
		 else if(KeyNum==4)
		 {
		   	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
				GPIO_SetBits(GPIOB,GPIO_Pin_12);
		 };
		switch(clkflag)
		{
				 case 1:
					 UI_Show_Clock();
					 OLED_ReverseArea(0,48,32,16);
					 OLED_Update();
					 break;		 
				 case 2:
						UI_Show_Clock();
						OLED_ReverseArea(96,48,32,16);
						OLED_Update();
						break;	 
		}		 
	 }	 
}
/*----------------------------------------------设置界面---------------------------------------------*/

int setflag=1;
/**
 * @brief 设置界面
*/
void UI_Show_SettingPage(void)
{
	OLED_ShowImage(0,0,16,16,Return);
  OLED_ShowString(0,16,"日期时间设置",OLED_8X16);


}
int UI_SettingPage(void)
{
    while(1)
	 {
		 KeyNum=Key_GetNum();
		 uint8_t setflag_temp=0;
	   if(KeyNum==1)         //上一项
		 {
			  setflag--;
				if(setflag<=0)setflag=2;
		 }
		 else if(KeyNum==2)    //下一项
		 {
		    setflag++;
			 if(setflag>=3)setflag=1;
		 }
		 else if(KeyNum==3)    //确定
		 {
		    OLED_Clear();
				OLED_Update();
				setflag_temp=setflag;
		 };
		 if(setflag_temp==1) {return 0;}
		 else if(setflag_temp==2){SetTime();}
		 switch(setflag)
		 {
				 case 1:
					 UI_Show_SettingPage();
					 OLED_ReverseArea(0,0,16,16);
					 OLED_Update();
					 break;		 
				 case 2:
						UI_Show_SettingPage();
						OLED_ReverseArea(0,16,96,16);
						OLED_Update();
						break;	 
		 }		 
	 }
}
/*----------------------------------------------滑动菜单界面---------------------------------------------*/

uint8_t pre_selection;		//表示上次选择选项
uint8_t target_selection;	//表示目标选项
uint8_t x_pre=48;					//表示上次选项的x坐标值
uint8_t Speed=4;					//表示移动速度
uint8_t move_flag;				//开始移动标志位,1表示开始移动,0表示停止移动
#define MENU_ICON_COUNT 7

int sliding_menu_flag=1; //必须为有效值,否则会乱码

/**
 * @brief 滑动菜单动画
*/
void Sliding_Menu_Animation(void)
{
    OLED_Clear();
		OLED_ShowImage(42,10,44,44,Frame); //显示所选框
		if(pre_selection<target_selection) //当前值小于目标值则右移
		{
			x_pre-=Speed;
			if(x_pre==0)
			{
				pre_selection++;
				move_flag=0;
				x_pre=48;
			}
		}
		if(pre_selection>target_selection) //当前值大于目标值则左移
		{
			x_pre+=Speed;
			if(x_pre==96)
			{
				pre_selection--;
				move_flag=0;
				x_pre=48;
			}
		}
		
		if(pre_selection>=1)  //确保在索引0时,左侧没有图标显示
		{
			OLED_ShowImage(x_pre-48,16,32,32,Sliding_Menu_Graph[pre_selection-1]);//所选项左侧图标显示
		}
		
		if(pre_selection>=2)//确保移动时左侧图标跟着移动
		{
			OLED_ShowImage(x_pre-96,16,32,32,Sliding_Menu_Graph[pre_selection-2]);
		}
		
		OLED_ShowImage(x_pre,16,32,32,Sliding_Menu_Graph[pre_selection]);      //所选项图标显示
		OLED_ShowImage(x_pre+48,16,32,32,Sliding_Menu_Graph[pre_selection+1]); //所选项右侧图标显示
		if(pre_selection+2 < MENU_ICON_COUNT) 
		{
			OLED_ShowImage(x_pre+96,16,32,32,Sliding_Menu_Graph[pre_selection+2]);
		}//确保移动时右侧图标跟着移动
		
		OLED_Update();

}

//选择移动方向的函数
void Set_Selection(uint8_t move_flag,uint8_t Pre_Selection,uint8_t Target_Selection)
{
    if(move_flag==1)
    {
        pre_selection=Pre_Selection;
        target_selection=Target_Selection;
    }
    Sliding_Menu_Animation();
}

void Menu_To_Function(void)
{
	for(uint8_t i=0;i<=6;i++)
	{
		OLED_Clear();
	  if(pre_selection>=1)  //确保在索引0时,左侧没有图标显示
		{
			OLED_ShowImage(x_pre-48,16+8*i,32,32,Sliding_Menu_Graph[pre_selection-1]);//所选项左侧图标显示
		}
			
		OLED_ShowImage(x_pre,16+8*i,32,32,Sliding_Menu_Graph[pre_selection]);      //所选项图标显示
		OLED_ShowImage(x_pre+48,16+8*i,32,32,Sliding_Menu_Graph[pre_selection+1]); //所选项右侧图标显示
		
		OLED_Update();

	}
}

int UI_Sliding_Menu(void)
{   
		move_flag=1;         
		uint8_t dirctflag=2;//移动方向标志位,置1移动到上一项,置2移动到下一项
    while(1)
	 {
		 KeyNum=Key_GetNum();
		 uint8_t sliding_menu_flag_temp=0;
	   if(KeyNum==1)         //上一项
		 {
				dirctflag=1;
				move_flag=1;
			  sliding_menu_flag--;
				if(sliding_menu_flag<=0)sliding_menu_flag=7;
		 }
		 else if(KeyNum==2)    //下一项
		 {  
				dirctflag=2;
				move_flag=1;
		    sliding_menu_flag++;
			 if(sliding_menu_flag>=8)sliding_menu_flag=1;
		 }
		 else if(KeyNum==3)    //确定
		 {
		    OLED_Clear();
				OLED_Update();
				sliding_menu_flag_temp=sliding_menu_flag;
		 };
		 if(sliding_menu_flag_temp==1) {return 0;}
		 else if(sliding_menu_flag_temp==2){Menu_To_Function();Stop_Watch();}
		 else if(sliding_menu_flag_temp==3){Menu_To_Function();LED();}
		 else if(sliding_menu_flag_temp==4){Menu_To_Function();MPU6050();}
		 else if(sliding_menu_flag_temp==5){Menu_To_Function();Game();}
		 else if(sliding_menu_flag_temp==6){Menu_To_Function();Emoji();}
		 else if(sliding_menu_flag_temp==7){Menu_To_Function();Gradienter();}
//		switch(sliding_menu_flag)
//		{
//				 case 1:
//					  if(dirctflag==1)Set_Selection(move_flag,1,0);
//						else if(dirctflag==2)Set_Selection(move_flag,0,0);
//						break;	 
//				 case 2:
//							if(dirctflag==1)Set_Selection(move_flag,2,1);
//						else if(dirctflag==2)Set_Selection(move_flag,0,1);
//						break; 
//				 case 3:
//							if(dirctflag==1)Set_Selection(move_flag,3,2);
//						else if(dirctflag==2)Set_Selection(move_flag,1,2);
//						break;
//				 case 4:
//							if(dirctflag==1)Set_Selection(move_flag,4,3);
//						else if(dirctflag==2)Set_Selection(move_flag,2,3);
//						break;
//				 case 5:
//							if(dirctflag==1)Set_Selection(move_flag,5,4);
//						else if(dirctflag==2)Set_Selection(move_flag,3,4);
//						break;
//				 case 6:
//							if(dirctflag==1)Set_Selection(move_flag,6,5);
//						else if(dirctflag==2)Set_Selection(move_flag,4,5);
//						break;
//				 case 7:
//							if(dirctflag==1)Set_Selection(move_flag,7,6);
//						else if(dirctflag==2)Set_Selection(move_flag,5,6);
//						break;			 
//		}		 
			if(sliding_menu_flag==1)
			{
				if(dirctflag==1)Set_Selection(move_flag,1,0);
				else if(dirctflag==2)Set_Selection(move_flag,0,0);
			}
			else
			{
				if(dirctflag==1)Set_Selection(move_flag,sliding_menu_flag,sliding_menu_flag-1);
				else if(dirctflag==2)Set_Selection(move_flag,sliding_menu_flag-2,sliding_menu_flag-1);
			}
	 }

}

/*----------------------------------------------秒表---------------------------------------------*/

uint8_t stop_watch_flag=1;
uint8_t start_time_flag;//1:开始,0:停止

int hour,min,sec;
void UI_Show_Stop_Watch(void)
{
	OLED_Clear();
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(32,20,OLED_8X16,"%02d-%02d-%02d",hour,min,sec);
	OLED_ShowString(8,44,"开始",OLED_8X16);
	OLED_ShowString(48,44,"停止",OLED_8X16);
	OLED_ShowString(88,44,"清除",OLED_8X16);
}

void Stop_Watch_Tick(void)
{
	static uint16_t Count;
	Count++;
	if(Count>=1000)
	{
	   Count=0;
		if(start_time_flag==1)
		{
			sec++;
			if(sec>=60)
			{
				sec=0;
				min++;
				if(min>=60)
				{
					min=0;
					hour++;
					if(hour>99)
					{
						 hour=0;
					}
				}
			}
		
		}
	}
}

int Stop_Watch(void)
{
    while(1)
	 {
		 KeyNum=Key_GetNum();
		 uint8_t stop_flag_temp=0;
		 if(KeyNum==1)         //上一项
		 {
				stop_watch_flag--;
				if(stop_watch_flag<=0)stop_watch_flag=4;
		 }
		 else if(KeyNum==2)    //下一项
		 {
				stop_watch_flag++;
			 if(stop_watch_flag>=5)stop_watch_flag=1;
		 }
		 else if(KeyNum==3)    //确定
		 {
				OLED_Clear();
				OLED_Update();
				stop_flag_temp=stop_watch_flag;
		 };
		 switch (stop_flag_temp) 
		 {
					case 1:
							return 0;           
					case 2:
							start_time_flag = 1;
							break;
					case 3:
							start_time_flag = 0;
							break;
					case 4:
							start_time_flag = 0;
							hour = min = sec = 0;
							break;
					default:                 
							break;
		 }
		 switch(stop_watch_flag)
		 {
				 case 1:
					 UI_Show_Stop_Watch();
					 OLED_ReverseArea(0,0,16,16);
					 OLED_Update();
					 break;		 
				 case 2:
						UI_Show_Stop_Watch();
						OLED_ReverseArea(8,44,32,16);
						OLED_Update();
						break;	 
					case 3:
						UI_Show_Stop_Watch();
						OLED_ReverseArea(48,44,32,16);
						OLED_Update();
						break;	 
					case 4:
						UI_Show_Stop_Watch();
						OLED_ReverseArea(88,44,32,16);
						OLED_Update();
						break;
					default:                 
						break;					
		 }		 
	 }
}

/*----------------------------------------------手电筒---------------------------------------------*/

uint8_t led_flag=1;

void UI_Show_LED(void)
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(20,20,"ON",OLED_12X24);
	OLED_ShowString(72,20,"OFF",OLED_12X24);
}

int LED(void)
{
		while(1)
	 {
		 KeyNum=Key_GetNum();
		 uint8_t led_flag_temp=0;
		 if(KeyNum==1)         //上一项
		 {
				led_flag--;
				if(led_flag<=0)led_flag=3;
		 }
		 else if(KeyNum==2)    //下一项
		 {
				led_flag++;
			 if(led_flag>=4)led_flag=1;
		 }
		 else if(KeyNum==3)    //确定
		 {
				OLED_Clear();
				OLED_Update();
				led_flag_temp=led_flag;
		 };
		 switch (led_flag_temp) 
		 {
					case 1:
							return 0;           
					case 2:
							LED1_ON();	
							break;
					case 3:
							LED1_OFF();
							break;
					default:                 
							break;
		 }
		 switch(led_flag)
		 {
				 case 1:
					 UI_Show_LED();
					 OLED_ReverseArea(0,0,16,16);
					 OLED_Update();
					 break;		 
				 case 2:
						UI_Show_LED();
						OLED_ReverseArea(20,20,24,24);
						OLED_Update();
						break;	 
					case 3:
						UI_Show_LED();
						OLED_ReverseArea(72,20,36,24);
						OLED_Update();
						break;	
					default:                 
						break;					
		 }		 
	 }
}

/*----------------------------------------------MPU6050---------------------------------------------*/

int16_t ax,ay,az,gx,gy,gz;		//MPU6050测得的三轴加速度和角速度
float roll_g,pitch_g,yaw_g;		//陀螺仪解算的欧拉角
float roll_a,pitch_a;					//加速度计解算的欧拉角
float Roll,Pitch,Yaw;					//互补滤波后的欧拉角
float a=0.9;									//互补滤波器系数
float Delta_t=0.005;					//采样周期
double pi=3.1415927;

//通过MPU6050的数据进行姿态解算的函数
void MPU6050_Calculation(void)
{
		Delay_ms(5);
		MPU6050_GetData(&ax,&ay,&az,&gx,&gy,&gz);
		
		//通过陀螺仪解算欧拉角
		roll_g=Roll+(float)gx*Delta_t;
		pitch_g=Pitch+(float)gy*Delta_t;
		yaw_g=Yaw+(float)gz*Delta_t;
		
		//通过加速度计解算欧拉角
		pitch_a=atan2((-1)*ax,az)*180/pi;
		roll_a=atan2(ay,az)*180/pi;
		
		//通过互补滤波器进行数据融合
		Roll=a*roll_g+(1-a)*roll_a;
		Pitch=a*pitch_g+(1-a)*pitch_a;
		Yaw=a*yaw_g;
	
}

//显示MPU6050界面的UI
void UI_Show_MPU6050(void)
{
		OLED_ShowImage(0,0,16,16,Return);
		OLED_Printf(0,16,OLED_8X16,"Roll: %.2f",Roll);
		OLED_Printf(0,32,OLED_8X16,"Pitch:%.2f",Pitch);
		OLED_Printf(0,48,OLED_8X16,"Yaw:  %.2f",Yaw);
}

//控制光标在MPU6050界面移动的函数
int MPU6050(void)
{
		while(1)
		{
			KeyNum=Key_GetNum();
			if(KeyNum==3)
			{
				OLED_Clear();
				OLED_Update();
				return 0;
			}
			
			OLED_Clear();
			MPU6050_Calculation();
			UI_Show_MPU6050();
			OLED_ReverseArea(0,0,16,16);
			OLED_Update();
		}
}

/*----------------------------------------------游戏---------------------------------------------*/

void UI_Show_Game(void)
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(0,16,"恐龙小游戏",OLED_8X16);
}

uint8_t game_flag=1;
int Game(void)
{
  while(1)
	 {
		 KeyNum=Key_GetNum();
		 uint8_t game_flag_temp=0;
		 if(KeyNum==1)         //上一项
		 {
				game_flag--;
				if(game_flag<=0)game_flag=2;
		 }
		 else if(KeyNum==2)    //下一项
		 {
				game_flag++;
			 if(game_flag>=3)game_flag=1;
		 }
		 else if(KeyNum==3)    //确定
		 {
				OLED_Clear();
				OLED_Update();
				game_flag_temp=game_flag;
		 };
		 switch (game_flag_temp) 
		 {
					case 1:
							return 0;           
					case 2:
							Dino_Game_Pos_Init();
							DinoGame_Animation();	
							game_flag_temp = 0;
							break;
					default:                 
							break;
		 }
		 switch(game_flag)
		 {
				 case 1:
					 UI_Show_Game();
					 OLED_ReverseArea(0,0,16,16);
					 OLED_Update();
					 break;		 
				 case 2:
						UI_Show_Game();
						OLED_ReverseArea(0,16,80,16);
						OLED_Update();
						break;	 	
					default:                 
						break;					
		 }		 
	 }
}

/*----------------------------------------------动态表情包---------------------------------------------*/

void UI_Show_Emoji(void)
{  
	 for(uint8_t i=0;i<3;i++)
	 {
	   OLED_Clear();
		 OLED_ShowImage(30,10+i,16,16,Eyebrow[0]);//左眉毛
		 OLED_ShowImage(82,10+i,16,16,Eyebrow[1]);//右眉毛
		 OLED_ShowImage(100,10,16,16,Eyebrow[2]);//右眉毛
		 OLED_DrawEllipse(40,32,6,6,1);//左眼
		 OLED_DrawEllipse(88,32,6,6,1);//右眼
		 OLED_ShowImage(54,40,20,20,Mouth);
		 OLED_Update();
		 Delay_ms(100);
	 }
	 for(uint8_t i=0;i<3;i++)
	 {
	   OLED_Clear();
		 OLED_ShowImage(30,12-i,16,16,Eyebrow[0]);//左眉毛
		 OLED_ShowImage(82,12-i,16,16,Eyebrow[1]);//右眉毛
		 OLED_ShowImage(100,10,16,16,Eyebrow[2]);//右眉毛
		 OLED_DrawEllipse(40,32,6,6,1);//左眼
		 OLED_DrawEllipse(88,32,6,6,1);//右眼
		 OLED_ShowImage(54,40,20,20,Mouth);
		 OLED_Update();
		 Delay_ms(100);
	 }
   Delay_ms(500);
}

int Emoji(void)
{
    while(1)
	 {
		 KeyNum=Key_GetNum();
		 if(KeyNum==3)    //退出
		 {
				OLED_Clear();
				OLED_Update();
			  return 0;
		 };
		 UI_Show_Emoji();		 
	 }
}


/*----------------------------------水平仪-------------------------------------*/
//显示水平仪
void UI_Show_Gradienter(void)
{
	MPU6050_Calculation();
	OLED_DrawCircle(64,32,30,0);
	OLED_DrawCircle(64-Roll,32+Pitch,4,1);
}

//用按键控制退出水平仪界面的函数
int Gradienter(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==3)
		{
			OLED_Clear();
			OLED_Update();
			return 0;
		}
		OLED_Clear();
		UI_Show_Gradienter();
		OLED_Update();
	}
}






















