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

@修改日期：2025-03-27
@修改内容：
@注意事项：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "LQ_TCP_Client.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：TCP_Client::TCP_Client()
 * @功能说明：TCP 客户端类的无参构造函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：TCP_Client cli;
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
TCP_Client::TCP_Client()
{
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：TCP_Client::TCP_Client()
 * @功能说明：TCP 客户端类的有参构造函数
 * @参数说明：ip   : 连接服务器的 IP 地址
 * @参数说明：port : 连接服务器的端口号
 * @函数返回：无
 * @调用方法：TCP_Client cli("127.0.0.1", 6666);
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
TCP_Client::TCP_Client(const string &ip, uint16_t port)
{
    this->TCP_client_init(ip, port);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：TCP_Client::TCP_client_init(const string &ip, uint32_t port)
 * @功能说明：TCP 客户端类的初始化函数
 * @参数说明：ip   : 连接服务器的 IP 地址
 * @参数说明：port : 连接服务器的端口号
 * @函数返回：成功返回0，套接字创建失败返回-1，服务器IP配置失败时返回-2，服务器连接失败返回-3
 * @调用方法：cli.TCP_client_init("127.0.0.1", 6666);
 * @备注说明：设置地址重用 -- 当服务器结束之后，再次运行会出现 bind 错误(地址被占用)，
 *                          原因是关闭套接字时，系统需要一个 TIME_WAIT 时间之后才可
 *                          以再次使用，使用地址重用函数可以解决这个问题
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8_t TCP_Client::TCP_client_init(const string &ip, uint16_t port)
{
    // 创建套接字
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd == -1)
    {
        perror("tcp_cli_socket:");
        return -1;
    }
    // 设置地址重用
    int val = 1;
    setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    // 连接服务器
    struct sockaddr_in seraddr;
    memset(&seraddr, 0, sizeof(seraddr));
    seraddr.sin_family = AF_INET;   // 将地址族设置为 AF_INET，表示使用 IPv4 地址
    seraddr.sin_port = htons(port); // 设置服务器端口号，使用 htons 将主机字节序转为网络字节序
    // 设置服务器 IP 地址
    if (inet_aton(ip.c_str(), &seraddr.sin_addr) == 0)
    {
        perror("tcp_cli_inet_aton:");
        close(this->sockfd);
        return -2;
    }
    // 连接服务器
    if (connect(this->sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) == -1)
    {
        perror("tcp_cli_connect:");
        close(this->sockfd);
        return -3;
    }

    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：ssize_t TCP_Client::TCP_Send(const void *buf, size_t len)
 * @功能说明：TCP 客户端类的发送消息函数
 * @参数说明：buf : 发送数据缓冲区指针
 * @参数说明：len : 需要发送的数据长度
 * @函数返回：成功返回发送的字节数，失败返回-1
 * @调用方法：cli.TCP_Send(buf, sizeof(buf));
 * @备注说明：无
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
 * @函数名称：ssize_t TCP_Client::TCP_Recv(void *buf, size_t len)
 * @功能说明：TCP 客户端类的读取消息函数
 * @参数说明：buf : 接收数据缓冲区指针
 * @参数说明：len : 需要接收的数据长度
 * @函数返回：成功返回读取的字节数，失败返回-1
 * @调用方法：cli.TCP_Recv(buf, sizeof(buf));
 * @备注说明：无
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
 * @函数名称：int TCP_Client::TCP_Get_socket()
 * @功能说明：TCP 客户端类的获取套接字函数
 * @参数说明：无
 * @函数返回：返回套接字
 * @调用方法：int sockfd = cli.TCP_Get_socket();
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int TCP_Client::TCP_Get_socket()
{
    return this->sockfd;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @函数名称：TCP_Client::~TCP_Client()
 * @功能说明：TCP 客户端类的析构函数
 * @参数说明：无
 * @函数返回：无
 * @调用方法：创建的对象生命周期结束后系统自动调用
 * @备注说明：无
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
TCP_Client::~TCP_Client()
{
    close(this->sockfd);
}
