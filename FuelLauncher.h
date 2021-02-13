#ifndef FUELLAUNCHER_H_
#define FUELLAUNCHER_H_

#include "Common.h"

//Constants Used To Calculate Motor Command For Service Function
//#define LAUNCHER_ELIM               (65.0)      //50.0 //85.0 //Error Limit
#define LAUNCHER_ELIM               (30.0)      //50.0 //85.0 //Error Limit - 2 wheel
#define LAUNCHER_K                  (7.0)		//(14.0) //16.0 //8.0 //10.0
#define LAUNCHER_KINT               (1.075)		//1.05)		//1.0 //0.9
#define LAUNCHER_KR                 (700.0)     //650 //Update value once calculated.
//one wheel#define LAUNCHER_TAU	            (0.21)      //0.26  //0.12  // Update value once calculated.
#define LAUNCHER_TAU	            (0.374)      //0.26  //0.12  // Update value once calculated. - 2 wheel
// one wheel #define BASELINE_LAUNCHER_INP       (350.0)     //Desired Speed
#define BASELINE_LAUNCHER_INP       (305.0) //(330.0)//(350.0)     //Desired Speed - 2 wheel
#define LAUNCHER_CHANGE_MIN         (-50.0)     //minimum possible delta Speed
#define LAUNCHER_CHANGE_MAX         (70.0)      //maximum possible delta Speed
#define SPEED_BUMP_INCREMENT        (5.0) //(10.0)
#define NUM_SPEEDUP_LOOPS           (3)

// Values used to generate motor command when encoder is broken
#define OVERRIDE_MOTOR_CMD          (0.55)
#define K_ADJUST_MOTOR              (0.01)

#define LUT_SIZE                    (3)

#define SERVO_START			        (0.9)
#define SERVO_END			        (0.66)

// Values used for the service mode.
#define IDLE                          (0)
#define SHOOTING                      (1)
#define SPEEDINGUP                    (2)


class TalonXVIII;

/*
 * FuelLauncher.h
 *
 *  Created on: Jan 20, 2017
 *      Author: Gaelhawks
 */
class FuelLauncher
{
	private:
		// Class Objects
#ifdef PRACTICE_BOT
		Talon *launcherMotor;
		Talon *mixMaster;
		Talon *loadWheel;
#else
		VictorSP *launcherMotor;
		VictorSP *mixMaster;
		VictorSP *loadWheel;
#endif
		Servo *shooterServo;

		Encoder *launcherEncoder;

		// Local Variables
		bool isShooterEnabled;
		bool isMixerEnabled;
		bool launcherAtSpeed;
		bool isTargetDetected;
		bool isEncoderBroken;

		// control system data
		double err;
		double err2;
		double xhatz; 	//Last xhat value
		double xhatzz;
		double xhatzzz;
		double xhat; 	//Current Encoder Position
		double xdhat; 	//(Derivative) (currentPosition - lastPosition)/looptime
		double xdhatTustin;
		double errint;

		// motor commands
		double mixCmd;
		double mixWheelMultiplier;	// to change direction for ball jams
		double loadCmd;
		double launchCmd;
		double speedCmd;	// desired speed (radians/sec)
		double speedCmdChange;
		double angleAdjust;

		// targeting data
		double deltaYPos;
		double boilerYPos;
		int lutIndex;

		// service routine data
		int launcherMode;
		int speedUpLoopCount;
		int printCount;

		double lutY[LUT_SIZE] = {143.0, 168.0, 205.0}; //Creates the lookup table for PixyCam Y-Values.
		//double lutS[LUT_SIZE] = {305.0, 315.0, 325.0}; //Creates the lookup table for Speed Values
		//double lutS[LUT_SIZE] = {303.0, 309.0, 327.0}; //was 329.0  Creates the lookup table for Speed Values
		double lutS[LUT_SIZE] = {308.0, 316.0, 337.0}; //was 329.0  Creates the lookup table for Speed Values
		double lutA[LUT_SIZE] = { 0.66,   0.8,   0.9}; //Creates the lookup table for Angle Values

		TalonXVIII *mainRobot;

   	public:
		FuelLauncher(TalonXVIII* pRobot);
		void LocalReset(void);
		void StopAll(void);
		void StartingConfig(void);
		void UpdateDash(void);
		void Service(void);

		void SpinUpLauncher(void);
		void StopLauncher(void);
		void DecreaseSpeedAdjustment(void);
		void IncreaseSpeedAdjustment(void);
		void ResetSpeedAdjustment(void);
		void OverrideEncoder(void);
		void CancelEncoderOverride(void);
		double GetTargetCenter(void);

		void EnableMixer(void);
		void StopMixer(void);
		void ReverseMixerWheel(bool reversed);

		void ShotAdjuster(double adjust);
		void LongShot(void);
		void ShortShot(void);

   	private:
		void ProcessBoilerTarget(void);
		void GenerateSpeedCmd(void);
		void GenerateLaunchCmd(void);
		void LauncherState(void);

		double Limit(double min, double max, double curValue);
		double Sign(double inputValue);
};



#endif /* FUELLAUNCHER_H_ */
