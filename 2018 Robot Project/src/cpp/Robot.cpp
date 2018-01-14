#include "Robot.h"
#include "maxutils/MaxTask.h"
#include "maxutils/MaxDataStream.h"

ControlTask * ControllerTask;

ControlTask * Robot::GetControllerTask()
{
	return ControllerTask;
}

void Robot::RobotInit() 
{
	MaxLog::InitializeMaxLog();

	MaxTaskSchedule * taskschedule = new MaxTaskSchedule();

	ControllerTask = new ControlTask(taskschedule);
	
	// Task names cannot contain spaces at this time
	taskschedule->AddTask(ControllerTask, "ControlTask", 100);
	taskschedule->AddTask(new SampleTask, "Task1", 10);
	taskschedule->AddTask(new SampleTask, "Task2", 1);
	taskschedule->AddTask(new SampleTask, "Task3", 100);
	taskschedule->AddTask(new SampleTask, "Task4", 100);
	taskschedule->AddTask(new MaxLog::MaxCautionManager(), "Caution_Manager", 1);

	taskschedule->LaunchTasks();
}

void Robot::Autonomous() { }
void Robot::OperatorControl() { }
void Robot::Test() { }

START_ROBOT_CLASS(Robot)