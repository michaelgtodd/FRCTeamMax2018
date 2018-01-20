#include "DrivingTask.h"
#include <iostream>
#include "maxutils/MaxDataStream.h"
#include "ControlTask.h"

void DrivingTask::Run()
{

}

void DrivingTask::Disable()
{

}

void DrivingTask::Autonomous()
{

}

void DrivingTask::ControllerUpdate(MaxControl * controls)
{
	RobotControl * controls_ = (RobotControl *)(controls);
}

void DrivingTask::Init()
{

}