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

#pragma once

#include "main.hpp"

/* 注意 IIC总线规定，IIC空闲时 SCL和SDA都为高电平 最好外部上拉（一定不能下拉） */
/* 模拟 IIC需要注意，IIC地址左移一位 例如MPU6050 模拟就是地址 0xD0 */
/* 想换用别的IO 直接修改宏定义 SOFT_IIC_SCL_PIN 、 SOFT_IIC_SDA_PIN 即可 */

/* 确认软件IIC所使用的引脚号 */
#define SOFT_IIC_SCL_PIN 50 /*!< 作为 SCL */
#define SOFT_IIC_SDA_PIN 51 /*!< 作为 SDA */

/* 修改信号线的输入输出模式 */
#define SDA_OUT (SOFT_IIC_SDA_PIN_fd.GpioDirection(GPIO_Mode_Out))
#define SDA_IN  (SOFT_IIC_SDA_PIN_fd.GpioDirection(GPIO_Mode_In))

/* 初始化信号线和时钟线 */
#define IIC_SCL_INIT (SOFT_IIC_SCL_PIN_fd = HWGpio(SOFT_IIC_SCL_PIN, GPIO_Mode_Out))
#define IIC_SDA_INIT (SOFT_IIC_SDA_PIN_fd = HWGpio(SOFT_IIC_SDA_PIN, GPIO_Mode_Out))

/* 时钟线高电平与低电平的宏 */
#define IIC_SCL_H (SOFT_IIC_SCL_PIN_fd.SetGpioValue(1))
#define IIC_SCL_L (SOFT_IIC_SCL_PIN_fd.SetGpioValue(0))

/* 信号线高电平与低电平的宏 */
#define IIC_SDA_H (SOFT_IIC_SDA_PIN_fd.SetGpioValue(1))
#define IIC_SDA_L (SOFT_IIC_SDA_PIN_fd.SetGpioValue(0))

/* 读取信号线的电平值 */
#define IIC_SDA_READ (SOFT_IIC_SDA_PIN_fd.GetGpioValue())

void IIC_Init(void);                // IIC初始化
void IIC_Start(void);               // 发送IIC开始信号
void IIC_Stop(void);                // 发送IIC停止信号
void IIC_Ack(void);                 // IIC发送ACK信号
void IIC_NAck(void);                // IIC不发送ACK信号
uint8_t IIC_WaitAck(void);          // IIC等待ACK信号
void IIC_SendByte(uint8_t data_t);  // IIC发送一个字节
uint8_t IIC_ReadByte(uint8_t ack);  // IIC读取一个字节
/* 模拟SPI写入数据及长度 */
int LQ_I2C_Write(uint8_t addr, uint8_t reg, uint16_t len, uint8_t *dat);
/* 模拟SPI读取数据及长度 */
int LQ_I2C_Read(uint8_t addr, uint8_t reg, uint16_t len, uint8_t *buf);
/* 模拟IIC读取指定设备 指定寄存器的一个值 */
uint8_t IIC_ReadByteFromSlave(uint8_t I2C_Addr, uint8_t reg, uint8_t *buf);
/* 模拟IIC读取指定设备 指定寄存器的n个值 */
uint8_t IIC_ReadMultByteFromSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data_t);
/* 模拟IIC写入指定设备 指定寄存器的一个值 */
uint8_t IIC_WriteByteToSlave(uint8_t I2C_Addr, uint8_t reg, uint8_t buf);
/* 模拟IIC写入指定设备 指定寄存器的n个值 */
uint8_t IIC_WriteMultByteToSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data_t);

