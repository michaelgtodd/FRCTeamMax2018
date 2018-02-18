#pragma once
#include "maxutils/MaxTask.h"
#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"
#include "WPILib.h"
#include <string.h>

enum ControlType
{
	JoystickType = 0,
	XboxType = 1
};

enum ControlLayout
{
	Tank = 0,
	Arcade = 1
};

enum FieldPos
{
	Left = -1,
	Center = 0,
	Right = 1
};

class RobotControl : public MaxControl
{
public:
	RobotControl();
	ControlType DriverType;
	ControlType SwitchesType;
	ControlLayout DriverMode;
	ControlLayout SwitchesMode;
	int DriverPreference;
	int SwitchesPreference;
	double SpeedLeft;
	double SpeedRight;
	double SpeedLift;
	double LeftArmPosition;
	double RightArmPosition;
	int LiftHeight;
	int SolenoidPos;
	bool ResetPos;
	bool Override;
};

class AutonomousControl : public MaxControl
{
public:
	FieldPos StartingPos;
	FieldPos SwitchPos;
	FieldPos ScalePos;
};

class ControlTask : public MaxTask
{
public:
	ControlTask(MaxTaskSchedule * taskschedule);
	void Always();
	void Run();
	void Disable();
	void ControllerUpdate(MaxControl * controls);
	void Autonomous();
	void ProcessOscData(osc::ReceivedMessage messages);
	void UpdateAutonomousData(AutonomousControl);
private:
	void Init();
	int run;
	MaxTaskSchedule * taskschedule_;
	RobotControl * Controls;
	AutonomousControl * Auto;
	double AxisPrimaryX;
	double AxisPrimaryY;
	double AxisSecondaryX;
	double AxisSecondaryY;
	int Output;
	int debugCounter;
	const int DEBUG_COUNTER_LIMIT = 60;
};

extern ControlTask ControlTaskInstance;
extern MaxTaskSchedule taskschedule;