#include "USART0.h"
#include <stdio.h>
#include <string.h>

#define USART0_DATA_ADDR					(uint32_t)&USART_DATA(USART0)

uint8_t 	g_rx_buffer[RX_BUFFER_LEN]; // \0
uint32_t 	g_rx_cnt = 0;


static void USART0_DMA_TX_config(){
	// ���ݵķ��ͣ����˷����ڴ� -> ����
	
	// ���䣺��̬�ڴ� -> USART0���� DMA1,CH7 �Ӽ�100 -> USART0_TX
	// SRC: memory0_addr
	// DST: periph_addr   
	
  // RCU ---------------------------------------------
  rcu_periph_clock_enable(USART0_TX_DMA_RCU);
	
  // DMA ---------------------------------------------
  /* ����DMA��ͨ�� deinitialize DMA a channel registers */
  dma_deinit(USART0_TX_DMA_PERIPH_CH);

  dma_single_data_parameter_struct init_struct;
  /* ��ʼ���ṹ����� initialize the DMA single data mode parameters struct with the default values */
  dma_single_data_para_struct_init(&init_struct);
  // ���ÿ�������
  init_struct.direction           = DMA_MEMORY_TO_PERIPH;

  // ����Դͷ��ַ
	//  init_struct.memory0_addr        = (uint32_t)dst;
  init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;

  // ����Ŀ���ַ
  init_struct.periph_addr         = USART0_DATA_ADDR;
  init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;

  // ���ݵĸ�������ȣ�ÿ�����ݵ�BIT����
	//  init_struct.number              = sizeof(src) / sizeof(src[0]);
  init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;

  // ѭ��ģʽ���رգ�
  init_struct.circular_mode       = DMA_CIRCULAR_MODE_DISABLE;
  // ���ȼ�
  init_struct.priority            = DMA_PRIORITY_LOW;

  /* ִ��DMA�ĳ�ʼ�� DMA single data mode initialize */
  dma_single_data_mode_init(USART0_TX_DMA_PERIPH_CH, &init_struct);
	
	/* ����ͨ���Ӽ� 100 DMA channel peripheral select */
  dma_channel_subperipheral_select(USART0_TX_DMA_PERIPH_CH, USART0_TX_DMA_CH_SUB);

}

static void USART0_DMA_RX_config(){

	// ���ݵķ��ͣ����˷������� -> �ڴ�
	
	// ���䣺USART0���� -> �̶��ڴ�
	
	// DMA1,CH2 �Ӽ�100 -> USART0_RX
	
	// SRC: periph_addr	��USART0_DATA_ADDR
	// DST: memory0_addr��g_rx_buffer
	
  // RCU ---------------------------------------------
  rcu_periph_clock_enable(USART0_RX_DMA_RCU);
	
  // DMA ---------------------------------------------
  /* ����DMA��ͨ�� deinitialize DMA a channel registers */
  dma_deinit(USART0_RX_DMA_PERIPH_CH);

  dma_single_data_parameter_struct init_struct;
  /* ��ʼ���ṹ����� initialize the DMA single data mode parameters struct with the default values */
  dma_single_data_para_struct_init(&init_struct);
  // ���ÿ�������
  init_struct.direction           = DMA_PERIPH_TO_MEMORY;

  // ����Դͷ��ַ
  init_struct.periph_addr         = USART0_DATA_ADDR;
  init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;

  // ����Ŀ���ַ
	init_struct.memory0_addr        = (uint32_t)g_rx_buffer;
  init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;

  // ���ݵĸ�������ȣ�ÿ�����ݵ�BIT����
	init_struct.number              = RX_BUFFER_LEN;
  init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;

  // ѭ��ģʽ���رգ�
  init_struct.circular_mode       = DMA_CIRCULAR_MODE_DISABLE;
  // ���ȼ�
  init_struct.priority            = DMA_PRIORITY_LOW;

  /* ִ��DMA�ĳ�ʼ�� DMA single data mode initialize */
  dma_single_data_mode_init(USART0_RX_DMA_PERIPH_CH, &init_struct);
	
	/* ����ͨ���Ӽ� 100 DMA channel peripheral select */
  dma_channel_subperipheral_select(USART0_RX_DMA_PERIPH_CH, USART0_RX_DMA_CH_SUB);
	
	/* ������DMA���գ�֪ͨDMAȥUSART0_DATA�������� */
	dma_channel_enable(USART0_RX_DMA_PERIPH_CH);

}

