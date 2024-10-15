#include "variables.hpp"

//Red Left
void auton_0() {
    chassis.setPose(0,0,0);//starting point - facing red right // starting 3 inches away from wall, lined up 13 form post, back to wall, flat 
    intakePOS = false;// "true" = top ring, "false"= bottom ring
    intakeLifter.set_value(intakePOS);
    chassis.moveToPose(25,15,90,2500,{.minSpeed=60}, false);// the "90" is the angle, x and y are too much
    // move to POSE is plan on facing a direction when done
    // move to POINT is quickest way 
    intake.move(127);// intake TOP red ring - remeber to not move intake too much
    delay(1000);//too long
    intake.brake();
    //chassis.turnTo(25,100,2500,true,127,false);//turn to alliance stake
    chassis.moveToPose(25,0,0,2500, {.minSpeed=80}, false); // go to alliance stake
    
    
    
    /*intake.move(127);//score 2 red rings - preload and ring from first intake
    delay(1000);
    intake.brake();
    chassis.moveToPose(25,0,0,2500, {.minSpeed=80}, false);
    chassis.moveToPose(40,20,0,2500,{.minSpeed=80}, false);//move to mogo
    chassis.turnTo(40,-100,2500,true,127,false);
    chassis.moveToPose(0,100,31,2500,{.minSpeed=80}, false);
    mogovalue = false; // "false"=close 
    mogomech.set_value(mogovalue); //mogo clamped on mogo
    chassis.turnTo(-100,100,2500,true,127,false); // make sure it gets the bottom ring
    intakePOS = true;// "true" = top ring, "false"= bottom ring
    intakeLifter.set_value(intakePOS);
    chassis.moveToPose(-50,100,270,2500,{.minSpeed=80}, false);
    intake.move(127);//score 1 red rings on mogo
    delay(2000);
    intake.brake();
    chassis.moveToPose(100,50,0,1500,{.minSpeed=80}, false);
    */





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