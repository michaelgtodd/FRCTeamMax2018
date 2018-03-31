#pragma once
#include "maxutils\MaxAutonomous.h"
#include "ControlTask.h"
#include "ctre/Phoenix.h"
#include <math.h>

class AutonomousLeft : public MaxAutonomousTask
{
public:
	void Init();
	void ControllerUpdate(MaxControl * controls);
	void Autonomous();
	void End();
	void Drive(double SpeedMin, double SpeedMax);
	void Drive2(double IdealSpeed);
	void Lift(int MaxHeight, double Speed);
	bool Turn(double Degrees, double SpeedLimit, double Tolerance);
	bool TimePassed(double Time);
	void Brake();
	void ResetSensor();
	int stage = 0;
	std::string GetName();
	double StageStartTime = 0;
private:
	double StartTime, CurrentTime;
	int runs = 0;
	double RunTime = 0;
	int LastMessage = 0;
	double LeftSpeed = 0, RightSpeed = 0;
	double Yaw = 0, YPR[3], InitialYaw;
	const double TurnTolerance = 2, TurnAdjustment = 0.025;
	const bool PigeonEnable = true;
	AutonomousControl control;
	SwitchPriority SwitchPriorityInput;
	FieldPos SwitchPosition;
	FieldPos ScalePosition;
	FieldPos FarSwitchPosition;
	TalonSRX * AutoMotorLift;
	TalonSRX * AutoPosArmLeft;
	TalonSRX * AutoPosArmRight;
	TalonSRX * DummyTalon;
	PigeonIMU * PigeonInput;
};