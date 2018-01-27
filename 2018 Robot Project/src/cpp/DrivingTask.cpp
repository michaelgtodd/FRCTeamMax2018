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
	LiftMotorL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLift);
	LiftMotorR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLift);
	GrabArmL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedArmLeft);
	GrabArmR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedArmRight);
	GrabWheelL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedGrabWheel);
	GrabWheelR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -ControlInput->SpeedGrabWheel);
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

	LeftMotor1 = new TalonSRX(0);
	LeftMotor2 = new TalonSRX(1);
	LeftMotor3 = new TalonSRX(2);
	RightMotor1 = new TalonSRX(13);
	RightMotor2 = new TalonSRX(14);
	RightMotor3 = new TalonSRX(15);
	GrabArmL = new TalonSRX(10);
	GrabArmR = new TalonSRX(5);
	GrabWheelL = new TalonSRX(11);
	GrabWheelR = new TalonSRX(4);
	LiftMotorL = new TalonSRX(12);
	LiftMotorR = new TalonSRX(3);
}