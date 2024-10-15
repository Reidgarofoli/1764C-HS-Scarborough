#include "variables.hpp"

//crosby was here
void auton_0() {







}

void auton_1() {


}

void auton_2() {

}

void auton_3() {

}

void auton_4() {
    chassis.setPose(0,0,0);

    //chassis.moveToPoint(0,0,1000000,true,90)
    //                    x, y, ignore, drctn t= frwrd, speed
    intake.move(100);//starts intake "move"=go
    delay(1000);
    intake.brake(); //stops intake "brake"= stop
    //hopefully intake spins and then stops = ring on alliance stake
    chassis.turnTo(0,100,10000,false,90);
    chassis.moveToPoint(0,3,2342342,false,90);
    chassis.turnTo(-50,3,232552,false,90);
    chassis.moveToPoint(-20,3,144441212,false,90);
    delay(100);
    //hopefully is now set up to grab goal
    mogomech.set_value(!mogovalue);
    mogovalue = !mogovalue;
    //first goal collected
}

void autonselect(int auton) {
    if (auton == 0){auton_0();}
    if (auton == 1){auton_1();}
    if (auton == 2){auton_2();}
    if (auton == 3){auton_3();}
    if (auton == 4){auton_4();}
    
}