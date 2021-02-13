/*
 * AutonomousMode.cpp
 *
 *  Created on: Feb 7, 2017
 *      Author: jacob_000
 */
#include "WPILib.h"
#include "TalonXVIII_main.h"
#include "AutonomousModes.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////  AUTONOMOUS MODES  //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TalonXVIII::AutoTrackGear()
{
	if(gearBlocks > 0)
	{
		rotateCmd = (GEAR_CENTER_TARGET - gear->GetTargetCenter()) * K_GEAR_TRACK;
		isAuto = false;
	}
	else
	{
		rotateCmd = 0.0;
	}
}

void TalonXVIII::AutoTrackBoiler()
{
	if(boilerBlocks > 0)
	{
		rotateCmd = (BOILER_CENTER_TARGET - shooter->GetTargetCenter()) * K_BOILER_TRACK;
		isAuto = false;
	}
}

void TalonXVIII::AutoModeHopperAndShootDC()
{
	boilerBlocks = shooterCam->AnalyzeTarget();

	switch(autoStage)
	{
		case 0: //Start encoders, reset values before beginning autonomous.
			distance->LocalReset();
			driveCmd = 0.0; sideCmd = 0.0; rotateCmd = 0.0;
			loopCount = 0;
			autoStage++;
			break;

		case 1: //Drive backwards from the wall in order to get perpendicular to the hopper.
			if((distance->GetAverageDistance() * -1.0) <= DISTANCE_HOPPER_CENTER)
			{
				driveCmd = -0.6; sideCmd = 0.0; rotateCmd = 0.0;
			}
			else
			{
				driveCmd = 0.1; sideCmd = 0.0; rotateCmd = 0.0;
				distance->LocalReset();
				loopCount = 0;
				gatherer->GrabFuel();
				autoStage++;
			}
			break;

		case 2: //Drive sideways to hit the hopper and pour the fuel into the robot + on the field.
			if(loopCount < AUTO_STRAFE_HIT)
			{
				 driveCmd = 0.1; rotateCmd = 0.0;
				 if(isBlueAlliance)
				 {
					 sideCmd = 0.6;
				 }
				 else
				 {
					 sideCmd = -0.6;
				 }
				 loopCount++;
			}
			else
			{
				driveCmd = 0.0; sideCmd = 0.0; rotateCmd = 0.0;
				distance->LocalReset();
				loopCount = 0;
				autoStage++;
			}
			break;

		case 3:
			if(distance->GetAverageDistance() <= AUTO_DRIVE_FORWARD_BOILER)
			{
				driveCmd = 0.2; sideCmd = 0.0; rotateCmd = 0.0;
			}
			else
			{
				driveCmd = -0.1; sideCmd = 0.0; rotateCmd = 0.0;
				distance->LocalReset();
				loopCount = 0;
				shooter->SpinUpLauncher();
				autoStage++;
			}
			break;

		case 4:
			if(loopCount < AUTO_TARGET_STRAFE)
			{
				driveCmd = 0.0;
				shooterTargeting = true;
				if(boilerBlocks > 0)
				{
					AutoTrackBoiler();
				}
				else
				{
					rotateCmd = 0.0;
				}

				 if(isBlueAlliance)
				 {
					 sideCmd = -0.6;
				 }
				 else
				 {
					 sideCmd = 0.6;
				 }
				 loopCount++;
			}
			else
			{
				driveCmd = 0.0; sideCmd = 0.0;
				if(boilerBlocks > 0)
				{
					AutoTrackBoiler();
				}
				else
				{
					rotateCmd = 0.0;
				}
				loopCount = 0;
				autoStage++;
				shooter->EnableMixer();
			}
			break;

		case 5:
			driveCmd = 0.0; sideCmd = 0.0;
			if(boilerBlocks > 0)
			{
				AutoTrackBoiler();
			}
			else
			{
				rotateCmd = 0.0;
			}
			break;
	}
}

