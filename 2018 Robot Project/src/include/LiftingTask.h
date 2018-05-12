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
	const int LEFT_ENCODER_OFFSET = 514 + 380;
	const int RIGHT_ENCODER_OFFSET = 112 - 380;

	/* Encoder Limits for Lift */
	const int LIFT_LOWER_LIMIT = -15460;
	const int LIFT_UPPER_LIMIT = 74750;


	const double GAIN_LEFT = 0.008; // 0.016;
	const double GAIN_RIGHT = 0.0065;

	double left_error_integrate = 0.0;
	double right_error_integrate = 0.0;

	const double MAX_INTEGRATOR_ERROR = 15.0;
	const double INTEGRATOR_DIVISOR = 30.0;
	const double GAIN_LEFT_INTEGRATE = 0.005;
	const double GAIN_RIGHT_INTEGRATE = 0.005;

	int runs = 0;

	void Init();
	void CurrentLimit(TalonSRX * talon, int amps);
	RobotControl * ControlInput;
	TalonSRX * GrabArmL;
	TalonSRX * GrabArmR;
	TalonSRX * MasterMotorLift;
	TalonSRX * SlaveMotorLift;
	TalonSRX * GrabWheelL;
	TalonSRX * GrabWheelR;
};