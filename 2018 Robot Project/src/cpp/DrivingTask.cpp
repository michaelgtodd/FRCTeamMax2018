#include "DrivingTask.h"
#include <iostream>
#include "maxutils/MaxDataStream.h"

void DrivingTask::Always()
{

}

void DrivingTask::Run()
{
	/*Set motor speed using data from joystick*/
	MasterMotorLeft->Set(ControlMode::PercentOutput, ControlInput->SpeedLeft);
	MasterMotorRight->Set(ControlMode::PercentOutput, ControlInput->SpeedRight);

	/*Print data to dashboard*/
	runs++;
	if (runs % 50 == 0)
	{
		if (ControlInput->DebugPrints == true)
		{
			//std::cout << "Speed Left:  " << ControlInput->SpeedLeft << std::endl;
			//std::cout << "Speed Right: " << ControlInput->SpeedRight << std::endl;
		}
		runs = 0;
	}
}

void DrivingTask::Disable()
{
	/*Set both motors to zero*/
	MasterMotorLeft->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
	MasterMotorRight->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, 0);
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
	runs = 0;

	/*Define Talons*/
	ControlInput = new RobotControl();
	MasterMotorLeft = new TalonSRX(0);
	SlaveMotorLeft = new TalonSRX(1);
	MasterMotorRight = new TalonSRX(15);
	SlaveMotorRight = new TalonSRX(14);

	/*Configure current limits*/
	ConfigureCurrentLimit(MasterMotorLeft);
	ConfigureCurrentLimit(SlaveMotorLeft);
	ConfigureCurrentLimit(SlaveMotorRight);
	ConfigureCurrentLimit(MasterMotorRight);

	/*Set control mode*/
	ConfigureDriveTalon(MasterMotorLeft);
	ConfigureDriveTalon(MasterMotorRight);
	SlaveMotorLeft->Set(ControlMode::Follower, 1);
	SlaveMotorRight->Set(ControlMode::Follower, 15);
}

void DrivingTask::ConfigureCurrentLimit(TalonSRX * talon)
{
	talon->ConfigContinuousCurrentLimit(25, 0);
	talon->ConfigPeakCurrentLimit(0, 0);
	talon->ConfigPeakCurrentDuration(0, 0);
	talon->EnableCurrentLimit(true);
	talon->ConfigClosedloopRamp(0.3, 10);
	talon->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
	talon->ConfigVoltageCompSaturation(12.0, 10);
	talon->EnableVoltageCompensation(true);
	talon->ConfigVoltageMeasurementFilter(32, 10);
}

void DrivingTask::ConfigureDriveTalon(TalonSRX * talon)
{
	talon->SetSensorPhase(false);
	talon->ConfigNominalOutputForward(0, 10);
	talon->ConfigNominalOutputReverse(0, 10);
	talon->ConfigPeakOutputForward(1, 10);
	talon->ConfigPeakOutputReverse(-1, 10);
}