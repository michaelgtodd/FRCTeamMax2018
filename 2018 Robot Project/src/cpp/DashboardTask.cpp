#include "DashboardTask.h"
#include <iostream>
#include "maxutils/MaxDataStream.h"


void DashboardTask::Run()
{
	MaxLog::TransmitDouble("/Joystick/1/XAxis", Control->ActiveJoystick->GetX());
	MaxLog::TransmitDouble("/Joystick/1/YAxis", Control->ActiveJoystick->GetY());
	MaxLog::TransmitDouble("/Joystick/1/ZAxis", Control->ActiveJoystick->GetZ());
}

void DashboardTask::Disable()
{

}

void DashboardTask::Autonomous()
{

}

void DashboardTask::ControllerUpdate(MaxControl * controls)
{
	Control = (RobotControl *)(controls);
}

void DashboardTask::Init()
{

}