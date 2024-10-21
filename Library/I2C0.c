#include "I2C0.h"

#if I2C0_SOFTWARE

#include "systick.h"

#define SDA(bit) 	gpio_bit_write(SDA_PORT, SDA_PIN, bit ? SET : RESET)
#define SCL(bit) 	gpio_bit_write(SCL_PORT, SCL_PIN, bit ? SET : RESET)

#if I2C_SPEED < 200000U
#define DELAY()  	delay_1us(5)
#else
#define DELAY()  	delay_1us(1)
#endif

// ����SDAΪ����ģʽ
#define SDA_IN()	gpio_mode_set(SDA_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, SDA_PIN);
// ����SDAΪ���ģʽ
#define SDA_OUT()	gpio_mode_set(SDA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, SDA_PIN);
// ��ȡSDA��ƽ
#define SDA_STATE() gpio_input_bit_get(SDA_PORT, SDA_PIN)

// SCL-PB6ʱ����,��©
// SDA-PB7������,��©
void I2C0_init(){
	// ��ʵ�֣�CPU����GPIO
	// Ӳʵ�֣������Ĵ�����AF���ã�Ӳ����·ʵ�������շ�
	
	// ��׼ģʽ 100Kbits/s�� ����ģʽ 400Kbits/s
	// 100Kbits/s -> 100 000 bit / 1 000 000 us  -> 1bit/10us => 5us
	// 400Kbits/s -> 400 000 bit / 1 000 000 us  -> 4bit/10us => 1.+us
//	__NOP();
	// SCL-PB6 (Ĭ�ϵ͵�ƽ)
	rcu_periph_clock_enable(SCL_RCU);
	gpio_mode_set(SCL_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, SCL_PIN);
	gpio_output_options_set(SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_MAX, SCL_PIN);
	
	// SDA-PB7 (Ĭ�ϵ͵�ƽ)
	rcu_periph_clock_enable(SDA_RCU);
	gpio_mode_set(SDA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, SDA_PIN);
	gpio_output_options_set(SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_MAX, SDA_PIN);
	
}

static void start();
static void send(uint8_t data);
static uint8_t recv();
static void send_ack();
static void send_nack();

static int8_t wait_ack();
static void stop();

/**********************************************************
 * @brief д����
 * @param addr: �豸��ַ 7bit
 * @param reg:  �Ĵ�����ַ
 * @param data: �ֽ�����
 * @param len: ���ݸ���
 * @return �Ƿ�д�ɹ�0�ɹ�������ʧ��
 **********************************************************/
int8_t I2C0_write(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t len){
	
	// ��ʼ
	start();
	
	// �����豸��ַ���豸д��ַ��
	send((addr << 1) | 0);
	// �ȴ���Ӧ
	if(wait_ack() != 0) return 1;
	
	// ���ͼĴ�����ַ
	send(reg);
	// �ȴ���Ӧ
	if(wait_ack() != 0) return 2;
	
	// ѭ��(����len)
	for(uint32_t i = 0; i < len; i++){
		// д��1�ֽ�����
		send(data[i]);
		// �ȴ���Ӧ
		if(wait_ack()) return 3;
	}
	
	// ֹͣ
	stop();
	
	return 0;
}

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
int8_t I2C0_read(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t len){
	uint8_t addr_write = (addr << 1) | 0;	// 8bit�豸��д��ַ 
	uint8_t addr_read  = (addr << 1) | 1;	// 8bit�豸�Ķ���ַ 
	
	// ��ʼ ---------------------------------- ͨ�ÿ�ͷ
	start();
	
	// �����豸д��ַ
	send(addr_write);
	// �ȴ���Ӧ
	if(wait_ack()) return 1;
	
	// ���ͼĴ�����ַ��Ҫ���ļĴ����׵�ַ��
	send(reg);
	// �ȴ���Ӧ
	if(wait_ack()) return 2;
	
	// ��ʼ ---------------------------------- ��ȡ��ͷ
	start();
	
	// �����豸����ַ
	send(addr_read);
	// �ȴ���Ӧ
	if(wait_ack()) return 3;
	
	/************** ѭ���������� **************/
	for(uint32_t i = 0;i < len; i++){ // һ��ѭ������һ���ֽ�
		// ����һ���ֽ�
		data[i] = recv();
		
		if (i != len - 1){
			// ������Ӧ (�����һ��)
			send_ack();			
		}else {
			// ���Ϳ���Ӧ
			send_nack();
		}
		
	}
	/*****************************************/
	
	// ֹͣ
	stop();
	return 0;
}

