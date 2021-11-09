#ifndef  __LCD_DMA_H
#define  __LCD_DMA_H
#include <SysCommDef.h>
#include "lv_port_disp.h"
typedef enum {
	
	INCREASE_ENABLE,
	INCREASE_DISABLE,

}transfer_source_pointer_increase_e;

typedef struct {

	u16   u16color_buffer;
	u16   *pu16mem_source;
	u32   u32pixel_count;
	u32   u32dma_index;
	transfer_source_pointer_increase_e  increase_mode;
	void (*user_transfer_complete_callback)(lv_disp_drv_t * disp_drv);	
}lcd_dma_t;

void DMA_Fill_Color(u16 X1, u16 Y1, u16 X2, u16 Y2, u16 *color,
	transfer_source_pointer_increase_e source_pointer_increase);

void display_fill(u16 X1, u16 Y1, u16 X2, u16 Y2, u16 color);
void display_bitmap(u16 X1, u16 Y1, u16 X2, u16 Y2, u16 *color);

void DMA_fsmc_Init(void);
#endif
