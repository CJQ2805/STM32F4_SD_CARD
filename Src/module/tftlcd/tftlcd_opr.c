#include <tftlcd_opr.h>
#include <log.h>
#include <osmonit.h>
#include <tftlcd_arg.h>
#include <SysCommDef.h>
#include "lv_port_disp.h"
#include "lv_demo_widgets.h"

char c[] = "TFTLCD TEST  CJQ";

static void tftlcd_task(void const * argument)
{
	logblue("%s",(char*)argument);
	logn();

	
	lv_init();
	lv_port_disp_init();
	lv_demo_widgets();
	
	for(;;){ 
		
		taskENTER_CRITICAL();		
		lv_task_handler();	
		taskEXIT_CRITICAL();
//		LCD_ShowString(10,80,400,32,32,c);	
//		LCD_ShowString(10,110,400,32,32,c);	
//		LCD_ShowString(10,130,400,32,32,c);	
//		LCD_ShowString(10,150,400,32,32,c);	
//		LCD_ShowString(10,180,400,32,32,c);	
//		LCD_ShowString(10,210,400,32,32,c);	
//		LCD_ShowString(10,240,400,32,32,c);		
//		LCD_ShowString(10,270,400,32,32,c);		
//		LCD_ShowString(10,300,400,32,32,c);		
//		LCD_ShowString(10,330,400,32,32,c);		
//		LCD_ShowString(10,370,400,32,32,c);		
//		LCD_ShowString(10,400,400,32,32,c);		
//		LCD_ShowString(10,430,400,32,32,c);		
//		LCD_ShowString(10,460,400,32,32,c);		
//		LCD_ShowString(10,500,400,32,32,c);		
//		LCD_ShowString(10,530,400,32,32,c);		
//		LCD_ShowString(10,560,400,32,32,c);		
//		LCD_Clear(WHITE);
		oSMonitOsDelay(1);
	}
}



osThreadId     lcd_task_handele_tid;

void create_tftlcd_task(void)
{
	osThreadDef(lcd, tftlcd_task, osPriorityHigh, 0, 512);
	lcd_task_handele_tid = osThreadCreate(osThread(lcd), "lcd task start");
    creat_task_info(lcd_task_handele_tid, "lcd task");
}
