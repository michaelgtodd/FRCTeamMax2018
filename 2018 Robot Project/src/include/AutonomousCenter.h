#pragma once
#include "maxutils\MaxAutonomous.h"
#include "ControlTask.h"

class AutonomousCenter : public MaxAutonomousTask
{
public:
	void Init();
	void ControllerUpdate(MaxControl * controls);
	void Autonomous();
	void End();
	std::string GetName();
private:
	AutonomousControl control;
	double StartTime = 0;
	double RunTime = 0;
	int LastMessage = 0;
};