/*
 * DriveDistance.h
 *
 *  Created on: Jan 22, 2017
 *      Author: jacob_000
 */

#ifndef SRC_DRIVEDISTANCE_H_
#define SRC_DRIVEDISTANCE_H_

#include "WPILib.h"
#include "Common.h"

#define WHEEL_CIRCUMFERENCE		(18.653)
#define ENCODER_PULSE_COUNT		(360.0)

class DriveDistance
{
private:
	Encoder *encoderFL;
	Encoder *encoderFR;
	Encoder *encoderBL;
	Encoder *encoderBR;

	double backDis;
	double frontDis;

	double distancePerPulse;

public:
	DriveDistance(void);
	void Service(void);
	void UpdateDash(void);
	void LocalReset(void);
	double GetRightDistance(void);
	double GetLeftDistance(void);
	double GetAverageDistance(void);
	void GetFrontDistance(void);
	void GetBackDistance(void);
	double GetLargerDistance(void);
};



#endif /* SRC_DRIVEDISTANCE_H_ */
