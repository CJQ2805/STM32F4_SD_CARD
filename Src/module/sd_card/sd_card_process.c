#include <sd_card_process.h>
#include "fatfs.h"
#include "string.h"

char test1_string[100] = "aa , bb , cc , dd \r\n";
char test2_string[100] = "ee , ff , gg , hh \r\n";


//struct sd_card_process_arg_t{



//};



void sd_card_write()
{
	FRESULT res;
    FILINFO finfo;
	FIL file;
	uint32_t fil_size;
	uint32_t bw;	
	res = f_open(&file, "0:/CJQ/TEST.csv", FA_OPEN_ALWAYS | FA_WRITE);	
	if (res == FR_OK)
	{
		fil_size = f_size(&file);
		res = f_lseek(&file,fil_size);
		res = f_write(&file, test1_string , strlen(test1_string), &bw);
		res = f_write(&file, test1_string , strlen(test1_string), &bw);
		res = f_write(&file, test1_string , strlen(test1_string), &bw);
		res = f_write(&file, test1_string , strlen(test1_string), &bw);
		res = f_write(&file, test1_string , strlen(test1_string), &bw);
		res = f_write(&file, test1_string , strlen(test1_string), &bw);
		res = f_write(&file, test1_string , strlen(test1_string), &bw);
		res = f_write(&file, test1_string , strlen(test1_string), &bw);
		res = f_write(&file, test1_string , strlen(test1_string), &bw);
		res = f_write(&file, test1_string , strlen(test1_string), &bw);
		res = f_write(&file, test1_string , strlen(test1_string), &bw);
		res = f_write(&file, test1_string , strlen(test1_string), &bw);

	}//if (result == FR_OK)

	f_close(&file);
}


