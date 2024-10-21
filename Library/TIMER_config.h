#ifndef __TIMER_CONFIG_H__
#define __TIMER_CONFIG_H__

#include "gd32f4xx.h"

#define USE_TIMER_0     0
#define USE_TIMER_1     0
#define USE_TIMER_2     0
#define USE_TIMER_3     0
#define USE_TIMER_4     0
#define USE_TIMER_5     0
#define USE_TIMER_6     0
#define USE_TIMER_7     0
#define USE_TIMER_8     1
#define USE_TIMER_9     0
#define USE_TIMER_10    0
#define USE_TIMER_11    0
#define USE_TIMER_12    0
#define USE_TIMER_13    0

#if USE_TIMER_0
// ����TIMER0������£�����Ҫ�������²���
#define TM0_PRESCALER   10
#define TM0_FREQ        1000
#define TM0_PERIOD      (SystemCoreClock / TM0_PRESCALER / TM0_FREQ)

// TIMER0CHx��Ӧ���������в���
// PA8, PE9
#define TM0_CH0        RCU_GPIOA, GPIOA, GPIO_PIN_8, GPIO_AF_1

// PA7, PB13, PE8
#define TM0_CH0_ON     RCU_GPIOA, GPIOA, GPIO_PIN_7, GPIO_AF_1
// PB0, PB14, PE1, PE10
#define TM0_CH1_ON     RCU_GPIOB, GPIOB, GPIO_PIN_0, GPIO_AF_1

#endif

#if USE_TIMER_1
// ����TIMER1������£�����Ҫ�������²���
#define TM1_PRESCALER   10
#define TM1_FREQ        1000
#define TM1_PERIOD      (SystemCoreClock / TM1_PRESCALER / TM1_FREQ)

// TIMER1CHx��Ӧ���������в���
// #define TM1_CH0        RCU_GPIOA, GPIOA, GPIO_PIN_8, GPIO_AF_2
#define TM1_CH1        RCU_GPIOB, GPIOB, GPIO_PIN_9, GPIO_AF_1

#endif

#if USE_TIMER_3
// ����TIMER3������£�����Ҫ�������²���
#define TM3_PRESCALER   10
#define TM3_FREQ        1000
#define TM3_PERIOD      (SystemCoreClock / TM3_PRESCALER / TM3_FREQ)

// TIMER3CHx��Ӧ���������в���
#define TM3_CH0        RCU_GPIOD, GPIOD, GPIO_PIN_12, GPIO_AF_2
#define TM3_CH1        RCU_GPIOD, GPIOD, GPIO_PIN_13, GPIO_AF_2
#define TM3_CH2        RCU_GPIOD, GPIOD, GPIO_PIN_14, GPIO_AF_2
#define TM3_CH3        RCU_GPIOD, GPIOD, GPIO_PIN_15, GPIO_AF_2
#endif

#if USE_TIMER_8
// ����TIMER8������£�����Ҫ�������²���
#define TM8_PRESCALER   100
#define TM8_FREQ        50
// ��ĸ 168 000 000 / 5000 >= 2567
#define TM8_PERIOD      (SystemCoreClock / TM8_PRESCALER / TM8_FREQ)

// TIMER8CHx��Ӧ���������в���
#define TM8_CH0        RCU_GPIOE, GPIOE, GPIO_PIN_5, GPIO_AF_3
#endif



#endif