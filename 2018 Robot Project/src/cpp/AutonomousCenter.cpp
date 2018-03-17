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

	std::cout << "Position: Left | Switch Priority: " << SwitchPriorityInput << std::endl;
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
	ResetSensor();
}

void AutonomousCenter::ControllerUpdate(MaxControl * controls)
{
	SwitchPriorityInput = ((RobotControl *)controls)->SwitchPrioritySelection;
}

void AutonomousCenter::Autonomous()
{
	switch (SwitchPriorityInput)
	{
	case SwitchPriority::Yes:

		if (SwitchPosition == FieldPos::Left)
		{
			//std::cout << "Switch code! Switch Priority: " << SwitchPriorityInput << std::endl;
			switch (stage)
			{
			case 0:
				StageStartTime = Timer::GetFPGATimestamp();
				stage++;
				break;
			case 1:
				control.SpeedLeft = 0;
				control.SpeedRight = 0;
				control.SpeedLift = 0;
				control.ArmPositionLeft = 180;
				control.ArmPositionRight = 180;
				ResetSensor();
				if ((Timer::GetFPGATimestamp() - StageStartTime) > 2.0)
					stage++;
				break;
			case 2:
				control.ArmPositionLeft = 100;
				control.ArmPositionRight = 260;
				stage++;
				break;
			case 3:
			{
				StageStartTime = Timer::GetFPGATimestamp();
				stage++;
				break;
			}
			case 4:
				control.SpeedLeft = 0;
				control.SpeedRight = 0;
				ResetSensor();
				if ((Timer::GetFPGATimestamp() - StageStartTime) > 2.0)
					stage++;
				break;
			case 5:
			{
				double Error1;
				double Power1 = Move(30.0, 0.2, &Error1);
				control.SpeedLeft = -Power1;
				control.SpeedRight = Power1;
				//std::cout << "Error1: " << Error1 << " Error2: " << Error2 << std::endl;
				if (fabs(Error1) < 500)
					stage++;
			}
			break;
			case 6:
				StageStartTime = Timer::GetFPGATimestamp();
				stage++;
				break;
			case 7:
				control.SpeedLeft = 0;
				control.SpeedRight = 0;
				control.SpeedLift = 0;
				ResetSensor();
				if ((Timer::GetFPGATimestamp() - StageStartTime) > 2.0)
					stage++;
				break;
			}
		}
		else //Right switch
		{
			
		}
		break;
	case SwitchPriority::No:
		if (ScalePosition == FieldPos::Left)
		{

		}
		else //Right scale
		{

		}
		break;
	default:
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

void AutonomousCenter::ResetSensor()
{
	AutoMotorLeft->SetSelectedSensorPosition(0, 0, 0);
	AutoMotorLeft->GetSensorCollection().SetQuadraturePosition(0, 0);
	AutoMotorRight->SetSelectedSensorPosition(0, 0, 0);
	AutoMotorRight->GetSensorCollection().SetQuadraturePosition(0, 0);
}

double AutonomousCenter::Move(double Inches, double SpeedLimit, double * Error)
{
	double TargetDistance = InchesToTicks(Inches);
	double Gain = 0.000025;
	*Error = TargetDistance - AutoMotorRight->GetSensorCollection().GetQuadraturePosition();
	double Power = *Error * Gain;
	Power = fmin(Power, SpeedLimit);
	Power = fmax(Power, -SpeedLimit);
	return Power;
	//std::cout << "Taraget: " << TargetDistance << " Actual: " << AutoMotorRight->GetSensorCollection().GetQuadraturePosition();
	//std::cout << " Error: " << Error << " Power: " << Power << std::endl;
}

double AutonomousCenter::Turn(double Degrees, double SpeedLimit, double * Error)
{
	double Distance = 80000.016 * 3.141592 * (Degrees / 360);
	return Distance;
}

double AutonomousCenter::Lift(double Ticks, double SpeedLimit, double * Error)
{
	double TargetDistance = Ticks;
	double Gain = 0.00005;
	*Error = TargetDistance - AutoMotorLift->GetSensorCollection().GetQuadraturePosition();
	double Power = *Error * Gain;
	Power = fmin(Power, SpeedLimit);
	Power = fmax(Power, -SpeedLimit);
	//std::cout << "Power: " << Power << " Target Distance: " << TargetDistance << " Error: " << *Error << " Actual: " << AutoMotorLift->GetSensorCollection().GetQuadraturePosition() << std::endl;
	return Power;
}

double AutonomousCenter::InchesToTicks(double Inches)
{
	return Inches * 1666.667;
}

std::string AutonomousCenter::GetName()
{
	return "AutonomousLeft";
}