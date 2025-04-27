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
#include "LQ_TCP_Client.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�TCP_Client::TCP_Client()
 * @����˵����TCP �ͻ�������޲ι��캯��
 * @����˵������
 * @�������أ���
 * @���÷�����TCP_Client cli;
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
TCP_Client::TCP_Client()
{
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�TCP_Client::TCP_Client()
 * @����˵����TCP �ͻ�������вι��캯��
 * @����˵����ip   : ���ӷ������� IP ��ַ
 * @����˵����port : ���ӷ������Ķ˿ں�
 * @�������أ���
 * @���÷�����TCP_Client cli("127.0.0.1", 6666);
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
TCP_Client::TCP_Client(const string &ip, uint16_t port)
{
    this->TCP_client_init(ip, port);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�TCP_Client::TCP_client_init(const string &ip, uint32_t port)
 * @����˵����TCP �ͻ�����ĳ�ʼ������
 * @����˵����ip   : ���ӷ������� IP ��ַ
 * @����˵����port : ���ӷ������Ķ˿ں�
 * @�������أ��ɹ�����0���׽��ִ���ʧ�ܷ���-1��������IP����ʧ��ʱ����-2������������ʧ�ܷ���-3
 * @���÷�����cli.TCP_client_init("127.0.0.1", 6666);
 * @��ע˵�������õ�ַ���� -- ������������֮���ٴ����л���� bind ����(��ַ��ռ��)��
 *                          ԭ���ǹر��׽���ʱ��ϵͳ��Ҫһ�� TIME_WAIT ʱ��֮��ſ�
 *                          ���ٴ�ʹ�ã�ʹ�õ�ַ���ú������Խ���������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8_t TCP_Client::TCP_client_init(const string &ip, uint16_t port)
{
    // �����׽���
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd == -1)
    {
        perror("tcp_cli_socket:");
        return -1;
    }
    // ���õ�ַ����
    int val = 1;
    setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    // ���ӷ�����
    struct sockaddr_in seraddr;
    memset(&seraddr, 0, sizeof(seraddr));
    seraddr.sin_family = AF_INET;   // ����ַ������Ϊ AF_INET����ʾʹ�� IPv4 ��ַ
    seraddr.sin_port = htons(port); // ���÷������˿ںţ�ʹ�� htons �������ֽ���תΪ�����ֽ���
    // ���÷����� IP ��ַ
    if (inet_aton(ip.c_str(), &seraddr.sin_addr) == 0)
    {
        perror("tcp_cli_inet_aton:");
        close(this->sockfd);
        return -2;
    }
    // ���ӷ�����
    if (connect(this->sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) == -1)
    {
        perror("tcp_cli_connect:");
        close(this->sockfd);
        return -3;
    }

    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�ssize_t TCP_Client::TCP_Send(const void *buf, size_t len)
 * @����˵����TCP �ͻ�����ķ�����Ϣ����
 * @����˵����buf : �������ݻ�����ָ��
 * @����˵����len : ��Ҫ���͵����ݳ���
 * @�������أ��ɹ����ط��͵��ֽ�����ʧ�ܷ���-1
 * @���÷�����cli.TCP_Send(buf, sizeof(buf));
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
ssize_t TCP_Client::TCP_Send(const void *buf, size_t len)
{
    ssize_t ret = send(this->sockfd, buf, len, 0);
    if (ret < 0)
    {
        perror("TCP_cli_send:");
    }
    return ret;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�ssize_t TCP_Client::TCP_Recv(void *buf, size_t len)
 * @����˵����TCP �ͻ�����Ķ�ȡ��Ϣ����
 * @����˵����buf : �������ݻ�����ָ��
 * @����˵����len : ��Ҫ���յ����ݳ���
 * @�������أ��ɹ����ض�ȡ���ֽ�����ʧ�ܷ���-1
 * @���÷�����cli.TCP_Recv(buf, sizeof(buf));
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
ssize_t TCP_Client::TCP_Recv(void *buf, size_t len)
{
    ssize_t ret = recv(this->sockfd, buf, len, 0);
    if (ret < 0)
    {
        perror("TCP_cli_recv:");
    }
    return ret;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�int TCP_Client::TCP_Get_socket()
 * @����˵����TCP �ͻ�����Ļ�ȡ�׽��ֺ���
 * @����˵������
 * @�������أ������׽���
 * @���÷�����int sockfd = cli.TCP_Get_socket();
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int TCP_Client::TCP_Get_socket()
{
    return this->sockfd;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�TCP_Client::~TCP_Client()
 * @����˵����TCP �ͻ��������������
 * @����˵������
 * @�������أ���
 * @���÷����������Ķ����������ڽ�����ϵͳ�Զ�����
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
TCP_Client::~TCP_Client()
{
    close(this->sockfd);
}
