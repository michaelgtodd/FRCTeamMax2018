#include "DrivingTask.h"
#include <iostream>
#include "maxutils/MaxDataStream.h"


void DrivingTask::Always()
{

}

void DrivingTask::Run()
{	
	// Make sure power to Talons doesn't go above 100%
	ControlInput->SpeedLeft = ControlInput->SpeedLeft >= 0.99 ? 0.99 : ControlInput->SpeedLeft;
	ControlInput->SpeedLeft = ControlInput->SpeedLeft <= -0.99 ? -0.99 : ControlInput->SpeedLeft;
	ControlInput->SpeedRight = ControlInput->SpeedRight >= 0.99 ? 0.99 : ControlInput->SpeedRight;
	ControlInput->SpeedRight = ControlInput->SpeedRight <= -0.99 ? -0.99 : ControlInput->SpeedRight;
	ControlInput->SpeedLift = ControlInput->SpeedLift >= 0.99 ? 0.99 : ControlInput->SpeedLift;
	ControlInput->SpeedLift = ControlInput->SpeedLeft <= -0.99 ? -0.99 : ControlInput->SpeedLift;
	
	// Set Talon power
	LeftMotor1->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLeft);
	LeftMotor2->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLeft);
	LeftMotor3->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLeft);
	RightMotor1->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedRight);
	RightMotor2->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedRight);
	RightMotor3->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedRight);
	LiftMotorL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLift);
	LiftMotorR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLift);
}

void DrivingTask::Disable()
{
	LeftMotor1->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	LeftMotor2->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	LeftMotor3->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	RightMotor1->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	RightMotor2->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	RightMotor3->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	GrabArmL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	GrabArmR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	GrabWheelL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	GrabWheelR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	LiftMotorL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	LiftMotorR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
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

	LeftMotor1 = new TalonSRX(1);
	LeftMotor2 = new TalonSRX(2);
	LeftMotor3 = new TalonSRX(3);
	RightMotor1 = new TalonSRX(4);
	RightMotor2 = new TalonSRX(5);
	RightMotor3 = new TalonSRX(6);
}