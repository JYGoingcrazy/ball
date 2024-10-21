#ifndef __USART0_H__
#define __USART0_H__

#include "gd32f4xx.h"
#include "USART_config.h"

void USART0_init();
// ����һ���ֽ�����
void USART0_send_data(uint8_t data);
// ���Ͷ���ֽ�
void USART0_send_array(uint8_t* data, uint32_t len);
// �����ַ����� ��\0���ַ���������ǣ�
void USART0_send_string(char* data);

#if USART0_TX_DMA_ENABLE

// ����һ���ֽ�����
void USART0_dma_send_data(uint8_t data);
// ���Ͷ���ֽ�
void USART0_dma_send_array(uint8_t* data, uint32_t len);
// �����ַ����� ��\0���ַ���������ǣ�
void USART0_dma_send_string(char* data);

#endif 

extern void USART0_on_recv(uint8_t* buffer, uint32_t len);

#endif