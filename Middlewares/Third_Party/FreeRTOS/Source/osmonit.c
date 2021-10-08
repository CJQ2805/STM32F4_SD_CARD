
#include <osmonit.h>
#include <log.h>
#include "key.h"

IWDG_HandleTypeDef hiwdg;  

#define IGNORE_TASK_NAME_DEFAULT_AMOUNT 7
#define IGNORE_TASK_NAME_AMOUNT  (IGNORE_TASK_NAME_DEFAULT_AMOUNT+OS_MONIT_IGNORE_TASK_NAME_AMOUNT2)

const char * g_apcIgnoreTaskName[IGNORE_TASK_NAME_AMOUNT] = {"defaultTask", "eth_net", "IDLE", "tcpip_thread", "EthIf", "OSMonit"};

#define  HEAP_WARN_1  0x01
#define  HEAP_WARN_2  0x10

typedef struct	_OSMonitStatBit_T {


	u32 b2HeapWarn	 	 	: 2;	
    u32 b1SysRestart        : 1;
}T_OSMonitStatBit;

struct _TaskEventVal_T {
    
    char         acTaskName[50];
    u8           u8Stat;
    EventBits_t  u32TaskEventVal;
};
  
struct _task_info {
    
    char         acTaskName[50];
    UBaseType_t     u32Val;
    TaskHandle_t pxTask;
};


typedef struct _OSMonitArg_T {
	u32                     u32FreeHeapSize; 
    u32                     u32EventVal;
    EventBits_t             tRxEventBits;
    u32                     u32timeconter;    
	EventGroupHandle_t      ptEvntGrupHandl;
    T_OSMonitStatBit        tOSMonitStatBit;
    UBaseType_t             uxArraySize;
    TaskStatus_t           *ptTaskStatusArray;
    struct _TaskEventVal_T *ptTaskEvnetVal;
    char acIgnoreTaskName[IGNORE_TASK_NAME_AMOUNT][50];
    u32                 u32TotalRunTime;
	void (*fInitCallBack)(void);
	int  (*fRunCallBack[OS_MONIT_RUN_CALL_BACK_INDEX_MAX])(void* pvarg);
    void* apvRunCallBackArg[OS_MONIT_RUN_CALL_BACK_INDEX_MAX];
    enum _OsMonitRunCallBackIndex_E eRunCallbackIndex;
    u8 u8SetIgnorTaskIndex;
    
    struct _task_info   ttask_info[20];
    u8      u8task_info_num;
	char    acinfo_buff[1000];
	
}T_OSMonitArg , *PT_OSMonitArg;

T_OSMonitArg g_tOSMonitArg;
osThreadId   g_ptOSMonitTaskHandle;

/*
    创建任务时调用，可将任务信息存入osmonit_arg后统一观察内存开销情况
*/
int creat_task_info(TaskHandle_t pxTask, char * pcname)
{
    strncpy(g_tOSMonitArg.ttask_info[g_tOSMonitArg.u8task_info_num].acTaskName,pcname,50-1);
    g_tOSMonitArg.ttask_info[g_tOSMonitArg.u8task_info_num].pxTask  = pxTask;
    g_tOSMonitArg.u8task_info_num++;
    return 0;
}

int updata_task_memory(void)
{
    for(int i = 0; i < g_tOSMonitArg.u8task_info_num; i++)
    {
        g_tOSMonitArg.ttask_info[i].u32Val = uxTaskGetStackHighWaterMark(g_tOSMonitArg.ttask_info[i].pxTask);
    }
    return 0;
}

int SetIgnorTaskName(char * pcName)
{
    u8 u8SetIgnorTaskIndex = IGNORE_TASK_NAME_DEFAULT_AMOUNT + g_tOSMonitArg.u8SetIgnorTaskIndex;

    if(g_tOSMonitArg.u8SetIgnorTaskIndex>=IGNORE_TASK_NAME_AMOUNT)
        return -1;
    g_apcIgnoreTaskName[u8SetIgnorTaskIndex] = pcName;
    g_tOSMonitArg.u8SetIgnorTaskIndex++;
    return 0;
}

/* IWDG init function */
__weak void IWDG_Init(void)
{

  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
  hiwdg.Init.Reload = 0xfff;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
  }

}


#ifndef CLOSE_OS_MONIT

