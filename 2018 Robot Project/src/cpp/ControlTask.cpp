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
	//std::cout << "Controller Type: " << Controls->ControllerType << " Controller Mode: " << Controls->ControllerMode << std::endl;
	Output = 0;

	//Lift and grab
	if (Controls->SwitchesType == XboxType)
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
		else
		{
			Controls->SpeedLift = 0;
		}
		if (fabs(SwitchesRight->GetRawAxis(1)) >= 0.25)
		{
			Controls->SpeedLift = -SwitchesRight->GetRawAxis(1);
		}
		else
		{
			Controls->SpeedLift = 0;
		}

		
		Controls->SpeedArmLeft = 0;
		Controls->SpeedArmRight = 0;
		if (fabs(SwitchesLeft->GetRawAxis(2)) >= 0.25)
		{
			Controls->SpeedArmLeft = -SwitchesLeft->GetRawAxis(2);
		}
		if (fabs(SwitchesRight->GetRawAxis(2)) >= 0.25)
		{
			Controls->SpeedArmRight = -SwitchesRight->GetRawAxis(2);
		}
		//Controls->SpeedGrabWheelLeft = SwitchesLeft->GetRawButton(6) - SwitchesLeft->GetRawButton(4);
		//Controls->SpeedGrabWheelRight = SwitchesRight->GetRawButton(5) - SwitchesRight->GetRawButton(3);
		
		if ((SwitchesRight->GetRawButton(1)) || (SwitchesLeft->GetRawButton(1)))
		{
			std::cout << "wheel running" << std::endl;
			Controls->SpeedGrabWheelLeft = 1;
			Controls->SpeedGrabWheelRight = 1;
		}
		else if ((SwitchesRight->GetRawButton(2)) || (SwitchesLeft->GetRawButton(2)))
		{
			Controls->SpeedGrabWheelLeft = -1;
			Controls->SpeedGrabWheelRight = -1;
		}
		else
		{
			Controls->SpeedGrabWheelLeft = 0;
			Controls->SpeedGrabWheelRight = 0;
		}
	
		delete (SwitchesLeft);
		delete (SwitchesRight);
	}

	
	 //Drive Motors
	if (Controls->ControllerMode == ControlLayout::Tank && Controls->ControllerType == JoystickType)
	{
			Joystick * Left = new Joystick(0);
			Joystick * Right = new Joystick(1);
			Controls->SpeedLeft = Left->GetRawAxis(1);
			Controls->SpeedRight = -Right->GetRawAxis(1);
			Controls->SolenoidPos = ((Left->GetRawButton(1) == true || Right->GetRawButton(1)) == true) ? -1 : 1;
			delete (Left);
			delete (Right);
			Output = 1;
	}
	else if (Controls->ControllerMode == ControlLayout::Tank && Controls->ControllerType == XboxType)
	{
		Joystick * Xbox = new Joystick(0);
		Controls->SpeedLeft = Xbox->GetRawAxis(1);
		Controls->SpeedRight = -Xbox->GetRawAxis(5);
		Controls->SolenoidPos = (Xbox->GetRawButton(4) == true) ? -1 : 1;
		delete (Xbox);
		Output = 2;
	}
	else if (Controls->ControllerMode == ControlLayout::Arcade && Controls->ControllerType == XboxType)
	{
		Joystick * Xbox = new Joystick(0);
		Controls->SpeedLeft = Xbox->GetRawAxis(4) + Xbox->GetRawAxis(1);
		Controls->SpeedRight = Xbox->GetRawAxis(4) - Xbox->GetRawAxis(1);
		Controls->SolenoidPos = (Xbox->GetRawButton(4) == true) ? -1 : 1;
		delete (Xbox);
		Output = 3;
	}
	else
	{
		Joystick * MainJoystick = new Joystick(0);
		Controls->SolenoidPos = (MainJoystick->GetRawButton(1) == true) ? -1 : 1;
		Controls->SpeedLeft = -MainJoystick->GetRawAxis(2) + MainJoystick->GetRawAxis(1);
		Controls->SpeedRight = -MainJoystick->GetRawAxis(2) - MainJoystick->GetRawAxis(1);
		delete (MainJoystick);
		Output = 4;
	}
	//std::cout << "Output: " << Output << " Controller Mode:" << Controls->ControllerMode << " Controller Type: " << Controls->ControllerType << std::endl;

	//Lifting_Task
	if (Controls->SwitchesType == JoystickType) 
	{
		Joystick * Left = new Joystick(2);
		Joystick * Right = new Joystick(3);
		Controls->SpeedLift = (fabs(Left->GetRawAxis(1) + Right->GetRawAxis(1)) < .25) ? 0 : (Left->GetRawAxis(1) + Right->GetRawAxis(1));
		delete (Left);
		delete (Right);
	}
	else if (Controls->ControllerType == XboxType)
	{
		Joystick * Xbox = new Joystick(2);
		Controls->SpeedLift = (fabs(Xbox->GetRawAxis(1) + Xbox->GetRawAxis(5)) < .25) ? 0 : (Xbox->GetRawAxis(1) + Xbox->GetRawAxis(5));
		delete (Xbox);
	}
	else 
	{

	}
	//else if (Controls->ControllerMode == ControlLayout::Arcade && Controls->ControllerType == XboxType) 
	//{
	//	Joystick * Xbox = new Joystick(2);
	//	Controls->SpeedLift = (fabs(Xbox->GetRawAxis(1)) < .1) ? 0 : (Xbox->GetRawAxis(1));
	//	delete (Xbox);
	//}
	//else 
	//{
	//	Joystick * MainJoystick = new Joystick(2);
	//	Controls->SpeedLift = (fabs(MainJoystick->GetRawAxis(1)) < .1) ? 0 : (MainJoystick->GetRawAxis(1));
	//	delete (MainJoystick);
	//}
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
	if (strcmp(messages.AddressPattern(), "/Dashboard/ControllerMode/") == 0)
	{
		osc::ReceivedMessageArgumentStream args = messages.ArgumentStream();
		const char * CharControllerType;
		args >> CharControllerType >> osc::EndMessage;

		if (strcmp(CharControllerType, "Joystick") == 0)
		{
			Controls->ControllerType = ControlType::JoystickType;
		}
		else if (strcmp(CharControllerType, "Xbox Controller") == 0)
		{
			Controls->ControllerType = ControlType::XboxType;
		}
		else
		{
			Controls->ControllerType = ControlType::JoystickType;
		}
	}
	if (strcmp(messages.AddressPattern(), "/Dashboard/SwitchesMode/") == 0)
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
	if (strcmp(messages.AddressPattern(), "/Dashboard/DriveModeMessage/") == 0)
	{
		osc::ReceivedMessageArgumentStream args = messages.ArgumentStream();
		const char * CharDriveModeMessage;
		args >> CharDriveModeMessage >> osc::EndMessage;
		if (strcmp(CharDriveModeMessage, "Arcade") == 0)
		{
			Controls->ControllerMode = ControlLayout::Arcade;
		}
		else if (strcmp(CharDriveModeMessage, "Tank") == 0)
		{
			Controls->ControllerMode = ControlLayout::Tank;
		}
		else
		{
			Controls->ControllerMode = ControlLayout::Arcade;
		}
	}
}

void ControlTask::Init()
{
	Controls = new RobotControl();

	debugCounter = 0;
}