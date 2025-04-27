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
#include "LQ_Uart.hpp"  

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�LS_UART::LS_UART()
 * @����˵������������޲ι��캯��
 * @����˵������
 * @�������أ���
 * @���÷�����LS_UART uart();
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
LS_UART::LS_UART()
{

}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�LS_UART::LS_UART(const string &uart, speed_t baudrate, uint8_t stop, uint8_t data, uint8_t check)
                : BaudRate(baudrate), Stop(stop), Data(data), Check(check)
 * @����˵����������Ĵ��ι��캯��
 * @����˵����uart     : ��ʹ�õĴ����豸�ļ�
 * @����˵����baudrate : ����������
 *      ��ѡ������: B0       B50      B75      B110     B134     B150
 *                   B200     B300     B600     B1200    B1800    B2400
 *                   B4800    B9600    B19200   B38400   B57600   B115200
 *                   B230400  B460800  B500000  B576000  B921600
 * @����˵����stop     : ֹͣλ����
 *      ��ѡ��ֹͣλ: LS_UART_STOP1 --> 1λֹͣλ
 *                   LS_UART_STOP2 --> 2λֹͣλ
 * @����˵����data     : ����λ����
 *      ��ѡ������λ: LS_UART_DATA5 --> 5λ����λ
 *                   LS_UART_DATA6 --> 6λ����λ
 *                   LS_UART_DATA7 --> 7λ����λ
 *                   LS_UART_DATA8 --> 8λ����λ
 * @����˵����check    : У��λ����
 *      ��ѡ��У��λ: LS_UART_NONE --> ��У��
 *                   LS_UART_ODD  --> żУ��
 *                   LS_UART_EVEN --> ��У��
 * @�������أ���
 * @���÷�����LS_UART uart(LS_UART1, B19200, LS_UART_STOP1, LS_UART_DATA8, LS_UART_NONE);
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
LS_UART::LS_UART(const string &uart, speed_t baudrate, uint8_t stop, uint8_t data, uint8_t check)
 : BaudRate(baudrate), Stop(stop), Data(data), Check(check)
{
    // �򿪴����豸�ļ�
    this->uart_fd = open(uart.c_str(), O_RDWR);  
    if (this->uart_fd == -1) {  
        printf("Unable to open dev!\n");  
        return;
    }
    // �������ó�ʼ��
    memset(&this->ts, 0, sizeof(this->ts));
    if (tcgetattr(this->uart_fd, &this->ts) != 0)
    {
        perror("Error tcgetattr");
        close(this->uart_fd);
        return;
    }
    this->ts.c_cflag &= ~CRTSCTS;                       // ��Ӳ��������
    this->ts.c_cflag |= CREAD | CLOCAL;                 // �򿪽�����������modem������
    this->ts.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);// ԭʼ����  
    this->ts.c_iflag &= ~(IXON | IXOFF | IXANY);		// �����������  
    this->ts.c_oflag &= ~OPOST; 			        	// ԭʼ���  
    this->ts.c_cc[VMIN] = 0; 							// ��ȡʱ���ȴ�  
    this->ts.c_cc[VTIME] = 5; 			    		    // 0.5�볬ʱ
    // ���ò�����
    this->SetBaudrate(this->BaudRate);
    // ����ֹͣλ
    this->SetStopBit(this->Stop);
    // ��������λ
    this->SetDataBit(this->Data);
    // ����У��λ
    this->SetChecBit(this->Check);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void LS_UART::SetBaudrate(speed_t baudrate)
 * @����˵�������ô��ڲ�����
 * @����˵����baudrate : ����������
 * @�������أ���
 * @���÷�����uart.SetBaudrate(B19200);
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LS_UART::SetBaudrate(speed_t baudrate)
{
    this->BaudRate = baudrate;
    cfsetispeed(&this->ts, baudrate);   // �������벨����
    cfsetospeed(&this->ts, baudrate);   // �������������
    if (tcsetattr(this->uart_fd, TCSANOW, &this->ts) != 0)
    {
        perror("Error set Baudrate");
        return;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void LS_UART::SetStopBit(uint8_t stop)
 * @����˵�������ô���ֹͣλ
 * @����˵����stop : ֹͣλ����
 * @�������أ���
 * @���÷�����uart.SetStopBit(LS_UART_STOP1);
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LS_UART::SetStopBit(uint8_t stop)
{
    this->Stop = stop;
    switch(this->Stop)
    {
        case LS_UART_STOP1:
            this->ts.c_cflag &= ~CSTOPB;break;  // ֹͣλΪ1
        case LS_UART_STOP2:
            this->ts.c_cflag |= CSTOPB;break;   // ֹͣλΪ2
        default:
            printf("Stop bit flag setting error");
            break;
    }
    if (tcsetattr(this->uart_fd, TCSANOW, &this->ts) != 0)
    {
        perror("Error set Stop Bit");
        return;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void LS_UART::SetDataBit(uint8_t data)
 * @����˵�������ô�������λ
 * @����˵����data : ֹͣλ����
 * @�������أ���
 * @���÷�����uart.SetDataBit(LS_UART_DATA8);
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LS_UART::SetDataBit(uint8_t data)
{
    this->Data = data;
    this->ts.c_cflag &= ~CSIZE;    // �������λ��С����
    switch(this->Data)
    {
        case LS_UART_DATA5: // ����λΪ5
            this->ts.c_cflag |= CS5;break;
        case LS_UART_DATA6: // ����λΪ6
            this->ts.c_cflag |= CS6;break;
        case LS_UART_DATA7: // ����λΪ7
            this->ts.c_cflag |= CS7;break;
        case LS_UART_DATA8: // ����λΪ8
            this->ts.c_cflag |= CS8;break;
        default:
            printf("Data bit flag setting error");
            break;
    }
    if (tcsetattr(this->uart_fd, TCSANOW, &this->ts) != 0)
    {
        perror("Error set Stop Bit");
        return;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void LS_UART::SetChecBit(uint8_t check)
 * @����˵�������ô���У��λ
 * @����˵����check : ֹͣλ����
 * @�������أ���
 * @���÷�����uart.SetChecBit(LS_UART_NONE);
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LS_UART::SetChecBit(uint8_t check)
{
    this->Check = check;
    this->ts.c_cflag &= ~PARENB;    // ���У��λ
    switch (this->Check)
    {
        case LS_UART_NONE:  // ��У��
            break;
        case LS_UART_ODD:   // żУ��
            this->ts.c_cflag |= PARENB;
            this->ts.c_cflag &= PARODD;
            break;
        case LS_UART_EVEN:  // ��У��
            this->ts.c_cflag |= PARENB;
            this->ts.c_cflag |= PARODD;
            break;
        default:
            printf("Check bit flag setting error");
            break;
    }
    if (tcsetattr(this->uart_fd, TCSANOW, &this->ts) != 0)
    {
        perror("Error set Check Bit");
        return;
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�ssize_t LS_UART::WriteData(const char *buf, ssize_t len)
 * @����˵�������ô��ڷ��ͺ���
 * @����˵����buf : ���ͻ�����
 * @����˵����len : �������ݳ���
 * @�������أ���
 * @���÷�����uart.WriteData(buf, sizeof(buf));
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
ssize_t LS_UART::WriteData(const char *buf, ssize_t len)
{
    return write(this->uart_fd, buf, len);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�ssize_t LS_UART::ReadData(char *buf, ssize_t len)
 * @����˵�������ô��ڽ��պ���
 * @����˵����buf : ���ջ�����
 * @����˵����len : �������ݳ���
 * @�������أ���
 * @���÷�����uart.ReadData(buf, sizeof(buf));
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
ssize_t LS_UART::ReadData(char *buf, ssize_t len)
{
    return read(this->uart_fd, buf, len);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�LS_UART::~LS_UART()
 * @����˵������������������캯��
 * @����˵������
 * @�������أ���
 * @���÷����������Ķ����������ڽ�����ϵͳ�Զ�����
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
LS_UART::~LS_UART()
{
    close(this->uart_fd);
}
