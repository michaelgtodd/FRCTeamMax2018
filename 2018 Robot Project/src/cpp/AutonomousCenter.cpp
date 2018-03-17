#define _USE_MATH_DEFINES
#include "AutonomousCenter.h"
#include "maxutils\MaxDataStream.h"
#include "ControlTask.h"
#include "iostream"
#include <math.h>

void AutonomousCenter::Init()
{
	MaxLog::LogInfo("Starting Center Auto");

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
		//Move Foward
		switch (stage)
		{
		case 0:
			//Reset
			StageStartTime = Timer::GetFPGATimestamp();
			ResetSensor();
			stage++;
			std::cout << "Stage 0" << std::endl;
			break;
		case 1:
			//Clamp
			control.ArmPositionRight = 280;
			control.ArmPositionLeft = 80;
			Brake();
			if ((Timer::GetFPGATimestamp() - StageStartTime) > 0.1)
				stage++;
			std::cout << "Stage 1" << std::endl;
			break;
		case 2:
			//Drive Forward
			if (Drive(70.0, 0.5, 2.0))
			{
				Brake();
				ResetSensor();
				StageStartTime = Timer::GetFPGATimestamp();
				std::cout << "Reached 50 inches. Preparing to turn." << std::endl;
				stage++;
			}
			std::cout << "Stage 2" << std::endl;
			break;
		case 3:
			//Wait
			if ((Timer::GetFPGATimestamp() - StageStartTime) > 0.1)
				stage++;
			std::cout << "Stage 3" << std::endl;
			break;
		default:
			std::cout << "default stage" << std::endl;
			break;
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

bool AutonomousCenter::Move(double DistanceInInches, double SpeedLimit, double ToleranceInInches)
{
	//Convert Distance and Tolerance to Ticks
	double DistanceInTicks = InchesToTicks(DistanceInInches);
	double ToleranceInTicks = InchesToTicks(ToleranceInInches);

	//Set the initial gains
	double RightGain = 0.000025, LeftGain = 0.000025;
	double InitialSpeed = 0.8 * SpeedLimit;
	
	//Set the target distance and find remaining distance
	RightTarget = DistanceInTicks, LeftTarget = DistanceInTicks;
	RightTravel = AutoMotorRight->GetSensorCollection().GetQuadraturePosition();
	LeftTravel = AutoMotorLeft->GetSensorCollection().GetQuadraturePosition();


	return false;
}

bool AutonomousCenter::Drive(double DistanceInInches, double SpeedLimit, double ToleranceInInches)
{
	// Convert Distance and Tolerance to Ticks
	double DistanceInTicks = InchesToTicks(DistanceInInches);
	double ToleranceInTicks = InchesToTicks(ToleranceInInches);

	// Set the constant gains.
	const double RightGain = 0.0001, LeftGain = 0.0001;

	// Set the Targets and Errors (Error is Target Minus Present Position)
	RightTarget = DistanceInTicks, LeftTarget = DistanceInTicks;
	RightError = RightTarget - fabs(AutoMotorRight->GetSensorCollection().GetQuadraturePosition());
	LeftError = LeftTarget - fabs(AutoMotorLeft->GetSensorCollection().GetQuadraturePosition());

	// Set the powers to 0 if we're within tolerance. If not, set to Error * Gain.
	double RightPower = fabs(RightError) < ToleranceInTicks ? 0 : RightError * RightGain;
	double LeftPower = fabs(LeftError) < ToleranceInTicks ? 0 : LeftError * LeftGain;

	// Apply a cap power for the max and minimum speed limits.
	RightPower = fmin(RightPower, -SpeedLimit);
	RightPower = fmax(RightPower, SpeedLimit);
	LeftPower = fmin(LeftPower, -SpeedLimit);
	LeftPower = fmax(LeftPower, SpeedLimit);

	// Set the active power.
	control.SpeedRight = RightPower;
	control.SpeedLeft = -LeftPower;

	// Testing prints.
	//std::cout << "Target: " << LeftTarget << " | " << RightTarget << std::endl;
	std::cout << "Target - Distance Traveled: " << LeftError << " | " << RightError << std::endl;
	std::cout << "Power: " << LeftPower << " | " << RightPower << std::endl;
	//std::cout << "Tolerance in Ticks: " << ToleranceInTicks << std::endl;

	// Skip to the next stage if we're within our tolerance.
	if (fabs(AutoMotorRight->GetSensorCollection().GetQuadraturePosition()) > RightTarget && 
		fabs(AutoMotorLeft->GetSensorCollection().GetQuadraturePosition()) > LeftTarget)
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