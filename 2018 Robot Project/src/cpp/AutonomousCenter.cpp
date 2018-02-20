#include "AutonomousCenter.h"
#include "maxutils\MaxDataStream.h"
#include "ControlTask.h"
#include "iostream"
#include <math.h>

void AutonomousCenter::Init()
{
	MaxLog::LogInfo("Starting Auto");

	std::cout << "Position: Center " << "Auto Init" << std::endl;

	StartTime = Timer::GetFPGATimestamp();
	LastMessage = 0;
}

void AutonomousCenter::ControllerUpdate(MaxControl * controls)
{
	// can get controller data here
}

void AutonomousCenter::Autonomous()
{
	double CurrentTime = Timer::GetFPGATimestamp();
	RunTime = CurrentTime - StartTime;
	if (floor(RunTime) > LastMessage)
	{
		LastMessage = floor(RunTime);
		std::cout << "Running at " << std::to_string(LastMessage) << " seconds" << std::endl;
		MaxLog::LogInfo("Running at " + std::to_string(LastMessage) + " seconds");
	}

	// Send data to the control task to send to other tasks]
	ControlTaskInstance.UpdateAutonomousData(control);
}

void AutonomousCenter::End()
{
	double CurrentTime = Timer::GetFPGATimestamp();
	RunTime = CurrentTime - StartTime;
	LastMessage = floor(RunTime);
	MaxLog::LogInfo("Ending Auto after " + std::to_string(LastMessage) + " seconds");
}

std::string AutonomousCenter::GetName()
{
	return "AutonomousCenter";
}
