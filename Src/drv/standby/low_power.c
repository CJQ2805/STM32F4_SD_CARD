#include "low_power.h"
#include <SysCommDef.h>
#include "rtc.h"
#include <osmonit.h>

// 待测试     by CJQ2805

void sys_standby(void)
{
	__HAL_RCC_AHB1_FORCE_RESET(); //复位所有 IO 口
	 while(WKUP_KD); //等待 WK_UP 按键松开(在有 RTC 中断时,
	//必须等 WK_UP 松开再进入待机)
	 __HAL_RCC_PWR_CLK_ENABLE(); //使能 PWR 时钟
	 __HAL_RCC_BACKUPRESET_FORCE(); //复位备份区域
	 HAL_PWR_EnableBkUpAccess(); //后备区域访问使能 
	 __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
	 __HAL_RTC_WRITEPROTECTION_DISABLE(&RTC_Handler);//关闭 RTC 写保护
	 
	 //关闭 RTC 相关中断，可能在 RTC 实验打开了
	 __HAL_RTC_WAKEUPTIMER_DISABLE_IT(&RTC_Handler,RTC_IT_WUT);
	 __HAL_RTC_TIMESTAMP_DISABLE_IT(&RTC_Handler,RTC_IT_TS);
	 __HAL_RTC_ALARM_DISABLE_IT(&RTC_Handler,RTC_IT_ALRA|RTC_IT_ALRB);

	
    //清除RTC相关中断标志位
    __HAL_RTC_ALARM_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_ALRAF|RTC_FLAG_ALRBF);
    __HAL_RTC_TIMESTAMP_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_TSF); 
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_WUTF);
    
    __HAL_RCC_BACKUPRESET_RELEASE();                    //备份区域复位结束
    __HAL_RTC_WRITEPROTECTION_ENABLE(&RTC_Handler);     //使能RTC写保护
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);                  //清除Wake_UP标志
	
		
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);           //设置WKUP用于唤醒
    HAL_PWR_EnterSTANDBYMode();                         //进入待机模式   	


}


//这里看情况加灯进行提示，看后期板子设计 
u8 key_wkup(void) 
{
  static u32 u32end_time = 0;
  static u32 u32start_time = 0;//记录松开的次数
  static u32 u32delay_time = 0;
  static u8 u8first_tick = 0;

	while(1)
	{
		if(WKUP_KD)//已经按下了
		{
			if(u8first_tick == 0)
			{
				u32start_time = HAL_GetTick();	
			}
			u8first_tick  = 1;
			u32end_time = HAL_GetTick();
			u32delay_time = u32end_time - u32start_time;
			logblue("u32delay_time = %d", u32delay_time);
		}
		else 
		{
			if(u32delay_time >= 3000)
			{
				return 1; //按下3s以上了			
			}
			else
			return 0;//按下小于3S，返回主页面			
		}
		
		oSMonitOsDelay(1);		
	}
		
}


//外部中断线0中断服务函数
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin==GPIO_PIN_0)//PA0
    {
        switch(key_wkup())	//判断按键状态
        {
			case WKUP_STANDBY:  	//长按3S进入待机
				
				sys_standby();//进入待机模式
			
			break;
			
			case WKUP_RETURN_HOMEPAGE:	//按一下返回主页
				
			
			break;
			
			default:
				
			break;
		}
    }    
}


//PA0 WKUP唤醒初始化
void WKUP_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;  //中断,上升沿
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    //检查是否是正常开机
    if(key_wkup()==0)
    {
        sys_standby();//不是开机，进入待机模式
    }

    HAL_NVIC_SetPriority(EXTI0_IRQn,0x02,0x02);//抢占优先级2，子优先级2
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

