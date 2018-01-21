#include "DashboardTask.h"
#include <iostream>
#include "maxutils/MaxDataStream.h"


void DashboardTask::Run()
{
	MaxLog::TransmitDouble("/Joystick/1/XAxis", 0);
	MaxLog::TransmitDouble("/Joystick/1/YAxis", 0);
	MaxLog::TransmitDouble("/Joystick/1/ZAxis", 0);
	MaxLog::TransmitString("/Error/Test", "this is a test");
	MaxLog::TransmitString("/Error/Test", "tom is dumb");
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