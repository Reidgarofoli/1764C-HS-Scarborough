#include "main.h"
#include "sylib/system.hpp"
#include "sylib/sylib.hpp"
#include "lemlib/api.hpp"
#include "pros/optical.hpp"
#include "lights.hpp"
#include "autons.hpp"


void on_center_button() {
	if (team == 'r'){team = 'b';} else {team = 'r';}
	pros::lcd::print(0, "auton:%d  team:%c", auton, team);
	lightsCheck();
}

void on_right_button() {
	if (auton != maxauto){
		auton++;
	} else{
		auton = 0;
	}
	pros::lcd::print(0, "auton:%d  team:%c", auton, team);
}
void on_left_button() {
	if (auton != 0){
		auton--;
	} else{
		auton = maxauto;
	}
	pros::lcd::print(0, "auton:%d  team:%c", auton, team);
}

void midlift(){
	while (true) {
		if (midlifter.get_position() != liftpos){
			midlifter.move_absolute(liftpos, 200);
		} else {
			midlifter.brake();
		}
		delay(20);
	}
}


void ringcheckers(){
	while(true){
		if (intaking){
			if (team == 'b'){
				if (round(optical_sensor.get_hue() / 15) == 0){
					intake.move(-127);

				} else {
					intake.move(127);
				}
			} else {
				if (round(optical_sensor.get_hue() / 15) == round(225 / 15)){
					intake.move(-127);
				} else {
					intake.move(127);
				}
			}
		}
		delay(20);	
	}
}

	


void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "code works");
	pros::lcd::print(0, "auton:%d  team:%c", auton, team);

	pros::lcd::register_btn0_cb(on_left_button);
	pros::lcd::register_btn1_cb(on_center_button);
	pros::lcd::register_btn2_cb(on_right_button);

	chassis.calibrate();
	sylib::initialize();

	Task liftTask(midlift);
	Task ringcheck(ringcheckers);
	lightsCheck();
	midlifter.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	intakeLifter.set_value(intakePOS);

	mogomech.set_value(mogovalue);
	optical_sensor.set_led_pwm(50);
}

void disabled() {
	while (true) {
		cycle();
		pros::delay(20);
	}
}

void competition_initialize() {
	lightsCheck();
}

void autonomous() {
	
	autonselect(auton);
}

void opcontrol() {
	while (true) {
		pros::lcd::print(1, "front motors: %f, %f",  LeftFront.get_temperature(),RightFront.get_temperature());
		pros::lcd::print(2, "middle motors: %f, %f", LeftMid.get_temperature(),RightMid.get_temperature());
		pros::lcd::print(3, "back motors: %f, %f",   LeftBack.get_temperature(),RightBack.get_temperature());
		int dir = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
		int turn = -controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
		LDrive.move(dir - turn);
		RDrive.move(dir + turn);

		if (controller.get_digital(E_CONTROLLER_DIGITAL_R2)) {
			intaking = true;
		} else if (controller.get_digital(E_CONTROLLER_DIGITAL_R1)) {
			intaking = false;
			intake.move(-127);
		} else {
			intaking = false;
			intake.move(0);
		}

		if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_L1)) {
			midliftPOS += 1;
			if (midliftPOS == 3) {
				midliftPOS = 0;
			}
			if (midliftPOS == 0){
				liftpos = lowmid;
			}
			if (midliftPOS == 1){
				liftpos = midmid;
			}
			if (midliftPOS == 2){
				liftpos = highmid;
			}
		}
		if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)) {
			mogovalue =!mogovalue;
			mogomech.set_value(mogovalue);
		}
		
		/*if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)) {
			intakePOS =! intakePOS;
			intakeLifter.set_value(intakePOS);
		}*/
		if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)){
			doinkerValue = !doinkerValue;
			doinker.set_value(doinkerValue);
		}
		pros::delay(20);

	}
}
