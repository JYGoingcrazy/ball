#include "bsp_keys.h"

void Keys_init() {

  // ��ʼ��������PC0 ��������
  // ʱ�ӳ�ʼ��
  rcu_periph_clock_enable(RCU_GPIOC);
  // ����GPIOģʽ���������� Ctrl + Space ������ʾ
  gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

}

FlagStatus pre_state0 = SET;
FlagStatus pre_state1 = SET;
FlagStatus pre_state2 = SET;
FlagStatus pre_state3 = SET;
void Keys_scan() {

  // PC0 ��֪���ŵ�ƽ�仯
  FlagStatus cur_state0 = gpio_input_bit_get(GPIOC, GPIO_PIN_0);
  if(cur_state0 != pre_state0) {
    if(cur_state0 == SET) { // ��ǰ�ߵ�ƽ����һ�ε͵�ƽ��̧��
      Keys_on_keyup(0);
    } else {		// ��ǰ�͵�ƽ����һ�θߵ�ƽ������
      Keys_on_keydown(0);
    }
    // ����״ֵ̬
    pre_state0 = cur_state0;
  }

  // PC1 ��֪���ŵ�ƽ�仯
  FlagStatus cur_state1 = gpio_input_bit_get(GPIOC, GPIO_PIN_1);
  if(cur_state1 != pre_state1) {
    if(cur_state1 == SET) { // ��ǰ�ߵ�ƽ����һ�ε͵�ƽ��̧��
      Keys_on_keyup(1);
    } else {		// ��ǰ�͵�ƽ����һ�θߵ�ƽ������
      Keys_on_keydown(1);
    }
    // ����״ֵ̬
    pre_state1 = cur_state1;
  }

  // PC2 ��֪���ŵ�ƽ�仯
  FlagStatus cur_state2 = gpio_input_bit_get(GPIOC, GPIO_PIN_2);
  if(cur_state2 != pre_state2) {
    if(cur_state2 == SET) { // ��ǰ�ߵ�ƽ����һ�ε͵�ƽ��̧��
      Keys_on_keyup(2);
    } else {		// ��ǰ�͵�ƽ����һ�θߵ�ƽ������
      Keys_on_keydown(2);
    }
    // ����״ֵ̬
    pre_state2 = cur_state2;
  }

  // PC3 ��֪���ŵ�ƽ�仯
  FlagStatus cur_state3 = gpio_input_bit_get(GPIOC, GPIO_PIN_3);
  if(cur_state3 != pre_state3) {
    if(cur_state3 == SET) { // ��ǰ�ߵ�ƽ����һ�ε͵�ƽ��̧��
      Keys_on_keyup(3);
    } else {		// ��ǰ�͵�ƽ����һ�θߵ�ƽ������
      Keys_on_keydown(3);
    }
    // ����״ֵ̬
    pre_state3 = cur_state3;
  }
}