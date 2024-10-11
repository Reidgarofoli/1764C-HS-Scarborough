#include "main.h"
#include "sylib/system.hpp"
#include "sylib/sylib.hpp"
#include "lemlib/api.hpp"
#include "pros/optical.hpp"
#include "variables.hpp"

// scores on alliance stake
//void auton_0() {
void auton_1() {
    chassis.setPose(0, 0, 0);
    chassis.moveToPose(0, 17.5, 0, 1500, {.minSpeed = 60}, false);

    chassis.moveToPose(6, 13, 270, 1500, {.forwards = false}, false);

    intake.move(127);
    delay(1500);
    intake.move(0);
}

// solo wp red team
//void auton_1() {
void auton_0() {
    chassis.setPose(0, 0, 0);
    chassis.moveToPose(0, -18, 0, 1500, {.forwards = false, .minSpeed = 100}, false);

    delay(400);
    mogovalue =! mogovalue;          // This is the code that toggles the mogo clamp
	mogomech.set_value(mogovalue); // either open or closed

    delay(200);
    
    intake.move(127);

    chassis.moveToPose(18, -18, 270, 1500, {.minSpeed = 100}, false);
}

void auton_2() {

}

void auton_3() {

}

void auton_4() {

}

void autonselect(int auton) {
    if (auton == 0){
        printf("it works auton 0\n");
        auton_0();}
    if (auton == 1){auton_1();}
    if (auton == 2){auton_2();}
    if (auton == 3){auton_3();}
    if (auton == 4){auton_4();}
    
}