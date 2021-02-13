/*
 * TalonXVIII_main.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: Gaelhawks
 */
#include "WPILib.h"
#include "TalonXVIII_main.h"

/**
 * This is a demo program showing the use of the RobotDrive class.
 */
TalonXVIII::TalonXVIII()
{
	joystick = new Joystick(0);
	gamepad = new Joystick(1);
	pdp = new PowerDistributionPanel();

	shooterCam = new PixyJunk(PIXY_BOILER_I2C_ADDRESS);
	gearCam = new PixyJunk(PIXY_GEAR_I2C_ADDRESS);
	shooterTargeting = false;
	shooterAiming = false;
    gearTargeting = false;

	lightController = new LightController(this);
	drive = new DriveTrain(this);
	distance = new DriveDistance();
	gatherer = new FuelGrab(this);
	shooter = new FuelLauncher(this);
	gear = new GearHandling(this);
	DemoModeChooser = new SendableChooser<int*>;
	AutoModeChooser = new SendableChooser<int*>;
	LiftChooser = new SendableChooser<int*>;
	ShooterChooser = new SendableChooser<int*>;

	ropeClimber = new Climber(this);

	dashCounter = 0;
	loopTime = LOOPTIME;
	pixyTime = PIXYTIME;
	startTime = 0.0;
	curTime = 0.0;
	waitTime = 0.0;
	loopOverTime = false;
	NumLoopsOverTime = 0;
	loopCount = 0;
	pixyStart = 0.0;
	pixyEnd = 0.0;
	boilerBlocks = 0;
	gearBlocks = 0;

	rotateCmd = 0.0;
	driveCmd = 0.0;
	sideCmd = 0.0;
	autoStage = 0;
	autoMode = 0;
	liftSelected = 0;
	isShooting = 0;
	isBlueAlliance = false;

	firstTime = true;
	buttonPressed = false;
	resyncPressed = false;
	isJoystickCmd = false;
	isAuto = false;
}

void TalonXVIII::RobotInit()
{
	CameraServer::GetInstance()->StartAutomaticCapture();

	/*DemoModeChooser->AddDefault("Normal Level (Full Drive)", &MODE_NORMAL);
	DemoModeChooser->AddObject("Beginner Level (25% Drive)", &MODE_BEGINNER);
	DemoModeChooser->AddObject("Novice Level (50% Drive)", &MODE_NOVICE);
	DemoModeChooser->AddObject("Intermediate Level (75% Drive)", &MODE_INTERMEDIATE);
	SmartDashboard::PutData("Demonstration Mode:", DemoModeChooser);*/

	AutoModeChooser->AddDefault("Reach Baseline", &AUTO_REACH_BASELINE);
	AutoModeChooser->AddObject("Deliver Gear", &AUTO_DELIVER_GEAR);
	AutoModeChooser->AddObject("40 ball hoptonomous", &AUTO_SHOOT_FORTY);
	AutoModeChooser->AddObject("Just Shoot", &AUTO_TURN_AND_SHOOT);
	SmartDashboard::PutData("Autonomous Mode:", AutoModeChooser);

	LiftChooser->AddDefault("Center Lift", &centerLift);
	LiftChooser->AddObject("Boiler Side", &boilerSide);
	LiftChooser->AddObject("Far Side", &farSide);
	SmartDashboard::PutData("Lift Position:", LiftChooser);

	ShooterChooser->AddDefault("Don't Shoot", &dontShoot);
	ShooterChooser->AddObject("Shoot", &shoot);
	SmartDashboard::PutData("Shooting:", ShooterChooser);

	if(frc::DriverStation::GetInstance().GetAlliance() == DriverStation::kBlue)
	{
		isBlueAlliance = true;
	}
	else
	{
		isBlueAlliance = false;
	}
	ServiceDash();
}

// Runs when disabled
void TalonXVIII::Disabled()
{

}

/**
 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
 * below the Gyro
 *
 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
 * If using the SendableChooser make sure to add them to the chooser code above as well.
 */
