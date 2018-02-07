#include "DrivingTask.h"
#include <iostream>
#include "maxutils/MaxDataStream.h"


void DrivingTask::Always()
{

}

void DrivingTask::Run()
{	
	LeftMotor1->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLeft);
	LeftMotor2->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLeft);
	LeftMotor3->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLeft);
	RightMotor1->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedRight);
	RightMotor2->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedRight);
	RightMotor3->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedRight);
	switch (ControlInput->SolenoidPos)
	{
	case -1:
		DriveShift->Set(frc::DoubleSolenoid::Value::kReverse);
		break;
	case 1:
		DriveShift->Set(frc::DoubleSolenoid::Value::kForward);
		break;
	default:
		DriveShift->Set(frc::DoubleSolenoid::Value::kReverse);
		break;
	}
}

void DrivingTask::Disable()
{
	LeftMotor1->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	LeftMotor2->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	LeftMotor3->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	RightMotor1->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	RightMotor2->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	RightMotor3->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
}

void DrivingTask::Autonomous()
{
	Disable();
}

void DrivingTask::ControllerUpdate(MaxControl * controls)
{
	ControlInput = (RobotControl *)(controls);
}

void DrivingTask::Init()
{
	ControlInput = new RobotControl();

	LeftMotor1 = new TalonSRX(0);
	LeftMotor2 = new TalonSRX(1);
	LeftMotor3 = new TalonSRX(2);
	RightMotor1 = new TalonSRX(13);
	RightMotor2 = new TalonSRX(14);
	RightMotor3 = new TalonSRX(15);
	DriveShift = new frc::DoubleSolenoid { 0, 1 };

	ConfigureCurrentLimit(LeftMotor1);
	ConfigureCurrentLimit(LeftMotor2);
	ConfigureCurrentLimit(LeftMotor3);
	ConfigureCurrentLimit(RightMotor1);
	ConfigureCurrentLimit(RightMotor2);
	ConfigureCurrentLimit(RightMotor3);
}

void DrivingTask::ConfigureCurrentLimit(TalonSRX * talon)
{
	talon->ConfigContinuousCurrentLimit(35, 0);
	talon->ConfigPeakCurrentLimit(0, 0);
	talon->ConfigPeakCurrentDuration(0, 0);
	talon->EnableCurrentLimit(true);
	talon->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
}