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
	const int LEFT_ENCODER_OFFSET = 2770;
	const int RIGHT_ENCODER_OFFSET = 3113;
	const double GAIN = 0.006;
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