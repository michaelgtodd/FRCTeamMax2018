#pragma once
#include "maxutils/MaxTask.h"
#include "ControlTask.h"

class DrivingTask : public MaxTask
{
public:
	void Run();
	void Disable();
	void Autonomous();
	void ControllerUpdate(MaxControl * controls);
private:
	void Init();
	RobotControl * ControlInput;
};