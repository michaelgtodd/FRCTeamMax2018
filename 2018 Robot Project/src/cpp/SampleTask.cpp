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

void SampleTask::Autonomous()
{

}

void SampleTask::ControllerUpdate(MaxControl controls)
{
	//MaxLog::LogInfo("here");
	//MaxControl * test = &controls;
	//RobotControl * controls_ = dynamic_cast<RobotControl *>(test);
	//std::string test2 = "test? :" + std::to_string(controls_->i);
	//MaxLog::LogInfo(test2);
}

void SampleTask::Init()
{

}