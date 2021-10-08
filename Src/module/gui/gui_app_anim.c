#include "gui_app_anim.h"
#include "lv_port_disp.h"

;void img_anim_exec_xcb(void *obj, lv_anim_value_t value)
{
	lv_obj_set_style_local_image_opa(obj, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, value); //Õº∆¨Õ∏√˜∂»…Ë÷√
}
