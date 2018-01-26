#pragma once
#include "WPILib.h"
#include "SampleTask.h"
#include "ControlTask.h"
#include "DrivingTask.h"
#include "DashboardTask.h"

class Robot : public frc::SampleRobot 
{
public:
	void RobotInit();
	void Autonomous();
	void OperatorControl();
	void Test();
};