#ifndef __I2C0_H__
#define __I2C0_H__

#include "gd32f4xx.h"
#include "I2C_config.h"



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
int8_t I2C0_write2(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t offset, uint32_t len);
	
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