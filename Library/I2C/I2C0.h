#ifndef __I2C0_H__
#define __I2C0_H__

#include "gd32f4xx.h"
#include "I2C_config.h"



void I2C0_init();

/**********************************************************
 * @brief 写数据
 * @param addr: 设备地址 7bit
 * @param reg:  寄存器地址
 * @param data: 字节数组
 * @param len: 数据个数
 * @return 是否写成功0成功，其他失败
 **********************************************************/
int8_t I2C0_write(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t len);
int8_t I2C0_write2(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t offset, uint32_t len);
	
/**********************************************************
 * @brief 读数据
 * @param addr: 设备地址 7bit
				举例： 0x51 -> 写0xA2, 读0xA3
			0x51 -> 0b01010001
			0xA2 ->  0b10100010		左移1位，末尾补0
			0xA3 ->  0b10100011		左移1位，末尾补1
 * @param reg:  寄存器地址
 * @param data: 字节数组
 * @param len:  数据个数
 * @return 是否读成功0成功，其他失败
 **********************************************************/
int8_t I2C0_read(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t len);

#endif