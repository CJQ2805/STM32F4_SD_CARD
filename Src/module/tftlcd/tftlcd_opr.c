#include <tftlcd_opr.h>
#include <log.h>
#include <osmonit.h>
#include <tftlcd_arg.h>
#include <SysCommDef.h>


char c[] = "TFTLCD TEST  CJQ";

static void tftlcd_task(void const * argument)
{
	logblue("%s",(char*)argument);
	logn();

	POINT_COLOR=RED;

//	LCD_ShowString(10,80,400,32,32,c);		
		
	for(;;){ 
		
	
		oSMonitOsDelay(1);
	}
}



osThreadId     lcd_task_handele_tid;

void create_tftlcd_task(void)
{
	osThreadDef(lcd, tftlcd_task, osPriorityNormal, 0, 256);
	lcd_task_handele_tid = osThreadCreate(osThread(lcd), "lcd task start");
    creat_task_info(lcd_task_handele_tid, "lcd task");
}
