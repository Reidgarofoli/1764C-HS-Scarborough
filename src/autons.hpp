#include "variables.hpp"

//Red Left
void auton_1() {
    int starttime = millis();
    chassis.setPose(0,0,90);//starting point - facing red right // starting 3 inches away from wall, lined up 13 form post, back to wall, flat 
    chassis.moveToPose(15,0,90,2000,{}, false);// the "90" is the angle, x and y are too much
    chassis.turnTo(15,-100, true, 127, false);
    chassis.moveToPose(15,-7,0,2000,{}, false);
    intake.move(127);
    delay(800);
    intake.brake();
    chassis.moveToPose(22,10,90,2000,{}, false);
    chassis.moveToPose(-7,30,145,2000,{.forwards=false}, false);
    mogovalue = true;
    mogomech.set_value(mogovalue);
    delay(500);
    chassis.turnTo(-30,30,2000, true, 127, false);
    chassis.moveToPose(5,30,270,2000,{.forwards=false}, false);
    intake.move(127);
    chassis.moveToPose(-30,30,315,2000,{.forwards=true}, false);
    chassis.moveToPose(-28,30,270,2000,{.forwards=false}, false);
    chassis.turnTo(-28,43,2000, true, 127, false);
    chassis.moveToPose(-28,43,315,2000,{.forwards=true}, false);
    delay(10000);
    mogovalue = false;
    mogomech.set_value(mogovalue);
    
    printf("time it took: %d\n", millis() - starttime);
}

//goal rush
void auton_0() {
    chassis.setPose(0,0,180);
    if (team == 'r'){
        chassis.moveToPose(0, 25, 180, 2000, {.forwards = false, .minSpeed = 95}, false);
        chassis.moveToPose(-7, 43, 150, 2000, {.forwards = false}, false);
        delay(300);
        mogovalue = true;
        mogomech.set_value(mogovalue);
        delay(300);
        intake.move(127);
        chassis.turnTo(-10, 30, 2000, true, 127, false);
        chassis.moveToPoint(-10, 30, 2000, true, 127, false);
        chassis.moveToPose(-5, 40, 180, 2000, {.forwards = false}, false);
        mogovalue = false;
        mogomech.set_value(mogovalue);
        chassis.turnTo(-20, 30, 2000, false, 127, false);
        chassis.moveToPose(-30, 35, 90, 2000, {.forwards = false}, false);
        mogovalue = true;
        mogomech.set_value(mogovalue);
    }
}

void auton_2() {

}

void auton_3() {

}

void auton_4() {//crosby's skills auton
    chassis.setPose(0,0,0); // set up right againg the alliance stake

    //chassis.moveToPoint(0,0,1000000,true,90)
    //                    x, y, ignore, drctn t= frwrd, speed
    intake.move(100);//starts intake "move"=go
    delay(1000);
   intake.brake(); //stops intake "brake"= stop
    delay(300);
    //hopefully intake spins and then stops = ring on alliance stake
 chassis.moveToPoint(0,24,10000,true,90);
chassis.moveToPoint(-18,16,10000,false,90);
  delay(2000);
  
  
    //chassis.moveToPoint(0,3,2342342,false,90);
//chassis.turnTo(-50,3,232552,false,90);
   // chassis.moveToPoint(-20,3,144441212,false,90);
   // delay(100);
    //hopefully is now set up to grab goal
   mogomech.set_value(!mogovalue);
   mogovalue = !mogovalue;
   delay(1000);
    //first goal collecteds
chassis.turnTo(-65,17,10000,true, 90);
chassis.moveToPoint(-65,17,10000,true, 45);
 
 intake.move(127);
 delay(8000);
 intake.brake();

chassis.moveToPoint(-65,-5,10000, false);//go to corner 
delay(2000);
mogomech.set_value(!mogovalue);
mogovalue = !mogovalue;
intake.brake();
}

void autonselect(int auton) {
    if (auton == 0){auton_0();}
    if (auton == 1){auton_1();}
    if (auton == 2){auton_2();}
    if (auton == 3){auton_3();}
    if (auton == 4){auton_4();}
    
}