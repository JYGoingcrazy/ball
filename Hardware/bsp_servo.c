#include "bsp_servo.h"
#include "TIMER.h"


void Servo_init(){
	
}

static float current_angle = 0;

#define SCALE_RADIO	 0.95
/**********************************************************
 * @brief ���ö���Ƕ�
 * @param angle: [0, 180] -> [500/20000,  2500/20000] -> * 100
	// [0, 180] -> [500, 2500] / 20000
	// [0, 180] -> [0.0, 1.0] -> [0, 2000] -> [500, 2500]
	// [500, 2500] / 20000 = [2.5%, 12.5%]
 * @return 
 **********************************************************/
void Servo_set_angle(float angle){
	// �޶���[0, 180]
	if(angle < 0) angle = 0;
	else if (angle > 180) angle = 180;
	
	// ������ĽǶȣ�ת�ɶ����Ҫռ�ձ���ֵ
	current_angle = angle;
	
	float duty = (500 + (angle / 180) * (2500 - 500)) / 20000;
	
	duty *= SCALE_RADIO;
//	float duty = 0.025 + angle / 1800;
	
	TIMER_channel_update(TIMER8, TIMER_CH_0, duty * 100);
}

float Servo_get_angle(){
	return current_angle;
}