

#ifndef   __OS_MONIT_H
#define   __OS_MONIT_H

#include <SysCommdef.h>

//#define CLOSE_OS_MONIT

#define OPEN_TASK_RUNTIME     1

#define  HEAP_SIZE_MIN   (5*1024)
#define  HEAP_SIZE_ERRO  (4*1024)


enum _OsMonitRunCallBackIndex_E{


    OS_MONIT_RUN_CALL_BACK_INDEX_MAX = 3,
    
};

#define OS_MONIT_IGNORE_TASK_NAME_AMOUNT2  0
void  oSMonitOsDelay(u32 millisec);

int RegisterRunCallback(int (*fRunCallBack)(void* pvarg), void * pvcallback_arg);
int SetIgnorTaskName(char * pcName);
void xOSMonitTask(void);
void  oSMonitOsDelay2(u32 millisec, TaskHandle_t xTaskToQuery);
void IWDG_Refresh(void);







#endif


