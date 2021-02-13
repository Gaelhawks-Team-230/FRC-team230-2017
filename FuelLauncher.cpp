
#include "WPILib.h"
#include "Common.h"
//#include "TalonXVIII_main.h"
#include "FuelLauncher.h"


// Constructor for FuelLauncher Class
// - Class for the fuel launching mechanism
FuelLauncher::FuelLauncher(TalonXVIII *pRobot)
{

	// Create objects needed by this class
#ifdef PRACTICE_BOT
	launcherMotor = new Talon(PWM_FUELLAUNCHER);
	mixMaster = new Talon(PWM_MIXMASTER);
	loadWheel = new Talon(PWM_INDEXWHEEL);
#else
	launcherMotor = new VictorSP(PWM_FUELLAUNCHER);
	mixMaster = new VictorSP(PWM_MIXMASTER);
	loadWheel = new VictorSP(PWM_INDEXWHEEL);
#endif
	shooterServo = new Servo(PWM_SHOOTER_SERVO);

	launcherEncoder = new Encoder(DIGIN_ENCODER_LAUNCHER_ONE, DIGIN_ENCODER_LAUNCHER_TWO);
	launcherEncoder->SetDistancePerPulse(1.0);
	launcherEncoder->Reset();

	mainRobot = pRobot;

	// Call the class initialization function
	LocalReset();
}

// Set initial values for ALL objects and variables
void FuelLauncher::LocalReset()
{
	// Initialize ALL local variables
	mixCmd = 0.0;
	mixWheelMultiplier = 1.0;
	loadCmd = 0.0;
	launchCmd = 0.0;
	speedCmd = 0.0;
	speedCmdChange = 0.0;
	speedUpLoopCount = 0;

	isShooterEnabled = false;
	isMixerEnabled = false;
	isTargetDetected = false;
	isEncoderBroken = false;
	launcherMode = IDLE;

	xhat = 0.0;
	err = 0.0;
	err2 = 0.0;
	xhatz = 0.0; 	//Last xhat value
	xhatzz = 0.0;
	xhatzzz = 0.0;
	xhat = 0.0; 	//Current Encoder Position
	xdhat = 0.0; 	//(Derivative) (currentPosition - lastPosition)/looptime
	xdhatTustin = 0.0;
	errint = 0.0;

	printCount = 0;
	lutIndex = 0;

	// Initialize ALL new objects
	// example: sampleMotor->Set(0.0);
	launcherMotor->Set(launchCmd);
	mixMaster->Set(mixCmd);
	loadWheel->Set(loadCmd);
	launcherEncoder->Reset();
}

void FuelLauncher::StopAll()
{
	// Stop all motors
	// Reset all local flags that control Service operations
	LocalReset();
}

void FuelLauncher::StartingConfig()
{
	// Bring mechanism into frame perimeter, etc.
}


//Enables the Launcher Service to start launching fuel.
void FuelLauncher::SpinUpLauncher()
{
   isShooterEnabled = true;
   if(launcherMode == IDLE)
   {
	   launcherMode = SPEEDINGUP;
   }
}

//Stops the Launcher.
void FuelLauncher::StopLauncher()
{
   launcherMode = IDLE;
   launchCmd = 0.0;
   isShooterEnabled = false;
   launcherEncoder->Reset();

   StopMixer();
   //speedCmdChange = 0.0;
}

//Starts the MixMaster.
void FuelLauncher::EnableMixer()
{
   isMixerEnabled = true;
}
//Stop the MixMaster From Spinning.
void FuelLauncher::StopMixer()
{
   isMixerEnabled = false;
}
void FuelLauncher::ReverseMixerWheel(bool reversed)
{
	if (reversed)
		mixWheelMultiplier = -1.0;
	else
		mixWheelMultiplier = 1.0;
}

void FuelLauncher::ProcessBoilerTarget()
{
	CompositeBlock *highGoalBlock = mainRobot->shooterCam->GetCompositeTarget();
	//boilerYPos = highGoalBlock->xPosition;
	boilerYPos = highGoalBlock->leftX;   // center of left side
}

