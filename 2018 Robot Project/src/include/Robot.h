#pragma once
#include "WPILib.h"

class Robot : public frc::SampleRobot 
{
public:
	void RobotInit();
	void Autonomous();
	void OperatorControl();
	void Test();
};