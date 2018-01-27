#include "ControlTask.h"
#include "maxutils/MaxDataStream.h"
#include "maxutils/MaxControls.h"

MaxTaskSchedule taskschedule;
ControlTask ControlTaskInstance(&taskschedule);

RobotControl::RobotControl()
{
	SpeedLeft = 0;
	SpeedRight = 0;
	SpeedLift = 0;
	SpeedArmLeft = 0;
	SpeedArmRight = 0;
	SpeedGrabWheel = 0;
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

}

void ControlTask::Always()
{
	//Lift and grab
	if (Controls->ControllerType == XboxType)
	{
		Joystick * Xbox = new Joystick(0);
		Controls->SpeedLift = Xbox->GetRawAxis(3) - Xbox->GetRawAxis(2);
		delete (Xbox);
	}
	else if (Controls->ControllerType == JoystickType)
	{
		Joystick * MainJoystick = new Joystick(0);
		Controls->SpeedLift = MainJoystick->GetRawButton(4) - MainJoystick->GetRawButton(2);
		delete (MainJoystick);
	}

	// Drive Motors
	if (Controls->ControllerType == ControlMode::Tank && Controls->ControllerType == JoystickType)
	{
			Joystick * Left = new Joystick(0);
			Joystick * Right = new Joystick(1);
			Controls->SpeedLeft = Left->GetRawAxis(1);
			Controls->SpeedRight = Right->GetRawAxis(1);
			delete (Left);
			delete (Right);
	}
	else if (Controls->ControllerType == ControlMode::Tank && Controls->ControllerType == XboxType)
	{
		Joystick * Xbox = new Joystick(0);
		Controls->SpeedLeft = Xbox->GetRawAxis(1);
		Controls->SpeedRight = Xbox->GetRawAxis(5);
		delete (Xbox);
	}
	else if (Controls->ControllerType == ControlMode::Arcade && Controls->ControllerType == JoystickType)
	{
		Joystick * MainJoystick = new Joystick(0);
		Controls->SpeedLeft = MainJoystick->GetRawAxis(1) + MainJoystick->GetRawAxis(2);
		Controls->SpeedRight = MainJoystick->GetRawAxis(1) - MainJoystick->GetRawAxis(2);
		Controls->SpeedLeft = Controls->SpeedLeft >= 0.99 ? 0.99 : Controls->SpeedLeft;
		Controls->SpeedLeft = Controls->SpeedLeft <= -0.99 ? -0.99 : Controls->SpeedLeft;
		Controls->SpeedRight = Controls->SpeedRight >= 0.99 ? 0.99 : Controls->SpeedRight;
		Controls->SpeedRight = Controls->SpeedRight <= -0.99 ? -0.99 : Controls->SpeedRight;
		delete (MainJoystick);
	}
	else if (Controls->ControllerType == ControlMode::Arcade && Controls->ControllerType == XboxType)
	{
		Joystick * Xbox = new Joystick(0);
		Controls->SpeedLeft = Xbox->GetRawAxis(1) + Xbox->GetRawAxis(2);
		Controls->SpeedRight = Xbox->GetRawAxis(1) - Xbox->GetRawAxis(2);
		delete (Xbox);
	}
	taskschedule_->DispatchControl(Controls);
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

void ControlTask::UpdateAutonomousData(AutonomousControl)
{
	
}

void ControlTask::ProcessOscData(osc::ReceivedMessage messages)
{

	if (strcmp(messages.AddressPattern(), "/Dashboard/ControllerType") == 0)
	{
		osc::ReceivedMessageArgumentStream args = messages.ArgumentStream();
		const char * CharControllerType;
		args >> CharControllerType >> osc::EndMessage;
		if (strcmp(CharControllerType, "Joystick"))
		{
			Controls->ControllerType = ControlType::JoystickType;
		}
		else if (strcmp(CharControllerType, "Xbox_Controller"))
		{
			Controls->ControllerType = ControlType::XboxType;
		}
		else
		{
			Controls->ControllerType = ControlType::JoystickType;
		}
		delete (CharControllerType);
	}
	if (strcmp(messages.AddressPattern(), "/Dashboard/DriveModeMessage") == 0)
	{
		osc::ReceivedMessageArgumentStream args = messages.ArgumentStream();
		const char * CharDriveModeMessage;
		args >> CharDriveModeMessage >> osc::EndMessage;
		if (strcmp(CharDriveModeMessage, "Arcade"))
		{
			Controls->ControllerMode = ControlMode::Arcade;
		}
		else if (strcmp(CharDriveModeMessage, "Tank"))
		{
			Controls->ControllerMode = ControlMode::Tank;
		}
		else
		{
			Controls->ControllerMode = ControlMode::Arcade;
		}
		delete (CharDriveModeMessage);
	}
}

void ControlTask::Init()
{
	Controls = new RobotControl();
}