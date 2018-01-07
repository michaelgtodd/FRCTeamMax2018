#pragma once
#include "MaxTask.h"

class SampleTask : public MaxTask
{
public:
	void Run();
	void Disable();
	void ControllerUpdate(MaxControls controls);
private:
	void Init();
};