#include "main.hpp"
#include<ctime>

HWGpio beep(61, GPIO_Mode_Out);

int main(){
    beep.SetGpioValue(0);   // 关闭蜂鸣器

    /* motor PWM */
    SetPWM LMotorPWM(PWM1, 100000, 10, "inversed"); // TIM2_CH1 GPIO87
    SetPWM RMotorPWM(PWM2, 100000, 10, "inversed"); // TIM2_CH2 GPIO88
    /* motor dir GPIO */
    HWGpio LMotoDir(74, GPIO_Mode_Out); // CAN3_RX  left motor dir
    HWGpio RMotoDir(75, GPIO_Mode_Out); // CAN3_TX  right motor dir
    /* servo */
    GtimPwm ServoPWM1(88, 2, LS_GTIM_INVERSED, 2000000 - 1, 150000); // TIM2_CH2 GPIO88
    GtimPwm ServoPWM2(89, 3, LS_GTIM_INVERSED, 2000000 - 1, 150000); // TIM2_CH3 GPIO89
    ServoPWM1.Enable();
    ServoPWM2.Enable();

    // encoder GPIO
    LS_PwmEncoder EncodersL(0, 73);
    LS_PwmEncoder EncodersR(3, 72);

    // PID 控制器
    PID LPID;
    PID RPID;

    // 初始化PID参数
    LPID.init(30000, 1000000); // 最大输出和最大误差积分
    RPID.init(30000, 1000000);

    // 设置PID参数（需要根据实际情况调整）
    LPID.setParams(20.0, 5, 10.0); // 左轮PID参数
    RPID.setParams(20.0, 5, 10.0); // 右轮PID参数

    // 设置目标速度（单位：每秒脉冲数）
    LPID.setTarget(7.0f); // 左轮目标速度
    RPID.setTarget(7.0f); // 右轮目标速度

    // 设置电机方向
    LMotoDir.SetGpioValue(0);
    RMotoDir.SetGpioValue(0);

    // 启动电机
    LMotorPWM.Enable();
    RMotorPWM.Enable();

    // 创建一个摄像头
    VideoCapture cap(0);
    if (!cap.isOpened())
    {
        cerr << "Error open video stream" << endl;
        return 0;
    }
    // 设置视频流编码器
    cap.set(cv::CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    // 设置摄像头图像宽高和帧率
    cap.set(CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CAP_PROP_FRAME_HEIGHT, 160);
    cap.set(CAP_PROP_FPS, 30);
    // 获取摄像头图像宽高和帧率
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    double frame_fps = cap.get(cv::CAP_PROP_FPS);
    printf("frame:%d*%d, fps:%3f", frame_width, frame_height, frame_fps);
    sleep(5);
    // 获取视频流
    Mat frame;

    while(true)
    {
        clock_t t0=clock();
        cap.read(frame);    // cap >> frame;
        if (frame.empty())
        {
            cerr << "Error read frame" << endl;
            break;
        }

        clock_t t1=clock();

        Image img(frame);
        img.getEdge();
        img.getRoadType();
        double dir=img.getDir();

        clock_t t2=clock();
        // cout<<dir<<"\n";

        double min_angle=-acos(-1)/4;
        double max_angle=acos(-1)/4;
        double angle=atan(dir);
        int duty_cycle_val=150000+15000*(angle/max_angle);
        if(duty_cycle_val>165000)
            duty_cycle_val = 165000;
        if(duty_cycle_val<135000)
            duty_cycle_val = 135000;
        //cout<<dir<<"=="<<duty_cycle_val<<endl;
        //int duty_cycle_val=165000+30000*(max_angle-angle);

        ServoPWM1.SetDutyCycle(duty_cycle_val);
        ServoPWM2.SetDutyCycle(duty_cycle_val);

        // 获取左右编码器的当前速度
        float leftSpeed = EncodersL.Update();
        float rightSpeed = EncodersR.Update();

        // 更新PID控制器
        LPID.updateSpeed(leftSpeed);
        RPID.updateSpeed(rightSpeed);

        // 获取PID控制器的输出值（占空比）
        float leftDutyCycle = LPID.getOutput();
        float rightDutyCycle = RPID.getOutput();

        // 设置电机的PWM占空比
        LMotorPWM.SetDutyCycle(leftDutyCycle);
        RMotorPWM.SetDutyCycle(rightDutyCycle);

        // 打印当前速度和目标速度
        // cout << " Left Speed = " << leftSpeed
        //      << ", Duty Cycle = " << leftDutyCycle 
        //      << ",Right Speed = " << rightSpeed
        //      << ", Duty Cycle = " << rightDutyCycle << endl;

        clock_t t3=clock();

        cout<<t1-t0<<" "<<t2-t1<<" "<<t3-t2<<"\n";

        usleep(1000); // 延时1ms
    }
    return 0;
}