double FuelLauncher::GetTargetCenter()
{
	CompositeBlock *highGoalBlock = mainRobot->shooterCam->GetCompositeTarget();
	return highGoalBlock->yPosition;
}

void FuelLauncher::DecreaseSpeedAdjustment()
{
	speedCmdChange += (-1.0 * SPEED_BUMP_INCREMENT);
	speedCmdChange = Limit(LAUNCHER_CHANGE_MIN, LAUNCHER_CHANGE_MAX, speedCmdChange);
}
void FuelLauncher::IncreaseSpeedAdjustment()
{
	speedCmdChange += SPEED_BUMP_INCREMENT;
	speedCmdChange = Limit(LAUNCHER_CHANGE_MIN, LAUNCHER_CHANGE_MAX, speedCmdChange);
}
void FuelLauncher::ResetSpeedAdjustment()
{
	speedCmdChange = 0;
}

void FuelLauncher::OverrideEncoder()
{
	isEncoderBroken = true;
}
void FuelLauncher::CancelEncoderOverride()
{
	isEncoderBroken = false;
}

//Calculate the launcher speed needed to shoot fuel into the top boiler
void FuelLauncher::GenerateSpeedCmd()
{
	if (mainRobot->shooterTargeting)
	{
		if (mainRobot->shooterCam->GetNumBlocks() != 0)
		{
			int n;
			bool isIndexFound = false;

			isTargetDetected = true;
			ProcessBoilerTarget();
			boilerYPos = Limit(lutY[0], lutY[LUT_SIZE-1], boilerYPos);
			lutIndex = 0;
			n = 0;

			while (!isIndexFound && (n < LUT_SIZE-1))
			{
				if (boilerYPos <= lutY[n+1])
				{
					isIndexFound = true;
				}
				n++;
				lutIndex = n;
			}
			deltaYPos = lutY[lutIndex]-lutY[lutIndex-1];
			speedCmd = lutS[lutIndex]*(boilerYPos-lutY[lutIndex-1])/deltaYPos + lutS[lutIndex-1]*(lutY[lutIndex]-boilerYPos)/deltaYPos;
			speedCmd = speedCmd + speedCmdChange;
			angleAdjust = lutA[lutIndex]*(boilerYPos-lutY[lutIndex-1])/deltaYPos + lutA[lutIndex-1]*(lutY[lutIndex]-boilerYPos)/deltaYPos;

			//speedCmd = BASELINE_LAUNCHER_INP + speedCmdChange;
		}
	}
	else
	{
		speedCmd = BASELINE_LAUNCHER_INP + speedCmdChange;
		isTargetDetected = false;
	}
}

//Calculate the launcher state and speed, and update values accordingly.
void FuelLauncher::LauncherState()
{
	xhatzzz = xhatzz;
	xhatzz = xhatz;
	xhatz = xhat;
	xhat = launcherEncoder->GetDistance()*2.0*PI/256.0;
	xdhat = (xhat - xhatz)/LOOPTIME;
	xdhat = Limit(-1000, 1000, xdhat); //Limits the speed to 1000radians/second
	xdhatTustin = (xhat + xhatz - xhatzz - xhatzzz)/(4.0 * LOOPTIME);
	xdhatTustin = Limit(-1000, 1000, xdhatTustin); //Limits the speed to 1000radians/second
}

//Service function to calculate the motor command for the Fuel Launcher.
void FuelLauncher::GenerateLaunchCmd()
{
	//compute velocity error
	err = speedCmd - xdhat;
	err2 = speedCmd - xdhatTustin;

	//bimodal control (on/off | pi)
	if (abs(err) > LAUNCHER_ELIM)
	{
		errint = LAUNCHER_KINT*speedCmd/LAUNCHER_K;
		launchCmd = 1.0*Sign(err);
	}
	else
	{
	    errint = errint + err2*LOOPTIME;
	    launchCmd = Limit(-1, 1, LAUNCHER_K/LAUNCHER_KR*(LAUNCHER_TAU*err2 + errint));
	}

	// if encoder is lost then use fixed motor command (with manual adjustments)
	if (isEncoderBroken)
	{
		launchCmd = OVERRIDE_MOTOR_CMD + speedCmdChange * K_ADJUST_MOTOR;
	    launchCmd = Limit(-1, 1, launchCmd);
	}
}

