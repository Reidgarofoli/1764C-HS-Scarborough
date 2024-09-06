#include "main.h"
#include "sylib/system.hpp"
#include "sylib/sylib.hpp"
#include "lemlib/api.hpp"
#include "pros/optical.hpp"

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

pros::Motor IntakeStageOne(20, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor IntakeStageTwo(19, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

#define threeWireExpander_Port 15

#define ptopin 'B' // D
pros::ADIDigitalOut pto ({{threeWireExpander_Port, ptopin}});
bool ptovalue = false;

#define mogo 4 // E
pros::ADIDigitalOut mogomech (mogo);
bool mogovalue = false;
#define mogopin 5 // E
pros::ADIDigitalOut mogomechtwo (mogopin);

#define cbnpin 6 // F
pros::ADIDigitalOut cbnmech (cbnpin);
bool cbnvalue = false;

#define fanumTaxDeployPort 'A'
pros::ADIDigitalOut fanumTaxDeploy ( {{threeWireExpander_Port, fanumTaxDeployPort}} );
pros::ADIDigitalOut fanumTax ( 3 );
bool fanumTaxLiftValue = true;
bool fanumTaxValue = true;
#define IntakePort 'F'
pros::ADIDigitalOut intakelift ( {{threeWireExpander_Port, IntakePort}} );
bool intakevalue = false;

char ptoShift = 'L'; // L=lift    D=drive
char armcycle = 'D'; // options: D=Down U=Up S=Slam
		
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

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "code works");

	chassis.calibrate();

	// set position to x:0, y:0, heading:0
    
}


void disabled() {}


void competition_initialize() {}


void autonomous() {
	chassis.setPose(0, 0, 0);
	
	mogomech.set_value(!mogovalue);
	mogomechtwo.set_value(!mogovalue);
	mogovalue = !mogovalue;
    
	chassis.moveToPoint(0, -35, 100000000, false);
	
	mogomech.set_value(!mogovalue);
	mogomechtwo.set_value(!mogovalue);
	mogovalue = !mogovalue;

	IntakeStageOne.move(127);

    chassis.moveToPoint(10, -28, 100000000);
	
	
	chassis.turnTo(10,-40, 100000);
	chassis.moveToPoint(20, 0, 100000);
	IntakeStageOne.move(0);
}

void shiftPTO(){
	if (ptoShift == 'L'){
		ptoShift = 'D';
		pto.set_value(false);
	} else {
		ptoShift = 'L';
		pto.set_value(true);
	}
}

void cycleArm(bool shouldICycleNow){
	if (shouldICycleNow){
		if (armcycle == 'D'){
			armcycle = 'U';
		} else if (armcycle == 'U'){
			armcycle = 'S';
		} else if (armcycle == 'S'){
			armcycle = 'D';
		}
	}
	if (armcycle == 'D'){
		cbnvalue = false;
		cbnmech.set_value(cbnvalue);
		intakevalue = true;
		intakelift.set_value(intakevalue);
		//motor.move_absolute(0, 100); 
	}
	if (armcycle == 'U'){
		cbnvalue = false;
		cbnmech.set_value(cbnvalue);
		intakevalue = false;
		intakelift.set_value(intakevalue);
		//motor.move_absolute(100, 100); 
	}
	if (armcycle == 'S'){
		cbnvalue = true;
		cbnmech.set_value(cbnvalue);
		intakevalue = false;
		intakelift.set_value(intakevalue);
		//motor.move_absolute(50, 100); 
	}
}

void opcontrol() {

	mogomech.set_value(mogovalue);
	while (true) {
		controller.print(2, 0, "pto shift:%c", ptoShift);
		int dir = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
		int turn = -controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
		LDrive.move(dir - turn);
		RDrive.move(dir + turn);
		if (ptoShift == 'D'){
			ptoL.move(dir - turn);
			ptoR.move(dir + turn);
		}
		if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_A)){
			shiftPTO();
		}
		if (controller.get_digital(E_CONTROLLER_DIGITAL_R2)){
			IntakeStageOne.move(127);
		} else if (controller.get_digital(E_CONTROLLER_DIGITAL_R1)){
			IntakeStageOne.move(-127);
		} else {
			IntakeStageOne.move(0);
		}

		if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_L1)){
			cycleArm(true);
			printf("L1 pressed, %c\n", armcycle);
		}
		if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_RIGHT)){
			fanumTaxLiftValue = !fanumTaxLiftValue;
			fanumTaxDeploy.set_value(fanumTaxLiftValue);
		}
		if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)){
			fanumTaxValue = !fanumTaxValue;
			fanumTax.set_value(fanumTaxValue);
		}

		

		if (controller.get_digital(E_CONTROLLER_DIGITAL_Y) && ptoShift == 'L'){
			ptoL.move(127);
			ptoR.move(127);
		} else if (controller.get_digital(E_CONTROLLER_DIGITAL_B) && ptoShift == 'L'){
			ptoL.move(-127);
			ptoR.move(-127);
		} else if (ptoShift == 'L') {
			ptoL.move(0);
			ptoR.move(0);
		}

		if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)){
			mogomech.set_value(!mogovalue);
			mogomechtwo.set_value(!mogovalue);
			mogovalue = !mogovalue;
		}
		pros::delay(20);
	}
}
