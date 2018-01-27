#pragma once
#include "maxutils/MaxTask.h"
#include "ControlTask.h"
#include "ctre/Phoenix.h"

class DrivingTask : public MaxTask
{
public:
	void Always();
	void Run();
	void Disable();
	void Autonomous();
	void ControllerUpdate(MaxControl * controls);
private:
	void Init();
	RobotControl * ControlInput;
	TalonSRX * LeftMotor1;
	TalonSRX * LeftMotor2;
	TalonSRX * LeftMotor3;
	TalonSRX * RightMotor1;
	TalonSRX * RightMotor2;
	TalonSRX * RightMotor3;
	TalonSRX * GrabArmL;
	TalonSRX * GrabArmR;
	TalonSRX * GrabWheelL;
	TalonSRX * GrabWheelR;
	TalonSRX * LiftMotorL;
	TalonSRX * LiftMotorR;
};