#include "main.h"
#include "sylib/system.hpp"
#include "sylib/sylib.hpp"
#include "lemlib/api.hpp"
#include "pros/optical.hpp"

extern lemlib::Chassis chassis;
void auton_0() {
    chassis.moveToPose(0,0,0,100);
}

void auton_1() {

}

void auton_2() {

}

void auton_3() {

}

void auton_4() {

}

void autonselect(int auton) {
    if (auton == 0){auton_0();}
    if (auton == 1){auton_1();}
    if (auton == 2){auton_2();}
    if (auton == 3){auton_3();}
    if (auton == 4){auton_4();}
    
}