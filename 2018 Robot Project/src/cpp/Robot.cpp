#include "Robot.h"
#include "MaxTask.h"
#include "SampleTask.h"

void Robot::RobotInit() 
{
	MaxTaskSchedule taskschedule;
	
	taskschedule.AddTask(new SampleTask, "Task1", 10);
	taskschedule.AddTask(new SampleTask, "Task2", 1);
	taskschedule.AddTask(new SampleTask, "Task3", 100);
	taskschedule.AddTask(new SampleTask, "Task4", 100);

	taskschedule.LaunchTasks();
}

void Robot::Autonomous() { }
void Robot::OperatorControl() { }
void Robot::Test() { }

START_ROBOT_CLASS(Robot)