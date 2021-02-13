/*
 * TalonXVIII_main.h
 *
 *  Created on: Jan 17, 2017
 *      Author: Gaelhawks
 */

#ifndef SRC_TALONXVIII_MAIN_H_
#define SRC_TALONXVIII_MAIN_H_

#include "Common.h"
#include "AutonomousModes.h"
#include "DriveDistance.h"
#include "DriveTrain.h"
#include "DriveDistance.h"
#include "FuelGrab.h"
#include "FuelLauncher.h"
#include "GearHandling.h"
#include "Climber.h"
//#include "TalonPID.h"
#include "PixyCameraI2C.h"
#include "LightController.h"

#define BOILER_CENTER_TARGET	(95)  //(105)  //(110)
#define GEAR_CENTER_TARGET		(160)
#define K_GEAR_TRACK			(-0.005) //(-0.01)
#define K_BOILER_TRACK			(0.0075) //(0.01)


class TalonXVIII: public frc::SampleRobot
{
public:
	frc::SendableChooser<int*> *DemoModeChooser;
	frc::SendableChooser<int*> *AutoModeChooser;
	frc::SendableChooser<int*> *LiftChooser;
	frc::SendableChooser<int*> *ShooterChooser;
	Joystick *joystick;
	bool isJoystickCmd;
	Joystick *gamepad;
    PowerDistributionPanel *pdp;

    Climber *ropeClimber;
    DriveDistance *distance;
	DriveTrain *drive;
	FuelGrab *gatherer;
	FuelLauncher *shooter;
	GearHandling *gear;

	PixyJunk *shooterCam;
	PixyJunk *gearCam;
	bool shooterTargeting;
	bool shooterAiming;
    bool gearTargeting;

	LightController *lightController;

	int dashCounter;
	int loopCount;
	bool isBlueAlliance;
	bool firstTime;
	bool buttonPressed;
	bool resyncPressed;
	bool isAuto;

	double loopTime;
	double startTime;
	double pixyStart;
	double pixyEnd;
	double pixyTime;

	double curTime;
	double waitTime;
	bool loopOverTime;
	int NumLoopsOverTime;

	int autoStage;
	int autoMode;
	int liftSelected;
	int isShooting;
	int boilerBlocks;
	int gearBlocks;

	double rotateCmd;
	double driveCmd;
	double sideCmd;

private:
	//Demo Modes For Robot
	int MODE_NORMAL                    = 0;
	int MODE_BEGINNER                  = 1;
	int MODE_NOVICE                    = 2;
	int MODE_INTERMEDIATE              = 3;
	// Autonomous positions
	int boilerSide 		= 1;
	int centerLift 		= 0;
	int farSide			= 2;
    // Autonomous Modes
	int AUTO_REACH_BASELINE		= 1;
	int AUTO_DELIVER_GEAR		= 2;
	int AUTO_SHOOT_FORTY		= 3;
	int AUTO_TURN_AND_SHOOT		= 4;
	// Autonomous Versions
	int dontShoot 		= 0;
	int shoot 			= 1;


public:
	TalonXVIII();
	void RobotInit(void);
	void Autonomous(void);
	void OperatorControl(void);
	void Disabled(void);
	void Test(void);
	double Limit(double min, double max, double curValue);
	void RobotStartConfig(void);
	void Omnicide(void);
	void CommunityService(void);
	void ServiceDash(void);

	// Autonomous Mode Functions: (see AutonomousModes.cpp)
	void AutoModeHopperAndShoot(void);
	void AutoModeHopperAndShootDC(void);
	void AutoModeReachBaseLine(void);
	void AutoJustShoot(void);
	void AutoModeFrontGear(int isShooting);
	void AutoModeSideGear(bool boilerSide, int isShooting);
	void AutoTrackGear(void);
	void AutoTrackBoiler(void);
};



#endif /* SRC_TALONXVIII_MAIN_H_ */
