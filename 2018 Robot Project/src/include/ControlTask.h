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

enum ControlMode
{
	Tank = 0,
	Arcade = 1
};

class RobotControl : public MaxControl
{
public:
	RobotControl();
	ControlType ControllerType;
	double SpeedLeft;
	double SpeedRight;
	double SpeedLift;
};

class ControlTask : public MaxTask
{
public:
	ControlTask(MaxTaskSchedule * taskschedule);
	void Run();
	void Disable();
	void ControllerUpdate(MaxControl * controls);
	void Autonomous();
	void ProcessOscData(osc::ReceivedMessage messages);
private:
	void Init();
	MaxTaskSchedule * taskschedule_;
	RobotControl * controls;
	double AxisPrimaryX;
	double AxisPrimaryY;
	double AxisSecondaryX;
	double AxisSecondaryY;
};