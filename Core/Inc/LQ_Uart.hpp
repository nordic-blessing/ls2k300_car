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

@修改日期：2025-03-18
@修改内容：
@注意事项：注意查看路径的修改
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#pragma once

#include <iostream>
#include <string>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

using namespace std;

#define UART1 "/dev/ttyS1" 

/* 停止位相关宏 */
enum
{
    LS_UART_STOP1,  // 1位停止位
    LS_UART_STOP2,  // 2位停止位
};

/* 数据位相关宏 */
enum
{
    LS_UART_DATA5,  // 5位数据位
    LS_UART_DATA6,  // 6位数据位
    LS_UART_DATA7,  // 7位数据位
    LS_UART_DATA8   // 8位数据位
};

// 校验位相关宏
enum
{
    LS_UART_NONE,   // 无校验
    LS_UART_ODD,    // 偶校验
    LS_UART_EVEN    // 奇校验
};

class LS_UART
{
public:
    /*!
     * @brief   串口无参构造函数
     */
    LS_UART();

    /*!
     * @brief   串口带参构造函数
     * @param   uart     : 所使用的串口设备文件
     * @param   baudrate : 波特率设置
     * @param   stop     : 停止位设置
     * @param   data     : 数据位设置
     * @param   check    : 校验位设置
     */
    LS_UART(const string &uart, speed_t baudrate, uint8_t stop, uint8_t data, uint8_t check);

    /*!
     * @brief   设置波特率
     * @param   baudrate : 设置波特率相关宏
     */
    void SetBaudrate(speed_t baudrate);

    /*!
     * @brief   设置停止位
     * @param   stop : 设置停止位相关宏
     */
    void SetStopBit(uint8_t stop);

    /*!
     * @brief   设置数据位
     * @param   stop : 设置数据位相关宏
     */
    void SetDataBit(uint8_t data);

    /*!
     * @brief   设置校验位
     * @param   stop : 设置校验位相关宏
     */
    void SetChecBit(uint8_t check);

    /*!
     * @brief   串口发送函数
     * @param   buf : 发送缓冲区
     * @param   len : 发送数据长度
     */
    ssize_t WriteData(const char *buf, ssize_t len);

    /*!
     * @brief   串口接收函数
     * @param   buf : 接收缓冲区
     * @param   len : 接收数据长度
     */
    ssize_t ReadData(char *buf, ssize_t len);

    /*!
     * @brief   串口析构函数
     * @param   stop : 设置停止位相关宏
     */
    ~LS_UART();

private:
    /*!
     * @brief   文件描述符
     */
    int uart_fd;

    /*!
     * @brief   配置串口的核心结构体
     */
    struct termios ts;

    speed_t BaudRate;   // 波特率
    uint8_t Stop;       // 停止位
    uint8_t Data;       // 数据位
    uint8_t Check;      // 校验位
};
