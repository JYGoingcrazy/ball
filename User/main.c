#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include <string.h>
#include "main.h"

#include "USART0.h"
#include "TIMER.h"


#include "bsp_servo.h"
#include "bsp_keys.h"
#include "bsp_VL53L0X.h"


/*******************************************
����Ŀ�꣺

*******************************************/
void USART0_on_recv(uint8_t* buffer, uint32_t len) {
    printf("recv[%d]-> %s\n", len, buffer);

}
void Keys_on_keyup(uint8_t index ) {
}

void Keys_on_keydown(uint8_t index ) {
    float cur_angle = Servo_get_angle();
    switch(index) {
    case 0:
        cur_angle -=30;
        break;
    case 1:
        cur_angle += 30;
        break;
    case 2:
        break;
    case 3:
        break;
    default:
        break;

    }
    Servo_set_angle(cur_angle);
    printf("update angle:%.1f\n",Servo_get_angle());

}
extern VL53L0X_Dev_t vl53l0x_dev;//�豸I2C���ݲ���
int main(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    systick_config();

    USART0_init();
    TIMER_init();

    Keys_init();
    Servo_init();
delay_1ms(100);

    printf("init\n");
    Servo_set_angle(90);
	
		
//	Servo_set_angle(90);
	uint8_t mode = 0;//0��Ĭ��;1:�߾���;2:������;3:����
  VL53L0X_Error Status=VL53L0X_ERROR_NONE;//����״̬

  nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);   // ���ȼ�����
  printf("start\r\n");

  while(vl53l0x_init(&vl53l0x_dev))//vl53l0x��ʼ��
  {
    printf("VL53L0X Error!!!\n\r");
    delay_1ms(100);
  }
  printf("VL53L0X OK\r\n");

  while(vl53l0x_set_mode(&vl53l0x_dev,mode))//���ò���ģʽ
  {
    printf("Mode Set Error\r\n");
    delay_1ms(100);
  }

    while(1) {
        Keys_scan();
        delay_1ms(10);
			
				//ִ�е��β�ಢ��ȡ����������
		Status = VL53L0X_PerformSingleRangingMeasurement(&vl53l0x_dev, &vl53l0x_data);
		if(Status == VL53L0X_ERROR_NONE){
			// �������û�д���
			printf("d: %4imm\r\n",vl53l0x_data.RangeMilliMeter);//��ӡ��������
		}else {
			printf("VL53L0X error: %d\n", Status);
		}
		delay_1ms(100);

    }
}