static void start(){
	
	// ��SDA��SCL����
	SDA(1);
	DELAY();
	SCL(1);
	DELAY();
	
	// ��SDA��SCL��������
	SDA(0);
	DELAY();
	SCL(0);
	DELAY();
}


static void send(uint8_t data){
	// ����8bit���ȷ���λ
	// 1010 1000
	// 0101 0000
	// 101 00000
	
	for(uint8_t i = 0; i < 8; i++){
		// ȡ�����λ
		if(data & 0x80){
			SDA(1); // �ߵ�ƽ
		}else {
			SDA(0); // �͵�ƽ
		}
		DELAY();
		
		// ������Ч��
		SCL(1);
		DELAY();
		SCL(0);
		DELAY();
		
		// ���������ƶ�һλ
		data <<= 1;
	}
}

uint8_t recv(){
	// �ͷ�SDA����Ȩ����������ģʽ
	SDA_IN();
	uint8_t cnt = 8; 			// 1byte�ֽڣ� 8bit����
	uint8_t data = 0x00;	// ���ֽ�������������
	
	while(cnt--){					// ����һ��bit(���յ����Ǹ�λ)
		// SCL����
		SCL(0); // �ȴ����豸׼������
		DELAY();
		
		SCL(1); // ����������Ч��
		
		// 0000 0000 -> 1110 1011
		// 0000 0010
		
		// 0000 0001  8
		// 0000 0011  7
		// 0000 0111  6
		// 0000 1110  5
		// 0001 1101  4
		// 0011 1010  3
		// 0111 0101  2
		// 1110 1011  1
		
		// ��������
		data <<= 1;
		// �����λ����Ϊ1
//		if(SDA_STATE()) data |= 0x01;
		if(SDA_STATE()) data++;
		
		DELAY();
	}
	// ���һ�ε͵�ƽ��������
	SCL(0);
	
	return data;
}

static void send_ack(){
	// ������ȡSDA����Ȩ
	SDA_OUT();
	
	// ����SDA
	SDA(0);
	DELAY();
	
	// ����SCL
	SCL(1);
	DELAY();
	
	// ����SCL (���豸��ȡSDA�ϵĵ�ƽ)
	SCL(0);
	DELAY();
}
static void send_nack(){
	// ������ȡSDA����Ȩ
	SDA_OUT();
	
	// ����SDA
	SDA(1);
	DELAY();
	
	// ����SCL
	SCL(1);
	DELAY();
	
	// ����SCL (���豸��ȡSDA�ϵĵ�ƽ)
	SCL(0);
	DELAY();

}

static int8_t wait_ack(){
	// ����SDA, �ȴ����豸����
	SDA(1);
	DELAY();
	
	// ����SCL��ͬʱ�ͷ�SDAȨ�ޣ��������ģʽ��
	SCL(1);
	SDA_IN();
	DELAY();
	
	if(SDA_STATE() == RESET){ // �͵�ƽ
		// ���豸������SDA��Ӧ��ɹ�
		SCL(0);
		// ��ȡ����Ȩ��������ģʽ
		SDA_OUT();
	}else {
		// ��ȡ����Ȩ��������ģʽ
		SDA_OUT();
		// ����Ӧ��
		stop();
		return 1;
	}
	
	return 0;
}

static void stop(){
	// ��ȡ����Ȩ��������ģʽ
	SDA_OUT();
	
	// ��SCL��SDA����
	SCL(0);
	SDA(0);
	DELAY();
	
	SCL(1);
	DELAY();
	SDA(1);
	DELAY();
}

