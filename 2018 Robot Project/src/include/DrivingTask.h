#pragma once
#define COMP_BOT true
#include "maxutils/MaxTask.h"
#include "ControlTask.h"
#include "ctre/Phoenix.h"

enum GearType
{
	Low = 0,
	High = 1
};

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
#if COMP_BOT
	void SetIndividualPIDConstants(TalonSRX * talon, double P, double I, double D, double F);
	void SetPIDConstants(GearType TargetGear);
#endif
	GearType ActiveGear;
	RobotControl * ControlInput;
	TalonSRX * LeftMotor1;
	TalonSRX * LeftMotor2;
#if COMP_BOT
	TalonSRX * LeftMotor3;
	TalonSRX * RightMotor1;
#endif
	TalonSRX * RightMotor2;
	TalonSRX * RightMotor3;
#if COMP_BOT
	DoubleSolenoid * DriveShift;
#endif
};