void TalonXVIII::AutoModeHopperAndShoot()
{
	boilerBlocks = shooterCam->AnalyzeTarget();

	switch(autoStage)
	{
		case 0: //Start encoders, reset values before beginning autonomous.
			distance->LocalReset();
			driveCmd = 0.0; sideCmd = 0.0; rotateCmd = 0.0;
			loopCount = 0;
			autoStage++;
			break;

		case 1: //Drive backwards from the wall in order to get perpendicular to the hopper.
			if((distance->GetAverageDistance() * -1.0) <= DISTANCE_HOPPER_CENTER)
			{
				driveCmd = -0.45; sideCmd = 0.0; rotateCmd = 0.0;
			}
			else
			{
				driveCmd = 0.1; sideCmd = 0.0; rotateCmd = 0.0;
				distance->LocalReset();
				loopCount = 0;
				gatherer->GrabFuel();
				shooter->SpinUpLauncher();
				autoStage++;
			}
			break;

		case 2: //Drive sideways to hit the hopper and pour the fuel into the robot + on the field.
			if(loopCount < AUTO_STRAFE_HIT)
			{
				 driveCmd = 0.10; rotateCmd = 0.0;
				 if(isBlueAlliance)
				 {
					 sideCmd = 0.6;
				 }
				 else
				 {
					 sideCmd = -0.6;
				 }
				 loopCount++;
			}
			else
			{
				driveCmd = 0.0; sideCmd = 0.0; rotateCmd = (BOILER_CENTER_TARGET - shooter->GetTargetCenter()) * K_BOILER_TRACK;
				distance->LocalReset();
				loopCount = 0;
				shooter->EnableMixer();
				autoStage++;
			}
			break;

		case 3://Strafe a bit away from hopper
			if(loopCount < AUTO_STRAFE_AWAY)
			{
				 driveCmd = 0.0; rotateCmd = 0.0;
				 if(isBlueAlliance)
				 {
					 sideCmd = -0.6;
				 }
				 else
				 {
					 sideCmd = 0.6;
				 }
				 loopCount++;
			}
			else
			{
				driveCmd = 0.0; sideCmd = 0.0; rotateCmd = 0.0;
				loopCount = 0;
				autoStage++;
				shooter->SpinUpLauncher();
			}
			break;

		case 4: //Begin driving foward towards the boiler.
			if(distance->GetAverageDistance() < AUTO_DRIVE_FORWARD_BOILER)
			{
				shooterAiming = true;
				driveCmd = 0.2; sideCmd = 0.0;
				if(boilerBlocks > 0)
				{
					AutoTrackBoiler();
				}
				else
				{
					rotateCmd = 0.0;
				}
			}
			else
			{
				driveCmd = 0.0; sideCmd = 0.0; rotateCmd = 0.0;
				shooterTargeting = true;
				distance->LocalReset();
				loopCount = 0;
				autoStage++;
			}
			break;

		case 5: //Line up with the boiler.
			if(loopCount < AUTO_TRACK_TIME)
			{
				driveCmd = 0.0; sideCmd = 0.0;
				printf("boilerBlocks: %d", boilerBlocks);
				if(boilerBlocks > 0)
				{
					AutoTrackBoiler();
				}
				else
				{
					rotateCmd = 0.0;
				}
				loopCount++;
			}
			else
			{
				//printf("Successfully Lined Up. Starting Launcher...");
				driveCmd = 0.0; rotateCmd = 0.0; sideCmd = 0.0;
				AutoTrackBoiler();
				distance->LocalReset();
				loopCount = 0;
			}
			break;

		default:
			break;
	}

}

