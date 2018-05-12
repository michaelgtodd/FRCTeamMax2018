#include "SampleAutonomous.h"
#include "maxutils\MaxDataStream.h"
#include "ControlTask.h"
#include "iostream"
#include <math.h>

void SampleAutonomous::Init()
{
	MaxLog::LogInfo("Starting Auto");

	/*Get field data*/
	std::string gameData;
	gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
	if (gameData.length() >= 3)
	{
		SwitchPosition = gameData[0] == 'L' ? FieldPos::Left : FieldPos::Right;
		ScalePosition = gameData[1] == 'L' ? FieldPos::Left : FieldPos::Right;
		FarSwitchPosition = gameData[2] == 'L' ? FieldPos::Left : FieldPos::Right;
	}

	/*Determine starting position*/
	Joystick * PosInput1 = new Joystick(0);
	Joystick * PosInput2 = new Joystick(2);
	if (PosInput2->GetRawAxis(3) <= 0.5)
	{
		StartingPosition = FieldPos::Center;
	}
	else
	{
		StartingPosition = PosInput1->GetRawAxis(3) <= 0.5 ? FieldPos::Left : FieldPos::Right;
	}
	delete PosInput1, PosInput2;

	/*Print field data to dashboard*/
	switch (StartingPosition)
	{
	case FieldPos::Left:
		std::cout << "Left auto starting." << std::endl;
		break;
	case FieldPos::Center:
		std::cout << "Center auto starting." << std::endl;
		break;
	case FieldPos::Right:
		std::cout << "Right auto starting." << std::endl;
		break;
	default:
		break;
	}

	std::cout << "Game String: " << gameData << std::endl;
	std::cout << "Switch: " << SwitchPosition << " Scale: " << ScalePosition << " Far Switch: " << FarSwitchPosition << std::endl;

	/*Reset values*/
	StartTime = Timer::GetFPGATimestamp();
	Stage = 0;
	CycleNumber = 0;
	SensorReset();
	WheelSpeed = 0;
	ArmDegree = 180;
	Brake();
}

void SampleAutonomous::ControllerUpdate(MaxControl * controls)
{
	SwitchPriorityInput = ((RobotControl *)controls)->SwitchPrioritySelection;
}

