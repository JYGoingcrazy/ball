#ifndef __BSP_SERVO_H__
#define __BSP_SERVO_H__

#include "gd32f4xx.h"

void Servo_init();

/**********************************************************
 * @brief ���ö���Ƕ�
 * @param angle: [0, 180]
 * @return 
 **********************************************************/
void Servo_set_angle(float angle);

float Servo_get_angle();

#endif