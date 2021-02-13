#ifndef FUELGRAB_H_
#define FUELGRAB_H_
/*
 * FuelGrab.h
 *
 *  Created on: Jan 20, 2017
 *      Author: Gaelhawks
 */

#include "Common.h"

class TalonXVIII;

class FuelGrab
{
	private:
		// Class Objects
#ifdef PRACTICE_BOT
		Talon *fuelIntake;
#else
		VictorSP *fuelIntake;
#endif

		// Local Variables
		bool isGathererEnabled;

		TalonXVIII *mainRobot;

   	public:
		FuelGrab(TalonXVIII *pRobot);
		void LocalReset(void);
		void StopAll(void);
		void GrabFuel(void);
		void StopGatherer(void);
		bool IsGathererEnabled(void);
		void StartingConfig(void);
		void UpdateDash(void);
		void Service(void);
};



#endif /* FUELGRAB_H_ */
