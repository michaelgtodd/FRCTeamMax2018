#pragma once
#include "maxutils\MaxAutonomous.h"
#include "ControlTask.h"
#include "ctre/Phoenix.h"
#include <math.h>


class SampleAutonomous : public MaxAutonomousTask
{
public:
	void Init();
	void ControllerUpdate(MaxControl * controls);
	void Autonomous();
	void End();
	void Brake();
	void TimeAdvance(double Time);
	void UpdateVariables();
	void UpdateMotors();
	void SensorReset();
	std::string GetName();
private:
	AutonomousControl control;
	SwitchPriority SwitchPriorityInput;
	FieldPos StartingPosition;
	FieldPos SwitchPosition;
	FieldPos ScalePosition;
	FieldPos FarSwitchPosition;
	TalonSRX * AutoMotorLift;
	TalonSRX * AutoPosArmLeft;
	TalonSRX * AutoPosArmRight;
	TalonSRX * DummyTalon;
	PigeonIMU * PigeonInput;
	double LeftSpeed, RightSpeed, LiftSpeed, ArmDegree;
	double StartTime = 0;
	double Stopwatch = 0;
	double RunTime = 0;
	int CycleNumber = 0;
	int Stage = 0;
};