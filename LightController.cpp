#include "WPILib.h"
#include "Common.h"
#include "LightController.h"


// CONSTRUCTOR for Sample Class
// - put a description of the mechanism for this class here :)
LightController::LightController(TalonXVIII *pRobot)
{
	// Create 2 objects for 2 different light ports
	light1 = new DigitalOutput(LED_LIGHT_OUTPUT_1);
	light2 = new DigitalOutput(LED_LIGHT_OUTPUT_2);

	mainRobot = pRobot;

	LocalReset();
}


// Set initial values for ALL objects and variables
void LightController::LocalReset()
{
	light1->Set(true);
	light2->Set(true);
	// Initialize ALL new objects
}

// Used when disable robot: Stop all motors and reset all state machines
void LightController::StopAll()
{
	LocalReset();
	// Stop all motors
}

//Initial Configuration
void LightController::StartingConfig()
{
	LocalReset();
}

// Update the display of which light display is showing
void LightController::UpdateDash()
{
}

// All classes need a Service function
void LightController::Service()
{
	if(mainRobot->ropeClimber->IsClimbing())
	{
		light1->Set(false);//22
		light2->Set(false);//23
	}
	else if(mainRobot->gatherer->IsGathererEnabled())
	{
		if(mainRobot->isBlueAlliance)
		{
			light1->Set(false);//22
			light2->Set(true);//23
		}
		else
		{
			light1->Set(true);//22
			light2->Set(false);//23
		}
	}
	else
	{
		light1->Set(true);//22
		light2->Set(true);//23
	}
}
