#include "DrivingTask.h"
#include <iostream>
#include "maxutils/MaxDataStream.h"

void DrivingTask::Always()
{
	//std::cout << "Speed Left: " << ControlInput->SpeedLeft << " Speed Right: " << ControlInput->SpeedRight << std::endl;
}

#if COMP_BOT
void DrivingTask::SetIndividualPIDConstants(TalonSRX * talon, double P, double I, double D, double F)
{
	talon->Config_kP(0, P, 0);
	talon->Config_kI(0, I, 0);
	talon->Config_kD(0, D, 0);
	talon->Config_kF(0, F, 0);
}

void DrivingTask::SetPIDConstants(GearType TargetGear)
{
	double P = 0;
	double I = 0;
	double D = 0;
	double F = 0;

	switch (TargetGear)
	{
	case High:
		P = 0.03;
		I = 0.0;
		D = 0.0;
		F = 0.06;
	case Low:
	default:
		P = 0.006;
		I = 0.0;
		D = 0.0;
		F = 0.1132;
	}

	SetIndividualPIDConstants(RightMotor3, P, I, D, F);
	SetIndividualPIDConstants(LeftMotor1, P, I, D, F);
}
#endif

void DrivingTask::Run()
{	
#if COMP_BOT
	double RightSpeed = RightMotor3->GetSelectedSensorVelocity(0);
	double LeftSpeed = LeftMotor1->GetSelectedSensorVelocity(0);

	if (ActiveGear == Low && fabs(LeftSpeed) > 10000.0 && fabs(RightSpeed) > 10000.0)
	{
		ActiveGear = High;
	}

	if (ActiveGear == High && (fabs(LeftSpeed) < 6000.0 || fabs(RightSpeed) < 6000.0))
	{
		ActiveGear = Low;
	}

	if (ControlInput->SolenoidPos == -1)
	{
		ActiveGear = Low;
	}

	switch (ActiveGear)
	{
	case High:
		DriveShift->Set(frc::DoubleSolenoid::Value::kForward);
		break;
	case Low:
	default:
		DriveShift->Set(frc::DoubleSolenoid::Value::kReverse);
		break;
	}
	SetPIDConstants(ActiveGear);

	double TargetVelocityL = ControlInput->SpeedLeft * 4096.0 * 2000.0 / 600.0;
	double TargetVelocityR = ControlInput->SpeedRight * 4060.0 * 2000.0 / 600.0;

	SetPIDConstants(ActiveGear);

	LeftMotor1->Set(ControlMode::Velocity, TargetVelocityL);
	RightMotor3->Set(ControlMode::Velocity, TargetVelocityR);

	runs++;
	if (runs % 10 == 0)
	{
		runs = 0;
		MaxLog::TransmitInt("/gear", 0);
		MaxLog::TransmitDouble("/lefttargetvel", TargetVelocityL);
		MaxLog::TransmitDouble("/righttargetvel", TargetVelocityR);
		MaxLog::TransmitDouble("/rightactualvel", RightMotor3->GetSelectedSensorVelocity(0));
		MaxLog::TransmitDouble("/leftactualvel", LeftMotor1->GetSelectedSensorVelocity(0));
	}

	//LeftMotor1->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	//std::cout << "Left Drive Motor:  " << LeftMotor1->GetSelectedSensorVelocity(0) << " Target: " << TargetVelocityL << std::endl;
#else
	LeftMotor1->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLeft);
	RightMotor3->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInut->SpeedRight);
#endif
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
	Run();
}

void DrivingTask::ControllerUpdate(MaxControl * controls)
{
	ControlInput = (RobotControl *)(controls);
}

void DrivingTask::Init()
{
	ControlInput = new RobotControl();

#if COMP_BOT
	ActiveGear = Low;
#endif

	LeftMotor1 = new TalonSRX(0);
	LeftMotor2 = new TalonSRX(1);
#if COMP_BOT
	LeftMotor3 = new TalonSRX(2);
	RightMotor1 = new TalonSRX(13);
#endif
	RightMotor2 = new TalonSRX(14);
	RightMotor3 = new TalonSRX(15);
#if COMP_BOT
	DriveShift = new frc::DoubleSolenoid { 0, 1 };
#endif
	runs = 0;

	ConfigureCurrentLimit(LeftMotor1);
	ConfigureCurrentLimit(LeftMotor2);
#if COMP_BOT
	ConfigureCurrentLimit(LeftMotor3);
	ConfigureCurrentLimit(RightMotor1);
#endif
	ConfigureCurrentLimit(RightMotor2);
	ConfigureCurrentLimit(RightMotor3);

	ConfigureDriveTalon(LeftMotor1);
	LeftMotor2->Set(ControlMode::Follower, 0);
#if COMP_BOT
	LeftMotor3->Set(ControlMode::Follower, 0);
	RightMotor1->Set(ControlMode::Follower, 15);
#endif
	RightMotor2->Set(ControlMode::Follower, 15);
	ConfigureDriveTalon(RightMotor3); 
	
#if COMP_BOT
	SetPIDConstants(ActiveGear);
#endif
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
}