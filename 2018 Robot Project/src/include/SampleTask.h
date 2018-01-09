#pragma once
#include "maxutils/MaxTask.h"

class SampleTask : public MaxTask
{
public:
	void Run();
	void Disable();
	void Autonomous();
	void ControllerUpdate(MaxControls controls);
private:
	void Init();
};