#else

#define I2C_RCU			RCU_I2C0
#define i2cx    		I2C0

// SCL-PB6ʱ����,��©
// SDA-PB7������,��©
void I2C0_init(){
	// ��ʵ�֣�CPU����GPIO
	// Ӳʵ�֣������Ĵ�����AF���ã�Ӳ����·ʵ�������շ�
	
	// ��׼ģʽ 100Kbits/s�� ����ģʽ 400Kbits/s
	// 100Kbits/s -> 100 000 bit / 1 000 000 us  -> 1bit/10us => 5us
	// 400Kbits/s -> 400 000 bit / 1 000 000 us  -> 4bit/10us => 1.+us
	
	// GPIO --------------------------------------------
	// SCL-PB6 (Ĭ�ϵ͵�ƽ)
	rcu_periph_clock_enable(SCL_RCU);
	gpio_mode_set(SCL_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, SCL_PIN);
	gpio_output_options_set(SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_MAX, SCL_PIN);
	gpio_af_set(SCL_PORT, GPIO_AF_4, SCL_PIN);
	
	// SDA-PB7 (Ĭ�ϵ͵�ƽ)
	rcu_periph_clock_enable(SDA_RCU);
	gpio_mode_set(SDA_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, SDA_PIN);
	gpio_output_options_set(SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_MAX, SDA_PIN);
	gpio_af_set(SDA_PORT, GPIO_AF_4, SDA_PIN);
	
	// I2C ---------------------------------------------
	/* rcu */
	rcu_periph_clock_enable(I2C_RCU);
	/* reset I2C */
	i2c_deinit(i2cx);
	/* ����I2C���� configure I2C0 clock */
	i2c_clock_config(i2cx, I2C_SPEED, I2C_DTCY_2);
	/* ����I2C��ַ��ʽ���Լ��ĵ�ַ configure I2C0 address */
	i2c_mode_addr_config(i2cx, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x00);
	/* ����I2C enable I2C0 */
	i2c_enable(i2cx);
	/* �յ�����ʱ�������Զ�ACK�ظ����� enable acknowledge */
	i2c_ack_config(i2cx, I2C_ACK_ENABLE);
}
#define	TIMEOUT	50000
static uint8_t I2C_wait(uint32_t flag) {
    uint16_t cnt = 0;

		// �����RESETһֱ�ȣ�ֱ��SET
    while(!i2c_flag_get(i2cx, flag)) {
        cnt++;
        if(cnt > TIMEOUT) return 1;
    }
    return 0;
}

static uint8_t I2C_waitn(uint32_t flag) {
    uint16_t cnt = 0;

		// �����SETһֱ�ȣ�ֱ��RESET
    while(i2c_flag_get(i2cx, flag)) {
        cnt++;
        if(cnt > TIMEOUT) return 1;
    }
		return 0; // �ɹ�
}
/**********************************************************
 * @brief д����
 * @param addr: �豸��ַ 7bit
 * @param reg:  �Ĵ�����ַ
 * @param data: �ֽ�����
 * @param len: ���ݸ���
 * @return �Ƿ�д�ɹ�0�ɹ�������ʧ��
 **********************************************************/