void TalonXVIII::Autonomous()
{
	autoMode = *((int*)AutoModeChooser->GetSelected());
	SmartDashboard::PutNumber("Auto Mode Selected: ", autoMode);
	liftSelected = *((int*)LiftChooser->GetSelected());
	SmartDashboard::PutNumber("Lift Selected: ", liftSelected);
	isShooting = *((int*)ShooterChooser->GetSelected());
	SmartDashboard::PutNumber("Is Shooting:", isShooting);

	if(firstTime)
	{
		firstTime = false;
	}
	isJoystickCmd = false;
	autoStage = 0;
	drive->GyroOn();
	//drive->GyroOff();
	shooterTargeting = false;
	shooterAiming = false;
    gearTargeting = false;
    isAuto = true;

	Wait(0.1);

#ifdef CHECK_LOOPTIMING
	double endTime;
	double totalLoopTime;
#endif


	int loopCounter = 0;
	double initialStartTime;
	initialStartTime = GetTime();
	startTime = initialStartTime - loopTime;
	if(frc::DriverStation::GetInstance().GetAlliance() == DriverStation::kBlue)
	{
		isBlueAlliance = true;
	}
	else
	{
		isBlueAlliance = false;
	}
	printf("Alliance: %s", isBlueAlliance?"Blue":"Red");

	while (IsAutonomous() && IsEnabled())
	{
		startTime += loopTime;
		loopCounter++;

		isAuto = true;

		switch(autoMode)
		{
			case 0:
			default:
				break;

			case 1://Reach Baseline
				AutoModeReachBaseLine();
				break;

			case 2://Deliver Gear
				if(liftSelected == boilerSide)
				{
					AutoModeSideGear(true, isShooting);
				}
				else if(liftSelected == farSide)
				{
					AutoModeSideGear(false, isShooting);
				}
				else
				{
					AutoModeFrontGear(isShooting);
				}
				break;

			case 3:
				AutoModeHopperAndShootDC();
				break;

			case 4:
				AutoJustShoot();
				break;
		}
		// ENFORCE PIXY TIMING
		pixyEnd = GetTime();

		//waitTime = pixyTime - (pixyEnd - pixyStart);
		waitTime = pixyTime - (pixyEnd - startTime);
		if (waitTime < 0.0)
		{
			printf("WARNING! PIXY IS OVERTIME by %f seconds\n", waitTime*-1);
			//loopOverTime = true;
			//NumLoopsOverTime++;
		}
		else
		{
			Wait(waitTime);
			//loopOverTime = false;

#ifdef CHECK_LOOPTIMING
			printf("\nPixy Elapsed: %f     Wait Time: %f\n", pixyEnd - pixyStart, waitTime);
#endif
		}


		/*
		** COMMUNITY SERVICE
		*/
		drive->DriveControl(driveCmd, rotateCmd, sideCmd, 0.0, 0.0, 0.0, false, false, isAuto);
		CommunityService();
		ServiceDash();

		// ENFORCE LOOP TIMING
		curTime = GetTime();
		waitTime = loopTime - (curTime - startTime);
		if (waitTime < 0.0)
		{
			printf("WARNING! LOOP IS OVERTIME by %f seconds\n", waitTime*-1);
			loopOverTime = true;
			NumLoopsOverTime++;
		}
		else
		{
			Wait(waitTime);
			loopOverTime = false;

#ifdef CHECK_LOOPTIMING
			endTime = GetTime();
			totalLoopTime = endTime - startTime;
			printf("startTime: %f  curTime : %f  endTime: %f	Wait Time: %f  This Loop Time: %f  Total Delta Time: %f [avg: %f] \n",
					startTime, curTime, endTime, waitTime, totalLoopTime, endTime-initialStartTime, (endTime-initialStartTime)/loopCounter);
#endif

		}
	}
	Omnicide();
};


/**
 * Runs the motors with arcade steering.
 */
