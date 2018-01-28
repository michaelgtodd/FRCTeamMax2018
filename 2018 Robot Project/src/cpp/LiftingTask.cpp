#include "LiftingTask.h"
#include <iostream>
#include "maxutils/MaxDataStream.h"


void LiftingTask::Always()
{

}

void LiftingTask::Run()
{
	GrabArmL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedArmLeft);
	GrabArmR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedArmRight);
	GrabWheelL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedGrabWheelLeft);
	GrabWheelR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedGrabWheelRight);
	LiftMotorL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLift);
	LiftMotorR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLift);
}

void LiftingTask::Disable()
{
	GrabArmL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	GrabArmR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	GrabWheelL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	GrabWheelR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	LiftMotorL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	LiftMotorR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
}

void LiftingTask::Autonomous()
{
	Disable();
}

void LiftingTask::ControllerUpdate(MaxControl * controls)
{
	ControlInput = (RobotControl *)(controls);
}

void LiftingTask::Init()
{
	ControlInput = new RobotControl();

	GrabArmL = new TalonSRX(10);
	GrabArmR = new TalonSRX(5);
	GrabWheelL = new TalonSRX(11);
	GrabWheelR = new TalonSRX(4);
	LiftMotorL = new TalonSRX(12);
	LiftMotorR = new TalonSRX(3);

	CurrentLimit(GrabArmL, 1);
	CurrentLimit(GrabArmR, 1);
	CurrentLimit(GrabWheelL, 20);
	CurrentLimit(GrabWheelR, 20);
	CurrentLimit(LiftMotorL, 20);
	CurrentLimit(LiftMotorR, 20);
}

void LiftingTask::CurrentLimit(TalonSRX * talon, int amps)
{
	talon->ConfigContinuousCurrentLimit(amps, 0);
	talon->ConfigPeakCurrentLimit(0, 0);
	talon->ConfigPeakCurrentDuration(0, 0);
	talon->EnableCurrentLimit(true);
}