#include "low_power.h"
#include <SysCommDef.h>
#include "rtc.h"
#include <osmonit.h>

// ������     by CJQ2805

void sys_standby(void)
{
	__HAL_RCC_AHB1_FORCE_RESET(); //��λ���� IO ��
	 while(WKUP_KD); //�ȴ� WK_UP �����ɿ�(���� RTC �ж�ʱ,
	//����� WK_UP �ɿ��ٽ������)
	 __HAL_RCC_PWR_CLK_ENABLE(); //ʹ�� PWR ʱ��
	 __HAL_RCC_BACKUPRESET_FORCE(); //��λ��������
	 HAL_PWR_EnableBkUpAccess(); //���������ʹ�� 
	 __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
	 __HAL_RTC_WRITEPROTECTION_DISABLE(&RTC_Handler);//�ر� RTC д����
	 
	 //�ر� RTC ����жϣ������� RTC ʵ�����
	 __HAL_RTC_WAKEUPTIMER_DISABLE_IT(&RTC_Handler,RTC_IT_WUT);
	 __HAL_RTC_TIMESTAMP_DISABLE_IT(&RTC_Handler,RTC_IT_TS);
	 __HAL_RTC_ALARM_DISABLE_IT(&RTC_Handler,RTC_IT_ALRA|RTC_IT_ALRB);

	
    //���RTC����жϱ�־λ
    __HAL_RTC_ALARM_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_ALRAF|RTC_FLAG_ALRBF);
    __HAL_RTC_TIMESTAMP_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_TSF); 
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_WUTF);
    
    __HAL_RCC_BACKUPRESET_RELEASE();                    //��������λ����
    __HAL_RTC_WRITEPROTECTION_ENABLE(&RTC_Handler);     //ʹ��RTCд����
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);                  //���Wake_UP��־
	
		
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);           //����WKUP���ڻ���
    HAL_PWR_EnterSTANDBYMode();                         //�������ģʽ   	


}


//���￴����ӵƽ�����ʾ�������ڰ������ 
u8 key_wkup(void) 
{
  static u32 u32end_time = 0;
  static u32 u32start_time = 0;//��¼�ɿ��Ĵ���
  static u32 u32delay_time = 0;
  static u8 u8first_tick = 0;

	while(1)
	{
		if(WKUP_KD)//�Ѿ�������
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
				return 1; //����3s������			
			}
			else
			return 0;//����С��3S��������ҳ��			
		}
		
		oSMonitOsDelay(1);		
	}
		
}


//�ⲿ�ж���0�жϷ�����
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin==GPIO_PIN_0)//PA0
    {
        switch(key_wkup())	//�жϰ���״̬
        {
			case WKUP_STANDBY:  	//����3S�������
				
				sys_standby();//�������ģʽ
			
			break;
			
			case WKUP_RETURN_HOMEPAGE:	//��һ�·�����ҳ
				
			
			break;
			
			default:
				
			break;
		}
    }    
}


//PA0 WKUP���ѳ�ʼ��
void WKUP_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;  //�ж�,������
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    //����Ƿ�����������
    if(key_wkup()==0)
    {
        sys_standby();//���ǿ������������ģʽ
    }

    HAL_NVIC_SetPriority(EXTI0_IRQn,0x02,0x02);//��ռ���ȼ�2�������ȼ�2
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

