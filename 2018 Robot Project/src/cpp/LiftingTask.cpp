#include "LiftingTask.h"
#include <iostream>
#include "maxutils/MaxDataStream.h"

int modulo(int x, int N)
{
	return (x % N + N) % N;
}

void LiftingTask::Always()
{
	int PulseWidthPosL = GrabArmL->GetSensorCollection().GetPulseWidthPosition();
	int PulseWidthPosR = GrabArmR->GetSensorCollection().GetPulseWidthPosition();

	/*Print data to dashboard*/
	runs++;
	if (runs > 4)
	{
		if (ControlInput->SwitchesDebug == true)
		{
			std::cout << "Left Arm Encoder:  " << modulo(PulseWidthPosL, 4096) << std::endl;
			std::cout << "Right Arm Encoder: " << modulo(PulseWidthPosR, 4096) << std::endl;
		}
		runs = 0;
	}
}

void LiftingTask::Run()
{
	/*Set wheel speed*/
	GrabWheelL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->WheelSpeed);
	GrabWheelR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, -ControlInput->WheelSpeed);

	/*Set lift speed*/
	MasterMotorLift->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, ControlInput->SpeedLift);

	/*Get encoder data from arms*/
	int PulseWidthPosL = GrabArmL->GetSensorCollection().GetPulseWidthPosition();
	int PulseWidthPosR = GrabArmR->GetSensorCollection().GetPulseWidthPosition();
	double DegreeLeftArmPosition = ((double)modulo(PulseWidthPosL - LEFT_ENCODER_OFFSET, 4096)) / 4096.0 * 360.0;
	double DegreeRightArmPosition = ((double)modulo(PulseWidthPosR - RIGHT_ENCODER_OFFSET, 4096)) / 4096.0 * 360.0;

	/*Set left arm based on encoder data*/
	double LeftError = ControlInput->LeftArmPosition - DegreeLeftArmPosition;
	left_error_integrate += LeftError / INTEGRATOR_DIVISOR;
	left_error_integrate = fabs(left_error_integrate) > MAX_INTEGRATOR_ERROR ? (fabs(left_error_integrate) / left_error_integrate) * MAX_INTEGRATOR_ERROR : left_error_integrate;
	if (fabs(LeftError) < 2.0)
	{
		left_error_integrate *= 0.8;
	}
	GrabArmL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, (LeftError * GAIN_LEFT) + (left_error_integrate * GAIN_LEFT_INTEGRATE));

	/*Set right arm based on encoder data*/
	double RightError = ControlInput->RightArmPosition - DegreeRightArmPosition;
	right_error_integrate += RightError / INTEGRATOR_DIVISOR;
	right_error_integrate = fabs(right_error_integrate) > MAX_INTEGRATOR_ERROR ? (fabs(right_error_integrate) / right_error_integrate) * MAX_INTEGRATOR_ERROR : right_error_integrate;
	if (fabs(RightError) < 2.0) 
	{
		right_error_integrate *= 0.8;
	}
	GrabArmR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, (RightError * GAIN_RIGHT) + (right_error_integrate * GAIN_RIGHT_INTEGRATE));

	/*Transmit and print data*/
	if (runs > 4) 
	{
		MaxLog::TransmitDouble("/lift/left/error", LeftError);
		MaxLog::TransmitDouble("/lift/right/error", RightError);

		MaxLog::TransmitDouble("/lift/left/ierror", left_error_integrate);
		MaxLog::TransmitDouble("/lift/right/ierror", right_error_integrate);

		MaxLog::TransmitDouble("/lift/left/target", ControlInput->LeftArmPosition);
		MaxLog::TransmitDouble("/lift/right/target", ControlInput->RightArmPosition);

		MaxLog::TransmitDouble("/lift/left/actual", DegreeLeftArmPosition);
		MaxLog::TransmitDouble("/lift/right/actual", DegreeRightArmPosition);

		if (ControlInput->SwitchesDebug == true)
		{
			/*Print data to dashboard*/
			std::cout << "Left angle:  " << DegreeLeftArmPosition << std::endl;
			std::cout << "Right angle: " << DegreeRightArmPosition << std::endl;
		}
	}
}

void LiftingTask::Disable()
{
	/*Set all motors to zero*/
	GrabWheelL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	GrabWheelR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	GrabArmL->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	GrabArmR->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	MasterMotorLift->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	MasterMotorLift->SetSelectedSensorPosition(0, 0, 0);
	MasterMotorLift->GetSensorCollection().SetQuadraturePosition(0, 0);
}

void LiftingTask::Autonomous()
{
	Run();
}

void LiftingTask::ControllerUpdate(MaxControl * controls)
{
	ControlInput = (RobotControl *)(controls);
}

void LiftingTask::Init()
{
	runs = 0;

	/*Define Talons*/
	ControlInput = new RobotControl();
	GrabArmL = new TalonSRX(5);
	GrabArmR = new TalonSRX(10);
	MasterMotorLift = new TalonSRX(12);
	SlaveMotorLift = new TalonSRX(3);
	GrabWheelL = new TalonSRX(4);
	GrabWheelR = new TalonSRX(11);

	/*Configure current limits*/
	CurrentLimit(GrabArmL, 4);														
	CurrentLimit(GrabArmR, 4);
	CurrentLimit(MasterMotorLift, 10);
	CurrentLimit(SlaveMotorLift, 10);
	CurrentLimit(GrabWheelL, 10);
	CurrentLimit(GrabWheelR, 10);

	/*Set control mode*/
	MasterMotorLift->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, 10);
	MasterMotorLift->ConfigForwardSoftLimitEnable(true, 10);
	MasterMotorLift->ConfigReverseSoftLimitEnable(true, 10);
	MasterMotorLift->ConfigClosedloopRamp(0.2, 10);
	SlaveMotorLift->ConfigClosedloopRamp(0.2, 10);
	SlaveMotorLift->Set(ControlMode::Follower, 12);
	MasterMotorLift->ConfigForwardSoftLimitThreshold(LIFT_UPPER_LIMIT, 10);
	MasterMotorLift->ConfigReverseSoftLimitThreshold(LIFT_LOWER_LIMIT, 10);
	GrabArmL->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 10);
	GrabArmR->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 10);
}

void LiftingTask::CurrentLimit(TalonSRX * talon, int amps)
{
	talon->ConfigContinuousCurrentLimit(amps, 0);
	talon->ConfigPeakCurrentLimit(0, 0);
	talon->ConfigPeakCurrentDuration(0, 0);
	talon->EnableCurrentLimit(true);
	talon->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
}