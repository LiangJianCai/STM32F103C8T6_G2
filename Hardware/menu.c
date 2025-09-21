#include "menu.h"

void menu_init(void)
{
		MyRTC_Init();
		Key_Init();
		LED_Init();
		MPU6050_Init();
		AD_Init();
	  OLED_Init();   // ��������ﻹû��
    OLED_Clear();  // ����������
    OLED_Update(); // �Ѻ���ˢ��ȥ
}

/*----------------------------------------------��ҳʱ��---------------------------------------------*/

uint8_t KeyNum;//��¼ѡ���ֵ
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
 * @brief ��ҳʱ����ʾ
 */
void UI_Show_Clock(void)
{
		 MyRTC_ReadTime();
		 Show_Battery();
	   OLED_Printf(0,0,OLED_6X8,"%d-%d-%d",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2]);
		 OLED_Printf(16,16,OLED_12X24,"%02d:%02d:%02d",MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
     OLED_ShowString(0,48,"�˵�",OLED_8X16);
		 OLED_ShowString(96,48,"����",OLED_8X16);	
}

 /**
 * @brief ��¼��ѡ��
 */
int UI_First_Page(void)
{
	 while(1)
	 {
		 KeyNum=Key_GetNum();
	   if(KeyNum==1)         //��һ��
		 {
			  clkflag--;
				if(clkflag<=0)clkflag=2;
		 }
		 else if(KeyNum==2)    //��һ��
		 {
		    clkflag++;
			 if(clkflag>=3)clkflag=1;
		 }
		 else if(KeyNum==3)    //ȷ��,������ѡ��
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
/*----------------------------------------------���ý���---------------------------------------------*/

int setflag=1;
/**
 * @brief ���ý���
*/
void UI_Show_SettingPage(void)
{
	OLED_ShowImage(0,0,16,16,Return);
  OLED_ShowString(0,16,"����ʱ������",OLED_8X16);


}
int UI_SettingPage(void)
{
    while(1)
	 {
		 KeyNum=Key_GetNum();
		 uint8_t setflag_temp=0;
	   if(KeyNum==1)         //��һ��
		 {
			  setflag--;
				if(setflag<=0)setflag=2;
		 }
		 else if(KeyNum==2)    //��һ��
		 {
		    setflag++;
			 if(setflag>=3)setflag=1;
		 }
		 else if(KeyNum==3)    //ȷ��
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
/*----------------------------------------------�����˵�����---------------------------------------------*/

uint8_t pre_selection;		//��ʾ�ϴ�ѡ��ѡ��
uint8_t target_selection;	//��ʾĿ��ѡ��
uint8_t x_pre=48;					//��ʾ�ϴ�ѡ���x����ֵ
uint8_t Speed=4;					//��ʾ�ƶ��ٶ�
uint8_t move_flag;				//��ʼ�ƶ���־λ,1��ʾ��ʼ�ƶ�,0��ʾֹͣ�ƶ�
#define MENU_ICON_COUNT 7

int sliding_menu_flag=1; //����Ϊ��Чֵ,���������

/**
 * @brief �����˵�����
*/
void Sliding_Menu_Animation(void)
{
    OLED_Clear();
		OLED_ShowImage(42,10,44,44,Frame); //��ʾ��ѡ��
		if(pre_selection<target_selection) //��ǰֵС��Ŀ��ֵ������
		{
			x_pre-=Speed;
			if(x_pre==0)
			{
				pre_selection++;
				move_flag=0;
				x_pre=48;
			}
		}
		if(pre_selection>target_selection) //��ǰֵ����Ŀ��ֵ������
		{
			x_pre+=Speed;
			if(x_pre==96)
			{
				pre_selection--;
				move_flag=0;
				x_pre=48;
			}
		}
		
		if(pre_selection>=1)  //ȷ��������0ʱ,���û��ͼ����ʾ
		{
			OLED_ShowImage(x_pre-48,16,32,32,Sliding_Menu_Graph[pre_selection-1]);//��ѡ�����ͼ����ʾ
		}
		
		if(pre_selection>=2)//ȷ���ƶ�ʱ���ͼ������ƶ�
		{
			OLED_ShowImage(x_pre-96,16,32,32,Sliding_Menu_Graph[pre_selection-2]);
		}
		
		OLED_ShowImage(x_pre,16,32,32,Sliding_Menu_Graph[pre_selection]);      //��ѡ��ͼ����ʾ
		OLED_ShowImage(x_pre+48,16,32,32,Sliding_Menu_Graph[pre_selection+1]); //��ѡ���Ҳ�ͼ����ʾ
		if(pre_selection+2 < MENU_ICON_COUNT) 
		{
			OLED_ShowImage(x_pre+96,16,32,32,Sliding_Menu_Graph[pre_selection+2]);
		}//ȷ���ƶ�ʱ�Ҳ�ͼ������ƶ�
		
		OLED_Update();

}

//ѡ���ƶ�����ĺ���
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
	  if(pre_selection>=1)  //ȷ��������0ʱ,���û��ͼ����ʾ
		{
			OLED_ShowImage(x_pre-48,16+8*i,32,32,Sliding_Menu_Graph[pre_selection-1]);//��ѡ�����ͼ����ʾ
		}
			
		OLED_ShowImage(x_pre,16+8*i,32,32,Sliding_Menu_Graph[pre_selection]);      //��ѡ��ͼ����ʾ
		OLED_ShowImage(x_pre+48,16+8*i,32,32,Sliding_Menu_Graph[pre_selection+1]); //��ѡ���Ҳ�ͼ����ʾ
		
		OLED_Update();

	}
}

int UI_Sliding_Menu(void)
{   
		move_flag=1;         
		uint8_t dirctflag=2;//�ƶ������־λ,��1�ƶ�����һ��,��2�ƶ�����һ��
    while(1)
	 {
		 KeyNum=Key_GetNum();
		 uint8_t sliding_menu_flag_temp=0;
	   if(KeyNum==1)         //��һ��
		 {
				dirctflag=1;
				move_flag=1;
			  sliding_menu_flag--;
				if(sliding_menu_flag<=0)sliding_menu_flag=7;
		 }
		 else if(KeyNum==2)    //��һ��
		 {  
				dirctflag=2;
				move_flag=1;
		    sliding_menu_flag++;
			 if(sliding_menu_flag>=8)sliding_menu_flag=1;
		 }
		 else if(KeyNum==3)    //ȷ��
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

/*----------------------------------------------���---------------------------------------------*/

uint8_t stop_watch_flag=1;
uint8_t start_time_flag;//1:��ʼ,0:ֹͣ

int hour,min,sec;
void UI_Show_Stop_Watch(void)
{
	OLED_Clear();
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(32,20,OLED_8X16,"%02d-%02d-%02d",hour,min,sec);
	OLED_ShowString(8,44,"��ʼ",OLED_8X16);
	OLED_ShowString(48,44,"ֹͣ",OLED_8X16);
	OLED_ShowString(88,44,"���",OLED_8X16);
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
		 if(KeyNum==1)         //��һ��
		 {
				stop_watch_flag--;
				if(stop_watch_flag<=0)stop_watch_flag=4;
		 }
		 else if(KeyNum==2)    //��һ��
		 {
				stop_watch_flag++;
			 if(stop_watch_flag>=5)stop_watch_flag=1;
		 }
		 else if(KeyNum==3)    //ȷ��
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

/*----------------------------------------------�ֵ�Ͳ---------------------------------------------*/

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
		 if(KeyNum==1)         //��һ��
		 {
				led_flag--;
				if(led_flag<=0)led_flag=3;
		 }
		 else if(KeyNum==2)    //��һ��
		 {
				led_flag++;
			 if(led_flag>=4)led_flag=1;
		 }
		 else if(KeyNum==3)    //ȷ��
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

int16_t ax,ay,az,gx,gy,gz;		//MPU6050��õ�������ٶȺͽ��ٶ�
float roll_g,pitch_g,yaw_g;		//�����ǽ����ŷ����
float roll_a,pitch_a;					//���ٶȼƽ����ŷ����
float Roll,Pitch,Yaw;					//�����˲����ŷ����
float a=0.9;									//�����˲���ϵ��
float Delta_t=0.005;					//��������
double pi=3.1415927;

//ͨ��MPU6050�����ݽ�����̬����ĺ���
void MPU6050_Calculation(void)
{
		Delay_ms(5);
		MPU6050_GetData(&ax,&ay,&az,&gx,&gy,&gz);
		
		//ͨ�������ǽ���ŷ����
		roll_g=Roll+(float)gx*Delta_t;
		pitch_g=Pitch+(float)gy*Delta_t;
		yaw_g=Yaw+(float)gz*Delta_t;
		
		//ͨ�����ٶȼƽ���ŷ����
		pitch_a=atan2((-1)*ax,az)*180/pi;
		roll_a=atan2(ay,az)*180/pi;
		
		//ͨ�������˲������������ں�
		Roll=a*roll_g+(1-a)*roll_a;
		Pitch=a*pitch_g+(1-a)*pitch_a;
		Yaw=a*yaw_g;
	
}

//��ʾMPU6050�����UI
void UI_Show_MPU6050(void)
{
		OLED_ShowImage(0,0,16,16,Return);
		OLED_Printf(0,16,OLED_8X16,"Roll: %.2f",Roll);
		OLED_Printf(0,32,OLED_8X16,"Pitch:%.2f",Pitch);
		OLED_Printf(0,48,OLED_8X16,"Yaw:  %.2f",Yaw);
}

//���ƹ����MPU6050�����ƶ��ĺ���
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

/*----------------------------------------------��Ϸ---------------------------------------------*/

void UI_Show_Game(void)
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(0,16,"����С��Ϸ",OLED_8X16);
}

uint8_t game_flag=1;
int Game(void)
{
  while(1)
	 {
		 KeyNum=Key_GetNum();
		 uint8_t game_flag_temp=0;
		 if(KeyNum==1)         //��һ��
		 {
				game_flag--;
				if(game_flag<=0)game_flag=2;
		 }
		 else if(KeyNum==2)    //��һ��
		 {
				game_flag++;
			 if(game_flag>=3)game_flag=1;
		 }
		 else if(KeyNum==3)    //ȷ��
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

/*----------------------------------------------��̬�����---------------------------------------------*/

void UI_Show_Emoji(void)
{  
	 for(uint8_t i=0;i<3;i++)
	 {
	   OLED_Clear();
		 OLED_ShowImage(30,10+i,16,16,Eyebrow[0]);//��üë
		 OLED_ShowImage(82,10+i,16,16,Eyebrow[1]);//��üë
		 OLED_ShowImage(100,10,16,16,Eyebrow[2]);//��üë
		 OLED_DrawEllipse(40,32,6,6,1);//����
		 OLED_DrawEllipse(88,32,6,6,1);//����
		 OLED_ShowImage(54,40,20,20,Mouth);
		 OLED_Update();
		 Delay_ms(100);
	 }
	 for(uint8_t i=0;i<3;i++)
	 {
	   OLED_Clear();
		 OLED_ShowImage(30,12-i,16,16,Eyebrow[0]);//��üë
		 OLED_ShowImage(82,12-i,16,16,Eyebrow[1]);//��üë
		 OLED_ShowImage(100,10,16,16,Eyebrow[2]);//��üë
		 OLED_DrawEllipse(40,32,6,6,1);//����
		 OLED_DrawEllipse(88,32,6,6,1);//����
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
		 if(KeyNum==3)    //�˳�
		 {
				OLED_Clear();
				OLED_Update();
			  return 0;
		 };
		 UI_Show_Emoji();		 
	 }
}


/*----------------------------------ˮƽ��-------------------------------------*/
//��ʾˮƽ��
void UI_Show_Gradienter(void)
{
	MPU6050_Calculation();
	OLED_DrawCircle(64,32,30,0);
	OLED_DrawCircle(64-Roll,32+Pitch,4,1);
}

//�ð��������˳�ˮƽ�ǽ���ĺ���
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






















