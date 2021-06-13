/*
 * PID.h
 *
 *  Created on: Jan 8, 2021
 *      Author: DucThang
 */
#include "main.h"
#ifndef SRC_PID_H_
#define SRC_PID_H_
float Error_L,Error_R;
typedef struct {
	/*Motor max RPM*/

	int MaxRPM;

	/*Controller gains*/
	float Kp;
	float Ki;
	float Kd;

	/*Output limits*/
	int OutMin;
	int OutMax;

	/*Sample time (in ms)*/

	int T;

	/*Controller "memory" */

	float previous_error;
	float propotional;
	float integral;
	float derivative;


	/*Controller output*/
	int adder_out;

} PIDController;

void PIDController_L_Init (PIDController *pid_L);
void PIDController_R_Init (PIDController *pid_R);
void PIDController_Car_Init (PIDController *Car);
int RPM_Convert_PWM_L (int16_t RPM,float EncoderRead,PIDController *pid_L);
int RPM_Convert_PWM_R (int16_t RPM,float EncoderRead,PIDController *pid_R);
int16_t Line_Follower_PID (int Setpoint ,int Error,PIDController *Car);
#endif /* SRC_PID_H_ */
