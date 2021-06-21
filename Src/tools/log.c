#include <log.h>
#include <SysCommDef.h>
struct _log_arg_t{

SemaphoreHandle_t log_ptMutex;

};

struct _log_arg_t  g_tlog_arg;


void Log_init()
{
	 g_tlog_arg.log_ptMutex = xSemaphoreCreateMutex();
	 if( NULL == g_tlog_arg.log_ptMutex )
	 {
	 	logerr("mutex malloc fail");
	 }
}


void LogTakeMutex(void)//(SemaphoreHandle_t ptMutex)
{
	if( NULL ==  g_tlog_arg.log_ptMutex )
	{
		//logerr("the ptMutex pointer is null");
		return;
	}
	xSemaphoreTake( g_tlog_arg.log_ptMutex, portMAX_DELAY);

}





void LogGiveMutex(void)//(SemaphoreHandle_t ptMutex)
{
	xSemaphoreGive(g_tlog_arg.log_ptMutex);
}

