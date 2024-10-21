#ifndef __EXTI_CONFIG_H__
#define __EXTI_CONFIG_H__

#include "gd32f4xx.h"


#define USE_EXTI0			0
#define USE_EXTI1			0
#define USE_EXTI2			0
#define USE_EXTI3			0
#define USE_EXTI4			0
#define USE_EXTI5			0
#define USE_EXTI6			0
#define USE_EXTI7			0
#define USE_EXTI8			0
#define USE_EXTI9			0
#define USE_EXTI10		0
#define USE_EXTI11		0
#define USE_EXTI12		0
#define USE_EXTI13		0
#define USE_EXTI14		0
#define USE_EXTI15		0

// EXTI0 ----------------------------------------
#if USE_EXTI0
// GPIO ��ز���
#define EXTI0_RCU				RCU_GPIOC
#define EXTI0_PORT			GPIOC
#define EXTI0_PUPD			GPIO_PUPD_PULLUP
// EXTI & NVIC ��ز���
#define EXTI0_SOURCE		EXTI_SOURCE_GPIOC
#define EXTI0_TRIG_TYPE	EXTI_TRIG_FALLING		// ���ı��� EXTI_TRIG_RISING, EXTI_TRIG_FALLING, EXTI_TRIG_BOTH
#define EXTI0_PRIORITY	2, 2
#endif

// EXTI1 ----------------------------------------

#if USE_EXTI1
// GPIO ��ز���
#define EXTI1_RCU				RCU_GPIOC
#define EXTI1_PORT			GPIOC
#define EXTI1_PUPD			GPIO_PUPD_PULLUP
// EXTI & NVIC ��ز���
#define EXTI1_SOURCE		EXTI_SOURCE_GPIOC
#define EXTI1_TRIG_TYPE	EXTI_TRIG_BOTH		// ���ı���
#define EXTI1_PRIORITY	2, 2
#endif

// EXTI1 ----------------------------------------

#if USE_EXTI5

#define EXTI5_SOFT			1 // 1. ����ʵ�� 0.Ӳ+��ʵ��

#if EXTI5_SOFT == 0
// GPIO ��ز���
#define EXTI5_RCU				RCU_GPIOB
#define EXTI5_PORT			GPIOB
#define EXTI5_PUPD			GPIO_PUPD_PULLUP
// EXTI & NVIC ��ز���
#define EXTI5_SOURCE		EXTI_SOURCE_GPIOB
#define EXTI5_TRIG_TYPE	EXTI_TRIG_FALLING		// ���ı���
#endif

#define EXTI5_PRIORITY	2, 2

#endif


#endif