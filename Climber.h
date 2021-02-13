/*
 * Climber.h
 *
 *  Created on: Jan 15, 2017
 *      Author: SDGAdmin
 */

#ifndef SRC_CLIMBER_H_
#define SRC_CLIMBER_H_

class TalonXVIII;

#define CLIMBER_STARTUP_CURRENT			(40)
#define CATCH_ROPE_CURRENT				(18)
#define WHEELS_OFF_CURRENT				(50)
#define CLIMBER_STALL_CURRENT			(66) //60)
#define STARTING_CURRENT_COUNT			(10)
#define CATCH_ROPE_COUNT				(10)
#define WHEELS_OFF_COUNT				(10)
#define	TARGET_STALL_COUNT				((int)(N1SEC * 0.5)) //Count we take per second times number of seconds we want
#define BEFORE_STALL_TIME				((int)(N1SEC * 2.0))

#define STARTUP							(0)
#define STARTMEASURING					(1)
#define CATCHROPE						(2)
#define WAITFORSTALL					(3)
#define CATCHROPECOUNT					(4)
#define WHEELSOFF						(5)
#define WHEELSOFFCOUNT					(6)
#define TIMESTALL						(7)

class Climber
{
public:
	//All functions needed in the cpp file
	Climber(TalonXVIII* pRobot);
	void LocalReset(void);
	void StopAll(void);
	void UpdateDash(void);
	double GetCurrent(void);
	void Service(void);
	void StartingConfig(void);
	void ClimbUp(void);
	void StopClimb(void);
	bool IsClimbing(void);

private:
	//Creating objects for class
#ifdef PRACTICE_BOT
	Talon *ropeclimber;
#else
	VictorSP *ropeclimber;
#endif

	TalonXVIII *mainRobot;

	//Declare variables to measure current, stall time and motor speed
	double climberMotorCurrent;
	int stallCurrentCount;
	int state;
	int transitionCount;
	float motorCmd;
};

#endif /* SRC_CLIMBER_H_ */
