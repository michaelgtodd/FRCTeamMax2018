#define _USE_MATH_DEFINES
#include "AutonomousCenter.h"
#include "maxutils\MaxDataStream.h"
#include "ControlTask.h"
#include "iostream"
#include <math.h>

void AutonomousCenter::Init()
{
	MaxLog::LogInfo("Starting Auto");

	//Get field data
	std::string gameData;
	gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
	if (gameData.length() >= 3)
	{
		SwitchPosition = gameData[0] == 'L' ? FieldPos::Left : FieldPos::Right;
		ScalePosition = gameData[1] == 'L' ? FieldPos::Left : FieldPos::Right;
		FarSwitchPosition = gameData[2] == 'L' ? FieldPos::Left : FieldPos::Right;
	}

	std::cout << "Position: Center | Switch Priority: " << SwitchPriorityInput << std::endl;
	std::cout << "Game String: " << gameData << std::endl;
	std::cout << "Switch: " << SwitchPosition << " Scale: " << ScalePosition << " Far Switch: " << FarSwitchPosition << std::endl;
	StartTime = Timer::GetFPGATimestamp();
	LastMessage = 0;
	stage = 0;

	AutoMotorLeft = new TalonSRX(0);
	AutoMotorRight = new TalonSRX(15);
	AutoMotorLift = new TalonSRX(12);
	AutoPosArmLeft = new TalonSRX(5);
	AutoPosArmRight = new TalonSRX(10);
	DummyTalon = new TalonSRX(1);
	PigeonInput = new PigeonIMU(DummyTalon);
	ResetSensor();
}

void AutonomousCenter::ControllerUpdate(MaxControl * controls)
{
	SwitchPriorityInput = ((RobotControl *)controls)->SwitchPrioritySelection;
}

void AutonomousCenter::Autonomous()
{
	if (SwitchPriorityInput != SwitchPriority::Kyle)
	{
		//Move Foward
		stage = 0;
		switch (stage)
		{
		case 0:
			//Reset
			StageStartTime = Timer::GetFPGATimestamp();
			ResetSensor();
			stage++;
			break;
		case 1:
			//Clamp
			control.ArmPositionRight = 280;
			control.ArmPositionLeft = 80;
			Brake();
			if ((Timer::GetFPGATimestamp() - StageStartTime) > 0.1)
				stage++;
			break;
		case 2:
			//Drive Forward
			if (Drive(50.0, 0.3, 2000.0))
			{
				Brake();
				ResetSensor();
				StageStartTime = Timer::GetFPGATimestamp();
				stage++;
			}
			break;
		case 3:
			//Wait
			if ((Timer::GetFPGATimestamp() - StageStartTime) > 0.1)
				stage++;
			break;
		default:
			break;
		}
		if (SwitchPosition == FieldPos::Left)
		{

		}
		else if (SwitchPosition == FieldPos::Right)
		{

		}
	}
	ControlTaskInstance.UpdateAutonomousData(control);
}

void AutonomousCenter::End()
{
	double CurrentTime = Timer::GetFPGATimestamp();
	RunTime = CurrentTime - StartTime;
	LastMessage = floor(RunTime);
	MaxLog::LogInfo("Ending Auto after " + std::to_string(LastMessage) + " seconds");
}

bool AutonomousCenter::Drive(double Inches, double SpeedLimit, double Tolerance)
{
	double Ticks = InchesToTicks(Inches);
	double RightGain = 0.000025, LeftGain = 0.000025;
	RightTarget = Ticks, LeftTarget = Ticks;
	RightError = RightTarget - AutoMotorRight->GetSensorCollection().GetQuadraturePosition();
	LeftError = LeftTarget - AutoMotorLeft->GetSensorCollection().GetQuadraturePosition();
	double RightPower = fabs(RightError) > Tolerance ? 0 : RightError * RightGain;
	double LeftPower = fabs(LeftError) > Tolerance ? 0 : LeftError * LeftGain;
	RightPower = fmin(RightPower, -SpeedLimit);
	RightPower = fmax(RightPower, SpeedLimit);
	LeftPower = fmin(LeftPower, -SpeedLimit);
	LeftPower = fmax(LeftPower, SpeedLimit);
	control.SpeedRight = RightPower;
	control.SpeedLeft = -LeftPower;
	if (fabs(RightError) > Tolerance && fabs(LeftError) > Tolerance)
		return true;
	else
		return false;
}

bool AutonomousCenter::Turn(double Degrees, double SpeedLimit, double Tolerance)
{

}

bool AutonomousCenter::Lift(double Height, double SpeedLimit, double Tolerance)
{
	double Gain = 0.00005;
	LiftTarget = Height;
	LiftError = LiftTarget - AutoMotorLift->GetSensorCollection().GetQuadraturePosition();
	double Power = fabs(LiftError) > Tolerance ? 0 : LiftError * Gain;
	Power = fmin(Power, -SpeedLimit);
	Power = fmax(Power, SpeedLimit);
	control.SpeedLift = Power;
	if (fabs(LiftError) > Tolerance)
		return true;
	else
		return false;
}

void AutonomousCenter::Brake()
{
	control.SpeedRight = 0;
	control.SpeedLeft = 0;
	RightTarget = 0;
	LeftTarget = 0;
	RightError = 0;
	LeftError = 0;
}

void AutonomousCenter::ResetSensor()
{
	AutoMotorLeft->SetSelectedSensorPosition(0, 0, 0);
	AutoMotorLeft->GetSensorCollection().SetQuadraturePosition(0, 0);
	AutoMotorRight->SetSelectedSensorPosition(0, 0, 0);
	AutoMotorRight->GetSensorCollection().SetQuadraturePosition(0, 0);
}

double AutonomousCenter::InchesToTicks(double Inches)
{
	return Inches * 1666.667;
}

std::string AutonomousCenter::GetName()
{
	return "AutonomousCenter";
}