void TalonXVIII::AutoJustShoot()
{
	boilerBlocks = shooterCam->AnalyzeTarget();

	switch(autoStage)
	{
		case 0: //Start encoders, reset values before beginning autonomous.
			distance->LocalReset();
			driveCmd = 0.0; sideCmd = 0.0; rotateCmd = 0.0;
			loopCount = 0;
			autoStage++;
			break;

		case 1:
			if(loopCount < AUTO_STRAFE_OUT)
			{
				 driveCmd = 0.0; rotateCmd = 0.0;
				 if(isBlueAlliance)
				 {
					 sideCmd = 0.6;
				 }
				 else
				 {
					 sideCmd = -0.6;
				 }
				 loopCount++;
			}
			else
			{
				driveCmd = 0.0; sideCmd = 0.0; rotateCmd = 0.0;
				loopCount = 0;
				autoStage++;
				shooter->SpinUpLauncher();
			}
			break;

		case 2: //Line up with the boiler.
			if(loopCount < AUTO_TRACK_TIME)
			{
				shooterAiming = true;
				driveCmd = 0.0; sideCmd = 0.0;
				printf("boilerBlocks: %d", boilerBlocks);
				if(boilerBlocks > 0)
				{
					AutoTrackBoiler();
				}
				else
				{
					if(isBlueAlliance)
					{
						rotateCmd = -40.0;
					}
					else
					{
						rotateCmd = 40.0;
					}
				}
				loopCount++;
			}
			else
			{
				//printf("Successfully Lined Up. Starting Launcher...");
				shooterTargeting = true;
				shooter->EnableMixer();
				driveCmd = 0.0; rotateCmd = 0.0; sideCmd = 0.0;
				AutoTrackBoiler();
				isAuto = false;
				distance->LocalReset();
				loopCount = 0;
			}
			break;
	}
}

void TalonXVIII::AutoModeReachBaseLine()
{
	switch(autoStage)
	{
		case 0://Reset
			distance->LocalReset();
			driveCmd = 0.0; rotateCmd = 0.0; sideCmd = 0.0;
			loopCount = 0;
			autoStage++;
			break;

		case 1://Drive to baseline
			if((distance->GetAverageDistance()* -1.0) <= DISTANCE_TO_BASELINE )
			{
				driveCmd = -0.45; rotateCmd = 0.0; sideCmd = 0.0;
			}
			else
			{
				driveCmd = 0.0; rotateCmd = 0.0; sideCmd = 0.0;
				distance->LocalReset();
				loopCount = 0;
				autoStage++;
			}
			break;
	}
}

