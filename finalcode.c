#pragma config(Sensor, S2,     ,               sensorEV3_Ultrasonic)
#pragma config(Sensor, S3,     ,               sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     ,               sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Motor,  motorA,           ,             tmotorEV3_Large, PIDControl, reversed, encoder)
#pragma config(Motor,  motorB,           ,             tmotorEV3_Large, PIDControl, reversed, encoder)
#pragma config(Motor,  motorC,           ,             tmotorEV3_Large, PIDControl, reversed, encoder)
#pragma config(Motor,  motorD,           ,             tmotorEV3_Large, PIDControl, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "PC_FileIO.c"


// B is Left Motor and C is Right Motor

//Prototype
void close_flap ();
void open_flap();
void wait_for_package_pickup();
void drive_back(int clicks, int power);
void turn(int power, int angle);
void drive_stairs (int power);
void reach_target(int room_number, int power);
void input ();


//Main
task main()
{

	SensorType[S4] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);


			motor[motorA] = 0;
			nMotorEncoder[motorA] = 0;
			while(SensorValue[S1] == 0)
			{}
			wait1Msec(2000);
			close_flap ();
			wait1Msec(500);
			drive_stairs(100);
			wait1Msec(500);
			motor[motorC] = motor[motorB] = 100;
			wait1Msec(1000);
			turn(100, 90);
			reach_target( 1, 60);
			wait1Msec(50);
			turn(15, 90);
			wait1Msec(50);
			motor[motorC] = motor[motorB] = 65;
			wait1Msec(500);
			motor[motorB] = motor[motorC] = 0;
			wait1Msec(50);
			drive_stairs(50);/*
			wait1Msec(50);
			turn(15, 180);
			wait1Msec(50);
			motor[motorB] = motor[motorC] = 25;
			wait1Msec(500);
			motor[motorB] = motor[motorC] = 0;
			wait1Msec(100);*/




	/*clearTimer(T1);

	time1[T1] = 0;
	wait1Msec(50);
	while (time1[T1] < 10000)
	{

		input();

		clearTimer(T1);

		while (time1[T1] < 10000)
		{}

	}*/
}



// Functions
void close_flap()
{

	nMotorEncoder[motorA] = 0;
	const int MOTOR_POWER = -7;
	motor[motorA] = MOTOR_POWER;
	while (abs(nMotorEncoder[motorA]) < 20)
	{}
	motor[motorA] = 0;
	wait1Msec(50);
}

void open_flap()
{
	nMotorEncoder[motorA] = 0;
	const int MOTOR_POWER = 7;
	motor[motorA] = MOTOR_POWER;
	while (abs(nMotorEncoder[motorA]) < 20)
	{}
	motor[motorA] = 0;
	wait1Msec(50);
}

void wait_for_package_pickup()
{
	open_flap();
	wait1Msec(50);
	while (SensorValue[S1] == 1)
	{}

	close_flap();

}

void drive_back(int clicks, int power)
{
motor[motorB] = motor[motorC] = -power;
nMotorEncoder[motorB] = 0;

while(abs(nMotorEncoder[motorB]) < abs(clicks))
{}

motor[motorB] = motor[motorC] = 0;

}

void turn(int power, int angle)
{
	SensorType[S4] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);

	motor[motorD] = 0;
	motor[motorB] = power;
	motor[motorC] = (power/4) * -1;
	while (abs(getGyroDegrees(S4)) < abs(angle))
	{}
	motor[motorB] = motor[motorC] = 0;
}

void drive_stairs(int power)
{
	resetGyro(S4);
	motor[motorB] = motor[motorC] = motor[motorD] = power;

//	float pid_power = 0;
	const int tolerance = 5;
//	const float kP = 10;
//	const float kI = 1;
//	const float kD = 1;
//	int error = INITIAL_ANGLE - getGyroDegrees(S4);
//	int prev_error = 0;
//	time1[T4] = 0;

	while (getColorName(S3) != colorYellow)
	{
		//motor[motorB] = motor[motorC] = motor[motorD] = -power;
		if (abs(getGyroDegrees(S4)) > abs(tolerance))
		{
	//		error = abs(getGyroDegrees(S4)) - INITIAL_ANGLE;
	//		pid_power = kP*error;
			if (getGyroDegrees(S4) > tolerance)
			{
				motor[motorB] = power;
				motor[motorC] = -power;
			}
			else if (getGyroDegrees(S4) < tolerance*-1)
			{
				motor[motorB] = -power;
				motor[motorC] = power;
			}
			while (abs(getGyroDegrees(S4)) > abs(tolerance))
			{}
			motor[motorB] = motor[motorC] = -power;
		}

	}
	motor[motorB] = motor[motorC] = motor[motorD] = 0;

}

void reach_target(int room_number, int power)
{
	int counter = 0;
	int clicks = 0;
	nMotorEncoder[motorB] = 0;
	motor[motorB] = motor[motorC] = power;

	while (counter < room_number)
	{
		 if (SensorValue[S2] < 30)
		{
			motor[motorB] = motor[motorC] = 0;
			while (SensorValue[S2] < 30)
			{}
			motor[motorB] = motor[motorC] = power;
		}

		if (getColorName(S3) == colorYellow)
		{
			while (getColorName(S3) == colorYellow)
			{}
			counter++;
		}
	}

	clicks = nMotorEncoder[motorB];
	motor[motorB] = motor[motorC] = 0;
	wait1Msec(50);
	wait_for_package_pickup ();
	wait1Msec(50);
	drive_back(clicks, 60);
}

void input ()
{
	TFileHandle fin;
	bool fileOkay = openReadPC(fin, "room.txt");
	if (!fileOkay)
	{
		displayString(5, "Failure");
		wait1Msec(1000);
	}
	else
	{
		int room_number = 0;
		while (readIntPC(fin, room_number))
		{
			motor[motorA] = 0;
			nMotorEncoder[motorA] = 0;
			while(SensorValue[S1] == 0)
			{}

			wait1Msec(2000);
			close_flap ();
			wait1Msec(500);
			drive_stairs(100);
			wait1Msec(50);/*
			motor[motorC] = motor[motorB] = 50;
			wait1Msec(500);
			motor[motorB] = motor[motorC] = 0;
			wait1Msec(50);
			turn(15, 90);
			wait1Msec(500);
			reach_target(room_number, 60);
			wait1Msec(50);
			turn(15, 90);
			wait1Msec(50);
			motor[motorC] = motor[motorB] = 65;
			wait1Msec(500);
			motor[motorB] = motor[motorC] = 0;
			wait1Msec(50);
			climb(50);
			wait1Msec(50);
			turn(15, 180);
			wait1Msec(50);
			motor[motorB] = motor[motorC] = 25;
			wait1Msec(500);
			motor[motorB] = motor[motorC] = 0;
			wait1Msec(100);*/

		}
	}
}