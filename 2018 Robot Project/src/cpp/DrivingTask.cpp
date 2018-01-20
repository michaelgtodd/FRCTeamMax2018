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
}

void DrivingTask::ConfigureCurrentLimit(TalonSRX * talon)
{
	talon->ConfigContinuousCurrentLimit(1, 0);
	talon->ConfigPeakCurrentLimit(0, 0);
	talon->ConfigPeakCurrentDuration(0, 0);
	talon->EnableCurrentLimit(true);
}