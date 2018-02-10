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

double AveragePowerValues[16];

void PowerReportingTask::Always()
{
	for (int i = 0; i < 16; i++)
	{
		AveragePowerValues[i] = (((AveragePowerValues[i] * 20.0) - AveragePowerValues[i]) + PDP->GetCurrent(i)) / 20.0;
	}

	for (int i = 0; i < 16; i++)
	{
		MaxLog::TransmitDouble("/power/" + std::to_string(i), AveragePowerValues[i]);
	}

	MaxLog::TransmitDouble("/Power/Total/", PDP->GetTotalCurrent());
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
	for (int i = 0; i < 16; i++)
	{
		AveragePowerValues[i] = 0;
	}
}