#define OPEN_OS_MONIT

#endif

#ifdef OPEN_OS_MONIT

void OSMonitInit(void)
{
	g_tOSMonitArg.ptEvntGrupHandl = xEventGroupCreate();
	
	if(NULL == g_tOSMonitArg.ptEvntGrupHandl)
	{
		
	}

	IWDG_Init();
	HAL_IWDG_Refresh(&hiwdg);
}

void IWDG_Refresh(void)
{
     HAL_IWDG_Refresh(&hiwdg);
}

void SetTaskMonitEvntGrupBits(EventBits_t uxBitsToSet)
{
	if(NULL == g_tOSMonitArg.ptEvntGrupHandl)
		return;
	xEventGroupSetBits(g_tOSMonitArg.ptEvntGrupHandl, uxBitsToSet);
}

void  oSMonitOsDelay(u32 millisec)
{

	u32 u32N100ms = 0, u32TaskIndex = 0;
	u8	u8Delay   = 0;
    int iFindTask = -1;
    char *pcTaskName = NULL;
	u32N100ms = millisec/100;
	u8Delay   = millisec%100;

    pcTaskName = pcTaskGetName(NULL);
    if(!pcTaskName)
    {
        osDelay(millisec);
        return;
    }
    for( ; u32TaskIndex < g_tOSMonitArg.uxArraySize; u32TaskIndex++)
    {
        if(!g_tOSMonitArg.ptTaskEvnetVal[u32TaskIndex].u8Stat)
            continue;
        if(strncmp(g_tOSMonitArg.ptTaskEvnetVal[u32TaskIndex].acTaskName, pcTaskName, strlen(g_tOSMonitArg.ptTaskEvnetVal[u32TaskIndex].acTaskName)) == 0)
        {   
            iFindTask = 1;
            break;
        }
        
    }
    if(1 != iFindTask)
    {
        //osDelay(1);
        osDelay(millisec);
        return;
    }

	while(u32N100ms)
	{
		u32N100ms--;
		SetTaskMonitEvntGrupBits(g_tOSMonitArg.ptTaskEvnetVal[u32TaskIndex].u32TaskEventVal);
		osDelay(100);
	}
    if(u8Delay)
    {
        SetTaskMonitEvntGrupBits(g_tOSMonitArg.ptTaskEvnetVal[u32TaskIndex].u32TaskEventVal);
        osDelay(u8Delay);
    }
}


void  oSMonitOsDelay2(u32 millisec, TaskHandle_t xTaskToQuery)
{

	u32 u32N100ms = 0, u32TaskIndex = 0;
	u8	u8Delay   = 0;
    int iFindTask = -1;
    char *pcTaskName = NULL;
	u32N100ms = millisec/100;
	u8Delay   = millisec%100;

    pcTaskName = pcTaskGetName(xTaskToQuery);
    if(!pcTaskName)
    {
        osDelay(millisec);
        return;
    }
    for( ; u32TaskIndex < g_tOSMonitArg.uxArraySize; u32TaskIndex++)
    {
        if(!g_tOSMonitArg.ptTaskEvnetVal[u32TaskIndex].u8Stat)
            continue;
        if(strncmp(g_tOSMonitArg.ptTaskEvnetVal[u32TaskIndex].acTaskName, pcTaskName, strlen(g_tOSMonitArg.ptTaskEvnetVal[u32TaskIndex].acTaskName)) == 0)
        {   
            iFindTask = 1;
            break;
        }
        
    }
    if(1 != iFindTask)
    {
        //osDelay(1);
        osDelay(millisec);
        return;
    }

	while(u32N100ms)
	{
		u32N100ms--;
		SetTaskMonitEvntGrupBits(g_tOSMonitArg.ptTaskEvnetVal[u32TaskIndex].u32TaskEventVal);
		osDelay(100);
	}
    if(u8Delay)
    {
        SetTaskMonitEvntGrupBits(g_tOSMonitArg.ptTaskEvnetVal[u32TaskIndex].u32TaskEventVal);
        osDelay(u8Delay);
    }
}





#else 
void OSMonitInit(void)
{

}

void IWDG_Refresh(void)
{
     
}

void SetTaskMonitEvntGrupBits(EventBits_t uxBitsToSet)
{
    
}

