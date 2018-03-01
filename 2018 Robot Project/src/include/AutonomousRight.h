#pragma once
#include "maxutils\MaxAutonomous.h"
#include "ControlTask.h"
#include "ctre/Phoenix.h"
#include <math.h>

class AutonomousRight : public MaxAutonomousTask
{
public:
	void Init();
	void ControllerUpdate(MaxControl * controls);
	void Autonomous();
	void End();
	void ResetSensor();
	double Move(double Inches, double SpeedLimit, double * Error);
	double Lift(double Ticks, double SpeedLimit, double * Error);
	double InchesToTicks(double Inches);
	int stage;
	std::string GetName();
	double StageStartTime = 0;
private:
	double StartTime = 0;
	double RunTime = 0;
	double Yaw = 0;
	int LastMessage = 0;
	AutonomousControl control;
	SwitchPriority SwitchPriorityInput;
	FieldPos SwitchPosition;
	FieldPos ScalePosition;
	FieldPos FarSwitchPosition;
	TalonSRX * AutoMotorLeft;
	TalonSRX * AutoMotorRight;
	TalonSRX * AutoMotorLift;
	TalonSRX * AutoPosArmLeft;
	TalonSRX * AutoPosArmRight;
	TalonSRX * DummyTalon;
	PigeonIMU * PigeonInput;
};