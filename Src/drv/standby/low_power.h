#ifndef __LOW_POWER_H
#define __LOW_POWER_H


enum _keyup_opr_e{

	WKUP_STANDBY = 0,
	WKUP_RETURN_HOMEPAGE,

};



#define WKUP_KD HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //PA0 检测是否外部WK_UP按键按下

#endif

