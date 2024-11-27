#include "pid.h"
float limit(float input, const float MAX_OUTPUT_ABS){
	input = input > +MAX_OUTPUT_ABS ? +MAX_OUTPUT_ABS : input;
	input = input < -MAX_OUTPUT_ABS ? -MAX_OUTPUT_ABS : input;
	return input;
}

void initPID(PID* pid, const float MAX_OUTPUT, const float MAX_E_I){
	//param
	pid->kp = 0;
    pid->ki = 0;
    pid->kd = 0;
	
	//error
	pid->error.now = 0;
	pid->error.last = 0;
	pid->error.integral = 0;

	//TIO
	pid->target = 0;
	pid->input = 0;
	pid->output = 0;
	
	//limit
	pid->MAX_OUTPUT = MAX_OUTPUT;
	pid->MAX_ERROR_INTEGRAL = MAX_E_I;
}

void updatePID(PID* pid, float input){
	pid->input = input;
	pid->error.now = pid->target - pid->input;
	pid->error.integral += pid->error.now;
	pid->error.integral = limit(pid->error.integral, pid->MAX_ERROR_INTEGRAL);
	
	pid->output = pid->kp * pid->error.now + pid->ki * pid->error.integral + pid->kd * (pid->error.now - pid->error.last);
	pid->output = limit(pid->output, pid->MAX_OUTPUT);
	
	pid->error.last = pid->error.now;
}

void setPIDParam(PID* pid, float kp, float ki, float kd){
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
}

void setPIDTarget(PID* pid, float target){
	pid->target = target;

}