#pragma once
#include <string>
#include "MaxControls.h"
#include <thread>
#include <vector>

class MaxTask 
{
public:
	void virtual Run() = 0;
	void virtual Disable() = 0;
	void virtual ControllerUpdate(MaxControls controls) = 0;
	void virtual Autonomous() = 0;

	std::string GetTaskName();
	uint32_t GetTaskPeriod();
	uint32_t GetAverageTaskPeriod();
	uint32_t GetAverageTaskDuration();

	void ExecInit(std::string taskname, uint32_t task_period);
	void Launch();
private:
	void virtual Init() = 0;
	void ThreadProcess();
	uint32_t task_period_;
	std::string taskname_;
	std::thread running_thread;
	double last_loop_end = 0;
	double average_task_differential = 0;
	double average_task_period = 0;
	double average_task_duration = 0;
	double max_task_duration = 0;
	double max_task_period = 0;
};

class MaxTaskSchedule
{
public:
	void AddTask(MaxTask* task, std::string taskname, uint32_t period);
	void LaunchTasks();
private:
	std::vector<MaxTask*> TaskList;
};

class MaxTaskStatisticsTask : public MaxTask
{
public:
	MaxTaskStatisticsTask(std::vector<MaxTask*> TaskList);
	void Run();
	void Disable();
	void Autonomous();
	void ControllerUpdate(MaxControls controls);
private:
	void Init();
	std::vector<MaxTask*> TaskList_;
};