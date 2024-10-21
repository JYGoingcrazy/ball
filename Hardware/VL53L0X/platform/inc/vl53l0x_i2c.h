#ifndef __VL53L0_I2C_H
#define __VL53L0_I2C_H

#include "gd32f4xx.h"

#ifndef u8
#define u8 uint8_t
#endif
#ifndef u16
#define u16 uint16_t
#endif
#ifndef u32
#define u32 uint32_t
#endif

#define RCU_SCL RCU_GPIOB
#define PORT_SCL GPIOB
#define GPIO_SCL GPIO_PIN_6

//�˿���ֲ
#define RCU_SDA RCU_GPIOB
#define PORT_SDA GPIOB
#define GPIO_SDA GPIO_PIN_7


//����SDA���ģʽ
#define SDA_OUT()	gpio_mode_set(PORT_SDA,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_SDA)
//����SDA����ģʽ
#define SDA_IN()	gpio_mode_set(PORT_SDA,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,GPIO_SDA)
//��ȡSDA���ŵĵ�ƽ�仯
#define SDA_GET()	gpio_input_bit_get(PORT_SDA,GPIO_SDA)
//SDA��SCL���
#define SDA(x)          gpio_bit_write(PORT_SDA,GPIO_SDA, (x?SET:RESET))
#define SCL(x)          gpio_bit_write(PORT_SCL,GPIO_SCL, (x?SET:RESET))

//״̬
#define STATUS_OK       0x00
#define STATUS_FAIL     0x01

//IIC��������
void VL53L0X_i2c_init(void);//��ʼ��IIC��IO��

u8 VL53L0X_write_byte(u8 address,u8 index,u8 data);              //IICдһ��8λ����
u8 VL53L0X_write_word(u8 address,u8 index,u16 data);             //IICдһ��16λ����
u8 VL53L0X_write_dword(u8 address,u8 index,u32 data);            //IICдһ��32λ����
u8 VL53L0X_write_multi(u8 address, u8 index,u8 *pdata,u16 count);//IIC����д

u8 VL53L0X_read_byte(u8 address,u8 index,u8 *pdata);             //IIC��һ��8λ����
u8 VL53L0X_read_word(u8 address,u8 index,u16 *pdata);            //IIC��һ��16λ����
u8 VL53L0X_read_dword(u8 address,u8 index,u32 *pdata);           //IIC��һ��32λ����
u8 VL53L0X_read_multi(u8 address,u8 index,u8 *pdata,u16 count);  //IIC������


#endif 


