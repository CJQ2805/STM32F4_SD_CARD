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

	POINT_COLOR=RED;

//	LCD_ShowString(10,80,400,32,32,c);		
	lv_init();
	lv_port_disp_init();
	lv_demo_widgets();
	
	for(;;){ 
		
		lv_task_handler();	
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
