#ifndef __IAP_H__
#define __IAP_H__
#include "sys.h"  

/************************************************
 * SECTOR DEFINE
************************************************/
#define ADDR_FLASH_SECTOR_0 ((u32)0x08000000)	 //扇区0起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_1 ((u32)0x08004000)	 //扇区1起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_2 ((u32)0x08008000)	 //扇区2起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_3 ((u32)0x0800C000)	 //扇区3起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_4 ((u32)0x08010000)	 //扇区4起始地址, 64 Kbytes
#define ADDR_FLASH_SECTOR_5 ((u32)0x08020000)	 //扇区5起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_6 ((u32)0x08040000)	 //扇区6起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_7 ((u32)0x08060000)	 //扇区7起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_8 ((u32)0x08080000)	 //扇区8起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_9 ((u32)0x080A0000)	 //扇区9起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_10 ((u32)0x080C0000) //扇区10起始地址,128 Kbytes
#define ADDR_FLASH_SECTOR_11 ((u32)0x080E0000) //扇区11起始地址,128 Kbytes

#define ADDR_FLASH_SECTOR_12 ((u32)0x08100000) //扇区12起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_13 ((u32)0x08104000) //扇区13起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_14 ((u32)0x08108000) //扇区14起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_15 ((u32)0x0810C000) //扇区15起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_16 ((u32)0x08110000) //扇区16起始地址, 64 Kbytes
#define ADDR_FLASH_SECTOR_17 ((u32)0x08120000) //扇区17起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_18 ((u32)0x08140000) //扇区18起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_19 ((u32)0x08160000) //扇区19起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_20 ((u32)0x08180000) //扇区20起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_21 ((u32)0x081A0000) //扇区21起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_22 ((u32)0x081C0000) //扇区22起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_23 ((u32)0x081E0000) //扇区23起始地址, 128 Kbytes

/************************************************
 * MEMORY MAP
************************************************/
#define BOOT_LOADER ADDR_FLASH_SECTOR_0
#define BOOT_INFO ADDR_FLASH_SECTOR_12
#define VERSION_INFO ADDR_FLASH_SECTOR_13
#define FLASH_RESERVED ADDR_FLASH_SECTOR_14
#define APP_1 ADDR_FLASH_SECTOR_5
#define APP_2 ADDR_FLASH_SECTOR_17

//boot flag
#define FROM_APP_1 0xAA
#define FROM_APP_2 0x55


typedef struct
{
	u32 head;					 //頭部，0x55555555
	u8 version[4];		 //待升級版本號，version[3]不使用，版本號位置為2.1.0
	u32 firmware_size; //待升級固件長度bytes
	u32 firmware_crc;	 //待升級固件crc
	u32 check;				 //本區頭部信息sum校驗值
} stu_boot_info;

typedef struct
{
	u32 head;			 //頭部，0x55555555
	u8 version[4]; //當前程序版本號，version[3]不使用，版本號位置為2.1.0
	u32 check;		 //當前頭部sum校驗值
} stu_version_info;

#endif






































