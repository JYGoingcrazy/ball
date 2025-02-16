#include "vl53l0x_i2c.h"
//#include "board.h"
#include "systick.h"



//VL53L0X I2C初始化
void VL53L0X_i2c_init(void)
{


  /* 使能时钟 */
    rcu_periph_clock_enable(RCU_SCL);
	rcu_periph_clock_enable(RCU_SDA);
    
	/* 配置SCL为输出模式 */
	gpio_mode_set(PORT_SCL,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_SCL);
	/* 配置为推挽输出 50MHZ */
	gpio_output_options_set(PORT_SCL,GPIO_OTYPE_OD,GPIO_OSPEED_50MHZ,GPIO_SCL);
	
	/* 配置SDA为输出模式 */
	gpio_mode_set(PORT_SDA,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_SDA);
	/* 配置为推挽输出 50MHZ */
	gpio_output_options_set(PORT_SDA,GPIO_OTYPE_OD,GPIO_OSPEED_50MHZ,GPIO_SDA);
}

/******************************************************************
 * 函 数 名 称：IIC_Start
 * 函 数 说 明：IIC起始时序
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
******************************************************************/
void IIC_Start(void)
{
  SDA_OUT();

  SDA(1);
  delay_1us(5);
  SCL(1);
  delay_1us(5);

  SDA(0);
  delay_1us(5);
  SCL(0);
  delay_1us(5);

}
/******************************************************************
 * 函 数 名 称：IIC_Stop
 * 函 数 说 明：IIC停止信号
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
******************************************************************/
void IIC_Stop(void)
{
  SDA_OUT();
  SCL(0);
  SDA(0);

  SCL(1);
  delay_1us(5);
  SDA(1);
  delay_1us(5);

}

/******************************************************************
 * 函 数 名 称：IIC_Send_Ack
 * 函 数 说 明：主机发送应答或者非应答信号
 * 函 数 形 参：0发送应答  1发送非应答
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
******************************************************************/
void IIC_Send_Ack(unsigned char ack)
{
  SDA_OUT();
  SCL(0);
  SDA(0);
  delay_1us(5);
  if(!ack) SDA(0);
  else	 SDA(1);
  SCL(1);
  delay_1us(5);
  SCL(0);
  SDA(1);
}


/******************************************************************
 * 函 数 名 称：I2C_WaitAck
 * 函 数 说 明：等待从机应答
 * 函 数 形 参：无
 * 函 数 返 回：0有应答  1超时无应答
 * 作       者：LC
 * 备       注：无
******************************************************************/
unsigned char I2C_WaitAck(void)
{

  char ack = 0;
  unsigned char ack_flag = 10;
  SCL(0);
  SDA(1);
  SDA_IN();
  delay_1us(5);
  SCL(1);
  delay_1us(5);

  while( (SDA_GET()==1) && ( ack_flag ) )
  {
    ack_flag--;
    delay_1us(5);
  }

  if( ack_flag <= 0 )
  {
    IIC_Stop();
    return 1;
  }
  else
  {
    SCL(0);
    SDA_OUT();
  }
  return ack;
}

/******************************************************************
 * 函 数 名 称：Send_Byte
 * 函 数 说 明：写入一个字节
 * 函 数 形 参：dat要写人的数据
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
******************************************************************/
void Send_Byte(uint8_t dat)
{
  int i = 0;
  SDA_OUT();
  SCL(0);//拉低时钟开始数据传输

  for( i = 0; i < 8; i++ )
  {
    SDA( (dat & 0x80) >> 7 );
    delay_1us(1);
    SCL(1);
    delay_1us(5);
    SCL(0);
    delay_1us(5);
    dat<<=1;
  }
}

/******************************************************************
 * 函 数 名 称：Read_Byte
 * 函 数 说 明：IIC读时序
 * 函 数 形 参：ack=1时，发送ACK，ack=0，发送nACK
 * 函 数 返 回：读到的数据
 * 作       者：LC
 * 备       注：无
******************************************************************/
unsigned char Read_Byte(unsigned char ack)
{
  unsigned char i,receive=0;
  SDA_IN();//SDA设置为输入
  for(i=0; i<8; i++ )
  {
    SCL(0);
    delay_1us(4);
    SCL(1);
    receive<<=1;
    if( SDA_GET() )receive++;
    delay_1us(4); //1
  }
  if (!ack)
    IIC_Send_Ack(1);//发送nACK
  else
    IIC_Send_Ack(0); //发送ACK
  return receive;
}


//IIC写一个字节数据
u8 VL_IIC_Write_1Byte(u8 SlaveAddress, u8 REG_Address,u8 REG_data)
{
  IIC_Start();
  Send_Byte(SlaveAddress);
  if(I2C_WaitAck())
  {
    IIC_Stop();//释放总线
    return 1;//没应答则退出

  }
  Send_Byte(REG_Address);
  I2C_WaitAck();
  Send_Byte(REG_data);
  I2C_WaitAck();
  IIC_Stop();

  return 0;
}

//IIC读一个字节数据
u8 VL_IIC_Read_1Byte(u8 SlaveAddress, u8 REG_Address,u8 *REG_data)
{
  IIC_Start();
  Send_Byte(SlaveAddress);//发写命令
  if(I2C_WaitAck())
  {
    IIC_Stop();//释放总线
    return 1;//没应答则退出
  }
  Send_Byte(REG_Address);
  I2C_WaitAck();
  IIC_Start();
  Send_Byte(SlaveAddress|0x01);//发读命令
  I2C_WaitAck();
  *REG_data = Read_Byte(0);
  IIC_Stop();

  return 0;
}

