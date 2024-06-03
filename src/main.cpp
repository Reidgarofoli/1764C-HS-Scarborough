
#include "main.h"
#include "sylib/system.hpp"
#include "sylib/sylib.hpp"
#include "lemlib/api.hpp"
#include "lemlib/asset.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/odom.hpp"
#include "lemlib/chassis/trackingWheel.hpp"


void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "code works");
}


void disabled() {}


void competition_initialize() {}


void autonomous() {}


void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup left_drive({1, 2, -3});
	pros::MotorGroup right_drive({4, 5, -6});


	while (true) {
		int dir = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
		int turn = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
		left_drive.move(dir - turn);
		right_drive.move(dir + turn);
		pros::delay(20);
	}
}