void TalonXVIII::AutoModeFrontGear(int isShooting)
{
	if(isShooting)
	{
		boilerBlocks = shooterCam->AnalyzeTarget();
	}
	gearBlocks = gearCam->AnalyzeTarget();

	//printf("autoStage %d  loopCount %d \n", autoStage, loopCount);

	switch(autoStage)
	{
		case 0://Reset
			distance->LocalReset();
			driveCmd = 0.0; rotateCmd = 0.0; sideCmd = 0.0;
			loopCount = 0;
			autoStage++;
			break;

		case 1://Drive to airship
			if((distance->GetAverageDistance() * -1.0) <= DISTANCE_TO_AIRSHIP)
			{
				driveCmd = -0.35; sideCmd = 0.0;
				AutoTrackGear();
			}
			else
			{
				driveCmd = 0.0; sideCmd = 0.0;
				rotateCmd = 0.0;
				distance->LocalReset();
				gear->GateDown();
				gear->EjectGear();
				loopCount = 0;
				autoStage++;
			}
			break;

		case 2://Open Gear door
			gear->EjectGear();
			if(loopCount < AUTO_GEAR_LOAD_WAIT)
			{
				loopCount++;
			}
			else
			{
				driveCmd = 0.0; rotateCmd = 0.0; sideCmd = 0.0;
				distance->LocalReset();
				loopCount = 0;
				autoStage++;
			}
			break;

		case 3://Back up half the distance from the wall to the baseline
			gear->EjectGear();
			if(distance->GetAverageDistance() < DISTANCE_AIRSHIP_BACKUP)
			{
				driveCmd = 0.2; rotateCmd = 0.0; sideCmd = 0.0;
			}
			else
			{
				driveCmd = 0.0; rotateCmd = 0.0; sideCmd = 0.0;
				gear->GateUp();
				gear->StopCollector();
				loopCount = 0;
				if(isShooting)
				{
					autoStage++;
				}
				else
				{
					autoStage = 20; // go to end, done
				}
			}
			break;

		case 4://Turn toward boiler
			loopCount++;
			if(loopCount < AUTO_TURN_90_DEGREES)
			{
				if(isBlueAlliance)
				{
					driveCmd = 0.0; rotateCmd = 90.0; sideCmd = 0.0;
				}
				else
				{
					driveCmd = 0.0; rotateCmd = -90.0; sideCmd = 0.0;
				}
			}
			else
			{
				driveCmd = 0.0; rotateCmd = 0.0; sideCmd = 0.0;
				distance->LocalReset();
				loopCount = 0;
				autoStage++;
			}
			break;

		case 5://Drive toward boiler
			if(distance->GetAverageDistance() <= AUTO_DRIVE_AIRSHIP_BOILER)
			{
				driveCmd = 0.35; rotateCmd = 0.0; sideCmd = 0.0;
			}
			else
			{
				driveCmd = 0.0; rotateCmd = 0.0; sideCmd = 0.0;
				distance->LocalReset();
				loopCount = 0;
				shooter->SpinUpLauncher();
				autoStage++;
			}
			break;

		case 6://start launcher and start tracking boiler
			if(loopCount < AUTO_TRACK_TIME)
			{
				shooterAiming = true;
				driveCmd = 0.0; sideCmd = 0.0;
				printf("boilerBlocks: %d", boilerBlocks);
				if(boilerBlocks > 0)
				{
					AutoTrackBoiler();
				}
				else
				{
					if(isBlueAlliance)
					{
						driveCmd = 0.0; rotateCmd = -10.0; sideCmd = 0.0;
						isAuto = true;
					}
					else
					{
						driveCmd = 0.0; rotateCmd = 10.0; sideCmd = 0.0;
						isAuto = true;
					}
				}
				loopCount++;
			}
			else
			{
				//printf("Successfully Lined Up. Starting Launcher...");
				shooterTargeting = true;
				shooter->EnableMixer();
				gatherer->GrabFuel();
				driveCmd = 0.0; rotateCmd = 0.0, sideCmd = 0.0;
				AutoTrackBoiler();
				distance->LocalReset();
				loopCount = 0;
			}
			break;

		default:
			break;
	}
}

/* Side Gear Auto:
 * 1) Drive a bit past the base line to reach the side lift
 * 2) Rotate a bit towards the lift (depending on what alliance either right or left, if we arent shooting we will most likely
 * be on the lift on the opposite side of the boiler), then auto-track once the robot has turned enough to center the robot
 * 3) Drive forward distance of the barrier length to reach the lift
 * 4) Release gear from mechanism and drive backwards
 */

