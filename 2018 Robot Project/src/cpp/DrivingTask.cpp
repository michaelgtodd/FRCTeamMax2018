#include "DrivingTask.h"
#include <iostream>
#include "maxutils/MaxDataStream.h"


void DrivingTask::Run()
{
	
}

void DrivingTask::Disable()
{

}

void DrivingTask::Autonomous()
{

}

void DrivingTask::ControllerUpdate(MaxControl * controls)
{
	ControlInput = (RobotControl *)(controls);
}

void DrivingTask::Init()
{
	ControlInput = new RobotControl();

	LeftMotor1 = new TalonSRX(1);
	LeftMotor2 = new TalonSRX(2);
	LeftMotor3 = new TalonSRX(3);
	RightMotor1 = new TalonSRX(4);
	RightMotor2 = new TalonSRX(5);
	RightMotor3 = new TalonSRX(6);
}