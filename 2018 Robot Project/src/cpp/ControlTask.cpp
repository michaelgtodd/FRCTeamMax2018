#include "ControlTask.h"
#include "maxutils/MaxDataStream.h"
#include "maxutils/MaxControls.h"
#include <iostream>
#include <math.h>

using namespace std;

MaxTaskSchedule taskschedule;
ControlTask ControlTaskInstance(&taskschedule);

RobotControl::RobotControl()
{
	DriverPreference = 0;
	SwitchesPreference = 0;
	SpeedLeft = 0;
	SpeedRight = 0;
	SpeedLift = 0;
	LeftArmPosition = 0;
	RightArmPosition = 0;
	LiftHeight = 0;
	SolenoidPos = -1;
	Override = false;
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
	//======================================================================================
	// Switches Controllers
	//======================================================================================	

	double LiftAxis;
	bool GrabButton, RetractButton, OverrideButton;
	Joystick * SwitchesJoystick = new Joystick(2);

	if (Controls->SwitchesType == XboxType)
	{

	}
	else
	{
		LiftAxis = SwitchesJoystick->GetRawAxis(1);
		GrabButton = SwitchesJoystick->GetRawButton(1);
		RetractButton = SwitchesJoystick->GetRawButton(2);
		OverrideButton = SwitchesJoystick->GetRawButton(8);
	}
	Controls->SpeedLift = (fabs(LiftAxis) > 0.25) ? LiftAxis : 0;

	if (RetractButton)
	{
		Controls->LeftArmPosition = 300;
		Controls->RightArmPosition = 60;
	}
	else if (GrabButton)
	{
		Controls->LeftArmPosition = 100;
		Controls->RightArmPosition = 260;
	}
	else
	{
		Controls->LeftArmPosition = 180;
		Controls->RightArmPosition = 180;
	}
	if (OverrideButton)
	{
		Controls->Override = true;
	}

	delete (SwitchesJoystick);

	//======================================================================================
	// Drive Controllers
	//======================================================================================

	/* Determine the axis and buttons that should be used. */
	double ForwardAxis, TwistAxis;
	bool ShiftButton;

	Joystick * DriveJoystick = new Joystick(0);

	if (Controls->DriverType == XboxType)
	{
		ForwardAxis = DriveJoystick->GetRawAxis(1);
		TwistAxis = DriveJoystick->GetRawAxis(4);
		ShiftButton = DriveJoystick->GetRawButton(6);
	}
	else // Controls->DriverType == JoystickType
	{
		ForwardAxis = DriveJoystick->GetRawAxis(1);
		TwistAxis = DriveJoystick->GetRawAxis(2);
		ShiftButton = DriveJoystick->GetRawButton(1);
	}

	Controls->SolenoidPos = (ShiftButton == true) ? -1 : 1;
	Controls->SpeedLeft = 0;
	Controls->SpeedRight = 0;
	double twist;
	if (fabs(TwistAxis) > 0.3)
	{
		twist = fabs(TwistAxis) - 0.3;
		twist /= 0.7;
		twist *= 0.3;
		double twistmin = fabs(ForwardAxis) * 0.7;
		twistmin += 0.5;
		twistmin *= 3.5;
		twist = fmin(twistmin, twist);
		std::cout << "twist1: " << twist;
		twist *= -1.0;
		twist *= (fabs(TwistAxis) / TwistAxis);

		std::cout << "twist: " << twist << std::endl;
	}
	else
	{
		twist = 0.0;
	}

	Controls->SpeedLeft = (twist)+(fabs(ForwardAxis) > 0.025 ? ForwardAxis : 0);
	Controls->SpeedRight = (twist)-(fabs(ForwardAxis) > 0.025 ? ForwardAxis : 0);
	delete (DriveJoystick);
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
	switch (Auto->StartingPos) {
	case Right:
		if (Auto->ScalePos == Left && Auto->SwitchPos == Left)
		{

		}
		else if (Auto->ScalePos == Left && Auto->SwitchPos == Right)
		{

		}
		else if (Auto->ScalePos == Right && Auto->SwitchPos == Left)
		{

		}
		else if (Auto->ScalePos == Right && Auto->SwitchPos == Right)
		{

		}
		else
		{

		}

		break;
	case Center:
		if (Auto->ScalePos == Left && Auto->SwitchPos == Left)
		{

		}
		else if (Auto->ScalePos == Left && Auto->SwitchPos == Right)
		{

		}
		else if (Auto->ScalePos == Right && Auto->SwitchPos == Left)
		{

		}
		else if (Auto->ScalePos == Right && Auto->SwitchPos == Right)
		{

		}
		else
		{

		}

		break;
	case Left:
		if (Auto->ScalePos == Left && Auto->SwitchPos == Left)
		{

		}
		else if (Auto->ScalePos == Left && Auto->SwitchPos == Right)
		{

		}
		else if (Auto->ScalePos == Right && Auto->SwitchPos == Left)
		{

		}
		else if (Auto->ScalePos == Right && Auto->SwitchPos == Right)
		{

		}
		else
		{

		}

		break;
	default:

		break;
	}
}

