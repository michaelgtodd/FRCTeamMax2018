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
	
	/* Encoder Offsets for Arms */
	const int LEFT_ENCODER_OFFSET = 43;
	const int RIGHT_ENCODER_OFFSET = 3707;

	/* Encoder Limits for Lift */
	const int LIFT_LOWER_LIMIT = -14500;
	const int LIFT_UPPER_LIMIT = 195000;


	const double GAIN_LEFT = 0.016;
	const double GAIN_RIGHT = 0.01;
	void Init();
	void CurrentLimit(TalonSRX * talon, int amps);
	RobotControl * ControlInput;
	TalonSRX * GrabArmL;
	TalonSRX * GrabArmR;
	TalonSRX * LiftMotorL;
	TalonSRX * LiftMotorR;
	TalonSRX * GrabWheelL;
	TalonSRX * GrabWheelR;
};