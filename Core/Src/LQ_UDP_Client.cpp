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
#include "LQ_UDP_Client.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�UDP_Client::UDP_Client()
 * @����˵����UDP �ͻ�������޲ι��캯��
 * @����˵������
 * @�������أ���
 * @���÷�����UDP_Client cli;
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
UDP_Client::UDP_Client()
{
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�UDP_Client::UDP_Client()
 * @����˵����UDP �ͻ�������вι��캯��
 * @����˵����ip   : ���ӷ������� IP ��ַ
 * @����˵����port : ���ӷ������Ķ˿ں�
 * @�������أ���
 * @���÷�����UDP_Client cli("127.0.0.1", 6666);
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
UDP_Client::UDP_Client(const string &ip, uint16_t port)
{
    this->UDP_client_init(ip, port);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�UDP_Client::UDP_client_init(const string &ip, uint32_t port)
 * @����˵����UDP �ͻ�����ĳ�ʼ������
 * @����˵����ip   : ���ӷ������� IP ��ַ
 * @����˵����port : ���ӷ������Ķ˿ں�
 * @�������أ��ɹ�����0���׽��ִ���ʧ�ܷ���-1��������IP����ʧ��ʱ����-2������������ʧ�ܷ���-3
 * @���÷�����cli.UDP_client_init("127.0.0.1", 6666);
 * @��ע˵�������õ�ַ���� -- ������������֮���ٴ����л���� bind ����(��ַ��ռ��)��
 *                          ԭ���ǹر��׽���ʱ��ϵͳ��Ҫһ�� TIME_WAIT ʱ��֮��ſ�
 *                          ���ٴ�ʹ�ã�ʹ�õ�ַ���ú������Խ���������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8_t UDP_Client::UDP_client_init(const string &ip, uint16_t port)
{
    // �����׽���
    this->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->sockfd == -1)
    {
        perror("UDP_cli_socket:");
        return -1;
    }
    // ���õ�ַ����
    int val = 1;
    setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    // ���ӷ�����
    memset(&this->addr, 0, sizeof(this->addr));
    this->addr.sin_family = AF_INET;   // ����ַ������Ϊ AF_INET����ʾʹ�� IPv4 ��ַ
    this->addr.sin_port = htons(port); // ���÷������˿ںţ�ʹ�� htons �������ֽ���תΪ�����ֽ���
    // ���÷����� IP ��ַ
    if (inet_pton(AF_INET, ip.c_str(), &this->addr.sin_addr) < 1)
    {
        perror("UDP_cli_inet_aton:");
        close(this->sockfd);
        return -2;
    }

    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�ssize_t UDP_Client::UDP_Send(const void *buf, size_t len)
 * @����˵����UDP �ͻ�����ķ�����Ϣ����
 * @����˵����buf : �������ݻ�����ָ��
 * @����˵����len : ��Ҫ���͵����ݳ���
 * @�������أ��ɹ����ط��͵��ֽ�����ʧ�ܷ���-1
 * @���÷�����cli.UDP_Send(buf, sizeof(buf));
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
ssize_t UDP_Client::UDP_Send(const void *buf, size_t len)
{
    ssize_t ret = sendto(this->sockfd, buf, len, 0, (struct sockaddr*)&this->addr, sizeof(this->addr));
    if (ret < 0)
    {
        perror("UDP_cli_send:");
    }
    return ret;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�ssize_t UDP_Client::UDP_Recv(void *buf, size_t len)
 * @����˵����UDP �ͻ�����Ķ�ȡ��Ϣ����
 * @����˵����buf : �������ݻ�����ָ��
 * @����˵����len : ��Ҫ���յ����ݳ���
 * @�������أ��ɹ����ض�ȡ���ֽ�����ʧ�ܷ���-1
 * @���÷�����cli.UDP_Recv(buf, sizeof(buf));
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
ssize_t UDP_Client::UDP_Recv(void *buf, size_t len)
{
    socklen_t addrLen = sizeof(this->addr);
    ssize_t ret = recvfrom(this->sockfd, buf, len, 0, (struct sockaddr*)&this->addr, &addrLen);
    if (ret < 0)
    {
        perror("UDP_cli_recv:");
    }
    return ret;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�int UDP_Client::UDP_Get_socket()
 * @����˵����UDP �ͻ�����Ļ�ȡ�׽��ֺ���
 * @����˵������
 * @�������أ������׽���
 * @���÷�����int sockfd = cli.UDP_Get_socket();
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int UDP_Client::UDP_Get_socket()
{
    return this->sockfd;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�int UDP_Client::UDP_Get_socket()
 * @����˵����UDP �ͻ�����Ļ�ȡ�׽��ֺ���
 * @����˵������
 * @�������أ������׽���
 * @���÷�����int sockfd = cli.UDP_Get_socket();
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
struct sockaddr_in UDP_Client::UDP_Get_sockaddr()
{
    return this->addr;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�UDP_Client::~UDP_Client()
 * @����˵����UDP �ͻ��������������
 * @����˵������
 * @�������أ���
 * @���÷����������Ķ����������ڽ�����ϵͳ�Զ�����
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
UDP_Client::~UDP_Client()
{
    close(this->sockfd);
}
