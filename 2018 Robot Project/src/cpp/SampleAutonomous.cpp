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

	/*Print field data to dashboard*/
	std::cout << "Center auto starting." << std::endl;
	std::cout << "Game String: " << gameData << std::endl;
	std::cout << "Switch: " << SwitchPosition << " Scale: " << ScalePosition << " Far Switch: " << FarSwitchPosition << std::endl;

	/*Reset values*/
	StartTime = Timer::GetFPGATimestamp();
	Stage = 0;
	CycleNumber = 0;
	SensorReset();
	ArmDegree = 180;
	Brake();
}

void SampleAutonomous::ControllerUpdate(MaxControl * controls)
{
	SwitchPriorityInput = ((RobotControl *)controls)->SwitchPrioritySelection;
	StartingPosition = ((RobotControl *)controls)->StartingPos;
}

void SampleAutonomous::Autonomous()
{
	if (CycleNumber >= 100)
	{
		//std::cout << "Stage number: " << Stage << std::endl;
	}

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
		LeftSpeed = SwitchPosition == FieldPos::Left ? -0.5 : 0.5;
		RightSpeed = SwitchPosition == FieldPos::Left ? 0.5 : -0.5;
		TimeAdvance(0.25);
		break;
	case 5:
		LeftSpeed = 0.5;
		RightSpeed = 0.5;
		LiftSpeed = 0.4;
		TimeAdvance(1.25);
		break;
	case 6:
		LeftSpeed = SwitchPosition == FieldPos::Left ? 0.5 : -0.5;
		RightSpeed = SwitchPosition == FieldPos::Left ? -0.5 : 0.5;
		TimeAdvance(0.25);
		break;
	case 7:
		LeftSpeed = 0.5;
		RightSpeed = 0.5;
		LiftSpeed = 0.4;
		TimeAdvance(0.75);
		break;
	default:
		LiftSpeed = 0;
		ArmDegree = 180;
		Brake();
		break;
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
}

void SampleAutonomous::SensorReset()
{

}

std::string SampleAutonomous::GetName()
{
	return "Sample_Auto";
}
