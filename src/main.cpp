#include "main.h"


void initialize() {
	pros::lcd::initialize();
}


void disabled() {}


void competition_initialize() {}


void autonomous() {}


void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup left_drive({1, 2, -3});
	pros::MotorGroup right_drive({4, 5, -6});


	while (true) {
		int dir = master.get_analog(ANALOG_LEFT_Y);
		int turn = master.get_analog(ANALOG_RIGHT_X);
		left_drive.move(dir - turn);
		right_drive.move(dir + turn);
		pros::delay(20);
	}
}