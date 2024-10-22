#ifndef __I2C_CONFIG_H__
#define __I2C_CONFIG_H__

#define USE_I2C0		1
#define USE_I2C1		0
#define USE_I2C2		0

// ============================== I2C0 =========================
#if USE_I2C0

#define I2C0_SOFTWARE		 1		// 1.软实现， 0.硬实现

// SCL PB6, PB8
#define I2C0_SCL_RCU			RCU_GPIOB
#define I2C0_SCL_PORT			GPIOB
#define I2C0_SCL_PIN			GPIO_PIN_6

// SDA PB7, PB9
#define I2C0_SDA_RCU			RCU_GPIOB
#define I2C0_SDA_PORT			GPIOB
#define I2C0_SDA_PIN			GPIO_PIN_7

#define I2C0_SPEED		100000U

#endif
// ============================== I2C1 =========================
#if USE_I2C1

#define I2C1_SOFTWARE		1		// 1.软实现， 0.硬实现

// SCL PB6, PB8
#define I2C1_SCL_RCU			RCU_GPIOB
#define I2C1_SCL_PORT			GPIOB
#define I2C1_SCL_PIN			GPIO_PIN_6

// SDA PB7, PB9
#define I2C1_SDA_RCU			RCU_GPIOB
#define I2C1_SDA_PORT			GPIOB
#define I2C1_SDA_PIN			GPIO_PIN_7

#define I2C1_SPEED		400000U

#endif


#endif