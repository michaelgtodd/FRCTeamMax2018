#pragma once
#include "maxutils\MaxAutonomous.h"

class SampleAutonomous : public MaxAutonomousTask
{
public:
	void Init();
	void ControllerUpdate(MaxControl * controls);
	void Autonomous();
	void End();
	std::string GetName();
private:
	double StartTime = 0;
	double RunTime = 0;
	int LastMessage = 0;
};