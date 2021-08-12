#include <sd_card_opr.h>
#include <SysCommDef.h>
#include <osmonit.h>
#include "fatfs.h"

#define  CJQTEST  "0:/CJQ"



static void sd_card_task(void const * argument)
{
	logblue("%s",(char*)argument);
	logn();
	my_fmount();
	sd_card_init();
	sd_card_write();
	for(;;){ 
		
	
		oSMonitOsDelay(1);
	}
}



int sd_card_init(void)
{
	FRESULT res;
    FILINFO finfo;
	if(f_stat(CJQTEST,&finfo) == FR_NO_FILE)
	{
		logerr("CJQTEST NO FILE");
		logblue("mkdir CJQTEST");
		res = f_mkdir(CJQTEST);
        if( res!=FR_OK && res!=FR_EXIST )
            return res;

	}
}


osThreadId     sd_card_task_handele_tid;
void create_sd_card_task(void)
{
	osThreadDef(sd_card, sd_card_task, osPriorityNormal, 0, 256);
	sd_card_task_handele_tid = osThreadCreate(osThread(sd_card), "sd_card task start");
    creat_task_info(sd_card_task_handele_tid, "sd_card task");
}