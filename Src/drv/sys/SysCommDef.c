
#include <SysCommDef.h>

  

uint32_t idAddr[]={ 0x1FFFF7AC,  /*STM32F0唯一ID起始地址*/
                    0x1FFFF7E8,  /*STM32F1唯一ID起始地址*/
                    0x1FFF7A10,  /*STM32F2唯一ID起始地址*/
                    0x1FFFF7AC,  /*STM32F3唯一ID起始地址*/
                    0x1FFF7A10,  /*STM32F4唯一ID起始地址*/
                    0x1FF0F420,  /*STM32F7唯一ID起始地址*/
                    0x1FF80050,  /*STM32L0唯一ID起始地址*/
                    0x1FF80050,  /*STM32L1唯一ID起始地址*/
                    0x1FFF7590,  /*STM32L4唯一ID起始地址*/
                    0x1FF0F420}; /*STM32H7唯一ID起始地址*/
 
/*获取MCU的唯一ID*/
void GetSTM32MCUID(u32 *id,MCUTypedef type)
{
  if(id!=NULL)
  {
    id[0]=*(volatile u32*)(idAddr[type]);
    id[1]=*(volatile u32*)(idAddr[type]+4);
    id[2]=*(volatile u32*)(idAddr[type]+8);
  }
}



//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}

//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}

void sys_reboot(void){
    INTX_DISABLE();
    __set_FAULTMASK(1);
    HAL_NVIC_SystemReset();
}



void delay_us(u32 u32nus)
{		
	u32nus *= 44;
	do{
		__NOP();
	}while(u32nus--);
	
}
void delay_ms(u32 u32time)
{
	while(u32time--)
	{
		delay_us(1000);
	}
}













