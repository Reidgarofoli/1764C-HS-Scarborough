#include "main.h"
#include "sylib/system.hpp"
#include "sylib/sylib.hpp"
#include "lemlib/api.hpp"

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::Motor RightFront(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor RightMid(2, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor RightBack(3, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);  
    pros::Motor_Group RDrive({RightFront, RightMid, RightBack}); 

pros::Motor LeftFront(4, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor LeftMid(5, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES); 
pros::Motor LeftBack(-6, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);    
    pros::Motor_Group LDrive({LeftFront, LeftMid, LeftBack}); 

pros::Motor ptoR(7, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor ptoL(8, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

char ptoShift = 'L'; // L=lift    D=drive
/*
		
//Drivetrain config 
lemlib::Drivetrain drivetrain {
	&LDrive, // left motor group
	&RDrive, // right motor group
	12.575, // 10 inch track width
	lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
	400, // drivetrain rpm is 360
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


// create the chassis
lemlib::Chassis chassis(drivetrain, lateralController, angularController, sensors);
*/
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "code works");

	//chassis.calibrate();

	// set position to x:0, y:0, heading:0
    //chassis.setPose(0, 0, 0);
    // turn to face heading 90 with a very long timeout
    //chassis.turnTo(-24, 0, 10000000);
	//chassis.moveToPoint(-24, 0, 10000000);
	//chassis.turnTo(-24, 24, 10000000);
}


void disabled() {}


void competition_initialize() {}


void autonomous() {}

void shiftPTO(){
	if (ptoShift == 'L'){
		ptoShift = 'D';
	} else {
		ptoShift = 'L';
	}
}

void opcontrol() {


	while (true) {
		int dir = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
		int turn = -controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
		printf("%d  %d\n", dir-turn, dir+turn);
		LDrive.move(dir - turn);
		RDrive.move(dir + turn);
		if (ptoShift == 'D'){
			ptoL.move(dir - turn);
			ptoR.move(dir + turn);
		}
		if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_A)){
			shiftPTO();
		}
		pros::delay(20);
	}
}