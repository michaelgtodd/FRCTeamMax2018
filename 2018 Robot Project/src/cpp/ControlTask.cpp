#include "ControlTask.h"
#include "maxutils/MaxDataStream.h"
#include "maxutils/MaxControls.h"

ControlTask::ControlTask(MaxTaskSchedule * taskschedule)
{
	taskschedule_ = taskschedule;
}

void ControlTask::Run()
{
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

void ControlTask::ProcessOscData(osc::ReceivedMessage messages)
{
	MaxLog::LogInfo("Hey, got a packet");
}

void ControlTask::Init()
{
	controls.ActiveJoystick = new Joystick(1);
}