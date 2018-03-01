#include "AutonomousRight.h"
#include "maxutils\MaxDataStream.h"
#include "ControlTask.h"
#include "iostream"
#include <math.h>

void AutonomousRight::Init()
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

	std::cout << "Position: Right | Switch Priority: " << SwitchPriorityInput << std::endl;
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

void AutonomousRight::ControllerUpdate(MaxControl * controls)
{
	SwitchPriorityInput = ((RobotControl *)controls)->SwitchPrioritySelection;
}

void AutonomousRight::Autonomous()
{
	switch (SwitchPriorityInput)
	{
	case SwitchPriority::Yes:
		if (SwitchPosition == FieldPos::Right)
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
			{
				control.ArmPositionLeft = 100;
				control.ArmPositionRight = 260;
				stage++;
			}
			break;
			case 3:
				StageStartTime = Timer::GetFPGATimestamp();
				stage++;
				break;
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
				double Power1 = Move(110.0, 0.2, &Error1);
				control.SpeedLeft = -Power1;
				control.SpeedRight = Power1;
				double Error2;
				double Power2 = Lift(80000, 0.6, &Error2);
				control.SpeedLift = Power2;
				//std::cout << "Error1: " << Error1 << " Error2: " << Error2 << std::endl;
				if ((fabs(Error2) < 2000) && (fabs(Error1) < 500))
					stage++;
			}
			break;
			case 6:
				control.SpeedLeft = 0;
				control.SpeedRight = 0;
				control.SpeedLift = 0;
				control.ArmPositionLeft = 180;
				control.ArmPositionRight = 180;
				std::cout << "Released" << std::endl;
				stage++;
			default:
				break;
			}
			//std::cout << "Stage: " << stage << std::endl;
			break;
		}
		else //Left switch
		{
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
			default:
				break;
			}
		}
		break;
	case SwitchPriority::No:
		if (ScalePosition == FieldPos::Right)
		{
			//std::cout << "Scale code!" << std::endl;
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
			{
				control.ArmPositionLeft = 100;
				control.ArmPositionRight = 260;
				stage++;
			}
			break;
			case 3:
				StageStartTime = Timer::GetFPGATimestamp();
				stage++;
				break;
			case 4:
				control.SpeedLeft = 0;
				control.SpeedRight = 0;
				ResetSensor();
				if ((Timer::GetFPGATimestamp() - StageStartTime) > 0.5)
					stage++;
				break;
			case 5:
			{
				double Error1;
				double Power1 = Move(265.0, 0.2, &Error1);
				control.SpeedLeft = -Power1;
				control.SpeedRight = Power1;
				double Error2;
				double Power2 = Lift(175000, 0.6, &Error2);
				control.SpeedLift = Power2;
				std::cout << "Error1: " << Error1 << " Error2: " << Error2 << std::endl;
				if ((fabs(Error2) < 2000) && (fabs(Error1) < 2000))
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
			case 8:
				control.SpeedLeft = 0;
				control.SpeedRight = 0;
				control.SpeedLift = 0;
				control.ArmPositionLeft = 180;
				control.ArmPositionRight = 180;
				std::cout << "Released" << std::endl;
				stage++;
				break;
			default:
				break;
			}
		}
		else //Left scale
		{
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
			default:
				break;
			}
		}
		break;
	case SwitchPriority::Kyle:
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
		{
			control.ArmPositionLeft = 100;
			control.ArmPositionRight = 260;
			stage++;
		}
		break;
		case 3:
			StageStartTime = Timer::GetFPGATimestamp();
			stage++;
			break;
		case 4:
			control.SpeedLeft = 0;
			control.SpeedRight = 0;
			ResetSensor();
			if ((Timer::GetFPGATimestamp() - StageStartTime) > 0.5)
				stage++;
			break;
		case 5:
		{
			double Error1;
			double Power1 = Move(100.0, 0.2, &Error1);
			control.SpeedLeft = -Power1;
			control.SpeedRight = Power1;
			if (fabs(Error1) < 2000)
				stage++;
			break;
		}
		default:
			break;
		}
		break;
	}
}

void AutonomousRight::End()
{
	double CurrentTime = Timer::GetFPGATimestamp();
	RunTime = CurrentTime - StartTime;
	LastMessage = floor(RunTime);
	MaxLog::LogInfo("Ending Auto after " + std::to_string(LastMessage) + " seconds");
}

void AutonomousRight::ResetSensor()
{
	AutoMotorLeft->SetSelectedSensorPosition(0, 0, 0);
	AutoMotorLeft->GetSensorCollection().SetQuadraturePosition(0, 0);
	AutoMotorRight->SetSelectedSensorPosition(0, 0, 0);
	AutoMotorRight->GetSensorCollection().SetQuadraturePosition(0, 0);
}

double AutonomousRight::Move(double Inches, double SpeedLimit, double * Error)
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

double AutonomousRight::Lift(double Ticks, double SpeedLimit, double * Error)
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

double AutonomousRight::InchesToTicks(double Inches)
{
	return Inches * 1666.667;
}

std::string AutonomousRight::GetName()
{
	return "AutonomousRight";
}