#pragma once
#define COMP_BOT true
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
	int runs;
	void Init();
	void ConfigureCurrentLimit(TalonSRX * talon);
	void ConfigureDriveTalon(TalonSRX * talon);
	RobotControl * ControlInput;
	TalonSRX * MasterMotorLeft;
	TalonSRX * SlaveMotorLeft;
	TalonSRX * SlaveMotorRight;
	TalonSRX * MasterMotorRight;
};