void  oSMonitOsDelay(u32 millisec)
{
    osDelay(millisec);
}

#endif


#define  OS_MONIT_TASK_DELAY_VAL 10 //MS

void oSMonitTimer(void)
{
   g_tOSMonitArg.u32timeconter++;

}

int oSMonitOuttime(u32 u32outtime)
{
   return ((g_tOSMonitArg.u32timeconter*OS_MONIT_TASK_DELAY_VAL%u32outtime) == 0);
}

void CheckHeapSize(void)
{
	g_tOSMonitArg.u32FreeHeapSize = xPortGetFreeHeapSize();
	//logblue("u32FreeHeapSize = %d", g_tOSMonitArg.u32FreeHeapSize);
	if(g_tOSMonitArg.u32FreeHeapSize < HEAP_SIZE_ERRO)
	{
		//logerr("FreeHeapSize  %u erro, reset" , g_tOSMonitArg.u32FreeHeapSize);
		osDelay(3000);
		g_tOSMonitArg.tOSMonitStatBit.b1SysRestart = 1;
	}
	else if( (g_tOSMonitArg.u32FreeHeapSize <= HEAP_SIZE_MIN)&&(g_tOSMonitArg.u32FreeHeapSize >= HEAP_SIZE_ERRO) )
	{
		//logerr("FreeHeapSize %u too small", g_tOSMonitArg.u32FreeHeapSize);
        g_tOSMonitArg.tOSMonitStatBit.b2HeapWarn = 1;
	}
}

void GetSystemStat(void)
{
    UBaseType_t uxArraySize = 0;
    unsigned int ulTotalRunTime = 0;
    u32 u32Size = 0;

    uxArraySize = uxTaskGetNumberOfTasks();
    if(g_tOSMonitArg.uxArraySize != uxArraySize)
    {
        g_tOSMonitArg.u32EventVal = 0;
        g_tOSMonitArg.uxArraySize = uxArraySize;
        if(g_tOSMonitArg.ptTaskStatusArray)
        {
            vPortFree(g_tOSMonitArg.ptTaskStatusArray);
            g_tOSMonitArg.ptTaskStatusArray = NULL;
            
        }
        u32Size = ( g_tOSMonitArg.uxArraySize * sizeof( TaskStatus_t ) );
        
        g_tOSMonitArg.ptTaskStatusArray = pvPortMalloc(u32Size);
        if(g_tOSMonitArg.ptTaskStatusArray)
        {
            memset(g_tOSMonitArg.ptTaskStatusArray, 0, u32Size);
        }

        if(g_tOSMonitArg.ptTaskEvnetVal)
        {
            vPortFree(g_tOSMonitArg.ptTaskEvnetVal);
            g_tOSMonitArg.ptTaskEvnetVal = NULL;
        }
             
        u32Size = g_tOSMonitArg.uxArraySize * sizeof( struct _TaskEventVal_T );
        g_tOSMonitArg.ptTaskEvnetVal = pvPortMalloc(u32Size);
        if(g_tOSMonitArg.ptTaskEvnetVal)
            memset(g_tOSMonitArg.ptTaskEvnetVal, 0, u32Size);
            
        
    }
    
    if(!g_tOSMonitArg.ptTaskStatusArray)
        return;
    uxArraySize = uxTaskGetSystemState(g_tOSMonitArg.ptTaskStatusArray,
                                       g_tOSMonitArg.uxArraySize,
                                      &ulTotalRunTime);
    g_tOSMonitArg.u32TotalRunTime = HAL_GetTick();
}

int isIgnorTask(const char * pcTaskName)
{
    int iRetVal = 0;
    u8 u8Size = sizeof(g_apcIgnoreTaskName)/sizeof(char*);
    
    for(u8 i = 0; i < u8Size; i++)
    {
        if(strncmp(g_tOSMonitArg.acIgnoreTaskName[i], pcTaskName, strlen(pcTaskName)) == 0)
        {
           iRetVal = 1; 
           break;
        } 
    }
    return iRetVal;
}

void AddIgnorTask(const char * pcname)
{ 
    static u8 i = 0;

    if(i >= IGNORE_TASK_NAME_AMOUNT)
        return;
    memset(g_tOSMonitArg.acIgnoreTaskName[i], 0, sizeof(g_tOSMonitArg.acIgnoreTaskName[i]));
    strncpy(g_tOSMonitArg.acIgnoreTaskName[i], pcname, sizeof(g_tOSMonitArg.acIgnoreTaskName[i]) - 1);
    i++;
}

