#include "gui_app_start.h"
#include "lv_port_disp.h"
#include "gui_app_anim.h"


LV_IMG_DECLARE(start);

static lv_obj_t *start_img; //���忪ʼ����ͼƬimg

lv_obj_t *scr;
//������ʼͼƬ
void app_show_start(void)
{

	scr = lv_scr_act();																		  //��ȡ��ǰ��Ծ����Ļ
	lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE); //���ñ�����ɫΪ��ɫ

	start_img = lv_img_create(scr, NULL);

	lv_obj_set_size(start_img, 134, 240); //���ô�С

	lv_obj_align(start_img, scr, LV_ALIGN_CENTER, 0, 0); //����ͼƬ�������

	lv_img_set_src(start_img, &start); //Ȼ����ʾ��ͼƬ

	lv_obj_set_style_local_image_opa(start_img, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_80); //ͼƬ͸��
}


//��ʼͼƬ�Ľ��䶯��
void start_img_anim(void)
{
	lv_anim_t anim;

	lv_anim_init(&anim);

	lv_anim_set_var(&anim, start_img); //��������

	lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)img_anim_exec_xcb); //���亯��
	lv_anim_set_time(&anim, lv_anim_speed_to_time(100, 0, 80));

	anim.start = LV_OPA_TRANSP;			//��ʼֵ
	anim.end = LV_OPA_COVER;			//����ֵ
	anim.path.cb = lv_anim_path_linear; //����
	anim.playback_time = 0;				//�ط�ʱ����Ϊ0��ִ�ж����ط�

	lv_anim_start(&anim);
}
