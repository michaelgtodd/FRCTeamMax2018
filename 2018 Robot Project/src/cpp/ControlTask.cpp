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
	DriverType = ControlType::JoystickType;
	SwitchesType = ControlType::JoystickType;
	DriverMode = ControlLayout::Arcade;
	SwitchesMode = ControlLayout::Arcade;
	DriveDebug = true;
	SwitchesDebug = true;
	SpeedLeft = 0;
	SpeedRight = 0;
	SpeedLift = 0;
	LeftArmPosition = 180;
	RightArmPosition = 180;
	StartingPos = FieldPos::Right;
	SwitchPrioritySelection = SwitchPriority::Kyle;
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
	//======================================================================================
	// Switches Controllers
	//======================================================================================	

	/*Get inputs from joystick based on controller type*/
	double LiftAxis;
	bool Clamp, Neutral, StartingPos, Retract;
	bool SpinIn, SpinOut;
	bool EnableSwitchesDebug;
	Joystick * SwitchesJoystick = new Joystick(2);

	if (Controls->SwitchesType == XboxType)
	{

	}
	else //Joystick arcade
	{
		Clamp = SwitchesJoystick->GetRawButton(1);
		Neutral = SwitchesJoystick->GetRawButton(2);
		StartingPos = SwitchesJoystick->GetRawButton(7);
		Retract = SwitchesJoystick->GetRawButton(8);
		SpinIn = SwitchesJoystick->GetPOV() == 180 ? true : false;
		SpinOut = SwitchesJoystick->GetPOV() == 0 ? true : false;
		LiftAxis = SwitchesJoystick->GetRawAxis(1);
		EnableSwitchesDebug = SwitchesJoystick->GetRawAxis(3) < 0 ? true : false;
	}

	/*Set lift speed*/
	Controls->SpeedLift = (fabs(LiftAxis) > 0.25) ? LiftAxis : 0;
	Controls->SpeedLift = fmin(Controls->SpeedLift, 0.2);

	/*Set arm position*/
	if (Neutral)
	{
		Controls->LeftArmPosition = 125;
	}
	else if (Retract)
	{
		Controls->LeftArmPosition = 340;
	}
	else if (StartingPos)
	{
		Controls->LeftArmPosition = 146;
	}
	else if (Clamp)
	{
		Controls->LeftArmPosition = 80;
	}

	/*Set speed of grab wheels*/
	if (SpinIn)
		Controls->WheelSpeed = 1;
	else if (SpinOut)
		Controls->WheelSpeed = -1;
	else
		Controls->WheelSpeed = 0;

	/*Finish up*/
	delete (SwitchesJoystick);
	Controls->SwitchesDebug = EnableSwitchesDebug;
	Controls->RightArmPosition = 360 - Controls->LeftArmPosition;

	//======================================================================================
	// Drive Controllers
	//======================================================================================

	/*Get inputs from joystick based on controller type*/
	double ForwardAxis, TwistAxis;
	Joystick * DriveJoystick = new Joystick(0);

	if (Controls->DriverType == XboxType)
	{
		ForwardAxis = -DriveJoystick->GetRawAxis(1);
		TwistAxis = -DriveJoystick->GetRawAxis(4);
	}
	else //Joystick arcade
	{
		ForwardAxis = -DriveJoystick->GetRawAxis(1);
		TwistAxis = -DriveJoystick->GetRawAxis(2);
	}
	Controls->SpeedLeft = 0;
	Controls->SpeedRight = 0;
	double twist;
	double twistmin = 0;
	if (fabs(TwistAxis) > 0.5)
	{
#if COMP_BOT
		twist = fabs(TwistAxis);
#endif
		twist *= -1.0;
		twist *= (fabs(TwistAxis) / TwistAxis);
		runs++;
	}
	else
	{
		twist = 0.0;
	}
	if (runs % 10 == 0)
	{
		MaxLog::TransmitDouble("/twist", twist);
		MaxLog::TransmitDouble("/twistmin", twistmin);
		runs = 0;
	}

	Controls->SpeedLeft = (twist)+(fabs(ForwardAxis) > 0.025 ? ForwardAxis : 0);
	Controls->SpeedRight = (twist)-(fabs(ForwardAxis) > 0.025 ? ForwardAxis : 0);
	delete (DriveJoystick);
	//std::cout << "About to send" << std::endl;
}