void SampleAutonomous::Autonomous()
{
	if (CycleNumber >= 100)
	{
		//std::cout << "Stage number: " << Stage << std::endl;
	}

	if (StartingPosition == FieldPos::Center)
	{
		switch (Stage)
		{
		case 0:
			LiftSpeed = 0.3;
			ArmDegree = 180;
			TimeAdvance(1);
			Brake();
			break;
		case 1:
			LiftSpeed = -0.3;
			TimeAdvance(1);
			break;
		case 2:
			ArmDegree = 40;
			TimeAdvance(1);
			break;
		case 3:
			LeftSpeed = 0.5;
			RightSpeed = 0.5;
			TimeAdvance(0.25);
			break;
		case 4:
			LeftSpeed = SwitchPosition == FieldPos::Left ? -0.7 : 0.6;
			RightSpeed = SwitchPosition == FieldPos::Left ? 0.7 : -0.6;
			TimeAdvance(0.25);
			break;
		case 5:
			LeftSpeed = 0.5;
			RightSpeed = 0.5;
			LiftSpeed = 0.6;
			TimeAdvance(SwitchPosition == FieldPos::Left ? 1.34 : 0.9);
			break;
		case 6:
			LeftSpeed = SwitchPosition == FieldPos::Left ? 0.5 : -0.5;
			RightSpeed = SwitchPosition == FieldPos::Left ? -0.5 : 0.5;
			TimeAdvance(SwitchPosition == FieldPos::Left ? 0.2 : 0.4);
			break;
		case 7:
			LeftSpeed = 0.2;
			RightSpeed = 0.2;
			LiftSpeed = 0.6;
			TimeAdvance(1.75);
			break;
		case 8:
			WheelSpeed = -1;
			Brake();
			TimeAdvance(0.75);
			break;
		case 9:
			LiftSpeed = 0;
			WheelSpeed = 0;
			ArmDegree = 110;
			LeftSpeed = -0.5;
			RightSpeed = -0.5;
			TimeAdvance(0.5);
			break;
		case 10:
			LeftSpeed = SwitchPosition == FieldPos::Left ? 0.5 : -0.45;
			RightSpeed = SwitchPosition == FieldPos::Left ? -0.5 : 0.45;
			LiftSpeed = SwitchPosition == FieldPos::Left ? -0.4 : -0.6;
			TimeAdvance(0.4);
			break;
		case 11:
			LeftSpeed = 0.3;
			RightSpeed = 0.3;
			WheelSpeed = 1;
			TimeAdvance(SwitchPosition == FieldPos::Left ? 1.25 : 1.5);
			break;
		case 12:
			ArmDegree = 40;
			LiftSpeed = 0;
			Brake();
			TimeAdvance(SwitchPosition == FieldPos::Left ? 0.5 : 0.75);
			break;
		case 13:
			LeftSpeed = -0.6;
			RightSpeed = -0.6;
			WheelSpeed = 0;
			LiftSpeed = SwitchPosition == FieldPos::Left ? 0.7 : 0.95;
			TimeAdvance(0.5);
			break;
		case 14:
			LeftSpeed = SwitchPosition == FieldPos::Left ? -0.5 : 0.5;
			RightSpeed = SwitchPosition == FieldPos::Left ? 0.5 : -0.5;
			TimeAdvance(SwitchPosition == FieldPos::Left ? 0.35 : 0.25);
			break;
		case 15:
			LeftSpeed = 0.3;
			RightSpeed = 0.3;
			TimeAdvance(1.5);
			break;
		case 16:
			WheelSpeed = -1;
			Brake();
			TimeAdvance(0.5);
			break;
		case 17:
			ArmDegree = 180;
			LiftSpeed = 0;
			LeftSpeed = -0.5;
			RightSpeed = -0.5;
			TimeAdvance(0.5);
			break;
		default:
			Brake();
			break;
		}
	}
	else //If switch is on same side
	{
		switch (Stage)
		{
		case 0:
			LiftSpeed = 0.3;
			ArmDegree = 180;
			TimeAdvance(1);
			Brake();
			break;
		case 1:
			LiftSpeed = -0.3;
			TimeAdvance(1);
			break;
		case 2:
			ArmDegree = 40;
			LiftSpeed = 0;
			TimeAdvance(1);
			break;
		case 3:
			LeftSpeed = 0.5;
			RightSpeed = 0.5;
			LiftSpeed = StartingPosition == SwitchPosition ? 0.7 : 0.0;
			TimeAdvance(1.75);
			break;
		case 4:
			Stage = 69;
			//Stage += StartingPosition == SwitchPosition ? 1 : 420;
			break;
		case 5:
			LeftSpeed = StartingPosition == FieldPos::Left ? 0.5 : -0.5;
			RightSpeed = StartingPosition == FieldPos::Left ? -0.5 : 0.5;
			TimeAdvance(0.5);
			break;
		case 6:
			LeftSpeed = 0.5;
			RightSpeed = 0.5;
			TimeAdvance(1);
			break;
		case 7:
			LiftSpeed = 0;
			WheelSpeed = -1;
			Brake();
			TimeAdvance(0.5);
			break;
		case 8:
			WheelSpeed = 0;
			ArmDegree = 180;
			LeftSpeed = -0.5;
			RightSpeed = -0.5;
			TimeAdvance(0.5);
			break;
		case 9:
			Stage = 69;
			LeftSpeed = StartingPosition == FieldPos::Left ? -0.5 : 0.5;
			RightSpeed = StartingPosition == FieldPos::Left ? 0.5 : -0.5;
			//LiftSpeed = -0.3;
			TimeAdvance(0.5);
			break;
		case 10:
			ArmDegree = 110;
			LeftSpeed = 0.5;
			RightSpeed = 0.5;
			TimeAdvance(0.5);
			break;
		case 11:
			LiftSpeed = 0;
			LeftSpeed = StartingPosition == FieldPos::Left ? 0.5 : -0.5;
			RightSpeed = StartingPosition == FieldPos::Left ? -0.5 : 0.5;
			TimeAdvance(0.5);
			break;
		case 12:
			LeftSpeed = 0.3;
			RightSpeed = 0.3;
			TimeAdvance(0.75);
			break;
		case 13:
			ArmDegree = 220;
			LeftSpeed = StartingPosition == FieldPos::Left ? 0.5 : -0.5;
			RightSpeed = StartingPosition == FieldPos::Left ? -0.5 : 0.5;
			TimeAdvance(0.5);
			break;
		case 14:
			WheelSpeed = 1;
			LeftSpeed = 0.3;
			RightSpeed = 0.3;
			TimeAdvance(1);
			break;
		case 15:
			ArmDegree = 110;
			Brake();
			TimeAdvance(0.5);
			break;
		case 16:
			ArmDegree = 40;
			TimeAdvance(0.5);
			break;
		case 17:
			WheelSpeed = 0;
			LiftSpeed = 0.9;
			LeftSpeed = -0.3;
			RightSpeed = -0.3;
			TimeAdvance(0.8);
			break;
		case 18:
			LeftSpeed = 0.5;
			RightSpeed = 0.5;
			TimeAdvance(.75);
			break;
		case 19:
			WheelSpeed = -1;
			TimeAdvance(0.5);
			break;
		case 20:
			ArmDegree = 180;
			LeftSpeed = -0.5;
			RightSpeed = -0.5;
			LiftSpeed = 0;
			WheelSpeed = 0;
			TimeAdvance(0.5);
			break;
		default:
			Brake();
			break;
		}
	}

	/*Update data*/
	UpdateMotors();
	UpdateVariables();

	/* Send data to the control task to send to other tasks*/
	ControlTaskInstance.UpdateAutonomousData(control);
}

void SampleAutonomous::End()
{
	UpdateVariables();
	MaxLog::LogInfo("Ending Auto after " + std::to_string(RunTime) + " seconds");
}

void SampleAutonomous::Brake()
{
	LeftSpeed = 0;
	RightSpeed = 0;
	control.SpeedLeft = 0;
	control.SpeedRight = 0;
}

void SampleAutonomous::TimeAdvance(double Time)
{
	if (RunTime - Stopwatch >= Time)
	{
		std::cout << "Stage " << Stage << " complete." << std::endl;
		Stopwatch = RunTime;
		SensorReset();
		Stage++;
	}
}

void SampleAutonomous::UpdateVariables()
{
	/*Update timer*/
	double CurrentTime = Timer::GetFPGATimestamp();
	RunTime = CurrentTime - StartTime;
	CycleNumber++;
}

void SampleAutonomous::UpdateMotors()
{
	control.SpeedLeft = LeftSpeed;
	control.SpeedRight = -RightSpeed;
	control.SpeedLift = LiftSpeed;
	control.ArmPositionLeft = ArmDegree;
	control.ArmPositionRight = 360 - ArmDegree;
	control.SpeedWheel = WheelSpeed;
}

void SampleAutonomous::SensorReset()
{

}

std::string SampleAutonomous::GetName()
{
	return "Sample_Auto";
}
