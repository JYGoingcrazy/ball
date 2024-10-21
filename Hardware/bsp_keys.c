#include "bsp_keys.h"

void Keys_init() {

  // 初始化按键：PC0 上拉输入
  // 时钟初始化
  rcu_periph_clock_enable(RCU_GPIOC);
  // 配置GPIO模式：上拉输入 Ctrl + Space 代码提示
  gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

}

FlagStatus pre_state0 = SET;
FlagStatus pre_state1 = SET;
FlagStatus pre_state2 = SET;
FlagStatus pre_state3 = SET;
void Keys_scan() {

  // PC0 感知引脚电平变化
  FlagStatus cur_state0 = gpio_input_bit_get(GPIOC, GPIO_PIN_0);
  if(cur_state0 != pre_state0) {
    if(cur_state0 == SET) { // 当前高电平，上一次低电平：抬起
      Keys_on_keyup(0);
    } else {		// 当前低电平，上一次高电平：按下
      Keys_on_keydown(0);
    }
    // 更新状态值
    pre_state0 = cur_state0;
  }

  // PC1 感知引脚电平变化
  FlagStatus cur_state1 = gpio_input_bit_get(GPIOC, GPIO_PIN_1);
  if(cur_state1 != pre_state1) {
    if(cur_state1 == SET) { // 当前高电平，上一次低电平：抬起
      Keys_on_keyup(1);
    } else {		// 当前低电平，上一次高电平：按下
      Keys_on_keydown(1);
    }
    // 更新状态值
    pre_state1 = cur_state1;
  }

  // PC2 感知引脚电平变化
  FlagStatus cur_state2 = gpio_input_bit_get(GPIOC, GPIO_PIN_2);
  if(cur_state2 != pre_state2) {
    if(cur_state2 == SET) { // 当前高电平，上一次低电平：抬起
      Keys_on_keyup(2);
    } else {		// 当前低电平，上一次高电平：按下
      Keys_on_keydown(2);
    }
    // 更新状态值
    pre_state2 = cur_state2;
  }

  // PC3 感知引脚电平变化
  FlagStatus cur_state3 = gpio_input_bit_get(GPIOC, GPIO_PIN_3);
  if(cur_state3 != pre_state3) {
    if(cur_state3 == SET) { // 当前高电平，上一次低电平：抬起
      Keys_on_keyup(3);
    } else {		// 当前低电平，上一次高电平：按下
      Keys_on_keydown(3);
    }
    // 更新状态值
    pre_state3 = cur_state3;
  }
}