void TalonXVIII::OperatorControl()
{
	int loopCounter=0;
	if(firstTime)
	{
		firstTime = false;
	}
	isJoystickCmd = true;
	shooterTargeting = false;
	shooterAiming = false;
    gearTargeting = false;

	if(frc::DriverStation::GetInstance().GetAlliance() == DriverStation::kBlue)
	{
		isBlueAlliance = true;
	}
	else
	{
		isBlueAlliance = false;
	}

#ifdef CHECK_LOOPTIMING
	double endTime;
	double totalLoopTime;
#endif

	double initialStartTime;
	initialStartTime = GetTime();
	startTime = initialStartTime - loopTime;

	while (IsOperatorControl() && IsEnabled())
	{
		startTime += loopTime;
		loopCounter++;

		// CLIMBER stuff
		if (gamepad->GetRawButton(BUTTON_BEGIN_CLIMB))
		{
			ropeClimber->ClimbUp();
		}
		if (gamepad->GetRawButton(BUTTON_STOP_CLIMB))
		{
			ropeClimber->StopClimb();
		}

		// GEAR stuff
		if (gamepad->GetPOV(0) == DPAD_OPEN_GEARDOOR)
		{
			gear->GateUp();
		}
		if (gamepad->GetPOV(0) == DPAD_CLOSE_GEARDOOR)
		{
			gear->GateDown();
		}
		if (gamepad->GetRawButton(BUTTON_GRAB_GEAR))
		{
			gear->CollectGear();
		}
		else
		{
			if(gamepad->GetRawButton(BUTTON_EJECT_GEAR))
			{
				gear->EjectGear();
			}
			else
			{
				gear->StopCollector();
			}
		}
		/*if (gamepad->GetRawButton(BUTTON_STOP_GEARDOOR))
		{
			gear->StopGate();
		}*/

		// MIXER stuff
		if (gamepad->GetRawButton(BUTTON_START_MIXER))
		{
			shooter->EnableMixer();
		}
		if (gamepad->GetRawButton(BUTTON_STOP_MIXER))
		{
			shooter->StopMixer();
		}
		if (gamepad->GetRawButton(BUTTON_REVERSE_MIXER))
		{
			shooter->ReverseMixerWheel(true);
		} 
		else
		{
			shooter->ReverseMixerWheel(false);
		}

		// GATHERER stuff
		if (gamepad->GetRawButton(BUTTON_START_GATHERER))
		{
			gatherer->GrabFuel();
		}
		else
		{
			gatherer->StopGatherer();
		}

		// SHOOTER stuff
		if (gamepad->GetRawButton(BUTTON_START_LAUNCHER))
		{
			shooter->SpinUpLauncher();
		}
		else
		{
			shooter->StopLauncher();
		}
		/*
		if (gamepad->GetPOV(0) == DPAD_LONG_SHOT_SERVO)
		{
			shooter->LongShot();
		}
		if (gamepad->GetPOV(0) == DPAD_SHORT_SHOT_SERVO)
		{
			shooter->ShortShot();
		}*/

		if (gamepad->GetRawAxis(SHOOTER_UPDOWN_AXIS) <= -0.8)
		{
			// Verify transition from not pressed to pressed
			if(!buttonPressed)
			{
				buttonPressed = true;
				shooter->IncreaseSpeedAdjustment();
			}
		}
		else if (gamepad->GetRawAxis(SHOOTER_UPDOWN_AXIS) >= 0.8)
		{
			// Verify transition from not pressed to pressed
			if(!buttonPressed)
			{
				buttonPressed = true;
				shooter->DecreaseSpeedAdjustment();
			}
		}
		else
		{
			buttonPressed = false;
		}

		if(gamepad->GetRawButton(BUTTON_RESET_SHOOT_CHANGE))
		{
			shooter->ResetSpeedAdjustment();
		}

		//Starting Config
		/*if(joystick->GetRawButton(STARTING_CONFIG))
		{
			RobotStartConfig();
		}*/

		// GYRO stuff
		if(joystick->GetRawButton(ENABLE_GYRO_BUTTON))
		{
			drive->GyroOn();
		}
		else //if(joystick->GetRawButton(DISABLE_GYRO_BUTTON))
		{
			drive->GyroOff();
        }

		/*
		** PIXY and TARGETING OPERATIONS
		*/
		pixyStart = GetTime();
		if	(joystick->GetRawButton(PIXY_RESYNC))
		{
			if(!resyncPressed)
			{
				resyncPressed = true;
				shooterCam->ResyncCamera();
				gearCam->ResyncCamera();
			}
		}
		else
		{
			resyncPressed = false;
		}

		if ((joystick->GetRawButton(BOILER_PIXY_AIMING) || joystick->GetRawButton(BOILER_AUTO_TARGETING)) && (!gearTargeting))//loopCounter%40 == 0
		{
			shooterAiming = true;
			if(joystick->GetRawButton(BOILER_AUTO_TARGETING))
			{
				shooterTargeting = true;
			}
			else
			{
				shooterTargeting = false;
			}
			boilerBlocks = shooterCam->AnalyzeTarget();   //Retrieve up to 100 objects, record the num of objects found in "numBlocks"

			//The following code sprays data to the Driver Station for debug purposes only
			//printf("BOILER pixy: "); //prints number of block to the console
			//shooterCam->PrintBlock(-1); // prints x, y, width, and etc. to the console (the vairables in the block object)
			//shooterCam->PrintBlock(0); // prints x, y, width, and etc. to the console (the vairables in the block object)
			//shooterCam->PrintBlock(1); // prints x, y, width, and etc. to the console (the vairables in the block object)
			//printf("\n"); //new line(space)

			if(boilerBlocks > 0)
			{
				rotateCmd = (BOILER_CENTER_TARGET - shooter->GetTargetCenter()) * K_BOILER_TRACK;
				sideCmd = joystick->GetRawAxis(STRAFE_AXIS);
				isJoystickCmd = false;
			}
			else
			{
			    sideCmd = joystick->GetRawAxis(STRAFE_AXIS);
				rotateCmd = joystick->GetRawAxis(ROTATE_AXIS);
				isJoystickCmd = true;
			}
		}
		else
		{
			shooterTargeting = false;
			shooterAiming = false;
			if (!gearTargeting)
			{
				sideCmd = joystick->GetRawAxis(STRAFE_AXIS);
				rotateCmd = joystick->GetRawAxis(ROTATE_AXIS);
				isJoystickCmd = true;
			}
		}

		if (joystick->GetRawButton(GEAR_PIXY_TARGETING) && !shooterAiming)//loopCounter%40 == 0
		{
		    gearTargeting = true;
		    gearBlocks = gearCam->AnalyzeTarget();   //Retrieve up to 100 objects, record the num of objects found in "numBlocks"
			//The following code sprays data to the Driver Station for debug purposes only
			//printf("GEAR pixy: ");
			//gearCam->PrintBlock(-1); // prints x, y, width, and etc. to the console (the vairables in the block object)
			//printf("\n"); //new line(space)

			if(gearBlocks > 0)
			{
				rotateCmd = (GEAR_CENTER_TARGET - gear->GetTargetCenter()) * K_GEAR_TRACK;
				sideCmd = joystick->GetRawAxis(STRAFE_AXIS);
				isJoystickCmd = false;
			}
			else
			{
			    sideCmd = joystick->GetRawAxis(STRAFE_AXIS);
				rotateCmd = joystick->GetRawAxis(ROTATE_AXIS);
				isJoystickCmd = true;
			}
		}
		else
		{
			gearTargeting = false;
			if (!shooterAiming)
			{
				sideCmd = joystick->GetRawAxis(STRAFE_AXIS);
				rotateCmd = joystick->GetRawAxis(ROTATE_AXIS);
				isJoystickCmd = true;
			}
		}
		// ENFORCE PIXY TIMING
		pixyEnd = GetTime();

		//waitTime = pixyTime - (pixyEnd - pixyStart);
		waitTime = pixyTime - (pixyEnd - startTime);
		if (waitTime < 0.0)
		{
			printf("WARNING! PIXY IS OVERTIME by %f seconds\n", waitTime*-1);
			//loopOverTime = true;
			//NumLoopsOverTime++;
		}
		else
		{
			Wait(waitTime);
			//loopOverTime = false;

#ifdef CHECK_LOOPTIMING
			printf("\nPixy Elapsed: %f     Wait Time: %f\n", pixyEnd - pixyStart, waitTime);
#endif
		}


		/*
		** COMMUNITY SERVICE
		*/
		if (!shooterTargeting)
		{
			shooter->ShotAdjuster(joystick->GetRawAxis(SERVO_AXIS));
		}

		//drive->DriveControl((joystick->GetRawAxis(SPEED_AXIS) * -1.0), rotateCmd, sideCmd, joystick->GetRawButton(ENABLE_PRECISE_TURN), isJoystickCmd, false);
		drive->DriveControl((joystick->GetRawAxis(SPEED_AXIS) * -1.0), rotateCmd, sideCmd, gamepad->GetRawAxis(3), gamepad->GetRawAxis(0), gamepad->GetRawAxis(2), false, isJoystickCmd, false);
		CommunityService();
		ServiceDash();
		// ENFORCE LOOP TIMING
		curTime = GetTime();
		waitTime = loopTime - (curTime - startTime);
		if (waitTime < 0.0)
		{
			printf("WARNING! LOOP IS OVERTIME by %f seconds\n", waitTime*-1);
			loopOverTime = true;
			NumLoopsOverTime++;
		}
		else
		{
			Wait(waitTime);
			loopOverTime = false;

#ifdef CHECK_LOOPTIMING
			endTime = GetTime();
			totalLoopTime = endTime - startTime;
			//printf("startTime: %f  curTime : %f  endTime: %f	Wait Time: %f  This Loop Time: %f  Total Delta Time: %f [avg: %f] \n",
			//		startTime, curTime, endTime, waitTime, totalLoopTime, endTime-initialStartTime, (endTime-initialStartTime)/loopCounter);
			printf("Wait Time: %f  \n",	   waitTime);
#endif
		}
	}
	Omnicide();
}

