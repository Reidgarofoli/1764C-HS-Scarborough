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
bool mogovalue = false; // false
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

#define rotate_port 9
pros::Rotation lift_angle(rotate_port);
int target_angle = 0; 	// 0 = up, 570 = down

int auton = 0;
char team = 'r';
bool isTheLiftAtTheDesignatedSpotYet = false;

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
int counter = 0;
int numColors = 255;
auto led = sylib::Addrled(threeWireExpander_Port,3,60);

void on_center_button() {
	if (team == 'r'){
		team = 'b';
	} else if (team == 'b'){
		team = 'r';
	}
	pros::lcd::print(0, "auton:%d  team:%c", auton, team);
	if (team == 'r'){
		led.set_all(0xff0000);
	} else if (team == 'b'){
		led.set_all(0x0000ff);
	}
}
void on_right_button() {
	if (auton != 3){
		auton++;
	}
	pros::lcd::print(0, "auton:%d  team:%c", auton, team);
}
void on_left_button() {
	if (auton != 0){
		auton--;
	}
	pros::lcd::print(0, "auton:%d  team:%c", auton, team);
}

long HSBtoRGB(float _hue, float _sat, float _brightness) {
    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;
    
    if (_sat == 0.0) {
        red = _brightness;
        green = _brightness;
        blue = _brightness;
    } else {
        if (_hue == 360.0) {
            _hue = 0;
        }

        int slice = _hue / 60.0;
        float hue_frac = (_hue / 60.0) - slice;

        float aa = _brightness * (1.0 - _sat);
        float bb = _brightness * (1.0 - _sat * hue_frac);
        float cc = _brightness * (1.0 - _sat * (1.0 - hue_frac));
        
        switch(slice) {
            case 0:
                red = _brightness;
                green = cc;
                blue = aa;
                break;
            case 1:
                red = bb;
                green = _brightness;
                blue = aa;
                break;
            case 2:
                red = aa;
                green = _brightness;
                blue = cc;
                break;
            case 3:
                red = aa;
                green = bb;
                blue = _brightness;
                break;
            case 4:
                red = cc;
                green = aa;
                blue = _brightness;
                break;
            case 5:
                red = _brightness;
                green = aa;
                blue = bb;
                break;
            default:
                red = 0.0;
                green = 0.0;
                blue = 0.0;
                break;
        }
    }

    long ired = red * 255.0;
    long igreen = green * 255.0;
    long iblue = blue * 255.0;
    
    return long((ired << 16) | (igreen << 8) | (iblue));
}
void liftControl(void){
	while (true){
		if (ptoShift == 'L'){
			if (target_angle != lift_angle.get_position() / 100){
				ptoR.move((lift_angle.get_position() / 100 - target_angle) / -4.46);
				ptoL.move((lift_angle.get_position() / 100 - target_angle) / -4.46);
				//if ((float)(lift_angle.get_position() / 100 - target_angle) / 4.46 > 0){
				//	if ((float)(lift_angle.get_position() / 100 - target_angle) / 4.46 > 30){
				//		ptoR.move(-127);
				//		ptoL.move(-127);
				//	} else {
				//		ptoR.move(-40);
				//		ptoL.move(-40);
				//	}
				//} else if ((float)(lift_angle.get_position() / 100 - target_angle) / 4.46 < 0){
				//	if ((float)(lift_angle.get_position() / 100 - target_angle) / 4.46 < -30){
				//		ptoR.move(127);
				//		ptoL.move(127);
				//	} else {
				//		ptoR.move(40);
				//		ptoL.move(40);
				//	}
				//} else {
				//	ptoR.brake();
				//	ptoL.brake();
				//}
				pros::lcd::print(1, "target:%d", target_angle);
				pros::lcd::print(2, "current:%d", lift_angle.get_position()/100);
				pros::lcd::print(3, "moving:%f", (float)(lift_angle.get_position() / 100 - target_angle));
				printf("moving:%f/n", (float)(lift_angle.get_position() / 100 - target_angle));
			} else {
				ptoR.brake();
				ptoL.brake();
			}
		} else {
			controller.clear_line(2);
		}
		delay(20);
	}
}
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "code works");

	pros::lcd::register_btn0_cb(on_left_button);
	pros::lcd::register_btn1_cb(on_center_button);
	pros::lcd::register_btn2_cb(on_right_button);

	chassis.calibrate();
	lift_angle.reset_position();
	sylib::initialize();
	if (team == 'r'){
		led.set_all(0xff0000);
	} else if (team == 'b'){
		led.set_all(0x0000ff);
	}

	mogovalue = false;
	mogomech.set_value(mogovalue);
	mogomechtwo.set_value(mogovalue);

	pros::Task masterLiftControl(liftControl);
}

