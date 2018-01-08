#include "MaxTask.h"
#include "WPILib.h"
#include "MaxDataStream.h"
#include <math.h>

MaxTaskStatisticsTask::MaxTaskStatisticsTask(std::vector<MaxTask*> TaskList)
{
	TaskList_ = TaskList;
}

void MaxTaskStatisticsTask::Run()
{
	for (std::vector<MaxTask*>::iterator i = TaskList_.begin();
		i != TaskList_.end();
		i++)
	{
		std::string baselabel = "/taskstats/" + (*i)->GetTaskName();
		MaxLog::TransmitInt(baselabel + "/period", (*i)->GetAverageTaskPeriod());
		MaxLog::TransmitInt(baselabel + "/duration", (*i)->GetAverageTaskDuration());
	}
	std::string baselabel = "/taskstats/Task_Stats_Task";
	MaxLog::TransmitInt(baselabel + "/period", GetAverageTaskPeriod());
	MaxLog::TransmitInt(baselabel + "/duration", GetAverageTaskDuration());
}

void MaxTaskStatisticsTask::Disable()
{
	Run();
}

void MaxTaskStatisticsTask::Autonomous()
{
	Run();
}

void MaxTaskStatisticsTask::ControllerUpdate(MaxControls controls)
{

}

void MaxTaskStatisticsTask::Init()
{

}

void MaxTaskSchedule::AddTask(MaxTask* task, std::string taskname, uint32_t period)
{
	TaskList.push_back(task);
	task->ExecInit(taskname, period);
}

void MaxTaskSchedule::LaunchTasks()
{
	MaxLog::LogInfo("Starting tasks");
	for (std::vector<MaxTask*>::iterator i = TaskList.begin();
		i != TaskList.end();
		i++)
	{
		(*i)->Launch();
	}

	MaxTask * stats_task = new MaxTaskStatisticsTask(TaskList);
	stats_task->ExecInit("StatsTask", 1);
	stats_task->Launch();
}

std::string MaxTask::GetTaskName()
{
	return taskname_;
}

uint32_t MaxTask::GetTaskPeriod()
{
	return task_period_;
}

uint32_t MaxTask::GetAverageTaskPeriod()
{
	return (uint32_t)ceil(average_task_period);
}

uint32_t MaxTask::GetAverageTaskDuration()
{
	return (uint32_t)ceil(average_task_duration);
}

void MaxTask::ExecInit(std::string taskname, uint32_t task_period)
{
	taskname_ = taskname;
	task_period_ = task_period;

	Init();
}

void MaxTask::Launch()
{
	MaxLog::LogInfo("Launching: " + taskname_);
	running_thread = std::thread(&MaxTask::ThreadProcess, this);
}

void MaxTask::ThreadProcess()
{
	while (true)
	{
		double loopStart = Timer::GetFPGATimestamp();

		Run();

		double loopEnd = Timer::GetFPGATimestamp();
		double loopDuration = loopEnd - loopStart;
		uint32_t loopExecutionTimeMS = (uint32_t)(loopDuration * 1000);
		if (task_period_ > 1)
		{
			average_task_duration =
				((average_task_duration * task_period_) - average_task_duration + loopExecutionTimeMS) / task_period_;
		}
		else
		{
			average_task_duration = loopExecutionTimeMS;
		}
		double current_loop_difference = (loopEnd - last_loop_end) * 1000;
		last_loop_end = loopEnd;

		if (task_period_ > 1)
		{
			average_task_differential =
				((average_task_differential * task_period_) - average_task_differential + current_loop_difference) / task_period_;
		}
		else
		{
			average_task_differential = current_loop_difference;
		}
		average_task_period = (1000 / average_task_differential);

		bool threadadvance = false;
		do {
			double currentTime = Timer::GetFPGATimestamp();
			uint32_t loopElapsedTimeMS = (uint32_t)((currentTime - loopStart) * 1000);
			if (loopElapsedTimeMS < (1000 / task_period_))
				std::this_thread::sleep_for(std::chrono::milliseconds(1000 / task_period_));
			else
				break;
		} while (true);
	}
}