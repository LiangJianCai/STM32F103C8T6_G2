#include "SetTime.h"

int Set_Time_flag=1; //Ĭ��ֵ����Ϊ��Чֵ

void Show_SetTime_FirstUI(void)
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(0,16,OLED_8X16,"��:%4d",MyRTC_Time[0]);
  OLED_Printf(0,32,OLED_8X16,"��:%2d",MyRTC_Time[1]);
	OLED_Printf(0,48,OLED_8X16,"��:%2d",MyRTC_Time[2]);
}	

void Show_SetTime_SecondUI(void)
{
	OLED_Printf(0,0,OLED_8X16,"ʱ:%2d",MyRTC_Time[3]);
  OLED_Printf(0,16,OLED_8X16,"��:%2d",MyRTC_Time[4]);
	OLED_Printf(0,32,OLED_8X16,"��:%2d",MyRTC_Time[5]);
}

//ͨ���ı�MyRTC_Time[]�����е�ֵ���ı���ҳʱ����ʾ��ʱ��
void Change_RTC_Time(uint8_t i,uint8_t flag)//flag=1:��1��flag=0:��1
{
	if(flag==1){MyRTC_Time[i]++;}
	else{MyRTC_Time[i]--;}
	MyRTC_SetTime();//����ʱ�䣬���ô˺�����ȫ��������ʱ��ֵˢ�µ�RTCӲ����·
}

int SetYear(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==1)//��ֵ��1
		{
			Change_RTC_Time(0,1);
		}
		else if(KeyNum==2)//��ֵ��1
		{
			Change_RTC_Time(0,0);
		}
		
		else if(KeyNum==3)//ȷ�ϣ����沢�˳�
		{
			return 0;
		}
		
		Show_SetTime_FirstUI();
		OLED_ReverseArea(24,16,32,16);
		OLED_Update();
		
	}
}

int SetMonth(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==1)//��ֵ��1
		{
			Change_RTC_Time(1,1);
			if(MyRTC_Time[1]>=13){MyRTC_Time[1]=1;MyRTC_SetTime();}
		}
		else if(KeyNum==2)//��ֵ��1
		{
			Change_RTC_Time(1,0);
			if(MyRTC_Time[1]<=0){MyRTC_Time[1]=12;MyRTC_SetTime();}
		}
		
		else if(KeyNum==3)//ȷ�ϣ����沢�˳�
		{
			return 0;
		}
		
		Show_SetTime_FirstUI();
		OLED_ReverseArea(24,32,16,16);
		OLED_Update();
		
	}
}

int SetDay(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==1)//��ֵ��1
		{
			Change_RTC_Time(2,1);
			if(MyRTC_Time[2]>=32){MyRTC_Time[2]=1;MyRTC_SetTime();}
		}
		else if(KeyNum==2)//��ֵ��1
		{
			Change_RTC_Time(2,0);
			if(MyRTC_Time[2]<=0){MyRTC_Time[2]=31;MyRTC_SetTime();}
		}
		
		else if(KeyNum==3)//ȷ�ϣ����沢�˳�
		{
			return 0;
		}
		
		Show_SetTime_FirstUI();
		OLED_ReverseArea(24,48,16,16);
		OLED_Update();
		
	}
}

int SetHour(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==1)//��ֵ��1
		{
			Change_RTC_Time(3,1);
			if(MyRTC_Time[3]>=24){MyRTC_Time[3]=0;MyRTC_SetTime();}
		}
		else if(KeyNum==2)//��ֵ��1
		{
			Change_RTC_Time(3,0);
			if(MyRTC_Time[3]<0){MyRTC_Time[3]=23;MyRTC_SetTime();}
		}
		
		else if(KeyNum==3)//ȷ�ϣ����沢�˳�
		{
			return 0;
		}
		
		Show_SetTime_SecondUI();
		OLED_ReverseArea(24,0,16,16);
		OLED_Update();
		
	}
}

int SetMin(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==1)//��ֵ��1
		{
			Change_RTC_Time(4,1);
			if(MyRTC_Time[4]>=60){MyRTC_Time[4]=0;MyRTC_SetTime();}
		}
		else if(KeyNum==2)//��ֵ��1
		{
			Change_RTC_Time(4,0);
			if(MyRTC_Time[4]<0){MyRTC_Time[4]=59;MyRTC_SetTime();}
		}
		
		else if(KeyNum==3)//ȷ�ϣ����沢�˳�
		{
			return 0;
		}
		
		Show_SetTime_SecondUI();
		OLED_ReverseArea(24,16,16,16);
		OLED_Update();
		
	}
}

int SetSec(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==1)//��ֵ��1
		{
			Change_RTC_Time(5,1);
			if(MyRTC_Time[5]>=60){MyRTC_Time[5]=0;MyRTC_SetTime();}
		}
		else if(KeyNum==2)//��ֵ��1
		{
			Change_RTC_Time(5,0);
			if(MyRTC_Time[5]<0){MyRTC_Time[5]=59;MyRTC_SetTime();}
		}
		
		else if(KeyNum==3)//ȷ�ϣ����沢�˳�
		{
			return 0;
		}
		
		Show_SetTime_SecondUI();
		OLED_ReverseArea(24,32,16,16);
		OLED_Update();
		
	}
}

int SetTime(void)
{
   while(1)
	 {
		 KeyNum=Key_GetNum();
		 uint8_t set_time_flag_temp=0;
	   if(KeyNum==1)         //��һ��
		 {
			  Set_Time_flag--;
				if(Set_Time_flag<=0)Set_Time_flag=7;
		 }
		 else if(KeyNum==2)    //��һ��
		 {
		    Set_Time_flag++;
			 if(Set_Time_flag>=8)Set_Time_flag=1;
		 }
		 else if(KeyNum==3)    //ȷ��
		 {
		    OLED_Clear();
				OLED_Update();
				set_time_flag_temp=Set_Time_flag;
		 };
		if(set_time_flag_temp==1){return 0;}
		else if(set_time_flag_temp==2){SetYear();}//��
		else if(set_time_flag_temp==3){SetMonth();}//��
		else if(set_time_flag_temp==4){SetDay();}//��
		else if(set_time_flag_temp==5){SetHour();}//ʱ
		else if(set_time_flag_temp==6){SetMin();}//��
		else if(set_time_flag_temp==7){SetSec();}//��
		switch(Set_Time_flag)
		{
				 case 1:
	         OLED_Clear();
					 Show_SetTime_FirstUI();
					 OLED_ReverseArea(0,0,16,16);
					 OLED_Update();
					 break;		 
				 case 2:
					 	OLED_Clear();
						Show_SetTime_FirstUI();
						OLED_ReverseArea(0,16,16,16);
						OLED_Update();
						break;
				 case 3:
					 	OLED_Clear();
						Show_SetTime_FirstUI();
						OLED_ReverseArea(0,32,16,16);
						OLED_Update();
						break;
				 case 4:
					 	OLED_Clear();
						Show_SetTime_FirstUI();
						OLED_ReverseArea(0,48,16,16);
						OLED_Update();
						break;
				 case 5:
					 	OLED_Clear();
						Show_SetTime_SecondUI();
						OLED_ReverseArea(0,0,16,16);
						OLED_Update();
						break;
				 case 6:
					 	OLED_Clear();
						Show_SetTime_SecondUI();
						OLED_ReverseArea(0,16,16,16);
						OLED_Update();
						break;
				 case 7:
					 	OLED_Clear();
						Show_SetTime_SecondUI();
						OLED_ReverseArea(0,32,16,16);
						OLED_Update();
						break;			 
		}		 
	 }	 
}
