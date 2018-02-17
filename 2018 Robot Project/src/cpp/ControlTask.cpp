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

	if (Controls->SwitchesType == XboxType && Controls->SwitchesMode == Tank)
	{
		
	}
	else if (Controls->SwitchesType == XboxType && Controls->SwitchesMode == Arcade)
	{

	}
	else if (Controls->SwitchesType == JoystickType && Controls->SwitchesMode == Arcade)
	{
		Joystick * MainJoystick = new Joystick(2);

		Controls->SpeedLift = (MainJoystick->GetRawAxis(1) > 0.25) ? MainJoystick->GetRawAxis(1) : 0;

		Controls->LeftArmPosition = (MainJoystick->GetRawButton(1)) ? 120 : 180;
		Controls->RightArmPosition = (MainJoystick->GetRawButton(1)) ? 240 : 189;

		delete(MainJoystick);
	}
	else //Joystick Arcade
	{
		Joystick * MainJoystick = new Joystick(2);

		Controls->SpeedLift = (fabs(MainJoystick->GetRawAxis(1)) > 0.25) ? MainJoystick->GetRawAxis(1) : 0;
		//std::cout << "Speed of Lift in Control Task:" << Controls->SpeedLift << std::endl;

		if (MainJoystick->GetRawButton(2))
		{
			Controls->LeftArmPosition = 300;
			Controls->RightArmPosition = 60;
			//Controls->SpeedLift = 0;
		}
		else if (MainJoystick->GetRawButton(1))
		{
			Controls->LeftArmPosition = 120;
			Controls->RightArmPosition = 240;
		}
		else
		{
			Controls->LeftArmPosition =  180;
			Controls->RightArmPosition = 180;
		}
		delete(MainJoystick);
	}

	//======================================================================================
	// Drive Controllers
	//======================================================================================	 

	if (Controls->DriverMode == ControlLayout::Tank && Controls->DriverType == JoystickType)
	{
			Joystick * Left = new Joystick(0);
			Joystick * Right = new Joystick(1);
			if (fabs(Left->GetRawAxis(1)) > 0.025)
			{
				Controls->SpeedLeft = Left->GetRawAxis(1);
			}
			else
			{
				Controls->SpeedLeft = 0;
			}
			if (fabs(Right->GetRawAxis(1)) > 0.025)
			{
				Controls->SpeedRight = -Right->GetRawAxis(1);
			}
			else
			{
				Controls->SpeedRight = 0;
			}

			Controls->SolenoidPos = ((Left->GetRawButton(1) == true || Right->GetRawButton(1)) == true) ? 1 : -1;
			
			delete (Left);
			delete (Right);
	}
	else if (false) //Drive and lift with a single joystick
	{
		Joystick * MainJoystick = new Joystick(0);
		Controls->SolenoidPos = (MainJoystick->GetRawButton(2) == true) ? -1 : 1;
		Controls->SpeedLeft = 0;
		Controls->SpeedRight = 0;
		Controls->SpeedLeft = ((fabs(MainJoystick->GetRawAxis(2)) > 0.025) ? -MainJoystick->GetRawAxis(2) : 0) + (fabs(MainJoystick->GetRawAxis(1)) > 0.025 ? MainJoystick->GetRawAxis(1) : 0);
		Controls->SpeedRight = ((fabs(MainJoystick->GetRawAxis(2)) > 0.025) ? -MainJoystick->GetRawAxis(2) : 0) - (fabs(MainJoystick->GetRawAxis(1)) > 0.025 ? MainJoystick->GetRawAxis(1) : 0);

		if (MainJoystick->GetPOV() == 0)
		{
			Controls->SpeedLift = 0.9;
		}
		else if (MainJoystick->GetPOV() == 180)
		{
			Controls->SpeedLift = -0.9;
		}
		else
		{
			Controls->SpeedLift = 0;
		}

		/*if (MainJoystick->GetRawButton(7))
		{
			Controls->LeftArmPosition = 345;
			Controls->RightArmPosition = 15;
			Controls->SpeedLift = 0;
		}
		else if (MainJoystick->GetRawButton(1))
		{
			Controls->LeftArmPosition = 120;
			Controls->RightArmPosition = 240;
		}
		else
		{
			Controls->LeftArmPosition = 180;
			Controls->RightArmPosition = 180;
		}*/
		delete (MainJoystick);
	}
	else if (Controls->DriverMode == ControlLayout::Tank && Controls->DriverType == XboxType)
	{
		Joystick * Xbox = new Joystick(0);
		Controls->SpeedLeft = Xbox->GetRawAxis(1);
		Controls->SpeedRight = -Xbox->GetRawAxis(5);
		Controls->SolenoidPos = (Xbox->GetRawButton(6) == true) ? -1 : 1;
		delete (Xbox);
	}
	else if (Controls->DriverMode == ControlLayout::Arcade && Controls->DriverType == XboxType)
	{
		Joystick * Xbox = new Joystick(0);
		Controls->SpeedLeft = -Xbox->GetRawAxis(4) + Xbox->GetRawAxis(1);
		Controls->SpeedRight = -Xbox->GetRawAxis(4) - Xbox->GetRawAxis(1);
		Controls->SolenoidPos = (Xbox->GetRawButton(6) == true) ? -1 : 1;
		delete (Xbox);
	}
	else // Joystick Arcade
	{
		Joystick * MainJoystick = new Joystick(0);
		Controls->SolenoidPos = (MainJoystick->GetRawButton(1) == true) ? -1 : 1;
		Controls->SpeedLeft = 0;
		Controls->SpeedRight = 0;
		Controls->SpeedLeft = ((fabs(MainJoystick->GetRawAxis(2)) > 0.025) ? -MainJoystick->GetRawAxis(2) : 0) + (fabs(MainJoystick->GetRawAxis(1)) > 0.025 ? MainJoystick->GetRawAxis(1) : 0);
		Controls->SpeedRight = ((fabs(MainJoystick->GetRawAxis(2)) > 0.025) ? -MainJoystick->GetRawAxis(2) : 0) - (fabs(MainJoystick->GetRawAxis(1)) > 0.025 ? MainJoystick->GetRawAxis(1) : 0);
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
	switch (Auto->StartingPos) {
	case Right :
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
	case Center :
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
	case Left :
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
	default :

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