void FuelLauncher::ShotAdjuster(double adjust)
{
	angleAdjust = SERVO_START+ ((SERVO_END - SERVO_START) * ((adjust * 0.5) + 0.5));
	shooterServo->Set(angleAdjust);
}
void FuelLauncher::LongShot()
{
	angleAdjust = 1.0;
	shooterServo->Set(angleAdjust);
}
void FuelLauncher::ShortShot()
{
	angleAdjust = 0.0;
	shooterServo->Set(angleAdjust);
}

//Update The Dashboard Functions Related To The FuelLauncher Class
void FuelLauncher::UpdateDash()
{
	//SmartDashboard::PutBoolean("Shooter Enabled: ", isShooterEnabled);
	SmartDashboard::PutBoolean("Mix-Master Enabled: ", isMixerEnabled);
	//SmartDashboard::PutNumber("Launcher Encoder: ", launcherEncoder->GetDistance());
	//SmartDashboard::PutNumber("Launcher Motor Command: ", launchCmd); //Used for testing launching mechanism, can be commented out after.
	SmartDashboard::PutNumber("Launcher SpeedCmd: ", speedCmd);
	SmartDashboard::PutBoolean("Boiler Target Detected: ", isTargetDetected);
	SmartDashboard::PutNumber("Angle Adjust: ", angleAdjust);
}

//Limits a value to a maximum and a minimum value.
double FuelLauncher::Limit(double min, double max, double curValue)
{
	if(curValue > max)
		return max;
	if(curValue < min)
		return min;
	return curValue;
}

//Finds the sign (Positive or Negative) of a given value.
double FuelLauncher::Sign(double inputValue)
{
	return (inputValue >= 0.0 ? 1.0 : -1.0);
}


// The Service Routine For The FuelLauncher Class
void FuelLauncher::Service()
{
	LauncherState();

	switch (launcherMode)
	{
		case SPEEDINGUP:
			//printCount++;
			//printf("%f  %f  %d\n", launcherEncoder->GetDistance(), launchCmd, printCount);
			//printf("%f 	%f 	%f  %f  %f\n", xdhat, xdhatTustin, launchCmd, speedCmd, errint);

	    	if (speedUpLoopCount > NUM_SPEEDUP_LOOPS)
	    	{
	    		launcherMode = SHOOTING;
	    		isShooterEnabled = true;
	    	}
	    	else
	    	{
	    		speedUpLoopCount++;
	    	}
	    	if (isMixerEnabled)
	    	{
	    		mixCmd = -0.6;
	    	}
		    GenerateSpeedCmd();
		    GenerateLaunchCmd();
	    	break;

	    case SHOOTING:
	    	//printCount++;
	    	//printf("%f  %f  %d\n", launcherEncoder->GetDistance(), launchCmd, printCount);
			//printf("%f 	%f 	%f  %f  %f\n", xdhat, xdhatTustin, launchCmd, speedCmd, errint);

	    	if (isMixerEnabled)
	    	{
	    		loadCmd = -0.5; //BEST SPEED
	    		mixCmd = -0.55; //-0.4;	//BEST SPEED
	    	}
	    	else
	    	{
	    		loadCmd = 0.0;
	    		mixCmd = 0.0;
	    	}
	       GenerateSpeedCmd();
		   GenerateLaunchCmd();
		   break;

	    case IDLE:
	    default:
		   launchCmd = 0.0;
		   mixCmd = 0.0;
		   loadCmd = 0.0;
		   speedUpLoopCount = 0;
		   StopMixer();
	       break;
	}

	mixMaster->Set(mixCmd*mixWheelMultiplier);
	loadWheel->Set(loadCmd);
	launcherMotor->Set(launchCmd);
	//printf("  launchCmd %f   xdhat %f \n", launchCmd, xdhat);
	shooterServo->Set(angleAdjust);
	//printf(" targetLocation %f   angleAdjust %f   speedCmd %f\n", boilerYPos, angleAdjust, speedCmd);
}

