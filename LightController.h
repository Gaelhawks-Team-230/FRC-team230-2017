#ifndef LIGHTCONTROLLER_H_
#define LIGHTCONTROLLER_H_
/*
 * LightController.h
 *
 *  Created on: Feb 4, 2017
 *      Author: Siddharth Jain
 */

class TalonXVIII;

class LightController
{
	private:
		// Create objects of 2 light Digital Outputs allowing 4 different lights
		DigitalOutput *light1;
		DigitalOutput *light2;

		TalonXVIII *mainRobot;

   	public:
		LightController(TalonXVIII *pRobot);
		void LocalReset(void);
		void StopAll(void);
		void StartingConfig(void);
		void UpdateDash(void);
		void Service(void);
		// 4 functions for 4 different light displays
		void LightShow1(void);
		void LightShow2(void);
		void LightShow3(void);
		void LightShow4(void);

};

#endif /* SAMPLE_H_ */
