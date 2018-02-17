#include "DrivingTask.h"
#include <iostream>
#include "maxutils/MaxDataStream.h"


void DrivingTask::Always()
{
	//std::cout << "Speed Left: " << ControlInput->SpeedLeft << " Speed Right: " << ControlInput->SpeedRight << std::endl;
}

void DrivingTask::Run()
{	
	//LeftMotor1->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLeft);
	//LeftMotor2->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLeft);
	//LeftMotor3->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLeft);
	//RightMotor1->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedRight);
	//RightMotor2->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedRight);
	//RightMotor3->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedRight);
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
	double TargetVelocityL = ControlInput->SpeedLeft * 4096.0 * 2000.0 / 600.0;
	double TargetVelocityR = ControlInput->SpeedRight * 4060.0 * 2000.0 / 600.0;
	LeftMotor1->Set(ControlMode::Velocity, TargetVelocityL);
	RightMotor3->Set(ControlMode::Velocity, TargetVelocityR);
	//LeftMotor1->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	//std::cout << "Left Drive Motor:  " << LeftMotor1->GetSelectedSensorVelocity(0) << " Target: " << TargetVelocityL << std::endl;
}

void DrivingTask::Disable()
{
	LeftMotor1->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	//LeftMotor2->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	//LeftMotor3->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	//RightMotor1->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	//RightMotor2->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
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

	ConfigureDriveTalon(LeftMotor1);
	LeftMotor2->Set(ControlMode::Follower, 0);
	LeftMotor3->Set(ControlMode::Follower, 0);
	RightMotor1->Set(ControlMode::Follower, 15);
	RightMotor2->Set(ControlMode::Follower, 15);
	ConfigureDriveTalon(RightMotor3);
}

void DrivingTask::ConfigureCurrentLimit(TalonSRX * talon)
{
	talon->ConfigContinuousCurrentLimit(25, 0);
	talon->ConfigPeakCurrentLimit(0, 0);
	talon->ConfigPeakCurrentDuration(0, 0);
	talon->EnableCurrentLimit(true);
	talon->ConfigClosedloopRamp(0.3, 10);
	talon->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
	talon->ConfigVoltageCompSaturation(12.0, 10);
	talon->EnableVoltageCompensation(true);
	talon->ConfigVoltageMeasurementFilter(32, 10);
}

void DrivingTask::ConfigureDriveTalon(TalonSRX * talon)
{
	talon->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, 10);
	talon->SetSensorPhase(false);
	talon->ConfigNominalOutputForward(0, 10);
	talon->ConfigNominalOutputReverse(0, 10);
	talon->ConfigPeakOutputForward(1, 10);
	talon->ConfigPeakOutputReverse(-1, 10);
	talon->Config_kF(0, 0.06, 10);
	talon->Config_kP(0, 0.03, 10);
	talon->Config_kI(0, 0, 10);
	talon->Config_kD(0, 0, 10);
}