#include "PowerReportingTask.h"
#include <iostream>
#include "maxutils/MaxDataStream.h"
#include "ControlTask.h"
#include "maxutils/MaxDataStream.h"

void PowerReportingTask::Run()
{

}

void PowerReportingTask::Disable()
{

}

void PowerReportingTask::Always()
{
	for (int i = 0; i < 16; i++)
	{
		MaxLog::TransmitDouble("/power/" + std::to_string(i), PDP->GetCurrent(0));
	}
	MaxLog::TransmitDouble("/Power/Total", PDP->GetTotalCurrent());
}

void PowerReportingTask::Autonomous()
{

}

void PowerReportingTask::ControllerUpdate(MaxControl * controls)
{

}

void PowerReportingTask::Init()
{
	PDP = new PowerDistributionPanel();
}