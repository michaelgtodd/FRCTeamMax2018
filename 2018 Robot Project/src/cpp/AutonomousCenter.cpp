#define _USE_MATH_DEFINES
#include "AutonomousCenter.h"
#include "maxutils\MaxDataStream.h"
#include "ControlTask.h"
#include "iostream"
#include <math.h>

void AutonomousCenter::Init()
{
	MaxLog::LogInfo("Starting Center Auto");

	/*Get field data*/
	std::string gameData;
	gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
	if (gameData.length() >= 3)
	{
		SwitchPosition = gameData[0] == 'L' ? FieldPos::Left : FieldPos::Right;
		ScalePosition = gameData[1] == 'L' ? FieldPos::Left : FieldPos::Right;
		FarSwitchPosition = gameData[2] == 'L' ? FieldPos::Left : FieldPos::Right;
	}

	/*Print field data to dashboard*/
	std::cout << "Position: Center | Switch Priority: " << SwitchPriorityInput << std::endl;
	std::cout << "Game String: " << gameData << std::endl;
	std::cout << "Switch: " << SwitchPosition << " Scale: " << ScalePosition << " Far Switch: " << FarSwitchPosition << std::endl;

	/*Define Talons for sensor input*/
	AutoMotorLift = new TalonSRX(12);
	DummyTalon = new TalonSRX(14);
	PigeonInput = new PigeonIMU(DummyTalon);

	/*Reset*/
	StartTime = Timer::GetFPGATimestamp();
	Brake();
	ResetSensor();
}

void AutonomousCenter::ControllerUpdate(MaxControl * controls)
{
	SwitchPriorityInput = ((RobotControl *)controls)->SwitchPrioritySelection;
}

void AutonomousCenter::Autonomous()
{
	/*Update variables*/
	CurrentTime = Timer::GetFPGATimestamp();
	PigeonInput->GetYawPitchRoll(YPR);
	Yaw = InitialYaw - YPR[0];

	/*Print data to dashboard*/
	runs++;
	if (runs >= 20)
	{
		std::cout << "Yaw: " << Yaw << std::endl;
		std::cout << "Speed: " << LeftSpeed << " | " << RightSpeed << std::endl;
		runs = 0;
	}

	if (SwitchPriorityInput == SwitchPriority::No)
	{
		/*Cross auto line*/
		switch (stage)
		{
		case 0:
			/*Clamp cube*/
			control.ArmPositionLeft = 40;
			control.ArmPositionRight = 320;
			Brake();
			ResetSensor();
			std::cout << "Stage 0 completed." << std::endl;
			stage++;
			break;
		case 1:
			/*Drive forward*/
			Drive(0.75, 1);
			if (TimePassed(7.5)) //If 7.5 seconds passed
			{
				Brake();
				ResetSensor();
				std::cout << " Stage 1 completed." << std::endl;
				stage++;
			}
			break;
		default:
			break;
		}
	}
	else if (SwitchPriorityInput == SwitchPriority::Yes)
	{
		/*Cross auto line*/
		switch (stage)
		{
		case 0:
			/*Clamp cube*/
			control.ArmPositionLeft = 40;
			control.ArmPositionRight = 320;
			Brake();
			ResetSensor();
			std::cout << "Stage 0 completed." << std::endl;
			stage++;
			break;
		case 1:
			/*Drive forward*/
		default:
			break;
		}
	}

	/*Send data to Driving and Lifting Task*/
	ControlTaskInstance.UpdateAutonomousData(control);
}

void AutonomousCenter::End()
{
	double CurrentTime = Timer::GetFPGATimestamp();
	RunTime = CurrentTime - StartTime;
	LastMessage = floor(RunTime);
	MaxLog::LogInfo("Ending Auto after " + std::to_string(LastMessage) + " seconds");
}

void AutonomousCenter::Drive2(double IdealSpeed)
{
	const double YawGain = 0.01;
	const double MaxYawContribution = 0.3;

	double YawCorrection = YawGain * Yaw;

	YawCorrection = fmax(fmin(MaxYawContribution, YawGain), -MaxYawContribution);

	control.SpeedRight = PigeonEnable == false ? IdealSpeed : IdealSpeed + YawCorrection;
	control.SpeedLeft = PigeonEnable == false ? IdealSpeed : IdealSpeed - YawCorrection;

	if (runs == 0)
	{
		std::cout << "YawCorrection: " << YawCorrection << std::endl;
	}
}

void AutonomousCenter::Drive(double SpeedMax, double SpeedMin)
{
	/*Ensure all values are non-zero*/
	LeftSpeed = LeftSpeed == 0 ? SpeedMax * 0.75 : LeftSpeed;
	RightSpeed = RightSpeed == 0 ? SpeedMax * 0.75 : RightSpeed;
	LeftSpeed = fmax(fmin(LeftSpeed, SpeedMax), SpeedMin);
	RightSpeed = fmax(fmin(RightSpeed, SpeedMax), SpeedMin);

	/*Determine which speed needs to be increased*/
	if (Yaw < -TurnTolerance) //Tilting to the left
	{
		if (RightSpeed == SpeedMin) //Left speed needs to be increased
		{
			LeftSpeed = LeftSpeed * (1 + TurnAdjustment);
		}
		else if (LeftSpeed == SpeedMax) //Right speed needs to be decreased
		{
			RightSpeed = RightSpeed * (1 - TurnAdjustment);
		}
		else //Neither speed is significantly larger
		{
			LeftSpeed = LeftSpeed * (1 + TurnAdjustment);
			RightSpeed = RightSpeed * (1 - TurnAdjustment);
		}
	}
	else if (Yaw > TurnTolerance) //Tilting to the right
	{
		if (LeftSpeed == SpeedMax) //Left speed needs to be decreased
		{
			LeftSpeed = LeftSpeed * (1 - TurnAdjustment);
		}
		else if (RightSpeed == SpeedMin) //Right speed needs to be increased
		{
			RightSpeed = RightSpeed * (1 + TurnAdjustment);
		}
		else //Neither speed is significantly larger
		{
			LeftSpeed = LeftSpeed * (1 - TurnAdjustment);
			RightSpeed = RightSpeed * (1 + TurnAdjustment);
		}
	}

	/*Set speed*/
	LeftSpeed = fmax(fmin(LeftSpeed, SpeedMax), SpeedMin);
	RightSpeed = fmax(fmin(RightSpeed, SpeedMax), SpeedMin);
	control.SpeedLeft = PigeonEnable == false ? SpeedMax : LeftSpeed;
	control.SpeedRight = PigeonEnable == false ? SpeedMax : -RightSpeed;
}

bool AutonomousCenter::Turn(double Degrees, double SpeedLimit, double Tolerance)
{

}

bool AutonomousCenter::Lift(double Height, double SpeedLimit, double Tolerance)
{

}

bool AutonomousCenter::TimePassed(double Time)
{
	if (CurrentTime - StartTime > Time)
		return true;
	else
		return false;
}

void AutonomousCenter::Brake()
{
	control.SpeedRight = 0;
	control.SpeedLeft = 0;
	LeftSpeed = 0;
	RightSpeed = 0;
}

void AutonomousCenter::ResetSensor()
{
	//PigeonInput->SetYaw(0, 5);
	PigeonInput->GetYawPitchRoll(YPR);
	InitialYaw = YPR[0];
}

std::string AutonomousCenter::GetName()
{
	return "AutonomousCenter";
}