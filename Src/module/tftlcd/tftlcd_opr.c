#include <tftlcd_opr.h>
#include <log.h>
#include <tftlcd_arg.h>
#include <SysCommDef.h>




static void tftlcd_task(void const * argument)
{
	logblue("%s",(char*)argument);
	logn();
	LCD_Init();
	POINT_COLOR=RED;
	LCD_ShowString(10,80,240,24,24,"TFTLCD TEST");		
	LCD_ShowString(10,40,240,32,32,"Apollo STM32F4/F7"); 		
	for(;;){ 
		
	
		
	}
}



osThreadId     lcd_task_handele_tid;

void create_tftlcd_task(void)
{
	osThreadDef(lcd, tftlcd_task, osPriorityNormal, 0, 256);
	lcd_task_handele_tid = osThreadCreate(osThread(lcd), "lcd task start");
    creat_task_info(lcd_task_handele_tid, "lcd task");
}
