#include "LiftingTask.h"
#include <iostream>
#include "maxutils/MaxDataStream.h"


void LiftingTask::Always()
{

}

int modulo(int x, int N)
{
	return (x % N + N) % N;
}

void LiftingTask::Run()
{
	//std::cout << ControlInput->SpeedArmLeft << " " << ControlInput->SpeedArmRight << std::endl;
	//GrabArmL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedArmLeft);
	//GrabArmR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedArmRight);
	GrabWheelL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedGrabWheelLeft);
	GrabWheelR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedGrabWheelRight);
	//std::cout << ControlInput->SpeedGrabWheelLeft << ControlInput->SpeedGrabWheelRight << std::endl;
	//if (fabs(ControlInput->SpeedLift) > 0)
	//{
		LiftMotorL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLift);
		LiftMotorR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLift);
	//}
	//else
	//{
	//	LiftMotorL->Set(ctre::phoenix::motorcontrol::ControlMode::Position, ControlInput->SpeedLift);
	//	LiftMotorR->Set(ctre::phoenix::motorcontrol::ControlMode::Position, ControlInput->SpeedLift);
	//}


	int PulseWidthPosL = GrabArmL->GetSensorCollection().GetPulseWidthPosition();
	//std::cout << "Left Arm Encoder: " << modulo(PulseWidthPosL, 4096) - LEFT_ENCODER_OFFSET << std::endl;
	int PulseWidthPosR = GrabArmR->GetSensorCollection().GetPulseWidthPosition();
	//std::cout << "Right Arm Encoder: " << modulo(PulseWidthPosR, 4096) - RIGHT_ENCODER_OFFSET << std::endl;
	double DegreeLeftArmPosition = ((double)modulo(PulseWidthPosL - LEFT_ENCODER_OFFSET, 4096)) / 4096.0 * 360.0;
	//std::cout << "Left Arm Encoder: " << DegreeLeftArmPosition;
	double DegreeRightArmPosition = ((double)modulo(PulseWidthPosR - RIGHT_ENCODER_OFFSET, 4096)) / 4096.0 * 360.0;
	//std::cout << " Right Arm Encoder: " << DegreeRightArmPosition << std::endl;


	double RightError = 240.0 - DegreeRightArmPosition;
	GrabArmR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, RightError * GAIN);
	//std::cout << " Right Error: " << RightError << "Output" << RightError * GAIN << std::endl;

	double LeftError = 120.0 - DegreeLeftArmPosition;
	GrabArmL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, LeftError * GAIN);
	//std::cout << "Left Error: " << LeftError << " Output: " << LeftError * GAIN << "Left Arm Position"<<DegreeLeftArmPosition << std::endl;
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

	GrabArmL = new TalonSRX(5);
	GrabArmR = new TalonSRX(10);
	GrabWheelL = new TalonSRX(11);
	GrabWheelR = new TalonSRX(4);
	LiftMotorL = new TalonSRX(12);
	LiftMotorR = new TalonSRX(3);

	CurrentLimit(GrabArmL, 2);
	CurrentLimit(GrabArmR, 2);
	CurrentLimit(GrabWheelL, 20);
	CurrentLimit(GrabWheelR, 20);
	CurrentLimit(LiftMotorL, 20);
	CurrentLimit(LiftMotorR, 20);


	GrabArmL->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 0);
	GrabArmR->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 0);
}

void LiftingTask::CurrentLimit(TalonSRX * talon, int amps)
{
	talon->ConfigContinuousCurrentLimit(amps, 0);
	talon->ConfigPeakCurrentLimit(0, 0);
	talon->ConfigPeakCurrentDuration(0, 0);
	talon->EnableCurrentLimit(true);
	talon->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
}