void DeleteMonitedTask(void)
{
    for(u8 i = 0; i < g_tOSMonitArg.uxArraySize; i++)
    {
         if(!g_tOSMonitArg.ptTaskEvnetVal[i].u8Stat)
            continue; 
    }
}

void SysMointTaskRegister(void)
{
    u32 j = 0;
    if(!g_tOSMonitArg.ptTaskStatusArray)
        return;
    
    if(!g_tOSMonitArg.ptTaskEvnetVal)
        return;
    for(u32 i= 0; i < g_tOSMonitArg.uxArraySize; i++)
    {
        //logblue("pcTaskName[%d] = %s", i, g_tOSMonitArg.ptTaskStatusArray[i].pcTaskName);
        if(eDeleted == g_tOSMonitArg.ptTaskStatusArray[i].eCurrentState)
        {
            //logblue("eDeleted");
            continue;
        }
        
        if(eInvalid == g_tOSMonitArg.ptTaskStatusArray[i].eCurrentState)
        {
            
            //logblue("eInvalid");
            continue;

        }

        if(isIgnorTask(g_tOSMonitArg.ptTaskStatusArray[i].pcTaskName) == 1)
        {
            
            //logblue("is ignortask");
            continue;
        }
        if(g_tOSMonitArg.ptTaskEvnetVal[j].u8Stat)
        {
            j++;
            //logblue("stat");
            continue;
        }
        //logblue("j = %d", j);
        g_tOSMonitArg.ptTaskEvnetVal[j].u8Stat = 1; 
        strncpy(g_tOSMonitArg.ptTaskEvnetVal[j].acTaskName, g_tOSMonitArg.ptTaskStatusArray[i].pcTaskName, sizeof(g_tOSMonitArg.ptTaskEvnetVal[j].acTaskName)-1);
        g_tOSMonitArg.ptTaskEvnetVal[j].acTaskName[sizeof(g_tOSMonitArg.ptTaskEvnetVal[j].acTaskName) - 1] = 0;
        g_tOSMonitArg.ptTaskEvnetVal[j].u32TaskEventVal = (1 << j);
        g_tOSMonitArg.u32EventVal |= g_tOSMonitArg.ptTaskEvnetVal[j].u32TaskEventVal;   
        //logblue("acTaskName[%d] = %s", j, g_tOSMonitArg.ptTaskEvnetVal[j].acTaskName);
        j++;
    }

}

void SysRestart(void)
{
    if(!g_tOSMonitArg.tOSMonitStatBit.b1SysRestart)
        return;
    logerr("restart!!!");
    //logblue("Eventval= %d",g_tOSMonitArg.u32EventVal);
    //for(UBaseType_t i = 0; i < g_tOSMonitArg.uxArraySize; i++)
    {
     //   logblue("%s stat %d", g_tOSMonitArg.ptTaskEvnetVal[i].acTaskName, g_tOSMonitArg.ptTaskEvnetVal[i].u8Stat);
      //  logblue("TaskEventVal = %d", g_tOSMonitArg.ptTaskEvnetVal[i].u32TaskEventVal);
        
    }
    osDelay(3000);
    HAL_NVIC_SystemReset();
}

void print_osmonit(void)
{
    logblue("Eventval= %d",g_tOSMonitArg.u32EventVal);
    for(UBaseType_t i = 0; i < g_tOSMonitArg.uxArraySize; i++)
    {
        logblue("%s stat %d", g_tOSMonitArg.ptTaskEvnetVal[i].acTaskName, g_tOSMonitArg.ptTaskEvnetVal[i].u8Stat);
        logblue("TaskEventVal = %08x", g_tOSMonitArg.ptTaskEvnetVal[i].u32TaskEventVal);
        
    }
}