void ControlTask::UpdateAutonomousData(AutonomousControl)
{

}

void ControlTask::ProcessOscData(osc::ReceivedMessage messages)
{
	if (strcmp(messages.AddressPattern(), "/Dashboard/DriverController/") == 0)
	{
		osc::ReceivedMessageArgumentStream args = messages.ArgumentStream();
		const char * CharControllerType;
		args >> CharControllerType >> osc::EndMessage;
		if (strcmp(CharControllerType, "Joystick") == 0)
		{
			Controls->DriverType = ControlType::JoystickType;
		}
		else if (strcmp(CharControllerType, "Xbox Controller") == 0)
		{
			Controls->DriverType = ControlType::XboxType;
		}
		else
		{
			Controls->DriverType = ControlType::JoystickType;
		}
	}

	if (strcmp(messages.AddressPattern(), "/Dashboard/DriveMode/") == 0)
	{
		osc::ReceivedMessageArgumentStream args = messages.ArgumentStream();
		const char * CharDriverMode;
		args >> CharDriverMode >> osc::EndMessage;
		if (strcmp(CharDriverMode, "Arcade") == 0)
		{
			Controls->DriverMode = ControlLayout::Arcade;
		}
		else if (strcmp(CharDriverMode, "Tank") == 0)
		{
			Controls->DriverMode = ControlLayout::Tank;
		}
		else
		{
			Controls->DriverMode = ControlLayout::Arcade;
		}
	}

	if (strcmp(messages.AddressPattern(), "/Dashboard/SwitchesController/") == 0)
	{
		osc::ReceivedMessageArgumentStream args = messages.ArgumentStream();
		const char * CharControllerType;
		args >> CharControllerType >> osc::EndMessage;
		if (strcmp(CharControllerType, "Joystick") == 0)
		{
			Controls->SwitchesType = ControlType::JoystickType;
		}
		else if (strcmp(CharControllerType, "Xbox Controller") == 0)
		{
			Controls->SwitchesType = ControlType::XboxType;
		}
		else
		{
			Controls->SwitchesType = ControlType::JoystickType;
		}
	}

	if (strcmp(messages.AddressPattern(), "/Dashboard/SwitchesMode/") == 0)
	{
		osc::ReceivedMessageArgumentStream args = messages.ArgumentStream();
		const char * CharSwitchesMode;
		args >> CharSwitchesMode >> osc::EndMessage;
		if (strcmp(CharSwitchesMode, "Tank") == 0)
		{
			Controls->SwitchesMode = ControlLayout::Tank;
		}
		else if (strcmp(CharSwitchesMode, "Xbox Controller") == 0)
		{
			Controls->SwitchesMode = ControlLayout::Arcade;
		}
		else
		{
			Controls->SwitchesMode = ControlLayout::Tank;
		}
	}

}

void ControlTask::Init()
{
	Controls = new RobotControl();

	debugCounter = 0;
}