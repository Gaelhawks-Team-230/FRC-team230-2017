#include "WPILib.h"
#include "Common.h"
#include "GearHandling.h"


// CONSTRUCTOR for Gear Handling Class
// Enables a motor to process gears
GearHandling::GearHandling(TalonXVIII* pRobot)
{
	// Create objects needed by this class
	gearDoor = new VictorSP(PWM_GEARMOTOR);
	gearCollector = new VictorSP(PWM_GEARCOLLECTOR);
	mainRobot = pRobot;

	// call the class initialization function
	LocalReset();
}


// Set initial values for ALL objects and variables
void GearHandling::LocalReset()
{
	stallCurrentCount = 0;
	backoffCount = 0;
	returnCount = 0;
	gearCurrent = 0.0;
	isMovingUp = false;
	isMovingDown = false;
	isBackoffMode = false;
	isCollectorOn = false;
	motorCmd = 0.0;
	collectorCmd = 0.0;
	stallCurrentLimit = 0.0;
	gearDoor->Set(motorCmd);
	gearCollector->Set(collectorCmd);
	targetCmd = 0.0;
}

// Used when disable robot: Stop all motors and reset all state machines
void GearHandling::StopAll()
{
	LocalReset();
}

// Create functions for needed operations
void GearHandling::GateDown()
{
	motorCmd = 0.3;
	stallCurrentLimit = GEAR_STALL_CURRENT_DOWN;
	isMovingUp = false;
	isMovingDown = true;
	isBackoffMode = false;
	returnCount = 0;
}

void GearHandling::GateUp()
{
	motorCmd = -0.6;
	stallCurrentLimit = GEAR_STALL_CURRENT_UP;
	isMovingUp = true;
	isMovingDown = false;
	isBackoffMode = false;
	returnCount = 0;
}

void GearHandling::StopGate()
{
	motorCmd = 0.0;
	isMovingUp = false;
	isMovingDown = false;
	isBackoffMode = false;
}

void GearHandling::CollectGear()
{
	collectorCmd = 1.0;
	isCollectorOn = true;
}

void GearHandling::EjectGear()
{
	collectorCmd = -1.0;
	isCollectorOn = true;
}

void GearHandling::StopCollector()
{
	collectorCmd = 0.0;
	isCollectorOn = false;
}

// Function responsible for the auto-calibration of the analog.

void GearHandling::GetGateCurrent()
{
	gearCurrent = mainRobot->pdp->GetCurrent(GEAR_CURRENT_IN);
}

// Used to prepare robot for field
// Bring mechanism into frame perimeter, etc.
void GearHandling::StartingConfig()
{
	GateUp();
}

bool GearHandling::IsGateMoving()
{
	if(isMovingDown || isMovingUp)
	{
		return true;
	}
	else
	{
		return false;
	}
}

double GearHandling::GetTargetCenter()
{
	CompositeBlock *liftTarget = mainRobot->gearCam->GetCompositeTarget();
	return liftTarget->xPosition;
}

double GearHandling::Limit(double min, double max, double curValue)
{
	if(curValue > max)
		return max;
	if(curValue < min)
		return min;
	return curValue;
}

// Update the display of all relevant information in your class
//  - this can include motor/pwm command values, flags, state, etc.
void GearHandling::UpdateDash()
{
	SmartDashboard::PutNumber("Gear motorCmd: ", motorCmd);
	//SmartDashboard::PutBoolean("Gear Gate moving: ", IsGateMoving());
}

void GearHandling::Service()
{
	GetGateCurrent();
	//22printf("Gear Current: %f stallCount\n", gearCurrent);
    if (isMovingDown)
    {
    	if(isBackoffMode)
    	{
    		backoffCount++;
    		if(backoffCount < BACKOFF_TIME)
    		{
    			motorCmd = -0.3;
    		}
    		else
    		{
    			motorCmd = 0.0;
				isMovingDown = false;
				isBackoffMode = false;
    		}
    	}
    	else
    	{
			if(gearCurrent > stallCurrentLimit)
			{
				stallCurrentCount++;
				if(stallCurrentCount > GEAR_STALL_COUNT)
				{
					if (motorCmd > 0.0)//when gate comes down
					{
						isBackoffMode = true;
						backoffCount = 0;
					}
					else
					{
						isBackoffMode = false;
						isMovingDown = false;
					}
					motorCmd = 0.0;
				}
			}
			else
			{
				stallCurrentCount = 0;
			}
		}
    }
	else if(isMovingUp)
	{
		if(returnCount > RETURN_TIME)
		{
			motorCmd = 0.0;
			returnCount = 0;
			isMovingUp = false;
		}
		else
		{
			returnCount++;
		}
	}
	else
	{
		motorCmd = 0.0;
	}

    gearCollector->Set(collectorCmd);
	gearDoor->Set(motorCmd);
}