void USART0_init() {
	
#if USART0_TX_DMA_ENABLE
	// ��ʼ��DMA TX ����---------------------------------------
	USART0_DMA_TX_config();
#endif

#if USART0_RX_DMA_ENABLE
	// ��ʼ��DMA RX ����---------------------------------------
	USART0_DMA_RX_config();
#endif
	
  // GPIO ��ʼ�� -----------------------------------------TX PB6, RX PB7 -> AF

  // ����GPIOʱ��
  rcu_periph_clock_enable(USART0_TX_RCU);
  // ����ģʽ���� 
  gpio_mode_set(USART0_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, USART0_TX_PIN);
  // �������ѡ��
  gpio_output_options_set(USART0_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART0_TX_PIN);
  // ����AF���ù���
  gpio_af_set(USART0_TX_PORT, GPIO_AF_7, USART0_TX_PIN);

  rcu_periph_clock_enable(USART0_RX_RCU);
  // ����ģʽ���� 
  gpio_mode_set(USART0_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, USART0_RX_PIN);
  // �������ѡ��
  gpio_output_options_set(USART0_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART0_RX_PIN);
  // ����AF���ù���
  gpio_af_set(USART0_RX_PORT, GPIO_AF_7, USART0_RX_PIN);


  // USART ��ʼ�� ----------------------------------------
  // ����USARTʱ��
  rcu_periph_clock_enable(RCU_USART0);
  // ����
  usart_deinit(USART0);
  // ���ô��ڲ�����������*������λ��У��λ��ֹͣλ
  /* configure usart baud rate value ������ */
  usart_baudrate_set(USART0, USART0_BAUDRATE);
  /* configure usart word length ����λ���� */
  usart_word_length_set(USART0, USART_WL_8BIT);
  /* configure usart parity function У��λ */
  usart_parity_config(USART0, USART_PM_NONE);
  /* configure usart stop bit length ֹͣλ*/
  usart_stop_bit_set(USART0, USART_STB_1BIT);
  /* �ȷ��͸�λ���ǵ�λ LSB��λ�ȷ�*/
  usart_data_first_config(USART0, USART_MSBF_LSB);
		
  /* configure USART transmitter ���÷��͹��� */
  usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
	/* configure USART receiver ���ý��չ��� */
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	
#if USART0_TX_DMA_ENABLE
	/* ����DMA���� configure USART DMA for transmission */
	usart_dma_transmit_config(USART0, USART_TRANSMIT_DMA_ENABLE);
#endif

#if USART0_RX_DMA_ENABLE
	/* ����DMA���� configure USART DMA for reception */
	usart_dma_receive_config(USART0, USART_RECEIVE_DMA_ENABLE);
#endif

	/* �����ж����ȼ� */ 
	nvic_irq_enable(USART0_IRQn, 2, 2);
	
	/* ���ô��ڽ����ж� */
	// ���ջ�������Ϊ���ж�
	usart_interrupt_enable(USART0, USART_INT_RBNE);
	// ���ڿ����ж�
	usart_interrupt_enable(USART0, USART_INT_IDLE);

  /* enable usart ����*/
  usart_enable(USART0);
	
//	printf("USART0_DATA_ADDR: 0x%X\n", USART0_DATA_ADDR); //0x40011004
}

// ����һ���ֽ�����
void USART0_send_data(uint8_t data) {
	// ��ͳ��
  // ͨ��USART0����
  usart_data_transmit(USART0, data);
  // �жϷ��ͻ������Ƿ�Ϊ�� (��Ϊ��RESET��һֱ�ȴ���ֱ��Ϊ��SET)
//	FlagStatus status = usart_flag_get(USART0, USART_FLAG_TBE);
  while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
	
}

// ���Ͷ���ֽ�
void USART0_send_array(uint8_t* data, uint32_t len) {
  // ��ͳ�����㣺1.dataָ�벻Ϊ�� 2. len���Ȳ���0
  while(data && len--) {
    USART0_send_data(*data);
    data++;
  }
	
}


// �����ַ����� ��\0���ַ���������ǣ�
void USART0_send_string(char* data) {
  // ���㣺1.dataָ�벻Ϊ�� 2. Ҫ���͵����ݲ���\0
  while(data && *data) {
    USART0_send_data((uint8_t)(*data));
    data++;
  }
}

#if USART0_TX_DMA_ENABLE

