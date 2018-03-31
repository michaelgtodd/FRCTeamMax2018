#define _USE_MATH_DEFINES
#include "AutonomousLeft.h"
#include "maxutils\MaxDataStream.h"
#include "ControlTask.h"
#include "iostream"
#include <math.h>

void AutonomousLeft::Init()
{
	MaxLog::LogInfo("Starting Left Auto");

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
	std::cout << "Position: Left | Switch Priority: " << SwitchPriorityInput << std::endl;
	std::cout << "Game String: " << gameData << std::endl;
	std::cout << "Switch: " << SwitchPosition << " Scale: " << ScalePosition << " Far Switch: " << FarSwitchPosition << std::endl;

	/*Define Talons for sensor input*/
	AutoMotorLift = new TalonSRX(12);
	DummyTalon = new TalonSRX(14);
	PigeonInput = new PigeonIMU(DummyTalon);

	/*Reset*/
	stage = 0;
	StartTime = Timer::GetFPGATimestamp();
	Brake();
	ResetSensor();
}

void AutonomousLeft::ControllerUpdate(MaxControl * controls)
{
	SwitchPriorityInput = ((RobotControl *)controls)->SwitchPrioritySelection;
}

void AutonomousLeft::Autonomous()
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

	if (SwitchPosition == FieldPos::Left)
	{

		/*Cross auto line and drop a cube*/
		switch (stage)
		{
		case 0:
			/*Drop cube*/
			control.ArmPositionLeft = 180;
			control.ArmPositionRight = 180;
			Lift(5000, 0.25);
			Drive(0.25, 0.1);

			if (TimePassed(1.5)) //If 1.5 seconds passed
			{
				Brake();
				ResetSensor();
				std::cout << " Stage 0 completed." << std::endl;
				stage++;
			}

			std::cout << " Stage 0 Boys " << std::endl;
			break;
		case 1:
			/*Move the lift down*/
			Lift(-14000, -0.1);

			if (TimePassed(3)) //If 3 second passed
			{
				Brake();
				ResetSensor();
				std::cout << " Stage 1 completed." << std::endl;
				stage++;
			}
			break;
		case 2:
			/*Drive forward and lift arm*/
			control.ArmPositionLeft = 40;
			control.ArmPositionRight = 320;
			Drive(0.75, 0.5);
			Lift(70000, 0.5);

			if (TimePassed(10)) //If 7 seconds passed
			{
				Brake();
				ResetSensor();
				std::cout << " Stage 1 completed." << std::endl;
				stage++;
			}
			break;
		case 3:
			/*Release the cube*/
			control.ArmPositionLeft = 180;
			control.ArmPositionRight = 180;
			break;
		default:
			break;
		}
	}
	else
	{
		/*Drive forward*/
		switch (stage)
		{
		case 0:
			/*Move*/
			Drive(0.75, 0.5);

			if (TimePassed(7)) //If 7 seconds passed
			{
				Brake();
				ResetSensor();
				std::cout << " Stage 0 completed." << std::endl;
				stage++;
			}
		default:
			break;
		}
	}
	
	/*Send data to Driving and Lifting Task*/
	ControlTaskInstance.UpdateAutonomousData(control);
}

void AutonomousLeft::End()
{
	double CurrentTime = Timer::GetFPGATimestamp();
	RunTime = CurrentTime - StartTime;
	LastMessage = floor(RunTime);
	MaxLog::LogInfo("Ending Auto after " + std::to_string(LastMessage) + " seconds");
}

void AutonomousLeft::Drive2(double IdealSpeed)
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

void AutonomousLeft::Drive(double SpeedMax, double SpeedMin)
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

bool AutonomousLeft::Turn(double Degrees, double SpeedLimit, double Tolerance)
{
	if (Degrees - Yaw > Tolerance)
	{

	}
}

void AutonomousLeft::Lift(int MaxHeight, double Speed)
{
	/*Determine lift height*/
	int LiftHeight = AutoMotorLift->GetSensorCollection().GetQuadraturePosition();

	/*Set speed*/
	control.SpeedLift = LiftHeight - MaxHeight > 250 ? 0 : Speed;
}

bool AutonomousLeft::TimePassed(double Time)
{
	if (CurrentTime - StartTime > Time)
		return true;
	else
		return false;
}

void AutonomousLeft::Brake()
{
	control.SpeedRight = 0;
	control.SpeedLeft = 0;
	control.SpeedLift = 0;
	LeftSpeed = 0;
	RightSpeed = 0;
}

void AutonomousLeft::ResetSensor()
{
	//PigeonInput->SetYaw(0, 5);
	PigeonInput->GetYawPitchRoll(YPR);
	InitialYaw = YPR[0];
}

std::string AutonomousLeft::GetName()
{
	return "AutonomousLeft";
}