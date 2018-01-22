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
	ControlType InputMode = JoystickType;
	ControlMode DriveMode = Tank;
	if (DriveMode == Tank && InputMode == JoystickType)
	{
			Joystick * Left = new Joystick(1);
			Joystick * Right = new Joystick(2);
			controls->SpeedLeft = Left->GetRawAxis(1);
			controls->SpeedRight = Right->GetRawAxis(1);
			delete (Left);
			delete (Right);
	}
	else if (DriveMode == Tank && InputMode == XboxType)
	{
		Joystick * Xbox = new Joystick(1);
		controls->SpeedLeft = Xbox->GetRawAxis(1);
		controls->SpeedRight = Xbox->GetRawAxis(5);
		delete (Xbox);
	}
	else if (DriveMode == Arcade && InputMode == JoystickType)
	{

	}
	else if (DriveMode == Arcade && InputMode == XboxType)
	{

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
}

void ControlTask::Init()
{
	controls = new RobotControl();

}