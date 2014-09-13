#include "WPILib.h"

class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot;
	Joystick stick;

public:
	RobotDemo():
		myRobot(1, 2),
		stick(1)
	{
		myRobot.SetExpiration(0.1);
	}

	void Autonomous() {
		AxisCamera &camera = AxisCamera::GetInstance();
	}

	void OperatorControl() {
		// Operator control is for dummies.
	}

	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

