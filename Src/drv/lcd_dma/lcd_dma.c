#include <lcd_dma.h>
#include "lv_port_disp.h"
#include <tftlcd_arg.h>
#include <SysCommDef.h>

DMA_HandleTypeDef     DMA_HandlerStructure;
extern lv_disp_drv_t * disp_drv_p;	
lcd_dma_t g_tlcd_dma;

void LCD_Address_Set(u16 u16x1,u16 u16y1, u16 u16x2, u16  u16y2)
{
	
//		LCD_WR_REG(0x2a00); 
//		LCD_WR_DATA(u16x1>>8);
//		LCD_WR_REG(0x2a00+1); 
//		LCD_WR_DATA(u16x1&0XFF);
//		LCD_WR_REG(0x2a00+2); 
//		LCD_WR_DATA(u16x2>>8); 
//		LCD_WR_REG(0x2a00+3); 
//		LCD_WR_DATA(u16x2&0XFF);  
//		LCD_WR_REG(0x2b00); 
//		LCD_WR_DATA(u16y1>>8);
//		LCD_WR_REG(0x2b00+1);
//		LCD_WR_DATA(u16y1&0XFF); 
//		LCD_WR_REG(0x2b00+2);
//		LCD_WR_DATA(u16y2>>8); 
//		LCD_WR_REG(0x2b00+3);
//		LCD_WR_DATA(u16y2&0XFF); 
//
		LCD_Set_Window(u16x1, u16y1,u16x2 - u16x1+1,u16y2 - u16y1+1); 	
		LCD_WR_REG(0x2c00);	

	
}



void dma_transfer_complete(DMA_HandleTypeDef * dmah)
{
	u32 u32dma_continue;
	u32 u32dma_source_addr;
	
	if(g_tlcd_dma.u32dma_index < g_tlcd_dma.u32pixel_count)
	{
		u32dma_continue = g_tlcd_dma.u32dma_index;
		
		if(g_tlcd_dma.u32dma_index + 0xffff < g_tlcd_dma.u32pixel_count)
		{
			g_tlcd_dma.u32dma_index  = g_tlcd_dma.u32dma_index + 0xffff;
			
		}else{
			
			g_tlcd_dma.u32dma_index  = g_tlcd_dma.u32pixel_count;
		}
		
		u32dma_source_addr = (u32)g_tlcd_dma.pu16mem_source + u32dma_continue;
		HAL_DMA_Start_IT(&DMA_HandlerStructure, (u32)u32dma_source_addr, (u32)&LCD->LCD_RAM, (g_tlcd_dma.u32dma_index-u32dma_continue));  //开启DMA传输			
	
	}else{
		lv_disp_flush_ready(disp_drv_p);	
		
	}
}

void dma_transfer_error(DMA_HandleTypeDef * dmah)
{
	printf("dma error!");
}

void DMA_fsmc_Init(void)
{  
	__HAL_RCC_DMA2_CLK_ENABLE();	

	DMA_HandlerStructure.Init.Channel = DMA_CHANNEL_1 ;   //通道0
	DMA_HandlerStructure.Init.Direction = DMA_MEMORY_TO_MEMORY; //存储器到存储器

	DMA_HandlerStructure.Init.PeriphInc = DMA_PINC_DISABLE;    	//外设是否递增
	DMA_HandlerStructure.Init.MemInc 	= DMA_MINC_DISABLE;      //内存是否递增
	DMA_HandlerStructure.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;     //半字 16位
	DMA_HandlerStructure.Init.PeriphDataAlignment = DMA_MDATAALIGN_HALFWORD;  //半字 16位
	DMA_HandlerStructure.Init.Priority = DMA_PRIORITY_HIGH;       //设置优先级 高
	DMA_HandlerStructure.Init.Mode = DMA_NORMAL;    
	DMA_HandlerStructure.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	DMA_HandlerStructure.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	DMA_HandlerStructure.Init.PeriphBurst = DMA_PBURST_SINGLE;
	DMA_HandlerStructure.Init.MemBurst    = DMA_MBURST_SINGLE;

	DMA_HandlerStructure.Instance = DMA2_Stream0; 		  //数据流
	if(	HAL_DMA_Init(&DMA_HandlerStructure) != HAL_OK)
	{
		return; 
	}
	
	DMA_HandlerStructure.XferCpltCallback = dma_transfer_complete;
	DMA_HandlerStructure.XferErrorCallback = dma_transfer_error;	

	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 3, 3);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);	
	
}




////开启一次DMA传输
//void DMA_Enable(u16 num)
//{
//	DMA1_Channel1->CCR&=~(1<<0);       //关闭DMA传输 
//	DMA1_Channel1->CNDTR=num;          //DMA1,传输数据量 
//	DMA1_Channel1->CCR|=1<<0;          //开启DMA传输
//}	

void display_fill(u16 X1, u16 Y1, u16 X2, u16 Y2, u16 color)
{
	g_tlcd_dma.u16color_buffer = color;
	DMA_Fill_Color(X1,Y1, X2, Y2, &g_tlcd_dma.u16color_buffer, INCREASE_DISABLE);
}

void display_bitmap(u16 X1, u16 Y1, u16 X2, u16 Y2, u16 *color)
{
	DMA_Fill_Color(X1, Y1, X2, Y2, color, INCREASE_ENABLE);
}



HAL_DMA_StateTypeDef DMA_state;

void DMA_Fill_Color(u16 X1, u16 Y1, u16 X2, u16 Y2, u16 *color,
	transfer_source_pointer_increase_e source_pointer_increase)
{
	DMA_state = HAL_DMA_GetState(&DMA_HandlerStructure);
	
	if(HAL_DMA_GetState(&DMA_HandlerStructure) != (HAL_DMA_STATE_READY)){
		
		printf("DMA is not ready!\r\n");
	}
	
	g_tlcd_dma.u32pixel_count = ((X2-X1+1) *(Y2-Y1+1) *2);
	g_tlcd_dma.pu16mem_source =  color;
	g_tlcd_dma.increase_mode = source_pointer_increase;

	if(g_tlcd_dma.u32pixel_count > 0xffff){
	
		g_tlcd_dma.u32dma_index = 0xffff;
		
	}else{
		
		g_tlcd_dma.u32dma_index = g_tlcd_dma.u32pixel_count;
		
	}
	
	if(INCREASE_DISABLE == g_tlcd_dma.increase_mode)
	{
		DMA_HandlerStructure.Instance->CR &= ~((u32)1 << 9);
	}else{
		DMA_HandlerStructure.Instance->CR |= DMA_PINC_ENABLE;
	}
    LCD_Address_Set(X1,Y1,X2,Y2);
//	LCD_WriteRAM_Prepare();     //开始写入GRAM
	HAL_DMA_Start_IT(&DMA_HandlerStructure, (u32)g_tlcd_dma.pu16mem_source, (u32)&LCD->LCD_RAM , g_tlcd_dma.u32dma_index );  //开启DMA传输	
}



void DMA2_Stream0_IRQHandler(void)
{	
	HAL_DMA_IRQHandler(&DMA_HandlerStructure);
}

