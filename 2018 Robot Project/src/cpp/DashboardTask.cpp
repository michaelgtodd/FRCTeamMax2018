#include "DashboardTask.h"
#include <iostream>
#include "maxutils/MaxDataStream.h"

#define JOYSTICK_COUNT 4

void DashboardTask::Always()
{
	for (int i = 0; i < JOYSTICK_COUNT; i++) 
	{
		Joystick * ActiveJoystick = new Joystick(i);
		for (int j = 0; j < ActiveJoystick->GetAxisCount(); j++)
		{
			std::string TransmitPath = "/Joystick/" + std::to_string(i) + "/Axis/" + std::to_string(j);
			MaxLog::TransmitDouble(TransmitPath, ActiveJoystick->GetRawAxis(j));
		}
		for (int k = 0; k <ActiveJoystick->GetButtonCount(); k++)
		{
			std::string TransmitPath = "/Joystick/" + std::to_string(i) + "/Button/" + std::to_string(k);
			MaxLog::TransmitInt(TransmitPath, ActiveJoystick->GetRawButton(k));
		}
		delete (ActiveJoystick);
	}
}

void DashboardTask::Run()
{

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