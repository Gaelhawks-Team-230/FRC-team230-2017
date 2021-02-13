
#include "WPILib.h"
#include "Common.h"
#include "FuelGrab.h"


// Constructor for FuelGrab Class
// - Class for the fuel pickup mechanism
FuelGrab::FuelGrab(TalonXVIII *pRobot)
{
	// Create objects needed by this class
#ifdef PRACTICE_BOT
	fuelIntake = new Talon(PWM_FUELPICKUP);
#else
	fuelIntake = new VictorSP(PWM_FUELPICKUP);
#endif

	mainRobot = pRobot;
	// call the class initialization function
	LocalReset();
}

// Set initial values for ALL objects and variables
void FuelGrab::LocalReset()
{
	// Initialize ALL new objects
	// example: sampleMotor->Set(0.0);
	// Initialize ALL local variables
	fuelIntake->Set(0.0);
	// example: isEnabled = false;
	isGathererEnabled = false;

}

// Stops all motors and updates local variables accordingly.
void FuelGrab::StopAll()
{
	// Stop all motors
	// example: sampleMotor->Set(0.0);
	fuelIntake->Set(0.0);
	// Reset all local flags that control Service operations
	// example: serviceMode = IDLE;
	isGathererEnabled = false;
}

void FuelGrab::GrabFuel()
{
   isGathererEnabled = true;
}

void FuelGrab::StopGatherer()
{
   isGathererEnabled = false;
}

bool FuelGrab::IsGathererEnabled()
{
	return isGathererEnabled;
}

void FuelGrab::StartingConfig()
{
	// Bring mechanism into frame perimeter, etc.
}

void FuelGrab::UpdateDash()
{
	//SmartDashboard::PutNumber("Sample PWM: ", sampleMotor->Get());
	//SmartDashboard::PutBoolean("Gatherer Enabled: ", isGathererEnabled);
}

// Service function for tasks that must occur or be updated every loop
void FuelGrab::Service()
{
	if(isGathererEnabled)
	{
		fuelIntake->Set(1.0);
	}
	else
	{
		fuelIntake->Set(0.0);
	}
}




