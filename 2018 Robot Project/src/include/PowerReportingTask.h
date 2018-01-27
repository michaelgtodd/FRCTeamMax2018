#pragma once
#include "maxutils/MaxTask.h"
#include "ctre/Phoenix.h"
#include "WPILib.h"

class PowerReportingTask : public MaxTask
{
public:
	void Always();
	void Run();
	void Disable();
	void Autonomous();
	void ControllerUpdate(MaxControl * controls);
private:
	void Init();
	frc::PowerDistributionPanel * PDP;
};