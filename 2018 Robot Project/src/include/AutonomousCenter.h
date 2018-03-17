#pragma once
#include "maxutils\MaxAutonomous.h"
#include "ControlTask.h"
#include "ctre/Phoenix.h"
#include <math.h>

class AutonomousCenter : public MaxAutonomousTask
{
public:
	void Init();
	void ControllerUpdate(MaxControl * controls);
	void Autonomous();
	void End();
	bool Drive(double InchesInInchces, double SpeedLimit, double ToleranceInInches);
	bool Move(double InchesInInches, double SpeedLimit, double ToleranceInInches);
	bool Lift(double Ticks, double SpeedLimit, double Tolerance);
	bool Turn(double Degrees, double SpeedLimit, double Tolerance);
	void Brake();
	void ResetSensor();
	double InchesToTicks(double Inches);
	int stage;
	std::string GetName();
	double StageStartTime = 0;
private:
	double StartTime = 0;
	double RunTime = 0;
	double LeftError = 0, LeftTarget = 0, LeftTravel = 0;
	double RightError = 0, RightTarget = 0, RightTravel = 0;
	double LiftError = 0, LiftTarget = 0;
	double * YPR[3];
	double * Yaw;
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