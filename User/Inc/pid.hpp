#ifndef PID_HPP
#define PID_HPP

#include <iostream>
#include <string>

struct Error {
    float now;
    float last;
    float pre;
    float integral;
};

class PID {
public:
    PID();
    void init(float MAX_OUTPUT, float MAX_E_I);
    void setParams(float kp, float ki, float kd);
    void setTarget(float target);
    float limitOutput(float value) const;
    void updateSpeed(float input);
    void updatePosition(float input);
    float getOutput() const { return output; }
    float getTarget() const { return target; }

private:
    float kp, ki, kd;
    float target;
    float input;
    float output, output_last;
    Error error;
    float MAX_OUTPUT, MAX_ERROR_INTEGRAL;
};

#endif