#ifndef __TIMER_H__
#define __TIMER_H__

#include "gd32f4xx.h"
#include "TIMER_config.h"

/**
 * @brief TIMER ��ʼ��
 * 
 */
void TIMER_init();

/**
 * @brief ����ָ��Timer��channel��ռ�ձ�
 * 
 * @param timer_periph  TIMERx(x=0,1,2,3,4,5,6,7,8,9,10,11,12,13)
 * @param channel   TIMER_CH_0, TIMER_CH_1, TIMER_CH_2, TIMER_CH_3
 * @param duty  ռ�ձ� [0, 100]
 */
void TIMER_channel_update(uint32_t timer_periph, uint16_t channel, float duty);

/**
 * @brief ����ָ��Timer������
 * 
 * @param timer_periph  TIMERx(x=0,1,2,3,4,5,6,7,8,9,10,11,12,13)
 * @param prescaler     ��Ƶϵ�� [1, 65536]
 * @param period        ���� [1, 65536]
 */
void TIMER_period_update(uint32_t timer_periph, uint16_t prescaler, uint32_t period);

#endif