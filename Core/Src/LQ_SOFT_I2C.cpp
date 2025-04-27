/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@编   写：龙邱科技
@邮   箱：chiusir@163.com
@编译IDE：Linux 环境、VSCode_1.93 及以上版本、Cmake_3.16 及以上版本
@使用平台：龙芯2K0300久久派和北京龙邱智能科技龙芯久久派拓展板
@相关信息参考下列地址
    网      站：http://www.lqist.cn
    淘 宝 店 铺：http://longqiu.taobao.com
    程序配套视频：https://space.bilibili.com/95313236
@软件版本：V1.0 版权所有，单位使用请先联系授权
@参考项目链接：https://github.com/AirFortressIlikara/ls2k0300_peripheral_library

@修改日期：2025-03-17
@修改内容：
@注意事项：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "LQ_SOFT_I2C.hpp"

/* 定义所使用的 IIC 引脚 */
HWGpio SOFT_IIC_SCL_PIN_fd;
HWGpio SOFT_IIC_SDA_PIN_fd;

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void iic_delay(u16 us)
@功能说明：IIC延时
@参数说明：us：延时时间
@函数返回：无
@调用方法：iic_delay();  //简单的延时
@备    注：修改这里可以调整IIC速率
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void iic_delay()
{
    /* 200MHz 系统时钟下 模拟IIC速度为 400Khz */
    for (uint8_t i = 0; i < 30; i++) // 修改这里可以调整IIC速率
        __asm("NOP"); /* delay */
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void IIC_Init(void)
 * @功能说明：模拟IIC初始化
 * @参数说明：无
 * @函数返回：无
 * @调用方法：IIC_Init();  //模拟IIC初始化
 * @备注说明：IIC管脚在LQ_SOFTI2C.h中定义
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void IIC_Init(void)
{
    IIC_SCL_INIT;
    IIC_SDA_INIT;
    IIC_SCL_H;
    IIC_SDA_H;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void IIC_Start(void)
 * @功能说明：模拟IIC起始信号
 * @参数说明：无
 * @函数返回：无
 * @调用方法：IIC_Start();
 * @备注说明：内部调用
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void IIC_Start(void)
{
    SDA_OUT; // sda线输出
    IIC_SDA_H;
    IIC_SCL_H;
    iic_delay();
    iic_delay();
    iic_delay();
    iic_delay();
    iic_delay();
    IIC_SDA_L; // 当CLK为高时，DATA从高变低
    iic_delay();
    iic_delay();
    iic_delay();
    IIC_SCL_L; // 钳住I2C总线，准备发送或接收数据
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void IIC_Stop(void)
 * @功能说明：模拟IIC停止信号
 * @参数说明：无
 * @函数返回：无
 * @调用方法：IIC_Stop();
 * @备注说明：内部调用
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void IIC_Stop(void)
{
    SDA_OUT; // sda线输出
    IIC_SCL_L;
    IIC_SDA_L; // 当CLK为高时，DATA从低变为高
    iic_delay();
    iic_delay();
    iic_delay();
    IIC_SCL_H;
    iic_delay();
    iic_delay();
    iic_delay();
    IIC_SDA_H; // 发送I2C总线结束信号
    iic_delay();
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void IIC_Ack(void)
 * @功能说明：模拟IIC产生ACK应答
 * @参数说明：无
 * @函数返回：无
 * @调用方法：IIC_Ack();
 * @备注说明：内部调用 主机接收完一个字节数据后，主机产生的ACK通知从机一个字节数据已正确接收
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void IIC_Ack(void)
{
    IIC_SCL_L;
    SDA_OUT;
    IIC_SDA_L;
    iic_delay();
    iic_delay();
    iic_delay();
    IIC_SCL_H;
    iic_delay();
    iic_delay();
    iic_delay();
    IIC_SCL_L;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void IIC_NAck(void)
 * @功能说明：模拟IIC不产生ACK应答
 * @参数说明：无
 * @函数返回：无
 * @调用方法：IIC_NAck();
 * @备注说明：内部调用 主机接收完最后一个字节数据后，主机产生的NACK通知从机发送结束，
 * @           释放SDA,以便主机产生停止信号
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void IIC_NAck(void)
{
    IIC_SCL_L;
    SDA_OUT;
    IIC_SDA_H;
    iic_delay();
    iic_delay();
    iic_delay();
    IIC_SCL_H;
    iic_delay();
    iic_delay();
    iic_delay();
    IIC_SCL_L;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：uint8_t SOFT_IIC_Wait_Ack(void)
 * @功能说明：模拟IIC等待应答信号
 * @参数说明：无
 * @函数返回：1，接收应答失败；0，接收应答成功
 * @调用方法：SOFT_IIC_Wait_Ack();
 * @备注说明：内部调用 有效应答：从机第9个 SCL=0 时 SDA 被从机拉低,并且 SCL = 1时 SDA依然为低
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8_t IIC_WaitAck(void)
{
    uint8_t ucErrTime = 0;
    SDA_IN; // SDA设置为输入  （从机给一个低电平做为应答）
    IIC_SDA_H;
    iic_delay();
    IIC_SCL_H;
    iic_delay();
    while (IIC_SDA_READ)
    {
        ucErrTime++;
        if (ucErrTime > 100)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL_L; // 时钟输出0
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：void IIC_Send_Byte(uint8_t data_t)
 * @功能说明：模拟IIC发送一个字节
 * @参数说明：data: 发送的字节
 * @函数返回：无
 * @调用方法：IIC_SendByte(0x12);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void IIC_SendByte(uint8_t data_t)
{
    uint8_t t;
    SDA_OUT;
    IIC_SCL_L; // 拉低时钟开始数据传输
    for (t = 0; t < 8; t++)
    {
        // IIC_SDA_READ = data_t&0x80;
        if (data_t & 0x80)
            IIC_SDA_H;
        else
            IIC_SDA_L;

        IIC_SCL_H;
        iic_delay();
        data_t <<= 1;
        iic_delay();
        iic_delay();
        IIC_SCL_L;
        iic_delay();
    }
    iic_delay();
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：uint8_t  IIC_Read_Byte(uint8_t ack)
 * @功能说明：模拟IIC读取一个字节
 * @参数说明：ack=1 时，主机数据还没接收完 ack=0 时主机数据已全部接收完成
 * @函数返回：接收到的字节
 * @调用方法：IC_ReadByte(0x12);
 * @备注说明：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8_t IIC_ReadByte(uint8_t ack)
{
    uint8_t i, receive = 0;
    SDA_IN; // SDA设置为输入模式 等待接收从机返回数据
    for (i = 0; i < 8; i++)
    {
        IIC_SCL_L;
        iic_delay();
        iic_delay();
        iic_delay();
        IIC_SCL_H;
        receive <<= 1;
        if (IIC_SDA_READ)
            receive++; // 从机发送的电平
        iic_delay();
    }
    if (ack)
        IIC_Ack(); // 发送ACK
    else
        IIC_NAck(); // 发送nACK
    return receive;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：int LQ_I2C_Write(uint8_t addr, uint8_t reg, uint16_t len, uint8_t *dat)
 * @功能说明：模拟SPI读写数据及长度
 * @参数说明：addr：标设备地址  reg：目标寄存器  len：写入长度  *dat：存放写入数据
 * @函数返回：1失败 0成功
 * @调用方法：LQ_I2C_Write(0x68, 0x38, 1, tmp)；
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int LQ_I2C_Write(uint8_t addr, uint8_t reg, uint16_t len, uint8_t *dat)
{
    uint8_t i = 0;

    IIC_Start();
    IIC_SendByte(addr << 1);
    IIC_WaitAck();

    IIC_SendByte(reg);
    IIC_WaitAck();

    for (i = 0; i < len; i++)
    {
        IIC_SendByte(dat[i]);
        IIC_WaitAck();
    }
    IIC_Stop();
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：int LQ_I2C_Read(uint8_t addr, uint8_t reg, uint16_t len, uint8_t *buf)
 * @功能说明：模拟SPI读写数据及长度
 * @参数说明：addr ：标设备地址
 * @参数说明：reg  ：目标寄存器
 * @参数说明：len  ：写入长度
 * @参数说明：*buf ：存放写入数据
 * @函数返回：1失败 0成功
 * @调用方法：LQ_I2C_Write(0x68, 0x38, 1, tmp)
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int LQ_I2C_Read(uint8_t addr, uint8_t reg, uint16_t len, uint8_t *buf)
{
    IIC_Start();
    IIC_SendByte(addr << 1);
    IIC_WaitAck();

    IIC_SendByte(reg);
    IIC_WaitAck();
    iic_delay();
    IIC_Start();

    IIC_SendByte((addr << 1) + 1);
    IIC_WaitAck();
    while (len)
    {
        if (len == 1)
            *buf = IIC_ReadByte(0);
        else
            *buf = IIC_ReadByte(1);
        buf++;
        len--;
    }
    IIC_Stop();
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：uint8_t IIC_ReadByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t *buf)
 * @功能说明：模拟IIC读取指定设备 指定寄存器的一个值
 * @参数说明：I2C_Addr：目标设备地址 reg ：目标寄存器 buf ：存放读出字节
 * @函数返回：1失败 0成功
 * @调用方法：IIC_ReadByteFromSlave(0xD0, 0x75, buf);   //读 IIC地址为 0xD0器件（SOFT_I2C）寄存器0x75
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8_t IIC_ReadByteFromSlave(uint8_t I2C_Addr, uint8_t reg, uint8_t *buf)
{
    IIC_Start();
    IIC_SendByte(I2C_Addr); // 发送从机地址
    if (IIC_WaitAck())      // 如果从机未应答则数据发送失败
    {
        IIC_Stop();
        return 1;
    }
    IIC_SendByte(reg); // 发送寄存器地址
    IIC_WaitAck();

    IIC_Start();
    IIC_SendByte(I2C_Addr + 1); // 进入接收模式
    IIC_WaitAck();
    *buf = IIC_ReadByte(0);
    IIC_Stop(); // 产生一个停止条件
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：uint8_t IIC_ReadMultByteFromSlave(uint8_t dev, uint8_t reg, uint8_t len, uint8_t *data_t)
 * @功能说明：模拟IIC读取指定设备 指定寄存器的n个值
 * @参数说明：dev：目标设备地址  reg：目标寄存器  len：读取长度  data：存放读出数据
 * @函数返回：1失败 0成功
 * @调用方法：IIC_ReadByteFromSlave(0xD0, 0X3B, 14, &data);   //读 14个字节
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8_t IIC_ReadMultByteFromSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data_t)
{
    uint8_t count = 0;
    uint8_t temp;
    IIC_Start();
    IIC_SendByte(dev); // 发送从机地址
    if (IIC_WaitAck())
    {
        IIC_Stop();
        return 1; // 从机地址写入失败
    }
    IIC_SendByte(reg); // 发送寄存器地址
    IIC_WaitAck();
    IIC_Start();
    IIC_SendByte(dev + 1); // 进入接收模式
    IIC_WaitAck();
    for (count = 0; count < length; count++)
    {
        if (count != (length - 1))
            temp = IIC_ReadByte(1); // 带ACK的读数据
        else
            temp = IIC_ReadByte(0); // 最后一个字节NACK

        data_t[count] = temp;
    }
    IIC_Stop(); // 产生一个停止条件
    // return count;
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：uint8_t IIC_WriteByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t data_t)
 * @功能说明：模拟IIC写指定设备 指定寄存器的一个值
 * @参数说明：I2C_Addr：目标设备地址 reg：目标寄存器data：写入的数据
 * @函数返回：1失败 0成功
 * @调用方法：IIC_ReadByteFromSlave(0xD0, 0X6B, 0X80);   //IIC地址为 0xD0器件（SOFT_I2C）寄存器0x6B 写入0x80
 * @备    注：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8_t IIC_WriteByteToSlave(uint8_t I2C_Addr, uint8_t reg, uint8_t data_t)
{
    IIC_Start();
    IIC_SendByte(I2C_Addr); // 发送从机地址
    if (IIC_WaitAck())
    {
        IIC_Stop();
        return 1; // 从机地址写入失败
    }
    IIC_SendByte(reg); // 发送寄存器地址
    IIC_WaitAck();
    IIC_SendByte(data_t);
    if (IIC_WaitAck())
    {
        IIC_Stop();
        return 1; // 数据写入失败
    }
    IIC_Stop(); // 产生一个停止条件

    // return 1; //status == 0;
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：uint8_t IIC_WriteMultByteToSlave(uint8_t dev, uint8_t reg, uint8_t len, uint8_t* data_t)
 * @功能说明：模拟IIC写指定设备 指定寄存器的n个值
 * @参数说明：dev ：目标设备地址
 * @参数说明：reg ：目标寄存器
 * @参数说明：len ：写入长度
 * @参数说明：data：存放写入数据
 * @函数返回：1失败 0成功
 * @调用方法：IIC_WriteMultByteToSlave(0xD0, 0X6B, 1, 0X80);   //向寄存器0x6B写入0x80
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8_t IIC_WriteMultByteToSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data_t)
{
    uint8_t count = 0;
    IIC_Start();
    IIC_SendByte(dev); // 发送从机地址
    if (IIC_WaitAck())
    {
        IIC_Stop();
        return 1; // 从机地址写入失败
    }
    IIC_SendByte(reg); // 发送寄存器地址
    IIC_WaitAck();
    for (count = 0; count < length; count++)
    {
        IIC_SendByte(data_t[count]);
        if (IIC_WaitAck()) // 每一个字节都要等从机应答
        {
            IIC_Stop();
            return 1; // 数据写入失败
        }
    }
    IIC_Stop(); // 产生一个停止条件

    return 0;
}
