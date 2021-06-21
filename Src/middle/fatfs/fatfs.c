/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include "fatfs.h"
#include <SysCommDef.h>
#include <osmonit.h>

typedef struct sd_file_arg_t{

	uint8_t retSD;    /* Return value for SD */
	char acSDPath[4];   /* SD logical drive path */
	FATFS ptSDFatFS;    /* File system object for SD logical drive */

	
}t_sd_file_arg, *pt_sd_file_arg;
/* USER CODE BEGIN Variables */
t_sd_file_arg   g_tsd_file_arg;
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  g_tsd_file_arg.retSD = FATFS_LinkDriver(&SD_Driver, g_tsd_file_arg.acSDPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}


FRESULT  my_fmount()
{
    FRESULT f_res;
    f_res   = f_mount(&g_tsd_file_arg.ptSDFatFS,(TCHAR const*)g_tsd_file_arg.acSDPath,1);
    switch(f_res)
    {
        case FR_NO_FILESYSTEM:
            logerr("SD NULL FatFs ...");
            break;
        case FR_OK:
			loggreen("SD LOAD FATFS OK  ");
			//logpurple("SD LOAD FatFs OK ...");
			//set_fatfs_mount_sta(MODULE_STAT_NORMAL);
            
            break;
        case FR_NOT_READY:
            logerr("SD FR_NOT_READY ...");
		    //set_fatfs_mount_sta(MODULE_STAT_FAULT);
            break;
        default:
            logerr("SD LOAD FatFs err .  -> %d",f_res);
	        //set_fatfs_mount_sta(MODULE_STAT_FAULT);
            break;
    }
	
	if(f_res    == FR_NO_FILESYSTEM)
    {
        f_res   = my_mkfs();
        if(f_res != FR_OK)
        {
            logerr("my_mkfs sd fail . %d",f_res);
        } 
    }
    if(f_res    == FR_NOT_READY)
    {
        while(1){
            oSMonitOsDelay(1);
        }
        //sys_reboot();
    }
    if(f_res    == FR_DISK_ERR)
    {
        while(1){
            oSMonitOsDelay(1);
        }
    }
    return f_res;


}




FRESULT my_mkfs(void)
{
    BYTE work[512];
    FRESULT f_res;     
    f_res=f_mkfs((TCHAR const*)g_tsd_file_arg.acSDPath,FM_ANY,0,work,512);		 		
    if(f_res == FR_OK)
    {
        logblue("SD CARD mkfs \n");
        f_res = f_mount(NULL,(TCHAR const*)g_tsd_file_arg.acSDPath,1);		        	
        f_res = f_mount(&g_tsd_file_arg.ptSDFatFS,(TCHAR const*)g_tsd_file_arg.acSDPath,1);         
    }
    else
    {
        logerr("SD CARD mkfs FAIL.\n");
    }
    return f_res;
}
/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

//得到磁盘剩余容量
//drv:磁盘编号("0:"/"1:")
//total:总容量	 （单位KB）
//free:剩余容量	 （单位KB）
//返回值:0,正常.其他,错误代码
u8 exf_getfree(u8 *drv,u32 *total,u32 *free)
{
	FATFS *fs1;
	u8 res;
    u32 fre_clust=0, fre_sect=0, tot_sect=0;
    //得到磁盘信息及空闲簇数量
    res =(u32)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//得到总扇区数
	    fre_sect=fre_clust*fs1->csize;			//得到空闲扇区数	   
#if _MAX_SS!=512				  				//扇区大小不是512字节,则转换为512字节
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		*total=tot_sect>>1;	//单位为KB
		*free=fre_sect>>1;	//单位为KB 
 	}
	return res;
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
