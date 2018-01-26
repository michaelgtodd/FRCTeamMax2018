#include "Robot.h"
#include "maxutils/MaxTask.h"
#include "maxutils/MaxDataStream.h"
#include "maxutils/MaxAutonomous.h"
#include "SampleAutonomous.h"

void Robot::RobotInit() 
{
	MaxLog::InitializeMaxLog();

	MaxAutonomousManagerInstance.RegisterAutonomous(new SampleAutonomous);

	// Task names cannot contain spaces at this time
	taskschedule.AddTask((MaxTask*)&ControlTaskInstance, "ControlTask", 100);
	taskschedule.AddTask((MaxTask*)&MaxAutonomousManagerInstance, "AutoManager", 100);
	taskschedule.AddTask(new DrivingTask, "DrivingTask", 100);
	taskschedule.AddTask(new SampleTask, "Task1", 10);
	taskschedule.AddTask(new SampleTask, "Task2", 1);
	taskschedule.AddTask(new SampleTask, "Task3", 100);
	taskschedule.AddTask(new SampleTask, "Task4", 100);
	taskschedule.AddTask(new DashboardTask, "Dash_Task", 20);
	taskschedule.AddTask(new MaxLog::MaxCautionManager(), "Caution_Manager", 1);

	taskschedule.LaunchTasks();
}

void Robot::Autonomous() { }
void Robot::OperatorControl() { }
void Robot::Test() { }

START_ROBOT_CLASS(Robot)