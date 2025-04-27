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
#pragma once

#include <string>

#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

class UDP_Client
{
public:
    /*!
     * @brief   UDP 客户端类的无参构造函数
     */
    UDP_Client();

    /*!
     * @brief   UDP 客户端类的有参构造函数
     * @param   ip   : 连接服务器的 IP 地址
     * @param   port : 连接服务器的端口号
     */
    UDP_Client(const string &ip, uint16_t port);

    /*!
     * @brief   UDP 客户端类的初始化函数
     * @param   ip   : 连接服务器的 IP 地址
     * @param   port : 连接服务器的端口号
     * @return  成功返回0，套接字创建失败返回-1，服务器IP配置失败时返回-2，服务器连接失败返回-3
     */
    uint8_t UDP_client_init(const string &ip, uint16_t port);

    /*!
     * @brief   UDP 客户端类的发送消息函数
     * @param   buf : 发送数据缓冲区指针
     * @param   len : 需要发送的数据长度
     * @return  成功返回发送的字节数，失败返回-1
     */
    ssize_t UDP_Send(const void *buf, size_t len);

    /*!
     * @brief   UDP 客户端类的读取消息函数
     * @param   buf : 接收数据缓冲区指针
     * @param   len : 需要接收的数据长度
     * @return  成功返回读取的字节数，失败返回-1
     */
    ssize_t UDP_Recv(void *buf, size_t len);

    /*!
     * @brief   UDP 客户端类的获取套接字函数
     * @return  返回套接字
     */
    int UDP_Get_socket();

    /*!
     * @brief   UDP 客户端类的获取地址信息结构体
     */
    struct sockaddr_in UDP_Get_sockaddr();

    /*!
     * @brief   UDP 客户端类的析构函数
     */
    ~UDP_Client();
private:

    /*!
     * @brief   UDP 客户端套接字
     */
    int sockfd;

    /*!
     * @brief   UDP 客户端地址信息结构体
     */
    struct sockaddr_in addr;
};
