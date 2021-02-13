/*
 * Climber.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: SDGAdmin
 */

#include "WPILib.h"
#include "Common.h"
#include "Climber.h"

//Climbs rope
Climber::Climber(TalonXVIII* pRobot)
{
	// creating objects
#ifdef PRACTICE_BOT
	ropeclimber = new Talon(PWM_ROPECLIMBER);
#else
	ropeclimber = new VictorSP(PWM_ROPECLIMBER);
#endif
	mainRobot = pRobot;

	LocalReset();
}

void Climber::LocalReset()
{
	//Whenever Local Reset, turn motor off and reset stallCurrentCount and transition count
	state = STARTUP;
	stallCurrentCount = 0;
	transitionCount = 0;
	motorCmd = 0.0;
	ropeclimber->Set(motorCmd);
}

//Stopping Everything
void Climber::StopAll()
{
	LocalReset();
}

//Climbing up by turning motor on
void Climber::ClimbUp()
{
	transitionCount = 0;
	stallCurrentCount = 0;
	motorCmd = 1.0;
	ropeclimber->Set(motorCmd);
}

//Whenever button pressed to stop climb, call local reset, turning off motor and resetting stallCurrentCount
void Climber::StopClimb()
{
	LocalReset();
	ropeclimber->Set(motorCmd);
}

//Get current from Power Distribution Panel and assign to and return variable
double Climber::GetCurrent()
{
	climberMotorCurrent = mainRobot->pdp->GetCurrent(CLIMBER_CURRENT_IN);
	return climberMotorCurrent;
}

//Checking if motor is climbing by seeing if the motor speed is 0 or not
bool Climber::IsClimbing()
{
	return (motorCmd != 0.0);
}

//Getting the current then presenting number on Dashboard
void Climber::UpdateDash()
{
	GetCurrent();
	SmartDashboard::PutNumber("Climber Motor Current: ", climberMotorCurrent);
}

void Climber::Service()
{

	if (!IsClimbing())
	{
		return;
	}
	else
	{

		switch (state)
		{
			case STARTUP: //Measure if current has reached 40 amps when the motor starts
				GetCurrent();
				if (climberMotorCurrent > CLIMBER_STARTUP_CURRENT)
				{
					transitionCount = 0;
					//state = STARTMEASURING;
					state = TIMESTALL;
				}
				break;

			case TIMESTALL: //Wait 2 seconds
				if (transitionCount > BEFORE_STALL_TIME)
				{
					state = WAITFORSTALL;
				}
				transitionCount++;
				break;

			case WAITFORSTALL: //Start checking for stall
				GetCurrent();
				if (climberMotorCurrent > CLIMBER_STALL_CURRENT)
				{
					//If yes, increase stall count
					stallCurrentCount++;
					//If the stall count is greater than Target stall Count, then stop motor
					if (stallCurrentCount > TARGET_STALL_COUNT)
					{
						motorCmd = 0.0;
					}
				}
				else
				{
					stallCurrentCount = 0;
				}
				ropeclimber->Set(motorCmd);
				break;
		}
	}
}


void Climber::StartingConfig()
{
	LocalReset();
}
