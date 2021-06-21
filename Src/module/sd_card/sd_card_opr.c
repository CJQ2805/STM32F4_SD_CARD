#include <sd_card_opr.h>
#include <SysCommDef.h>
#include "fatfs.h"


static void sd_card_task(void const * argument)
{
	logblue("%s",(char*)argument);
	logn();
	my_fmount();


	
	for(;;){ 
		
	
		
	}
}




osThreadId     sd_card_task_handele_tid;

void create_sd_card_task(void)
{
	osThreadDef(sd_card, sd_card_task, osPriorityNormal, 0, 256);
	sd_card_task_handele_tid = osThreadCreate(osThread(sd_card), "sd_card task start");
    creat_task_info(sd_card_task_handele_tid, "sd_card task");
}