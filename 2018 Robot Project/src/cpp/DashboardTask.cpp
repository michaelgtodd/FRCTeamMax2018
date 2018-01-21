#include "DashboardTask.h"
#include <iostream>
#include "maxutils/MaxDataStream.h"


void DashboardTask::Run()
{
	MaxLog::TransmitDouble("/Joystick/1/XAxis", Control->ActiveJoystick->GetX());
	MaxLog::TransmitDouble("/Joystick/1/YAxis", Control->ActiveJoystick->GetY());
	MaxLog::TransmitDouble("/Joystick/1/ZAxis", Control->ActiveJoystick->GetZ());
	MaxLog::TransmitString("/Error/Test", "this is a test");
	MaxLog::TransmitString("/Error/Test", "tom is dumb");
	MaxLog::TransmitInt("/Joystick/1/B1", (int)Control->ActiveJoystick->GetRawButton(1));
	MaxLog::TransmitInt("/Joystick/1/B2", (int)Control->ActiveJoystick->GetRawButton(2));
	MaxLog::TransmitInt("/Joystick/1/B3", (int)Control->ActiveJoystick->GetRawButton(3));
	MaxLog::TransmitInt("/Joystick/1/B4", (int)Control->ActiveJoystick->GetRawButton(4));
	MaxLog::TransmitInt("/Joystick/1/B5", (int)Control->ActiveJoystick->GetRawButton(5));
	MaxLog::TransmitInt("/Joystick/1/B6", (int)Control->ActiveJoystick->GetRawButton(6));
	MaxLog::TransmitInt("/Joystick/1/B7", (int)Control->ActiveJoystick->GetRawButton(7));
	MaxLog::TransmitInt("/Joystick/1/B8", (int)Control->ActiveJoystick->GetRawButton(8));
	MaxLog::TransmitInt("/Joystick/1/B9", (int)Control->ActiveJoystick->GetRawButton(9));
	MaxLog::TransmitInt("/Joystick/1/B10", (int)Control->ActiveJoystick->GetRawButton(10));
	MaxLog::TransmitInt("/Joystick/1/B11", (int)Control->ActiveJoystick->GetRawButton(11));
	MaxLog::TransmitInt("/Joystick/1/B12", (int)Control->ActiveJoystick->GetRawButton(12));
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