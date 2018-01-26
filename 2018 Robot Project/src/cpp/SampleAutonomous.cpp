#include "SampleAutonomous.h"
#include "maxutils\MaxDataStream.h"
#include "ControlTask.h"
#include <math.h>

void SampleAutonomous::Init()
{
	MaxLog::LogInfo("Starting Auto");

	StartTime = Timer::GetFPGATimestamp();
	LastMessage = 0;
}

void SampleAutonomous::ControllerUpdate(MaxControl * controls)
{
	// can get controller data here
}

void SampleAutonomous::Autonomous()
{
	double CurrentTime = Timer::GetFPGATimestamp();
	RunTime = CurrentTime - StartTime;
	if (floor(RunTime) > LastMessage)
	{
		LastMessage = floor(RunTime);
		MaxLog::LogInfo("Running at " + std::to_string(LastMessage) + " seconds");
	}

	// Send data to the control task to send to other tasks
	AutonomousControl control;
	ControlTaskInstance.UpdateAutonomousData(control);
}

void SampleAutonomous::End()
{
	double CurrentTime = Timer::GetFPGATimestamp();
	RunTime = CurrentTime - StartTime;
	LastMessage = floor(RunTime);
	MaxLog::LogInfo("Ending Auto after " + std::to_string(LastMessage) + " seconds");
}

std::string SampleAutonomous::GetName()
{
	return "Sample_Auto";
}