//IIC写n字节数据
u8 VL_IIC_Write_nByte(u8 SlaveAddress, u8 REG_Address,u16 len, u8 *buf)
{
  IIC_Start();
  Send_Byte(SlaveAddress);//发写命令
  if(I2C_WaitAck())
  {
    IIC_Stop();//释放总线
    return 1;//没应答则退出
  }
  Send_Byte(REG_Address);
  I2C_WaitAck();
  while(len--)
  {
    Send_Byte(*buf++);//发送buff的数据
    I2C_WaitAck();
  }
  IIC_Stop();//释放总线

  return 0;

}

//IIC读n字节数据
u8 VL_IIC_Read_nByte(u8 SlaveAddress, u8 REG_Address,u16 len,u8 *buf)
{
  IIC_Start();
  Send_Byte(SlaveAddress);//发写命令
  if(I2C_WaitAck())
  {
    IIC_Stop();//释放总线
    return 1;//没应答则退出
  }
  Send_Byte(REG_Address);
  I2C_WaitAck();

  IIC_Start();
  Send_Byte(SlaveAddress|0x01);//发读命令
  I2C_WaitAck();
  while(len)
  {
    if(len==1)
    {
      *buf = Read_Byte(0);
    }
    else
    {
      *buf = Read_Byte(1);
    }
    buf++;
    len--;
  }
  IIC_Stop();//释放总线

  return 0;

}

//VL53L0X 写多个数据
//address:地址
//index:偏移地址
//pdata:数据指针
//count:长度 最大65535
u8 VL53L0X_write_multi(u8 address, u8 index,u8 *pdata,u16 count)
{
  u8 status = STATUS_OK;

  if(VL_IIC_Write_nByte(address,index,count,pdata))
  {
    status  = STATUS_FAIL;
  }
  return status;
}


//VL53L0X 读多个数据
//address:地址
//index:偏移地址
//pdata:数据指针
//count:长度 最大65535
u8 VL53L0X_read_multi(u8 address,u8 index,u8 *pdata,u16 count)
{
  u8 status = STATUS_OK;

  if(VL_IIC_Read_nByte(address,index,count,pdata))
  {
    status  = STATUS_FAIL;
  }

  return status;
}

//VL53L0X 写1个数据(单字节)
//address:地址
//index:偏移地址
//data:数据(8位)
u8 VL53L0X_write_byte(u8 address,u8 index,u8 data)
{
  u8 status = STATUS_OK;

  status = VL53L0X_write_multi(address,index,&data,1);

  return status;
}

//VL53L0X 写1个数据(双字节)
//address:地址
//index:偏移地址
//data:数据(16位)
u8 VL53L0X_write_word(u8 address,u8 index,u16 data)
{
  u8 status = STATUS_OK;

  u8 buffer[2];

  //将16位数据拆分成8位
  buffer[0] = (u8)(data>>8);//高八位
  buffer[1] = (u8)(data&0xff);//低八位

  if(index%2==1)
  {
    //串行通信不能处理对非2字节对齐寄存器的字节
    status = VL53L0X_write_multi(address,index,&buffer[0],1);
    status = VL53L0X_write_multi(address,index,&buffer[0],1);
  } else
  {
    status = VL53L0X_write_multi(address,index,buffer,2);
  }

  return status;
}

//VL53L0X 写1个数据(四字节)
//address:地址
//index:偏移地址
//data:数据(32位)
u8 VL53L0X_write_dword(u8 address,u8 index,u32 data)
{

  u8 status = STATUS_OK;

  u8 buffer[4];

  //将32位数据拆分成8位
  buffer[0] = (u8)(data>>24);
  buffer[1] = (u8)((data&0xff0000)>>16);
  buffer[2] = (u8)((data&0xff00)>>8);
  buffer[3] = (u8)(data&0xff);

  status = VL53L0X_write_multi(address,index,buffer,4);

  return status;

}


//VL53L0X 读1个数据(单字节)
//address:地址
//index:偏移地址
//data:数据(8位)
u8 VL53L0X_read_byte(u8 address,u8 index,u8 *pdata)
{
  u8 status = STATUS_OK;

  status = VL53L0X_read_multi(address,index,pdata,1);

  return status;

}

//VL53L0X 读个数据(双字节)
//address:地址
//index:偏移地址
//data:数据(16位)
u8 VL53L0X_read_word(u8 address,u8 index,u16 *pdata)
{
  u8 status = STATUS_OK;

  u8 buffer[2];

  status = VL53L0X_read_multi(address,index,buffer,2);

  *pdata = ((u16)buffer[0]<<8)+(u16)buffer[1];

  return status;

}

//VL53L0X 读1个数据(四字节)
//address:地址
//index:偏移地址
//data:数据(32位)
u8 VL53L0X_read_dword(u8 address,u8 index,u32 *pdata)
{
  u8 status = STATUS_OK;

  u8 buffer[4];

  status = VL53L0X_read_multi(address,index,buffer,4);

  *pdata = ((u32)buffer[0]<<24)+((u32)buffer[1]<<16)+((u32)buffer[2]<<8)+((u32)buffer[3]);

  return status;

}
