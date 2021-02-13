/*
 * DriveDistance.cpp
 *
 *  Created on: Jan 22, 2017
 *      Author: jacob_000
 */
#include "WPILib.h"
#include "TalonXVIII_main.h"
#include "DriveDistance.h"


DriveDistance::DriveDistance()
{
	distancePerPulse = (WHEEL_CIRCUMFERENCE/ENCODER_PULSE_COUNT);
	encoderFL = new Encoder(FRONT_LEFT_ENCODER_ONE, FRONT_LEFT_ENCODER_TWO, false);
	encoderFR = new Encoder(FRONT_RIGHT_ENCODER_ONE, FRONT_RIGHT_ENCODER_TWO, false);
	encoderBL = new Encoder(BACK_LEFT_ENCODER_ONE, BACK_LEFT_ENCODER_TWO, false);
	encoderBR = new Encoder(BACK_RIGHT_ENCODER_ONE, BACK_RIGHT_ENCODER_TWO, false);

	encoderFL->SetDistancePerPulse(-1.0 * distancePerPulse);
	encoderFR->SetDistancePerPulse(distancePerPulse);
	encoderBL->SetDistancePerPulse(-1.0 * distancePerPulse);
	encoderBR->SetDistancePerPulse(distancePerPulse);

	LocalReset();
}

void DriveDistance::LocalReset()
{
	encoderFL->Reset();
	encoderFR->Reset();
	encoderBL->Reset();
	encoderBR->Reset();

	backDis = 0.0;
	frontDis = 0.0;
}

void DriveDistance::Service()
{

}

double DriveDistance::GetLeftDistance()
{
	double leftDis;
	leftDis = (encoderFL->GetDistance() + encoderBL->GetDistance())/2;
	return leftDis;
}

double DriveDistance::GetRightDistance()
{
	double rightDis;
	rightDis = (encoderFR->GetDistance() + encoderBR->GetDistance())/2;
	return rightDis;
}

double DriveDistance::GetAverageDistance()
{
	double averageDis;
	//averageDis = (GetRightDistance() + GetLeftDistance())/2;
	averageDis = GetLargerDistance();
	return averageDis;
}

void DriveDistance::GetFrontDistance()
{
	frontDis = (encoderFR->GetDistance() + encoderFL->GetDistance())/2;
}

void DriveDistance::GetBackDistance()
{
	backDis = (encoderBR->GetDistance() + encoderBL->GetDistance())/2;
}

double DriveDistance::GetLargerDistance()
{
	GetFrontDistance();
	GetBackDistance();

	if(fabs(frontDis) > fabs(backDis))
	{
		return frontDis;
	}
	else
	{
		return backDis;
	}
}

void DriveDistance::UpdateDash()
{
	SmartDashboard::PutNumber("Average Distance", GetAverageDistance());
	//SmartDashboard::PutNumber("Distance left", GetLeftDistance());
	//SmartDashboard::PutNumber("Distance right", GetRightDistance());
	SmartDashboard::PutNumber("FL Distance", encoderFL->GetDistance());
	SmartDashboard::PutNumber("FR Distance", encoderFR->GetDistance());
	SmartDashboard::PutNumber("BL Distance", encoderBL->GetDistance());
	SmartDashboard::PutNumber("BR Distance", encoderBR->GetDistance());
}





