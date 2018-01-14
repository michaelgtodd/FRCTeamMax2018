#pragma once
#include "maxutils/MaxTask.h"
#include "ControlTask.h"

class DashboardTask : public MaxTask
{
public:
	void Run();
	void Disable();
	void Autonomous();
	void ControllerUpdate(MaxControl * controls);
private:
	RobotControl * Control;
	void Init();
};