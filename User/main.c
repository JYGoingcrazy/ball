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
任务目标：

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
extern VL53L0X_Dev_t vl53l0x_dev;//设备I2C数据参数
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
	uint8_t mode = 0;//0：默认;1:高精度;2:长距离;3:高速
  VL53L0X_Error Status=VL53L0X_ERROR_NONE;//工作状态

  nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);   // 优先级分组
  printf("start\r\n");

  while(vl53l0x_init(&vl53l0x_dev))//vl53l0x初始化
  {
    printf("VL53L0X Error!!!\n\r");
    delay_1ms(100);
  }
  printf("VL53L0X OK\r\n");

  while(vl53l0x_set_mode(&vl53l0x_dev,mode))//配置测量模式
  {
    printf("Mode Set Error\r\n");
    delay_1ms(100);
  }

    while(1) {
        Keys_scan();
        delay_1ms(10);
			
				//执行单次测距并获取测距测量数据
		Status = VL53L0X_PerformSingleRangingMeasurement(&vl53l0x_dev, &vl53l0x_data);
		if(Status == VL53L0X_ERROR_NONE){
			// 测量结果没有错误
			printf("d: %4imm\r\n",vl53l0x_data.RangeMilliMeter);//打印测量距离
		}else {
			printf("VL53L0X error: %d\n", Status);
		}
		delay_1ms(100);

    }
}