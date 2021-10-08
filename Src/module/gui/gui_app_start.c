#include "gui_app_start.h"
#include "lv_port_disp.h"
#include "gui_app_anim.h"


LV_IMG_DECLARE(start);

static lv_obj_t *start_img; //定义开始背景图片img

lv_obj_t *scr;
//创建开始图片
void app_show_start(void)
{

	scr = lv_scr_act();																		  //获取当前活跃的屏幕
	lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE); //设置背景颜色为黑色

	start_img = lv_img_create(scr, NULL);

	lv_obj_set_size(start_img, 134, 240); //设置大小

	lv_obj_align(start_img, scr, LV_ALIGN_CENTER, 0, 0); //设置图片中央对齐

	lv_img_set_src(start_img, &start); //然后显示此图片

	lv_obj_set_style_local_image_opa(start_img, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_80); //图片透明
}


//开始图片的渐变动画
void start_img_anim(void)
{
	lv_anim_t anim;

	lv_anim_init(&anim);

	lv_anim_set_var(&anim, start_img); //动画对象

	lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)img_anim_exec_xcb); //渐变函数
	lv_anim_set_time(&anim, lv_anim_speed_to_time(100, 0, 80));

	anim.start = LV_OPA_TRANSP;			//起始值
	anim.end = LV_OPA_COVER;			//结束值
	anim.path.cb = lv_anim_path_linear; //线性
	anim.playback_time = 0;				//回放时间设为0不执行动画回放

	lv_anim_start(&anim);
}
