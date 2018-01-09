#include "Robot.h"
#include "maxutils/MaxTask.h"
#include "maxutils/MaxDataStream.h"

#include "SampleTask.h"
#include "ControlTask.h"

void Robot::RobotInit() 
{
	MaxTaskSchedule * taskschedule = new MaxTaskSchedule();
	
	// Task names cannot contain spaces at this time
	taskschedule->AddTask(new SampleTask, "Task1", 10);
	taskschedule->AddTask(new SampleTask, "Task2", 1);
	taskschedule->AddTask(new SampleTask, "Task3", 100);
	taskschedule->AddTask(new SampleTask, "Task4", 100);
	taskschedule->AddTask(new ControlTask(taskschedule), "ControlTask", 100);
	taskschedule->AddTask(new MaxLog::MaxCautionManager(), "Caution_Manager", 1);

	taskschedule->LaunchTasks();
}

void Robot::Autonomous() { }
void Robot::OperatorControl() { }
void Robot::Test() { }

START_ROBOT_CLASS(Robot)