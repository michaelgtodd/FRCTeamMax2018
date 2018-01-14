#pragma once
#include "maxutils/MaxTask.h"

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"

class RobotControl : public MaxControl
{
public:
	int i = 7;
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
};