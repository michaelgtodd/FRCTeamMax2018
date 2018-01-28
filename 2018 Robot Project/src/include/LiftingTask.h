#pragma once
#include "maxutils/MaxTask.h"
#include "ControlTask.h"
#include "ctre/Phoenix.h"

class LiftingTask : public MaxTask
{
public:
	void Always();
	void Run();
	void Disable();
	void Autonomous();
	void ControllerUpdate(MaxControl * controls);
private:
	void Init();
	void CurrentLimit(TalonSRX * talon, int amps);
	RobotControl * ControlInput;
	TalonSRX * GrabArmL;
	TalonSRX * GrabArmR;
	TalonSRX * GrabWheelL;
	TalonSRX * GrabWheelR;
	TalonSRX * LiftMotorL;
	TalonSRX * LiftMotorR;
};