#include "ControlTask.h"
#include "maxutils/MaxDataStream.h"
#include "maxutils/MaxControls.h"

RobotControl::RobotControl()
{
	SpeedLeft = 0;
	SpeedRight = 0;
	SpeedLift = 0;
}

ControlTask::ControlTask(MaxTaskSchedule * taskschedule)
{
	AxisPrimaryX = 0;
	AxisPrimaryY = 0;
	AxisSecondaryX = 0;
	AxisSecondaryY = 0;
	taskschedule_ = taskschedule;
}

void ControlTask::Run()
{
	
	ControlMode DriveMode = Tank;
	if (DriveMode == Tank && controls->ControllerType == JoystickType)
	{
			Joystick * Left = new Joystick(1);
			Joystick * Right = new Joystick(2);
			controls->SpeedLeft = Left->GetRawAxis(1);
			controls->SpeedRight = Right->GetRawAxis(1);
			delete (Left);
			delete (Right);
	}
	else if (DriveMode == Tank && controls->ControllerType == XboxType)
	{
		Joystick * Xbox = new Joystick(1);
		controls->SpeedLeft = Xbox->GetRawAxis(1);
		controls->SpeedRight = Xbox->GetRawAxis(5);
		delete (Xbox);
	}
	else if (DriveMode == Arcade && controls->ControllerType == JoystickType)
	{
		Joystick * MainJoystick = new Joystick(1);
		controls->SpeedLeft = MainJoystick->GetRawAxis(1) + MainJoystick->GetRawAxis(2);
		controls->SpeedRight = MainJoystick->GetRawAxis(1) - MainJoystick->GetRawAxis(2);
		delete (MainJoystick);
	}
	else if (DriveMode == Arcade && controls->ControllerType == XboxType)
	{
		Joystick * Xbox = new Joystick(1);
		controls->SpeedLeft = Xbox->GetRawAxis(1) + Xbox->GetRawAxis(2);
		controls->SpeedRight = Xbox->GetRawAxis(1) - Xbox->GetRawAxis(2);
		delete (Xbox);
	}
	taskschedule_->DispatchControl(controls);
}

void ControlTask::Disable()
{

}

void ControlTask::ControllerUpdate(MaxControl * controls)
{

}

void ControlTask::Autonomous()
{

}

void ControlTask::ProcessOscData(osc::ReceivedMessage messages)
{
	MaxLog::LogInfo("Hey, got a packet");
	if (strcmp(messages.AddressPattern(), "/Dashboard/ControllerType") == 0)
	{
		osc::ReceivedMessageArgumentStream args = messages.ArgumentStream();
		const char * CharControllerType;
		args >> CharControllerType >> osc::EndMessage;
		if (strcmp(CharControllerType, "Joystick"))
		{
			controls->ControllerType = ControlType::JoystickType;
		}
		else if (strcmp(CharControllerType, "Xbox_Controller"))
		{
			controls->ControllerType = ControlType::XboxType;
		}
		else
		{
			controls->ControllerType = ControlType::JoystickType;
		}
	}
	
}

void ControlTask::Init()
{
	controls = new RobotControl();

}