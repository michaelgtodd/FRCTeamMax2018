#define _USE_MATH_DEFINES
#include "AutonomousLeft.h"
#include "maxutils\MaxDataStream.h"
#include "ControlTask.h"
#include "iostream"
#include <math.h>

void AutonomousLeft::Init()
{
	MaxLog::LogInfo("Starting Auto");

	std::cout << "Position: Left " << "Auto Init" << std::endl;

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

void AutonomousLeft::ControllerUpdate(MaxControl * controls)
{
	SwitchPriorityInput = ((RobotControl * ) controls)->SwitchPrioritySelection;
}

void AutonomousLeft::Autonomous()
{
	// Send data to the control task to send to other tasks
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
		double Power1 = Move(100.0, 0.2, &Error1);
		control.SpeedLeft = -Power1;
		control.SpeedRight = Power1;
		std::cout << "Error: " << Error1 << std::endl;
		if (fabs(Error1) < 500)
			stage++;
		double Error2;
		double Power2 = Lift(50000, 0.6, &Error2);
		control.SpeedLift = Power2;
		if (fabs(Error2) < 100)
			stage++;
	}
	break;
	default:
		break;
	}
	ControlTaskInstance.UpdateAutonomousData(control);
	std::cout << "Stage: " << stage << std::endl;
}

void AutonomousLeft::End()
{
	double CurrentTime = Timer::GetFPGATimestamp();
	RunTime = CurrentTime - StartTime;
	LastMessage = floor(RunTime);
	MaxLog::LogInfo("Ending Auto after " + std::to_string(LastMessage) + " seconds");
}

void AutonomousLeft::ResetSensor()
{
	AutoMotorLeft->SetSelectedSensorPosition(0, 0, 0);
	AutoMotorLeft->GetSensorCollection().SetQuadraturePosition(0, 0);
	AutoMotorRight->SetSelectedSensorPosition(0, 0, 0);
	AutoMotorRight->GetSensorCollection().SetQuadraturePosition(0, 0);
}

double AutonomousLeft::Move(double Inches, double SpeedLimit, double * Error)
{
	double TargetDistance = InchesToTicks(Inches);
	double Gain = 0.0000125;
	*Error = TargetDistance - AutoMotorRight->GetSensorCollection().GetQuadraturePosition();
	double Power = *Error * Gain;
	Power = fmin(Power, SpeedLimit);
	Power = fmax(Power, -SpeedLimit);
	return Power;
	//std::cout << "Taraget: " << TargetDistance << " Actual: " << AutoMotorRight->GetSensorCollection().GetQuadraturePosition();
	//std::cout << " Error: " << Error << " Power: " << Power << std::endl;
}

double AutonomousLeft::Lift(double Ticks, double SpeedLimit, double * Error)
{
	double TargetDistance = Ticks;
	double Gain = 0.001;
	*Error = TargetDistance - AutoMotorLift->GetSensorCollection().GetQuadraturePosition();
	double Power = *Error * Gain;
	Power = fmin(Power, SpeedLimit);
	Power = fmax(Power, -SpeedLimit);
	std::cout << "Power: " << Power << " Target Distance: " << TargetDistance << " Error: " << *Error << " Actual: " << AutoMotorLift->GetSensorCollection().GetQuadraturePosition() << std::endl;
	return Power;
}

double AutonomousLeft::InchesToTicks(double Inches)
{
	return Inches * 1666.667;
}

std::string AutonomousLeft::GetName()
{
	return "AutonomousLeft";
}