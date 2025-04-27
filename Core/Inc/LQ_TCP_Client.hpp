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

@�޸����ڣ�2025-03-27
@�޸����ݣ�
@ע�����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#pragma once

#include <string>

#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

class TCP_Client
{
public:
    /*!
     * @brief   TCP �ͻ�������޲ι��캯��
     */
    TCP_Client();

    /*!
     * @brief   TCP �ͻ�������вι��캯��
     * @param   ip   : ���ӷ������� IP ��ַ
     * @param   port : ���ӷ������Ķ˿ں�
     */
    TCP_Client(const string &ip, uint16_t port);

    /*!
     * @brief   TCP �ͻ�����ĳ�ʼ������
     * @param   ip   : ���ӷ������� IP ��ַ
     * @param   port : ���ӷ������Ķ˿ں�
     * @return  �ɹ�����0���׽��ִ���ʧ�ܷ���-1��������IP����ʧ��ʱ����-2������������ʧ�ܷ���-3
     */
    uint8_t TCP_client_init(const string &ip, uint16_t port);

    /*!
     * @brief   TCP �ͻ�����ķ�����Ϣ����
     * @param   buf : �������ݻ�����ָ��
     * @param   len : ��Ҫ���͵����ݳ���
     * @return  �ɹ����ط��͵��ֽ�����ʧ�ܷ���-1
     */
    ssize_t TCP_Send(const void *buf, size_t len);

    /*!
     * @brief   TCP �ͻ�����Ķ�ȡ��Ϣ����
     * @param   buf : �������ݻ�����ָ��
     * @param   len : ��Ҫ���յ����ݳ���
     * @return  �ɹ����ض�ȡ���ֽ�����ʧ�ܷ���-1
     */
    ssize_t TCP_Recv(void *buf, size_t len);

    /*!
     * @brief   TCP �ͻ�����Ļ�ȡ�׽��ֺ���
     * @return  �����׽���
     */
    int TCP_Get_socket();

    /*!
     * @brief   TCP �ͻ��������������
     */
    ~TCP_Client();
private:

    /*!
     * @brief   TCP �ͻ����׽���
     */
    int sockfd;
};