int8_t I2C0_write(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t len){
	uint8_t write_addr = addr << 1;
	
	/************* start ***********************/
	// �ȴ�I2C����
	if(I2C_waitn(I2C_FLAG_I2CBSY)) return 1;
	// start
	i2c_start_on_bus(i2cx);
	// �ȴ�I2C���豸�ɹ�������ʼ�ź�
	if(I2C_wait(I2C_FLAG_SBSEND)) return 2;
	
	/************* device address **************/
	// �����豸��ַ
	i2c_master_addressing(i2cx, write_addr, I2C_TRANSMITTER);
	// �ȴ���ַ�������
	if(I2C_wait(I2C_FLAG_ADDSEND)) return 3;
	i2c_flag_clear(i2cx, I2C_FLAG_ADDSEND);
	
	/************ register address ************/
	// �Ĵ�����ַ
	// �ȴ��������ݻ�����Ϊ��
	if(I2C_wait(I2C_FLAG_TBE)) return 4;
	// ��������
	i2c_data_transmit(i2cx, reg);
	// �ȴ����ݷ������
	if(I2C_wait(I2C_FLAG_BTC)) return 5;
	
	/***************** data ******************/
	// ��������
	for(uint32_t i = 0; i < len; i++) {
			uint32_t d = data[i];
			// �ȴ��������ݻ�����Ϊ��
			if(I2C_wait(I2C_FLAG_TBE)) return 6;
			// ��������
			i2c_data_transmit(i2cx, d);
			// �ȴ����ݷ������
			if(I2C_wait(I2C_FLAG_BTC)) return 7;
	}
	/***************** stop ********************/
	// stop
	i2c_stop_on_bus(i2cx);

	if(I2C_waitn(I2C_CTL0(i2cx)&I2C_CTL0_STOP)) return 8;
	
	return 0;
}

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
int8_t I2C0_read(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t len){
	uint8_t write_addr = addr << 1;
	uint8_t read_addr  = (addr << 1) + 1;
	
	// �ȴ�I2C����
	if(I2C_waitn(I2C_FLAG_I2CBSY)) return 1;
	
	/************* start ***********************/
	// ���������ź�
	i2c_start_on_bus(i2cx);
	// �ȴ�I2C���豸�ɹ�������ʼ�ź�
	if(I2C_wait(I2C_FLAG_SBSEND)) return 2;
	
	/************* device address **************/
	// ���ʹ��豸��ַ
	i2c_master_addressing(i2cx, write_addr, I2C_TRANSMITTER);

	if(I2C_wait(I2C_FLAG_ADDSEND)) return 3;
	i2c_flag_clear(i2cx, I2C_FLAG_ADDSEND);
	
	/********** register address **************/
	// �ȴ����ͻ�����	
	if(I2C_wait(I2C_FLAG_TBE)) return 4;
	// ���ͼĴ�����ַ
	i2c_data_transmit(i2cx, reg);
	// �ȴ������������	
	if(I2C_wait(I2C_FLAG_BTC)) return 5;
	
	/************* start ***********************/
	// �����������ź�
	i2c_start_on_bus(i2cx);
	if(I2C_wait(I2C_FLAG_SBSEND)) return 7;
	
	/************* device address **************/
	// ���ʹ��豸��ַ
	i2c_master_addressing(i2cx, read_addr, I2C_RECEIVER);
	if(I2C_wait(I2C_FLAG_ADDSEND)) return 8;
	i2c_flag_clear(i2cx, I2C_FLAG_ADDSEND);
	
	/************* data **************/
	// ack
	i2c_ack_config(i2cx, I2C_ACK_ENABLE);
	// ����λ�ã�����һ�����ݺ��Զ�����ACK
	i2c_ackpos_config(i2cx, I2C_ACKPOS_CURRENT);
	// ȷ��ACK������
	if(I2C_wait(I2C_CTL0(i2cx) & I2C_CTL0_ACKEN)) return 11;

	// ��ȡ����
	for (uint32_t i = 0; i < len; i++) {
			if (i == len - 1) {
					// �ڶ�ȡ���һ���ֽ�֮ǰ������ACK������Ϊ�Զ� NACK
					i2c_ack_config(i2cx, I2C_ACK_DISABLE);
			}

			// �ȴ����ջ�������Ϊ��
			if(I2C_wait(I2C_FLAG_RBNE)) return 10;
			
			// �������ݲ��ŵ�data������
			data[i] = i2c_data_receive(i2cx);
	}
	
	/***************** stop ********************/
	i2c_stop_on_bus(i2cx);
	if(I2C_waitn(I2C_CTL0(i2cx)&I2C_CTL0_STOP)) return 12;
	
	return 0;
}

#endif