#include "main.h"
#include "sylib/system.hpp"
#include "sylib/sylib.hpp"
#include "lemlib/api.hpp"
#include "pros/optical.hpp"
#include "lights.hpp"
#include "autons.hpp"

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::Motor RightFront(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor RightMid(2, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor RightBack(3, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);  
    pros::Motor_Group RDrive({RightFront, RightMid, RightBack}); 

pros::Motor LeftFront(4, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor LeftMid(5, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES); 
pros::Motor LeftBack(-6, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);    
    pros::Motor_Group LDrive({LeftFront, LeftMid, LeftBack}); 

pros::Motor intake(7, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);    

bool intaking = false;
//speed of intake
int intakespeed = 127;
int maxauto = 4;
int auton = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////Lemlib stuff//////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Drivetrain config 
lemlib::Drivetrain drivetrain {
	&LDrive, // left motor group
	&RDrive, // right motor group
	14.497386, // 14.5 inch track width
	lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
	400, // drivetrain rpm is 400
	8 // Its 8 because we have traction wheels
};
		
// left tracking using drive motors
lemlib::TrackingWheel left_tracking(
	&LDrive, //Look at the left drive
	3.25, //3.25 inch wheels
	-7.251, //6.2875 incheas left of the center
	400  //Max RPM of 400
);

// right tracking using drive motors
lemlib::TrackingWheel right_tracking(
	&RDrive, //Look at the left drive
	3.25, //3.25 inch wheels
	7.251, //6.2875 incheas right of the center
	400 //Max RPM of 400
);
		
// inertial sensor
pros::Imu inertial(11);
		
// Sensors for odometry 
lemlib::OdomSensors sensors {
	&left_tracking, // vertical tracking wheel 1
	&right_tracking, // vertical tracking wheel 2
	nullptr, // no horizontal tracking
	nullptr, // no second horizontal tracking wheel
	&inertial // inertial sensor
};


// forward/backward PID
lemlib::ControllerSettings lateralController {
	10, // proportional gain (kP)
	0, // integral gain (kI)
	3, // derivative gain (kD)
	3, // anti windup
	1, // small error range, in inches
	100, // small error range timeout, in milliseconds
	3, // large error range, in inches
	500, // large error range timeout, in milliseconds
	20 // maximum acceleration (slew)
};

// turning PID
lemlib::ControllerSettings angularController {
	2, // proportional gain (kP)
	0, // integral gain (kI)
	10, // derivative gain (kD)
	3, // anti windup
	1, // small error range, in degrees
	100, // small error range timeout, in milliseconds
	3, // large error range, in degrees
	500, // large error range timeout, in milliseconds
	0 // maximum acceleration (slew
};

lemlib::Chassis chassis(drivetrain, lateralController, angularController, sensors);

//a funtion withc depending on press will move intake
void intakusmaximus_fn(){
	while (true){
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)){
			delay(300);
			//if button i sstill pressed we make the motor gor revers
			if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
				intake.move(-intakespeed);
			} else {
				//we change it to active or inactive than run it
				intaking = !intaking;
				if (intaking){
					intake.move(intakespeed);
				} else{
					intake.brake();
				}

			}
		}
		delay(20);
	}

}

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

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "code works");

	pros::lcd::register_btn0_cb(on_left_button);
	pros::lcd::register_btn1_cb(on_center_button);
	pros::lcd::register_btn2_cb(on_right_button);

	chassis.calibrate();
	sylib::initialize();
	
	lightsCheck();

	//Calling task they continusly doe sintakus maximus
	Task intakusmaximus(intakusmaximus_fn);
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
		int dir = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
		int turn = -controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
		LDrive.move(dir - turn);
		RDrive.move(dir + turn);


		
		pros::delay(20);

	}
}