void disabled() {
	while (true) {
		for (int i = 0; i < 60; i++){
            float colorNumber = (counter + i) % (numColors * 2) > numColors ? (counter + i) % (numColors * 2) - numColors: (counter + i) % (numColors * 2);
            
            float saturation = 1;
            float brightness = 1;
            float hue = (colorNumber / float(numColors)) * 360;
            long color = HSBtoRGB(hue, saturation, brightness);
            
            int red = color >> 16 & 255;
            int green = color >> 8 & 255;
            int blue = color & 255; 

            led.set_pixel((red*65536) + (green*256) + blue, i);

            //leds[i] = CRGB ( red, green, blue );
        }
	
	    counter = (counter + 1) % (numColors * 2);
		pros::delay(20);
	}
}

void competition_initialize() {
	if (team == 'r'){
		led.set_all(0xff0000);
	} else if (team == 'b'){
		led.set_all(0x0000ff);
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
		target_angle = 570;
		cbnvalue = false;
		cbnmech.set_value(cbnvalue);
		intakevalue = true;
		intakelift.set_value(intakevalue);
	}
	if (armcycle == 'U'){
		target_angle = -50;
		cbnvalue = true;
		cbnmech.set_value(cbnvalue);
		intakevalue = false;
		intakelift.set_value(intakevalue);
	}
	if (armcycle == 'S'){
		target_angle = 300;
		cbnvalue = true;
		cbnmech.set_value(cbnvalue);
		intakevalue = false;
		intakelift.set_value(intakevalue);
	}
}
void autonomous() {
	if (auton == 0){
		//empty auton
	} else if (auton == 1){
		if (team == 'r'){
			chassis.setPose(0, 0, 0);
			
			chassis.moveToPoint(0, -30, 100000000, false, 90);
			delay(1200);
			mogomech.set_value(!mogovalue);
			mogomechtwo.set_value(!mogovalue);
			mogovalue = !mogovalue;

			delay(500);

			IntakeStageOne.move(127);

			chassis.turnTo(12, -20, 100000000, true, 80);
			chassis.moveToPoint(12, -20, 100000000, true, 80);
			chassis.moveToPoint(28, -40, 100000000, true, 80);
		} else if (team == 'b'){
			chassis.setPose(0, 0, 0);
			
			chassis.moveToPoint(0, -25, 10000, false, 90);
			delay(1000);
			mogomech.set_value(!mogovalue);
			mogomechtwo.set_value(!mogovalue);
			mogovalue = !mogovalue;

			delay(500);

			IntakeStageOne.move(127);

			chassis.turnTo(-20, -35, 1000, true, 80);
			delay(1000);
			chassis.moveToPoint(-20, -35, 10000, true, 80);
			delay(1000);
			chassis.turnTo(-20, -40, 1000, true, 80);
			delay(1000);
			chassis.moveToPoint(-20, -45, 10000, true, 80);
		}
	} else if (auton == 2) {
		if (team == 'r'){
			chassis.setPose(0, 0, 0);
			
			chassis.moveToPoint(0, -30, 1000, false, 80);
			delay(1200);
			mogomech.set_value(!mogovalue);
			mogomechtwo.set_value(!mogovalue);
			mogovalue = !mogovalue;

			delay(500);

			IntakeStageOne.move(127);

			chassis.turnTo(-18, -30, 1000, true, 90);
			chassis.moveToPoint(-30, -30, 1000, true, 90);
		} else if (team == 'b'){
			chassis.setPose(0, 0, 0);
			
			chassis.moveToPoint(0, -30, 1000, false, 80);
			delay(1200);
			mogomech.set_value(!mogovalue);
			mogomechtwo.set_value(!mogovalue);
			mogovalue = !mogovalue;

			delay(500);

			IntakeStageOne.move(127);

			chassis.turnTo(18, -30, 1000, true, 60);
			chassis.moveToPoint(40, -30, 1000, true, 60);
			delay(3000);
			intakevalue = false;
			intakelift.set_value(intakevalue);
			delay(200);
			chassis.moveToPoint(-48, -10, 1000, true, 60);
			delay(5000);
			intakevalue = true;
			intakelift.set_value(intakevalue);
		}
	} else if(auton == 3) {
		if (team == 'r'){
			chassis.setPose(0, 0, 0);
			chassis.moveToPoint(0, 25, 1000, true, 80);
			chassis.moveToPoint(0, 22, 1000, false, 80);
			cycleArm(true);
			chassis.turnTo(10, 22, 1000, true, 80);
			chassis.moveToPoint(5, 22, 1000, true, 80);
			chassis.moveToPoint(0, 22, 1000, false, 80);
			cycleArm(true);
			chassis.moveToPoint(-10, 22, 1000, false, 80);
		}
	}
}
void checkPTO(){
	if (ptoShift == 'D'){
		ptoL.set_brake_mode(E_MOTOR_BRAKE_COAST);
		ptoR.set_brake_mode(E_MOTOR_BRAKE_COAST);
		pto.set_value(false);
	} else {
		ptoL.set_brake_mode(E_MOTOR_BRAKE_HOLD);
		ptoR.set_brake_mode(E_MOTOR_BRAKE_HOLD);
		pto.set_value(true);
	}
}
void shiftPTO(){
	if (ptoShift == 'L'){
		ptoShift = 'D';
		ptoL.set_brake_mode(E_MOTOR_BRAKE_COAST);
		ptoR.set_brake_mode(E_MOTOR_BRAKE_COAST);
		pto.set_value(false);
	} else {
		ptoShift = 'L';
		ptoL.set_brake_mode(E_MOTOR_BRAKE_HOLD);
		ptoR.set_brake_mode(E_MOTOR_BRAKE_HOLD);
		pto.set_value(true);
	}
}

void opcontrol() {
	checkPTO();
	cycleArm(false);
	mogomech.set_value(mogovalue);

	while (true) {
		if (mogovalue == true){
			if (team == 'r'){
				led.set_all(0xff0000);
			} else if (team == 'b'){
				led.set_all(0x0000ff);
			}
		} else {
			led.set_all(0x00ff00);
		}
		int dir = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
		int turn = -controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
		LDrive.move(dir - turn);
		RDrive.move(dir + turn);
		if (ptoShift == 'D'){
			ptoL.move(dir - turn);
			ptoR.move(dir + turn);
			//printf("L:%d R:%d\n", dir - turn, dir + turn);
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
			//printf("L1 pressed, %c\n", armcycle);
		}
		if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_RIGHT)){
			fanumTaxLiftValue = !fanumTaxLiftValue;
			fanumTaxDeploy.set_value(fanumTaxLiftValue);
		}
		if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)){
			fanumTaxValue = !fanumTaxValue;
			fanumTax.set_value(fanumTaxValue);
		}

		if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)){
			mogomech.set_value(!mogovalue);
			mogomechtwo.set_value(!mogovalue);
			mogovalue = !mogovalue;
		}
		pros::delay(20);
	}
}