void TalonXVIII::AutoModeSideGear(bool boilerSide, int isShooting)
{
	if(isShooting)
	{
		boilerBlocks = shooterCam->AnalyzeTarget();
	}
	gearBlocks = gearCam->AnalyzeTarget();

	switch(autoStage)
	{
		case 0://Reset
			distance->LocalReset();
			driveCmd = 0.0; rotateCmd = 0.0; sideCmd = 0.0;
			loopCount = 0;
			autoStage++;
			break;

		case 1://Drive a bit past the baseline to reach the side lift
			if((distance->GetAverageDistance() * -1.0) < DISTANCE_SIDE_LIFT)
			{
				driveCmd = -0.35; rotateCmd = 0.0; sideCmd = 0.0;
			}
			else
			{
				driveCmd = 0.0; rotateCmd = 0.0; sideCmd = 0.0;
				distance->LocalReset();
				loopCount = 0;
				autoStage++;
			}
			break;

		case 2://Turn a bit towards side lift (Positive = left, Negative = right)
			if(loopCount < AUTO_TURN_60_DEGREES)
			{
				if(boilerSide)
				{
					if(isBlueAlliance)
					{
						driveCmd = 0.0; rotateCmd = 60.0; sideCmd = 0.0;
					}
					else
					{
						driveCmd = 0.0; rotateCmd = -60.0; sideCmd = 0.0;
					}
				}
				else
				{
					if(isBlueAlliance)
					{
						driveCmd = 0.0; rotateCmd = -60.0; sideCmd = 0.0;
					}
					else
					{
						driveCmd = 0.0; rotateCmd = 60.0; sideCmd = 0.0;
					}
				}
				loopCount++;
			}
			else
			{
				driveCmd = 0.0; sideCmd = 0.0;
				AutoTrackGear();
				distance->LocalReset();
				loopCount = 0;
				autoStage++;
			}
			break;

		case 3:
			if(loopCount < GEAR_TRACK_TIME)
			{
				gearTargeting = true;
				driveCmd = 0.0; sideCmd = 0.0;
				AutoTrackGear();
				loopCount++;
			}
			else
			{
				driveCmd = 0.0; sideCmd = 0.0;
				AutoTrackGear();
				distance->LocalReset();
				loopCount = 0;
				autoStage++;
			}
			break;

		case 4://Begin tracking lift target while driving forward towards the lift
			if((distance->GetAverageDistance() * -1.0) < DISTANCE_BARRIER_TO_LIFT && loopCount < AUTO_GEAR_DRIVE_TIME)
			{
				driveCmd = -0.2; sideCmd = 0.0;
				AutoTrackGear();
				loopCount++;
			}
			else
			{
				driveCmd = 0.0; rotateCmd = 0.0; sideCmd = 0.0;
				distance->LocalReset();
				gear->GateDown();
				gear->EjectGear();
				loopCount = 0;
				autoStage++;
			}
			break;


		case 5://Open gear gate and wait a bit for gear to drop into lift
			gear->EjectGear();
			if(loopCount < AUTO_GEAR_LOAD_WAIT)
			{
				loopCount++;
			}
			else
			{
				driveCmd = 0.0; rotateCmd = 0.0; sideCmd = 0.0;
				loopCount = 0;
				autoStage++;
			}
			break;

		case 6://drive back a bit, a third the distance from the wall to the baseline
			gear->EjectGear();
			if(distance->GetAverageDistance() < DISTANCE_TO_BASELINE/2)
			{
				driveCmd = 0.35; rotateCmd = 0.0; sideCmd = 0.0;
			}
			else
			{
				driveCmd = 0.0; rotateCmd = 0.0; sideCmd = 0.0;
				gear->GateUp();
				gear->StopCollector();
				loopCount = 0;
				if(isShooting)
				{
					autoStage++;
					shooter->SpinUpLauncher();
					gear->GateUp();
				}
				else
				{
					autoStage = 20;
					gear->GateUp();
				}
			}
			break;

		case 7://start launcher and start tracking boiler
			if(loopCount < AUTO_TRACK_TIME)
			{
				shooterAiming = true;
				shooterTargeting = true;
				driveCmd = 0.0; sideCmd = 0.0;
				printf("boilerBlocks: %d", boilerBlocks);
				if(boilerBlocks > 0)
				{
					AutoTrackBoiler();
				}
				else
				{
					if(isBlueAlliance)
					{
						driveCmd = 0.0; rotateCmd = -10.0; sideCmd = 0.0;
						isAuto = true;
					}
					else
					{
						driveCmd = 0.0; rotateCmd = 10.0; sideCmd = 0.0;
						isAuto = true;
					}
				}
				loopCount++;
			}
			else
			{
				//printf("Successfully Lined Up. Starting Launcher...");
				shooterTargeting = true;
				shooter->EnableMixer();
				gatherer->GrabFuel();
				driveCmd = 0.0; rotateCmd = 0.0; sideCmd = 0.0;
				AutoTrackBoiler();
				distance->LocalReset();
				loopCount = 0;
			}
			break;
	}
}





