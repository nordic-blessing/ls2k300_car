/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@��   д������Ƽ�
@��   �䣺chiusir@163.com
@����IDE��Linux ������VSCode_1.93 �����ϰ汾��Cmake_3.16 �����ϰ汾
@ʹ��ƽ̨����о2K0300�þ��ɺͱ����������ܿƼ���о�þ�����չ��
@�����Ϣ�ο����е�ַ
    ��      վ��http://www.lqist.cn
    �� �� �� �̣�http://longqiu.taobao.com
    ����������Ƶ��https://space.bilibili.com/95313236
@����汾��V1.0 ��Ȩ���У���λʹ��������ϵ��Ȩ
@�ο���Ŀ���ӣ�https://github.com/AirFortressIlikara/ls2k0300_peripheral_library

@�޸����ڣ�2025-03-18
@�޸����ݣ�
@ע�����ע��鿴·�����޸�
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

/* ֹͣλ��غ� */
enum
{
    LS_UART_STOP1,  // 1λֹͣλ
    LS_UART_STOP2,  // 2λֹͣλ
};

/* ����λ��غ� */
enum
{
    LS_UART_DATA5,  // 5λ����λ
    LS_UART_DATA6,  // 6λ����λ
    LS_UART_DATA7,  // 7λ����λ
    LS_UART_DATA8   // 8λ����λ
};

// У��λ��غ�
enum
{
    LS_UART_NONE,   // ��У��
    LS_UART_ODD,    // żУ��
    LS_UART_EVEN    // ��У��
};

class LS_UART
{
public:
    /*!
     * @brief   �����޲ι��캯��
     */
    LS_UART();

    /*!
     * @brief   ���ڴ��ι��캯��
     * @param   uart     : ��ʹ�õĴ����豸�ļ�
     * @param   baudrate : ����������
     * @param   stop     : ֹͣλ����
     * @param   data     : ����λ����
     * @param   check    : У��λ����
     */
    LS_UART(const string &uart, speed_t baudrate, uint8_t stop, uint8_t data, uint8_t check);

    /*!
     * @brief   ���ò�����
     * @param   baudrate : ���ò�������غ�
     */
    void SetBaudrate(speed_t baudrate);

    /*!
     * @brief   ����ֹͣλ
     * @param   stop : ����ֹͣλ��غ�
     */
    void SetStopBit(uint8_t stop);

    /*!
     * @brief   ��������λ
     * @param   stop : ��������λ��غ�
     */
    void SetDataBit(uint8_t data);

    /*!
     * @brief   ����У��λ
     * @param   stop : ����У��λ��غ�
     */
    void SetChecBit(uint8_t check);

    /*!
     * @brief   ���ڷ��ͺ���
     * @param   buf : ���ͻ�����
     * @param   len : �������ݳ���
     */
    ssize_t WriteData(const char *buf, ssize_t len);

    /*!
     * @brief   ���ڽ��պ���
     * @param   buf : ���ջ�����
     * @param   len : �������ݳ���
     */
    ssize_t ReadData(char *buf, ssize_t len);

    /*!
     * @brief   ������������
     * @param   stop : ����ֹͣλ��غ�
     */
    ~LS_UART();

private:
    /*!
     * @brief   �ļ�������
     */
    int uart_fd;

    /*!
     * @brief   ���ô��ڵĺ��Ľṹ��
     */
    struct termios ts;

    speed_t BaudRate;   // ������
    uint8_t Stop;       // ֹͣλ
    uint8_t Data;       // ����λ
    uint8_t Check;      // У��λ
};