/**
 * Runs during test mode
 */
void TalonXVIII::Test()
{

}


double TalonXVIII::Limit(double min, double max, double curValue)
{
	if (curValue > max)
		return max;
	if (curValue < min)
		return min;
	return curValue;
}

void TalonXVIII::RobotStartConfig()
{
	gear->StartingConfig();
	gatherer->StartingConfig();
	shooter->StartingConfig();
	ropeClimber->StartingConfig();
	lightController->StartingConfig();
}

void TalonXVIII::Omnicide()
{
	drive->StopAll();
	gatherer->StopAll();
	shooter->StopAll();
	gear->StopAll();
	ropeClimber->StopAll();
	lightController->StopAll();
	distance->LocalReset();

	buttonPressed = false;
}

void TalonXVIII::CommunityService()
{
	gatherer->Service();
	shooter->Service();
	gear->Service();
	ropeClimber->Service();
	//pixyCamera->Service();
	lightController->Service();
}

void TalonXVIII::ServiceDash()
{
	if (dashCounter == 10)
	{
		SmartDashboard::PutBoolean("loopOverTime: ", loopOverTime);
		SmartDashboard::PutNumber("waitTime: ", waitTime);
		SmartDashboard::PutData("Demonstration Mode:", DemoModeChooser);
	    ropeClimber->UpdateDash();
	    shooter->UpdateDash();
	    gatherer->UpdateDash();
	    drive->UpdateDash();
	    gear->UpdateDash();
	    distance->UpdateDash();
        lightController->UpdateDash();
	    dashCounter = 0;
	}
	else
	{
		dashCounter++;
	}	
}


START_ROBOT_CLASS(TalonXVIII)