void ControlTask::Always()
{
	//======================================================================================
	// Autonomous Selection
	//======================================================================================
	switch (Controls->StartingPos)
	{
	case FieldPos::Left:
		MaxAutonomousManagerInstance.SelectAutonomous("AutonomousLeft");
		//std::cout << "Left auto" << std::endl;
		break;
	case FieldPos::Center:
		MaxAutonomousManagerInstance.SelectAutonomous("AutonomousCenter");
		//std::cout << "Center auto" << std::endl;
		break;
	case FieldPos::Right:
		MaxAutonomousManagerInstance.SelectAutonomous("AutonomousRight");
		//std::cout << "Right auto" << std::endl;
		break;
	default:
		break;
	}
	// This must remain in Always()
	taskschedule_->DispatchControl(Controls);
}

void ControlTask::Disable()
{
	Controls->SpeedLeft = 0;
	Controls->SpeedRight = 0;
	Controls->SpeedLift = 0;
	Controls->LeftArmPosition = 180;
	Controls->RightArmPosition = 180;
}

void ControlTask::ControllerUpdate(MaxControl * controls)
{

}

void ControlTask::Autonomous()
{

}

void ControlTask::UpdateAutonomousData(AutonomousControl AutoControlInput)
{
	Controls->LeftArmPosition = AutoControlInput.ArmPositionLeft;
	Controls->RightArmPosition = AutoControlInput.ArmPositionRight;
	Controls->SpeedLeft = AutoControlInput.SpeedLeft;
	Controls->SpeedRight = AutoControlInput.SpeedRight;
	//std::cout << "In to controls: " << AutoControlInput.ArmPositionRight << " Out of control: " << Controls->RightArmPosition << std::endl;
	Controls->SpeedLift = AutoControlInput.SpeedLift;
}

void ControlTask::ProcessOscData(osc::ReceivedMessage messages)
{
	//======================================================================================
	// Get data from the dashboard
	//======================================================================================

	/*Get starting position*/
	if (strcmp(messages.AddressPattern(), "/Dashboard/AutoPositionMessage/") == 0)
	{
		osc::ReceivedMessageArgumentStream args = messages.ArgumentStream();
		const char * CharAutoPositionMessage;
		args >> CharAutoPositionMessage >> osc::EndMessage;
		if (strcmp(CharAutoPositionMessage, "Left") == 0)
		{
			Controls->StartingPos = FieldPos::Left;
		}
		else if (strcmp(CharAutoPositionMessage, "Mid") == 0)
		{
			Controls->StartingPos = FieldPos::Center;
		}
		else
		{
			Controls->StartingPos = FieldPos::Right;
		}
	}

	/*Get switch priority*/
	if (strcmp(messages.AddressPattern(), "/Dashboard/AutoSwitchPriority/") == 0)
	{
		osc::ReceivedMessageArgumentStream args = messages.ArgumentStream();
		const char * CharAutoSwitchPriority;
		args >> CharAutoSwitchPriority >> osc::EndMessage;
		if (strcmp(CharAutoSwitchPriority, "Yes") == 0)
		{
			Controls->SwitchPrioritySelection = SwitchPriority::Yes;
		}
		else if (strcmp(CharAutoSwitchPriority, "No") == 0)
		{
			Controls->SwitchPrioritySelection = SwitchPriority::No;
		}
		else
		{
			Controls->SwitchPrioritySelection = SwitchPriority::Kyle;
		}
	}

	/*Get driver controller type*/
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

	/*Get driver controller mode*/
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

	/*Get switches controller type*/
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

	/*Get switches controller mode*/
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
	runs = 0;
	debugCounter = 0;
}