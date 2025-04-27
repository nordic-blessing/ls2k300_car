#include "pid.hpp"

// 构造函数
PID::PID() {
    // 默认初始化
    kp = ki = kd = 0.0f;
    target = input = output = output_last = 0.0f;
    error.now = error.last = error.pre = error.integral = 0.0f;
    MAX_OUTPUT = MAX_ERROR_INTEGRAL = 0.0f;
}

// 初始化PID参数
void PID::init(float MAX_OUTPUT, float MAX_E_I) {
    kp = ki = kd = 0.0f;
    target = input = output = output_last = 0.0f;
    error.now = error.last = error.pre = error.integral = 0.0f;
    this->MAX_OUTPUT = MAX_OUTPUT;
    this->MAX_ERROR_INTEGRAL = MAX_E_I;
}

// 设置PID参数
void PID::setParams(float kp, float ki, float kd) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

// 设置目标值
void PID::setTarget(float target) {
    this->target = target;
}

// 限幅函数
float PID::limitOutput(float value) const {
    if (value > MAX_OUTPUT) return MAX_OUTPUT;
    if (value < -MAX_OUTPUT) return -MAX_OUTPUT;
    return value;
}

// 更新PID控制器（增量式）
void PID::updateSpeed(float input) {
    this->input = input;
    error.now = target - input;

    output = output_last
             + kp * (error.now - error.last)
             + ki * error.now
             + kd * (error.now - 2 * error.last + error.pre);
    output = limitOutput(output);

    error.pre = error.last;
    error.last = error.now;
    output_last = output;
}

// 更新PID控制器（位置式）
void PID::updatePosition(float input) {
    this->input = input;
    error.now = target - input;
    error.integral += error.now;
    error.integral = limitOutput(error.integral);

    output = kp * error.now
             + ki * error.integral
             + kd * (error.now - error.last);
    output = limitOutput(output);

    error.last = error.now;
}