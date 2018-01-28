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
	SpeedGrabWheelLeft = 0;
	SpeedGrabWheelRight = 0;
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

	}
	else if (Controls->ControllerType == JoystickType)
	{
		Joystick * SwitchesLeft = new Joystick(2);
		Joystick * SwitchesRight = new Joystick(3);
		if (SwitchesLeft->GetRawAxis(1) >= 0.25)
		{
			Controls->SpeedLift = 1.25 * (SwitchesLeft->GetRawAxis(1) - 0.25);
		}
		else if (SwitchesLeft->GetRawAxis(1) <= -0.25)
		{
			Controls->SpeedLift = 1.25 * (SwitchesLeft->GetRawAxis(1) + 0.25);
		}
		if (SwitchesRight->GetRawAxis(1) >= 0.25)
		{
			Controls->SpeedLift = Controls->SpeedLift - 1.25 * (SwitchesRight->GetRawAxis(1) - 0.25);
		}
		else if (SwitchesRight->GetRawAxis(1) <= -0.25)
		{
			Controls->SpeedLift = Controls->SpeedLift - 1.25 * (SwitchesRight->GetRawAxis(1) + 0.25);
		}
		Controls->SpeedLift = Controls->SpeedLift >= 0.99 ? 0.99 : Controls->SpeedLift;
		Controls->SpeedLift = Controls->SpeedLift <= -0.99 ? -0.99 : Controls->SpeedLift;
		Controls->SpeedGrabWheelLeft = SwitchesLeft->GetRawButton(0) - SwitchesLeft->GetRawButton(1);
		Controls->SpeedGrabWheelRight = SwitchesRight->GetRawButton(5) - SwitchesRight->GetRawButton(3);
		if ((SwitchesRight->GetRawButton(0)) || (SwitchesLeft->GetRawButton(0)))
		{
			Controls->SpeedGrabWheelLeft = 1;
			Controls->SpeedGrabWheelRight = 1;
		}
		Controls->SpeedArmLeft = SwitchesLeft->GetRawAxis(2);
		Controls->SpeedArmRight = SwitchesRight->GetRawAxis(2);
		delete (SwitchesLeft);
		delete (SwitchesRight);
	}

	// Drive Motors
	if (Controls->ControllerType == ControlLayout::Tank && Controls->ControllerType == JoystickType)
	{
			Joystick * Left = new Joystick(0);
			Joystick * Right = new Joystick(1);
			Controls->SpeedLeft = Left->GetRawAxis(1);
			Controls->SpeedRight = Right->GetRawAxis(1);
			delete (Left);
			delete (Right);
	}
	else if (Controls->ControllerType == ControlLayout::Tank && Controls->ControllerType == XboxType)
	{
		Joystick * Xbox = new Joystick(0);
		Controls->SpeedLeft = Xbox->GetRawAxis(1);
		Controls->SpeedRight = Xbox->GetRawAxis(5);
		delete (Xbox);
	}
	else if (Controls->ControllerType == ControlLayout::Arcade && Controls->ControllerType == JoystickType)
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
	else if (Controls->ControllerType == ControlLayout::Arcade && Controls->ControllerType == XboxType)
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
			Controls->ControllerMode = ControlLayout::Arcade;
		}
		else if (strcmp(CharDriveModeMessage, "Tank"))
		{
			Controls->ControllerMode = ControlLayout::Tank;
		}
		else
		{
			Controls->ControllerMode = ControlLayout::Arcade;
		}
		delete (CharDriveModeMessage);
	}
}

void ControlTask::Init()
{
	Controls = new RobotControl();
}