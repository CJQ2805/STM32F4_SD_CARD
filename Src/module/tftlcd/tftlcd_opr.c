#include <tftlcd_opr.h>
#include <log.h>
#include <osmonit.h>
#include <tftlcd_arg.h>
#include <SysCommDef.h>
#include "lv_port_disp.h"
#include "lv_demo_widgets.h"
#include "gui_app_start.h"
#include "gui_app_test.h"

char c[] = "TFTLCD TEST  CJQ2805";

#define PAGE_REG(name)\
do{\
	extern void PageRegiser_##name(u8 pageID);\
	PageRegiser_##name(PAGE_##name);\
}while(0)


static void tftlcd_task(void const * argument)
{
	logblue("%s",(char*)argument);
	logn();
//   app_show_start();
//	start_img_anim();	

//	gui_app_test();
//	img_test();
// 	lv_task_handler(); //LVGL更新显示 
//	oSMonitOsDelay(1000);	
//	start_img_anim();
//	lv_task_handler();

//	LCD_LED= 1;   //背光设置   1：点亮
//	oSMonitOsDelay(1000);	
	lv_demo_widgets();
	for(;;){ 
			
		lv_task_handler();	
		oSMonitOsDelay(1);
	}
}



osThreadId     lcd_task_handele_tid;

void create_tftlcd_task(void)
{
	osThreadDef(lcd, tftlcd_task, osPriorityHigh, 0, 512*4);
	lcd_task_handele_tid = osThreadCreate(osThread(lcd), "lcd task start");
    creat_task_info(lcd_task_handele_tid, "lcd task");
}
