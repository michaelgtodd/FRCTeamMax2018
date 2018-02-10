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
	if (Controls->SwitchesType == XboxType && Controls->SwitchesMode == Tank)
	{
		Joystick * Xbox = new Joystick(2);

		if (Xbox->GetRawAxis(2) < 0.05 && Xbox->GetRawAxis(3) < 0.05)
		{
			Controls->SpeedLift = 0;
		}
		Controls->SpeedLift = Xbox->GetRawAxis(3) - Xbox->GetRawAxis(2);

		Controls->SpeedGrabWheelLeft = Xbox->GetRawButton(5) - Xbox->GetRawButton(6);
		Controls->SpeedGrabWheelRight = -Controls->SpeedGrabWheelRight;

		if (fabs(Xbox->GetRawAxis(0)) > 0.05)
		{
			Controls->SpeedArmLeft = Xbox->GetRawAxis(0);
		}
		else
		{
			Controls->SpeedArmLeft = 0;
		}
		if (fabs(Xbox->GetRawAxis(4)) > 0.05)
		{
			Controls->SpeedArmRight = Xbox->GetRawAxis(4);
		}
		else
		{
			Controls->SpeedArmRight = 0;
		}
		delete(Xbox);
	}
	else if (Controls->SwitchesType == XboxType && Controls->SwitchesMode == Arcade)
	{
		Joystick * Xbox = new Joystick(2);
		
		if (Xbox->GetRawAxis(2) < 0.05 && Xbox->GetRawAxis(3) < 0.05)
		{
			Controls->SpeedLift = 0;
		}
		else
		{
			Controls->SpeedLift = Xbox->GetRawAxis(3) - Xbox->GetRawAxis(2);
		}

		delete (Xbox);
	}
	else if (Controls->SwitchesType == JoystickType && Controls->SwitchesMode == Arcade)
	{
		Joystick * MainJoystick = new Joystick(2);

		if (fabs(MainJoystick->GetRawAxis(1)) < 0.05)
		{
			Controls->SpeedLift = 0;
		}
		else
		{
			Controls->SpeedLift = MainJoystick->GetRawAxis(1);
		}

		delete(MainJoystick);
	}
	else //Joystick tank
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
		if (fabs(SwitchesLeft->GetRawAxis(2)) >= 0.50)
		{
			Controls->SpeedArmLeft = -SwitchesLeft->GetRawAxis(2);
		}
		if (fabs(SwitchesRight->GetRawAxis(2)) >= 0.50)
		{
			Controls->SpeedArmRight = -SwitchesRight->GetRawAxis(2);
		}
		
		if ((SwitchesRight->GetRawButton(1)) || (SwitchesLeft->GetRawButton(1)))
		{
			//std::cout << "wheel running" << std::endl;
			Controls->SpeedGrabWheelLeft = 1;
			Controls->SpeedGrabWheelRight = -1;
		}
		else if ((SwitchesRight->GetRawButton(2)) || (SwitchesLeft->GetRawButton(2)))
		{
			Controls->SpeedGrabWheelLeft = -1;
			Controls->SpeedGrabWheelRight = 1;
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
			Output = 1;
	}
	else if (Controls->DriverMode == ControlLayout::Tank && Controls->DriverType == XboxType)
	{
		Joystick * Xbox = new Joystick(0);
		Controls->SpeedLeft = Xbox->GetRawAxis(1);
		Controls->SpeedRight = -Xbox->GetRawAxis(5);
		Controls->SolenoidPos = (Xbox->GetRawButton(6) == true) ? -1 : 1;
		delete (Xbox);
		Output = 2;
	}
	else if (Controls->DriverMode == ControlLayout::Arcade && Controls->DriverType == XboxType)
	{
		Joystick * Xbox = new Joystick(0);
		Controls->SpeedLeft = -Xbox->GetRawAxis(4) + Xbox->GetRawAxis(1);
		Controls->SpeedRight = -Xbox->GetRawAxis(4) - Xbox->GetRawAxis(1);
		Controls->SolenoidPos = (Xbox->GetRawButton(6) == true) ? -1 : 1;
		delete (Xbox);
		Output = 3;
	}
	else
	{
		Joystick * MainJoystick = new Joystick(0);
		Controls->SolenoidPos = (MainJoystick->GetRawButton(1) == true) ? -1 : 1;
		Controls->SpeedLeft = 0;
		Controls->SpeedRight = 0;
		Controls->SpeedLeft = (fabs(MainJoystick->GetRawAxis(2)) > 0.025) ? 0 : MainJoystick()
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
	else if (Controls->DriverType == XboxType)
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
	//cout << messages.AddressPattern() << endl;

	if (strcmp(messages.AddressPattern(), "/Dashboard/DriverController/") == 0)
	{
		osc::ReceivedMessageArgumentStream args = messages.ArgumentStream();
		const char * CharControllerType;
		args >> CharControllerType >> osc::EndMessage;

		cout << "Driver Controller: " << CharControllerType << endl;

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

		cout << "Driver Mode: " << CharDriverMode << endl;

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

		//cout << "Switch Controller: " << CharControllerType << endl;


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

		cout << "Switch Mode: " << CharSwitchesMode << endl;


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