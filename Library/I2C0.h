#ifndef __I2C0_H__
#define __I2C0_H__

#include "gd32f4xx.h"

#define I2C0_SOFTWARE		1		// 1.��ʵ�֣� 0.Ӳʵ��

// SCL PB6, PB8
#define SCL_RCU			RCU_GPIOB
#define SCL_PORT		GPIOB
#define SCL_PIN			GPIO_PIN_6

// SDA PB7, PB9
#define SDA_RCU			RCU_GPIOB
#define SDA_PORT		GPIOB
#define SDA_PIN			GPIO_PIN_7

#define I2C_SPEED		400000U

void I2C0_init();

/**********************************************************
 * @brief д����
 * @param addr: �豸��ַ 7bit
 * @param reg:  �Ĵ�����ַ
 * @param data: �ֽ�����
 * @param len: ���ݸ���
 * @return �Ƿ�д�ɹ�0�ɹ�������ʧ��
 **********************************************************/
int8_t I2C0_write(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t len);

/**********************************************************
 * @brief ������
 * @param addr: �豸��ַ 7bit
				������ 0x51 -> д0xA2, ��0xA3
			0x51 -> 0b01010001
			0xA2 ->  0b10100010		����1λ��ĩβ��0
			0xA3 ->  0b10100011		����1λ��ĩβ��1
 * @param reg:  �Ĵ�����ַ
 * @param data: �ֽ�����
 * @param len:  ���ݸ���
 * @return �Ƿ���ɹ�0�ɹ�������ʧ��
 **********************************************************/
int8_t I2C0_read(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t len);

#endif