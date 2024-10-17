#include "variables.hpp"

//Red Left
void auton_0() {
    chassis.setPose(0,0,90);//starting point - facing red right // starting 3 inches away from wall, lined up 13 form post, back to wall, flat 
    chassis.moveToPose(15,0,90,2500,{}, false);// the "90" is the angle, x and y are too much
    chassis.turnTo(15,-100, true, 127, false);
    chassis.moveToPose(15,-7,0,2500,{}, false);
    intake.move(127);
    delay(800);
    intake.brake();
    chassis.moveToPose(8,10,315,2500,{}, false);
    //chassis.turnTo(-3,28,1500, false, 127, false);
    chassis.moveToPose(-10,32,135,2500,{.forwards=false}, false);
    mogovalue = false;
    mogomech.set_value(mogovalue);
    

    






}

void auton_1() {


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