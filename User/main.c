#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include <string.h>
#include "main.h"

#include "USART0.h"
#include "TIMER.h"


#include "bsp_servo.h"
#include "bsp_keys.h"


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

    while(1) {
        Keys_scan();
        delay_1ms(10);

    }
}