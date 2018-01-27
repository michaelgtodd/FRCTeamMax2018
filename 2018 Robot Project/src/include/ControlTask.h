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

class RobotControl : public MaxControl
{
public:
	RobotControl();
	ControlType ControllerType;
	ControlLayout ControllerMode;
	double SpeedLeft;
	double SpeedRight;
	double SpeedLift;
	double SpeedArmLeft;
	double SpeedArmRight;
	double SpeedGrabWheelLeft;
	double SpeedGrabWheelRight;
};

class AutonomousControl
{
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
	MaxTaskSchedule * taskschedule_;
	RobotControl * Controls;
	double AxisPrimaryX;
	double AxisPrimaryY;
	double AxisSecondaryX;
	double AxisSecondaryY;
};

extern ControlTask ControlTaskInstance;
extern MaxTaskSchedule taskschedule;