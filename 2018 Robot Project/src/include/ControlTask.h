#pragma once
#include "maxutils/MaxTask.h"

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
private:
	void Init();
	MaxTaskSchedule * taskschedule_;
};