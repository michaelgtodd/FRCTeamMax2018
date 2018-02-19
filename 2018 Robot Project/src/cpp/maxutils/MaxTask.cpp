#include "maxutils/MaxTask.h"
#include "WPILib.h"
#include "pthread.h"
#include "maxutils/MaxDataStream.h"
#include <math.h>
#include <iostream>
#include "Robot.h"
#include "RobotState.h"

MaxTaskStatisticsTask::MaxTaskStatisticsTask(std::vector<MaxTask*> TaskList)
{
	TaskList_ = TaskList;
}

void MaxTaskStatisticsTask::Always()
{
	int j = 1;
	for (std::vector<MaxTask*>::iterator i = TaskList_.begin();
		i != TaskList_.end();
		i++)
	{
		std::string baselabel = "/TaskStats/" + std::to_string(j);
		MaxLog::TransmitString(baselabel + "/Name", (*i)->GetTaskName());
		MaxLog::TransmitInt(baselabel + "/Period", (*i)->GetAverageTaskPeriod());
		MaxLog::TransmitInt(baselabel + "/Duration", (*i)->GetAverageTaskDuration());
		j++;
	}
	std::string baselabel = "/TaskStats/" + std::to_string(j);
	MaxLog::TransmitString(baselabel + "/Name", "TaskStatsTask");
	MaxLog::TransmitInt(baselabel + "/Period", GetAverageTaskPeriod());
	MaxLog::TransmitInt(baselabel + "/Duration", GetAverageTaskDuration());
}

void MaxTaskStatisticsTask::Run()
{

}

void MaxTaskStatisticsTask::Disable()
{

}

void MaxTaskStatisticsTask::Autonomous()
{

}

void MaxTaskStatisticsTask::ControllerUpdate(MaxControl * controls)
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
	int priority = 99;

	sched_param sch;
	sch.sched_priority = priority;

	if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sch) != 0)
	{
		std::cout << "Failed to set task: " << "DS_Task" << " priority: " << priority << " Error: " << " " << strerror(errno) << std::endl;
		MaxLog::LogError("Failed to set task: DS_Task priority: " + std::to_string(priority) + " Error: " + strerror(errno));
	}
	else {
		std::cout << "Set priority for task: " << "DS_Task" << " priority: " << priority << std::endl;
		MaxLog::LogInfo("Set priority for task: DS_Task priority: " + std::to_string(priority));
	}

	MaxLog::LogInfo("Starting tasks");
	priority = 80;
	for (std::vector<MaxTask*>::iterator i = TaskList.begin();
		i != TaskList.end();
		i++)
	{
		(*i)->Launch(priority);
		priority--;
	}

	MaxTask * stats_task = new MaxTaskStatisticsTask(TaskList);
	stats_task->ExecInit("StatsTask", 1);
	stats_task->Launch(98);
}

void MaxTaskSchedule::DispatchControl(MaxControl * ControlUpdate)
{
	for (std::vector<MaxTask*>::iterator i = TaskList.begin();
		i != TaskList.end();
		i++)
	{
		(*i)->ControllerUpdate(ControlUpdate);
	}
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
	uint32_t capped_period = std::min(task_period, (uint32_t) 200);
	capped_period = std::max((uint32_t) 1, capped_period);
	task_period_ = capped_period;

	Init();
}

void MaxTask::Launch(int priority)
{
	MaxLog::LogInfo("Launching: " + taskname_);
	running_thread = std::thread(&MaxTask::ThreadProcess, this);

	sched_param sch;
	sch.sched_priority = priority;

	if (pthread_setschedparam(running_thread.native_handle(), SCHED_FIFO, &sch) != 0)
	{
		std::cout << "Failed to set task: " << taskname_ << " priority: " << priority << " Error: " << " " << strerror(errno) << std::endl;
		MaxLog::LogError("Failed to set task: " + taskname_ + " priority: " + std::to_string(priority) + " Error: " + strerror(errno));
	}
	else {
		std::cout << "Set priority for task: " << taskname_ << " priority: " << priority << std::endl;
		MaxLog::LogInfo("Set priority for task: " + taskname_ + " priority: " + std::to_string(priority));
	}
}

void MaxTask::ThreadProcess()
{
	while (true)
	{
		double loopStart = Timer::GetFPGATimestamp();
		
		if (RobotState::IsOperatorControl() && RobotState::IsEnabled())
		{
			Run();
		}
		else if (RobotState::IsAutonomous() && RobotState::IsEnabled())
		{
			Autonomous();
		}
		else
		{
			Disable();
		}
		Always();

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

		bool slept = false;
		do {
			double currentTime = Timer::GetFPGATimestamp();
			uint32_t loopElapsedTimeMS = (uint32_t)((currentTime - loopStart) * 1000);
			if (loopElapsedTimeMS < (1000 / task_period_))
			{
				slept = true;
				uint32_t minsleep = std::min(((uint32_t)1000 / task_period_) - loopElapsedTimeMS, (uint32_t) 5);
				std::this_thread::sleep_for(std::chrono::milliseconds(minsleep));
			}
			else
			{
				if(!slept)
					std::this_thread::sleep_for(std::chrono::milliseconds(5));
				break;
			}
		} while (true);
	}
}