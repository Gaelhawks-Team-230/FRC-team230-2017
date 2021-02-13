/*
 * DriveTrain.h
 *
 *  Created on: Jan 20, 2017
 *      Author: Gaelhawks
 */
#include "WPILib.h"
#include "Common.h"

#ifndef SRC_DRIVETRAIN_H_
#define SRC_DRIVETRAIN_H_

class TalonXVIII;

#define K_GYRO		 (20.0)
#define K_ROBOT		 (610)
#define TAU_ROBOT	 (0.15)
#define GAMEPAD_GAIN        (0.5)  //(0.75)  //(0.4)
#define GAMEPAD_ROTATE_GAIN (0.5)  //(0.75)  //(0.4)
#define GAMEPAD_STRAFE_GAIN (0.75) //(0.75)  //(0.4)

#define M_CMD			(0.7)
#define GAMEPAD_M_CMD	(0.7)

#define MAX_COMMAND		(1.0)
#define MIN_COMMAND		(-1.0)

class DriveTrain
{
private:

#ifdef PRACTICE_BOT
	Talon *frontLeftMotor;
	Talon *frontRightMotor;
	Talon *backLeftMotor;
	Talon *backRightMotor;
#else
	VictorSP *frontLeftMotor;
	VictorSP *frontRightMotor;
	VictorSP *backLeftMotor;
	VictorSP *backRightMotor;
#endif

	TalonXVIII *mainRobot;

#ifdef USE_GYRO
	ADXRS450_Gyro *gyro;
	double gyroVel;
	double gyroAngle;
	double gyroErr;
	double gyroErrInt;
#endif
	bool gyroOn;

	double frontLeftMotorCmd;
	double frontRightMotorCmd;
	double backLeftMotorCmd;
	double backRightMotorCmd;

	double Limit(double num);

public:
	DriveTrain(TalonXVIII *robot);
	void DriveControl(float speedCmd, float rotateCmd, float sideCmd, float gamepadSpeedCmd, float gamepadRotateCmd, float gamepadSideCmd, bool preciseTurn, bool isJoystickCmd, bool isAuto);
	void ResetGyro(void);
	void GyroOn(void);
	void GyroOff(void);
	double GyroControl(double motorCmd);
	void GetGyroVelocity(void);
	double GetGyroAngle(void);
	void LocalReset(void);
	void UpdateDash(void);
	void StopAll(void);
};



#endif /* SRC_DRIVETRAIN_H_ */
