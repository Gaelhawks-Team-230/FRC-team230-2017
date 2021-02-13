/*
 * AutonomousModes.h
 *
 *  Created on: Feb 7, 2017
 *      Author: jacob_000
 */

#ifndef SRC_AUTONOMOUS_MODES_H_
#define SRC_AUTONOMOUS_MODES_H_

// NOTE: negative rotate command turns counter-clockwise
#define AUTO_GEAR_LOAD_WAIT		((int)(0.4 * N1SEC))  //((int)(0.25 * N1SEC))
#define AUTO_TURN_90_DEGREES	((int)(1.0 * (N1SEC)))
#define AUTO_TURN_60_DEGREES	((int)(1.0 * (N1SEC)))
#define AUTO_STRAFE_AWAY		((int)(0.75 * N1SEC))
#define AUTO_TARGET_STRAFE		((int)(1.0 * (N1SEC)))
#define AUTO_STRAFE_OUT			((int)(1.0 * N1SEC))
#define AUTO_STRAFE_HIT			((int)(3.0 * N1SEC))
#define AUTO_TRACK_TIME			((int)(1.0 * N1SEC))
#define GEAR_TRACK_TIME			((int)(0.15 * N1SEC))
#define AUTO_GEAR_DRIVE_TIME	((int)(3.0 * N1SEC))
#define SHOOTER_START_TIME		((int)(1.0 * N1SEC))

#define K_AUTO_DRIVE			(0.01)

#define DISTANCE_TO_BASELINE			(93.3)//inches
#define DISTANCE_TO_AIRSHIP				(80.0)//115 //inches
#define DISTANCE_FRONT_GEAR_TARGETTING	(55.0)//inches
#define DISTANCE_FRONT_TARGET_LIFT		(50.0)
#define DISTANCE_HOPPER_STRAFE			(50.0)//inches
#define DISTANCE_HOPPER_FRONT			(78.5)//inches
#define DISTANCE_HOPPER_CENTER			(97.0)//102.0)//82.0 //118 //inches
#define DISTANCE_HOPPER_BACK			(126.5)//inches
#define DISTANCE_SIDE_LIFT				(70.0)//114.5 //inches
#define DISTANCE_FRONT_BARRIER			(12.75)//inches
#define DISTANCE_BARRIER_TO_LIFT		(40.0)//inches
#define DISTANCE_FRONT_LIFT_KEY			(69.25)//inches
#define DISTANCE_AIRSHIP_BACKUP			(42.0)

#define AUTO_DRIVE_BACKWARD_GEAR    (-5.0) //inches
#define AUTO_DRIVE_RIGHT_HOPPER     (10.0) //inches
#define AUTO_DRIVE_FORWARD_BOILER   (48.0) //55.0 //24.0 //inches
#define AUTO_DRIVE_AIRSHIP_BOILER   (50.0) //inches


/*BOILER:
 * RIGHT WHEN RED ALLIANCE
 * LEFT WHEN BLUE ALLIANCE
 */


#endif /* SRC_AUTONOMOUS_MODES_H_ */
