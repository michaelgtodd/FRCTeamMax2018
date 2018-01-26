#include "SampleTask.h"
#include <iostream>
#include "maxutils/MaxDataStream.h"
#include "ControlTask.h"

void SampleTask::Run()
{

}

void SampleTask::Disable()
{

}

void SampleTask::Always()
{

}

void SampleTask::Autonomous()
{

}

void SampleTask::ControllerUpdate(MaxControl * controls)
{
	RobotControl * controls_ = (RobotControl *)(controls);
}

void SampleTask::Init()
{

}