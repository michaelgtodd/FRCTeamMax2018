#include "AutonomousLeft.h"
#include "maxutils\MaxDataStream.h"
#include "ControlTask.h"
#include "iostream"
#include <math.h>

void AutonomousLeft::Init()
{
	MaxLog::LogInfo("Starting Auto");

	std::cout << "Position: Left " << "Auto Init" << std::endl;

	StartTime = Timer::GetFPGATimestamp();
	LastMessage = 0;
}

void AutonomousLeft::ControllerUpdate(MaxControl * controls)
{
	// can get controller data here
}

void AutonomousLeft::Autonomous()
{
	double CurrentTime = Timer::GetFPGATimestamp();
	RunTime = CurrentTime - StartTime;
	if (floor(RunTime) > LastMessage)
	{
		LastMessage = floor(RunTime);
		std::cout << "Running at " << std::to_string(LastMessage) << " seconds" << std::endl;
		MaxLog::LogInfo("Running at " + std::to_string(LastMessage) + " seconds");
	}

	// Send data to the control task to send to other tasks
	AutonomousControl control;
	ControlTaskInstance.UpdateAutonomousData(control);
}

void AutonomousLeft::End()
{
	double CurrentTime = Timer::GetFPGATimestamp();
	RunTime = CurrentTime - StartTime;
	LastMessage = floor(RunTime);
	MaxLog::LogInfo("Ending Auto after " + std::to_string(LastMessage) + " seconds");
}

std::string AutonomousLeft::GetName()
{
	return "AutonomousLeft";
}