// DMA����һ���ֽ�����
void USART0_dma_send_data(uint8_t data){
	// DMA������send_array
	USART0_send_array(&data, 1);
}
// DMA���Ͷ���ֽ�
void USART0_dma_send_array(uint8_t* data, uint32_t len) {
	// DMA: ��������Դ���������ݸ�������ʼ���� (Ĭ�ϲ�����)
		// �޸�Դ��ַ(�ڴ��ַ)
	dma_memory_address_config(USART0_TX_DMA_PERIPH_CH, DMA_MEMORY_0,(uint32_t) data);
	// �޸İ��˸���
	dma_transfer_number_config(USART0_TX_DMA_PERIPH_CH, len);
	// ֪ͨDMA��ʼ����
	dma_channel_enable(USART0_TX_DMA_PERIPH_CH);
	
	// ��������ʽ�صȴ�dmaִ�����
	while(dma_flag_get(USART0_TX_DMA_PERIPH_CH, DMA_FLAG_FTF) == RESET);
	dma_flag_clear(USART0_TX_DMA_PERIPH_CH, DMA_FLAG_FTF);
}
// DMA�����ַ����� ��\0���ַ���������ǣ�
void USART0_dma_send_string(char* data) {

	USART0_send_array((uint8_t*)data, strlen(data));
}
#endif

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
  USART0_send_data((uint8_t)ch);
  return ch;
}

/*********************************
�жϺ��������ж��¼�������ִ��
1. �����жϺ�����ԭ��flag���кܶ�
2. ��Ҫ�������ĸ���Ǵ������ж�
3. ͨ����Ҫ�ֶ�������
*********************************/
void USART0_IRQHandler(){
	
#if USART0_RX_DMA_ENABLE
	// �����жϣ����һ���ֽڹ�һС�ᣨus���ͻ�ִ�У�
	if(SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE)){
		// ֻ��ͨ���ٴν���һ�����ݣ���������
		usart_data_receive(USART0); // �����һ�����ݣ������������ݲ�����
		/* ��ȡʣ��δ������������� get the number of remaining data to be transferred by the DMA */
		uint32_t remaining_cnt = dma_transfer_number_get(USART0_RX_DMA_PERIPH_CH);
		
//		printf("remaining_cnt: %d\n", remaining_cnt);
		g_rx_cnt = RX_BUFFER_LEN - remaining_cnt;

#if USART0_RECV_CALLBACK
		// ����ַ���������ǣ������ӡ�ַ�������
		g_rx_buffer[g_rx_cnt] = '\0';
		USART0_on_recv(g_rx_buffer, g_rx_cnt);
#endif
		
		// ֹͣ����
		dma_channel_disable(USART0_RX_DMA_PERIPH_CH);
		// ������FTF
		dma_flag_clear(USART0_RX_DMA_PERIPH_CH, DMA_FLAG_FTF);
		// ��������
		dma_channel_enable(USART0_RX_DMA_PERIPH_CH);
	}
#else
	// ���ջ�������Ϊ���жϱ�ǣ��յ��κ�һ���ֽھ�ִ�У�
	if(SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)){
		// ������
		usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
//		printf("<RBNE>\n");
		// ��ȡ�Ĵ����������
		uint8_t data = usart_data_receive(USART0);
//		send_data(data);
		// �����ݻ��浽buffer
		g_rx_buffer[g_rx_cnt++] = data;
		
		// ���⻺�����������ѡ��
		if (g_rx_cnt >= RX_BUFFER_LEN) g_rx_cnt = 0;
	}
	
	// �����жϣ����һ���ֽڹ�һС�ᣨus���ͻ�ִ�У�
	if(SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE)){
//		printf("<IDLE>\n");
//		usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE); // ��Ч��ʽ
		// ֻ��ͨ���ٴν���һ�����ݣ���������
		usart_data_receive(USART0); // �����һ�����ݣ������������ݲ�����
		
#if USART0_RECV_CALLBACK
		// ����ַ���������ǣ������ӡ�ַ�������
		g_rx_buffer[g_rx_cnt] = '\0';
		USART0_on_recv(g_rx_buffer, g_rx_cnt);
#endif
		// �ѻ�����[0, g_rx_cnt)ȫ������Ϊ0����ѡ��
//		memset(g_rx_buffer, 0, g_rx_cnt);		
		g_rx_cnt = 0; // ��������
	}
#endif
}
