#include "ControlTask.h"
#include "maxutils/MaxDataStream.h"
#include "maxutils/MaxControls.h"

ControlTask::ControlTask(MaxTaskSchedule * taskschedule)
{
	taskschedule_ = taskschedule;
}

void ControlTask::Run()
{
	RobotControl controls;
	taskschedule_->DispatchControl(&controls);
}

void ControlTask::Disable()
{

}

void ControlTask::ControllerUpdate(MaxControl * controls)
{

}

void ControlTask::Autonomous()
{

}

void ControlTask::Init()
{

}