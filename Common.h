/*
 * Common.h
 *
 *  Created on: Jan 18, 2017
 *      Author: Gaelhawks
 */

#ifndef SRC_COMMON_H_
#define SRC_COMMON_H_

#include "WPILib.h"
#include "TalonXVIII_main.h"

/**********************\
 *  Feature Enablers  *
\**********************/
#define USE_GYRO
//#define PRACTICE_BOT  1
//#define CHECK_LOOPTIMING 1


/*******************\
 *  Global Values  *
\*******************/
#define PI	 						(3.14159265)
#define RAD_TO_DEG 					(180.0/PI)
#define DEG_TO_RAD					(PI/180.0)
#define X_SCALING_FACTOR 			(1)
#define Y_SCALING_FACTOR			(0.65)

#define JOYSTICK_DEADBAND			(0.3)
#define DRIVE_DEADBAND				(0.1)
#define GAMEPAD_DEADBAND			(0.1)

#define LOOPTIME 					(0.02)   // (0.005)   // (0.01)
#define PIXYTIME					(0.008)
#define SAMPLES_PER_SECOND			(1.0/LOOPTIME)
#define N1SEC  						((int) SAMPLES_PER_SECOND)

#define PIXY_BOILER_I2C_ADDRESS    	(0x54)
#define PIXY_GEAR_I2C_ADDRESS       (0x5A)
#define MAX_PIXY_BLOCKS				(2)

typedef enum
{
	// example: USB_JOY_DRIVE	= 1,
	// example: USB_GAMEPAD		= 2,

} usbs;

typedef enum
{
	CLIMBER_CURRENT_IN		= 2,
	//GEAR_CURRENT_IN			= 8,   // practice bot
	GEAR_CURRENT_IN			= 9,
} pdp;

typedef enum
{
	PWM_BACK_LEFT		= 0,
	PWM_BACK_RIGHT		= 1,
	PWM_FRONT_LEFT		= 2,
	PWM_FRONT_RIGHT		= 3,
	PWM_FUELLAUNCHER 	= 4,
	PWM_ROPECLIMBER		= 5,
	PWM_SHOOTER_SERVO	= 6,
	PWM_MIXMASTER		= 7,
	PWM_FUELPICKUP		= 8,
	PWM_INDEXWHEEL      = 9,
	PWM_GEARMOTOR   	= 11,
	PWM_GEARCOLLECTOR	= 10,
} pwms;

typedef enum
{
	GEAR_ANALOG_INPUT	 			 = 0,
	//example GYRO_ANALOG_INPUT   = 1,
	//example GRAB_ANALOG_INPUT   = 0,

} analogs;

typedef enum
{
	// example: CLAW_PISTON1		= 0,
	// example: CLAW_PISTON2		= 4,

}solenoids;

typedef enum
{
	DIGIN_ENCODER_LAUNCHER_ONE		= 8,
	DIGIN_ENCODER_LAUNCHER_TWO		= 9,
	FRONT_LEFT_ENCODER_ONE			= 4,
	FRONT_LEFT_ENCODER_TWO			= 5,
	FRONT_RIGHT_ENCODER_ONE			= 6,
	FRONT_RIGHT_ENCODER_TWO			= 7,
	BACK_LEFT_ENCODER_ONE			= 0,
	BACK_LEFT_ENCODER_TWO			= 1,
	BACK_RIGHT_ENCODER_ONE			= 2,
	BACK_RIGHT_ENCODER_TWO			= 3,
	LED_LIGHT_OUTPUT_1				= 22,
	LED_LIGHT_OUTPUT_2				= 23,
} digins;

/*
** List of gamepad (USB_GAMEPAD below) button and axis assignments
*/

typedef enum
{
	ENABLE_GYRO_BUTTON 			= 1,
	ENABLE_PRECISE_TURN			= 2,
	BOILER_PIXY_AIMING       	= 3, //4
	GEAR_PIXY_TARGETING         = 14, //5
	BOILER_AUTO_TARGETING		= 15,
	PIXY_RESYNC					= 16,
} joystick_buttons;

typedef enum
{
	ROTATE_AXIS                 = 5,
	SPEED_AXIS                  = 1,
	STRAFE_AXIS                 = 0,
	SERVO_AXIS					= 6,

} joystick_axes;

typedef enum
{
	//example SHOOT_LEVEL_AXIS          = 3,
	SHOOTER_UPDOWN_AXIS			= 1,

} gamepad_axes;

typedef enum
{
	DPAD_OPEN_GEARDOOR  				= 0,
	DPAD_LONG_SHOT_SERVO  		        = 90,
	DPAD_CLOSE_GEARDOOR					= 180,
	DPAD_SHORT_SHOT_SERVO		        = 270,
} gamepad_pov;

typedef enum
{
	BUTTON_START_MIXER						= 1,
	BUTTON_STOP_MIXER                       = 2,
	BUTTON_STOP_CLIMB						= 3,
	BUTTON_BEGIN_CLIMB						= 4,
	BUTTON_START_LAUNCHER                   = 5,
	BUTTON_EJECT_GEAR						= 7,
    BUTTON_START_GATHERER					= 6,
	BUTTON_GRAB_GEAR						= 8,
	BUTTON_RESET_SHOOT_CHANGE   	        = 9,
	//BUTTON_STOP_GEARDOOR                    = 10,
	BUTTON_REVERSE_MIXER				    = 10,
	//BUTTON_LIGHT_SHOW_1					= 3,
	//BUTTON_LIGHT_SHOW_2					= 4,
} gamepad_buttons;


#endif /* SRC_COMMON_H_ */