void EventMonit(void)
{
    
    if(!g_tOSMonitArg.ptEvntGrupHandl)
        return;
    if(!oSMonitOuttime(1000))
        return;
    if(!g_tOSMonitArg.u32EventVal)
    {
        //logyellow("refresh");
        IWDG_Refresh();
        return;
    }
    g_tOSMonitArg.tRxEventBits = xEventGroupWaitBits(g_tOSMonitArg.ptEvntGrupHandl, g_tOSMonitArg.u32EventVal, pdTRUE, pdTRUE, 1);
    //logblue("g_tOSMonitArg.tRxEventBits = %08X, g_tOSMonitArg.u32EventVal = %08X", g_tOSMonitArg.tRxEventBits, g_tOSMonitArg.u32EventVal);
//    print_osmonit();
    if(g_tOSMonitArg.u32EventVal ==  g_tOSMonitArg.tRxEventBits)
    {
        //logyellow("refresh");
        IWDG_Refresh();
    }
}

void RunFunction(void)
{
    if(!oSMonitOuttime(5000))
        return;
    CheckHeapSize();
    GetSystemStat();
    SysMointTaskRegister();
    //print_osmonit();
}


void RunCallback(void)
{
    static enum _OsMonitRunCallBackIndex_E eRunCallBackIndex = (enum _OsMonitRunCallBackIndex_E)0;

    if(!g_tOSMonitArg.fRunCallBack[eRunCallBackIndex])
    {
        if(++eRunCallBackIndex >= OS_MONIT_RUN_CALL_BACK_INDEX_MAX)
        {
            eRunCallBackIndex = (enum _OsMonitRunCallBackIndex_E)0;
        }  
        return;
    }
    
    g_tOSMonitArg.fRunCallBack[eRunCallBackIndex](g_tOSMonitArg.apvRunCallBackArg[eRunCallBackIndex]);

    if(++eRunCallBackIndex >= OS_MONIT_RUN_CALL_BACK_INDEX_MAX)
    {
        eRunCallBackIndex = (enum _OsMonitRunCallBackIndex_E)0;
    }

}

int RegisterRunCallback(int  (*fRunCallBack)(void* pvarg), void * pvcallback_arg)
{
    enum _OsMonitRunCallBackIndex_E eRunCallBackIndex = g_tOSMonitArg.eRunCallbackIndex;

    if(g_tOSMonitArg.eRunCallbackIndex >= OS_MONIT_RUN_CALL_BACK_INDEX_MAX)
        return -1;
    g_tOSMonitArg.fRunCallBack[eRunCallBackIndex]      = fRunCallBack;
    g_tOSMonitArg.apvRunCallBackArg[eRunCallBackIndex] = pvcallback_arg;
    g_tOSMonitArg.eRunCallbackIndex++;
    return 0;
}

void IgnorTaskInit(void)
{
    u8 u8Size = sizeof(g_apcIgnoreTaskName)/sizeof(char*);
    
    for(u8 i =0; i < u8Size; i++ )
    {
        AddIgnorTask(g_apcIgnoreTaskName[i]); 
//        AddIgnorTask("tcpip_thread");
//        AddIgnorTask("EthIf");
    }

}
/**
	@brief	查看任务运行时间， 使用了TIM3定时器，耗费极大系统资源
	仅调试可开启使用
*/

#if OPEN_TASK_RUNTIME

void get_task_run_time_stat(void)
{
	u8 u8key = 0;
	
//	u8key = KEY_Scan(0);
	if(u8key == WKUP_PRES)
	{
		vTaskGetRunTimeStats(g_tOSMonitArg.acinfo_buff);
		printf("%s\r\n",g_tOSMonitArg.acinfo_buff);
		osDelay(1);		
	}	
	
}

#elif

__weak void get_task_run_time_stat(void)
{


}

#endif

void OSMonitTask(void const * argument)
{
    if(g_tOSMonitArg.fInitCallBack)
        g_tOSMonitArg.fInitCallBack();
    OSMonitInit(); 
    IgnorTaskInit();

	for(;;)
	{
        oSMonitTimer();
        EventMonit();
        RunCallback();
        RunFunction();
        SysRestart();
        updata_task_memory();
//		get_task_run_time_stat();
		//osDelay(OS_MONIT_TASK_DELAY_VAL);
		oSMonitOsDelay(10);
	}	
}


void xOSMonitTask(void)
{
	osThreadDef(OSMonit, OSMonitTask, osPriorityHigh, 0, 256);
	g_ptOSMonitTaskHandle = osThreadCreate(osThread(OSMonit), NULL);
    creat_task_info(g_ptOSMonitTaskHandle, "osmonit task");
}









