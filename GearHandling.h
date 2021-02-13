#ifndef GEARHANDLING_H_
#define GEARHANDLING_H_

class TalonXVIII;

#define DEGREES_PER_VOLT             (152.0/4.0) // Goes from half a volt up to four volts. This is over a 300 degree rotation span.
#define CALIBRATION_ANGLE			 (0.0)
#define DOOR_CLOSED_ANGLE            (0.5)
#define DOOR_OPEN_ANGLE              (30.0)

#define TARGET_WIDTH_SIZE			 (7)
#define MAX_TARGET_SIZE				 (105)

#define GEAR_K                       (0.03)
#define GEAR_POSITION_TIME			 ((int)(1.0 * (N1SEC)))
#define BACKOFF_TIME				 (2) //((int)(0.5 * (N1SEC)))

#ifdef PRACTICE_BOT
#define GEAR_STALL_CURRENT			(2.5)
#else
#define GEAR_STALL_CURRENT_UP		(3.0)
#define GEAR_STALL_CURRENT_DOWN		(2.0)
#endif
#define GEAR_STALL_COUNT			((int)((N1SEC) * 0.5))
#define RETURN_TIME					((int)((N1SEC) * 1.0))

/*
 * GearHandling.h
 *
 *  Created on: Jan 25, 2015
 *      Author: David Keith
 */
class GearHandling
{
	private:
		TalonXVIII *mainRobot;

		VictorSP *gearDoor;
		VictorSP *gearCollector;
		// Declare local variables that need to be saved for future processing
		double motorCmd;
		double collectorCmd;
		double stallCurrentLimit;
		bool isMovingUp;
		bool isMovingDown;
		bool isCollectorOn;
		bool isBackoffMode;
		double gearCurrent;
		int stallCurrentCount;
		int backoffCount;
		int returnCount;

		double liftTargetWidths[TARGET_WIDTH_SIZE] = { 50,  60,  70,  80,  90, 100, 110};// 55,  65,  75,  85,
		double lift_xPositions[TARGET_WIDTH_SIZE]  = {126, 118, 112, 103,  88,  71,  57};//124, 115, 110,  92,
		double targetCmd;



   	public:
		GearHandling(TalonXVIII* pRobot);
		void LocalReset(void);
		void StopAll(void);
		void GateDown(void);
		void GateUp(void);
		void StopGate(void);
		void CollectGear(void);
		void EjectGear(void);
		void StopCollector(void);
		double GetTargetCenter(void);
		void GetGateCurrent(void);
		void StartingConfig(void);
		bool IsGateMoving(void);
		double Limit(double min, double max, double curValue);
		void UpdateDash(void);
		void Service(void);
};



#endif /* _H_ */
