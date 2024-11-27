#ifndef __PID_H
#define __PID_H

typedef struct{
	float now;
	float last;
	float integral;
}Error;

typedef struct{
	// kp ki kd
	float kp, ki, kd;
	
	// TIO
	float target, input, output;
	
	// Error
	Error error;
	
	//limit
	float MAX_OUTPUT;
	float MAX_ERROR_INTEGRAL;
} PID;


void initPID(PID* pid, const float MAX_OUTPUT, const float MAX_E_I);
void updatePID(PID* pid, float input);
void setPIDParam(PID* pid, float kp, float ki, float kd);
void setPIDTarget(PID* pid, float target);



#endif /* __PID_H */

