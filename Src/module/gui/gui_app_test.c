#include "gui_app_test.h"
#include "lv_port_disp.h"
#include "gui_app_anim.h"

lv_style_t lv_style_plain_color;
lv_style_t lv_style_pretty;
static lv_obj_t * timg1;
static lv_obj_t *tstart_img; //定义开始背景图片img

LV_IMG_DECLARE(WaveShare_LOGO)
LV_IMG_DECLARE(heart)

void img_test(void)
{
    timg1 = lv_scr_act();	
	lv_obj_set_style_local_bg_color(timg1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE); 	
	tstart_img = lv_img_create(timg1, NULL);
	lv_obj_set_size(tstart_img, 600, 600); //设置大小
//    lv_img_set_src(start_img, &heart);
	lv_obj_align(tstart_img, timg1, LV_ALIGN_CENTER, 0, 0);
//    lv_obj_t * img2 = lv_img_create(lv_scr_act(), NULL);
//    lv_img_set_src(img2, LV_SYMBOL_AUDIO LV_SYMBOL_VIDEO
//                        LV_SYMBOL_OK  LV_SYMBOL_LIST
//                        LV_SYMBOL_CLOSE LV_SYMBOL_HOME
//                        LV_SYMBOL_WIFI LV_SYMBOL_BATTERY_EMPTY
//                        LV_SYMBOL_BLUETOOTH );
//    lv_obj_align(img2, img1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
	
	lv_img_set_src(tstart_img, &WaveShare_LOGO); //然后显示此图片
	lv_obj_set_style_local_image_opa(tstart_img, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0); //图片透明
}


void gui_app_test(void)
{
//    lv_obj_t * obj1;
//    tv = lv_tabview_create(lv_scr_act(), NULL);
//    lv_obj_set_style_local_pad_left(tv, LV_TABVIEW_PART_TAB_BG, LV_STATE_DEFAULT, LV_HOR_RES / 2);	
//	
//    obj1 = lv_obj_create(lv_scr_act(), NULL);
//    lv_obj_set_size(obj1, 200, 100);
//            if(lv_theme_get_flags() & LV_THEME_MATERIAL_FLAG_DARK)
//                lv_switch_on(obj1, LV_ANIM_OFF);
//  lv_obj_set_event_cb(obj1, color_chg_event_cb);
//    lv_obj_set_pos(obj1, LV_DPX(10), LV_DPX(10));	
//  lv_obj_set_style(obj1, &lv_style_plain_color);
//	lv_obj_set_style_local_value_str(obj1, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, "Dark");
//	lv_obj_set_style_local_value_align(obj1, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, LV_ALIGN_OUT_RIGHT_TOP);
//	lv_obj_set_style_local_value_ofs_x(obj1, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, LV_DPI/35);
//    lv_obj_align(obj1, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);

//    static const char * btns[] = {"Cancel", "Ok", ""};

//    lv_obj_t * m = lv_msgbox_create(lv_scr_act(), NULL);
//    lv_msgbox_add_btns(m, btns);
//    lv_obj_t * btnm = lv_msgbox_get_btnmatrix(m);
//    lv_btnmatrix_set_btn_ctrl(btnm, 1, LV_BTNMATRIX_CTRL_CHECK_STATE);
//    lv_obj_t * obj2;
//    obj2 = lv_obj_create(lv_scr_act(), obj1);
//    lv_obj_set_style(obj2, &lv_style_pretty_color);
//    lv_obj_align(obj2, NULL, LV_ALIGN_CENTER, 0, 0);

}



//开始图片的渐变动画
void start_img_anim(void)
{
	lv_anim_t tanim;

	lv_anim_init(&tanim);

	lv_anim_set_var(&tanim, tstart_img); //动画对象

	lv_anim_set_exec_cb(&tanim, (lv_anim_exec_xcb_t)img_anim_exec_xcb); //渐变函数
	lv_anim_set_time(&tanim, lv_anim_speed_to_time(100, 0, 10000));

	tanim.start = LV_OPA_TRANSP;			//起始值
	tanim.end = LV_OPA_COVER;			//结束值
	tanim.path.cb = lv_anim_path_linear; //线性
	tanim.playback_time = 0;				//回放时间设为0不执行动画回放
//	anim.time = 100000;
	lv_anim_start(&tanim);
}




//删除开始图片
void app_del_start(void)
{
	lv_obj_del(tstart_img); //删除启动图片对象
}


