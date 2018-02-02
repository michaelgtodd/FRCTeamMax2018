#include "ControlTask.h"
#include "maxutils/MaxDataStream.h"
#include "maxutils/MaxControls.h"
#include <iostream>
#include <math.h>

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
	SolenoidPos = 0;
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
	else
	{
		Joystick * SwitchesLeft = new Joystick(2);
		Joystick * SwitchesRight = new Joystick(3);

		Controls->SpeedLift = 0;
		if (fabs(SwitchesLeft->GetRawAxis(1)) >= 0.25)
		{
			Controls->SpeedLift = -SwitchesLeft->GetRawAxis(1);
		}
		if (fabs(SwitchesRight->GetRawAxis(1)) >= 0.25)
		{
			Controls->SpeedLift = -SwitchesRight->GetRawAxis(1);
		}
<<<<<<< HEAD

=======
		Controls->SpeedLift = std::max(-1, Controls->SpeedLift);
		Controls->SpeedLift = std::min(1, Controls->SpeedLift);
>>>>>>> 3f7f51856daf615bfdb80be5e16c1f00a5aad0c3

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
	if (Controls->ControllerMode == ControlLayout::Tank && Controls->ControllerType == JoystickType)
	{
			Joystick * Left = new Joystick(0);
			Joystick * Right = new Joystick(1);
			Controls->SpeedLeft = Left->GetRawAxis(1);
			Controls->SpeedRight = Right->GetRawAxis(1);
			Controls->SolenoidPos = ((Left->GetRawButton(0) + Right->GetRawButton(0)) >= 1) ? 1 : -1;
			delete (Left);
			delete (Right);
	}
	else if (Controls->ControllerMode == ControlLayout::Tank && Controls->ControllerType == XboxType)
	{
		Joystick * Xbox = new Joystick(0);
		Controls->SpeedLeft = Xbox->GetRawAxis(1);
		Controls->SpeedRight = Xbox->GetRawAxis(5);
		Controls->SolenoidPos = (Xbox->GetRawButton(4) == 1) ? 1 : -1;
		delete (Xbox);
	}
	else if (Controls->ControllerMode == ControlLayout::Arcade && Controls->ControllerType == XboxType)
	{
		Joystick * Xbox = new Joystick(0);
		Controls->SpeedLeft = Xbox->GetRawAxis(1) + Xbox->GetRawAxis(2);
		Controls->SpeedRight = Xbox->GetRawAxis(1) - Xbox->GetRawAxis(2);
		Controls->SolenoidPos = (Xbox->GetRawButton(4) == 1) ? 1 : -1;
		delete (Xbox);
	}
	else
	{
		Joystick * MainJoystick = new Joystick(0);
		Controls->SolenoidPos = (MainJoystick->GetRawButton(1) == 1) ? 1 : -1;
		Controls->SpeedLeft = -MainJoystick->GetRawAxis(1) + MainJoystick->GetRawAxis(2);
		Controls->SpeedRight = -MainJoystick->GetRawAxis(1) - MainJoystick->GetRawAxis(2);
		Controls->SpeedLeft = -Controls->SpeedLeft;
		delete (MainJoystick);
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