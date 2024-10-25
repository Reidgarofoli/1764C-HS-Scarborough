#include "variables.hpp"

//Red Left
void auton_0() {
    chassis.setPose(0,0,180);
    if (team == 'r'){
        int start = millis();
        chassis.moveToPose(0, 25, 180, 2000, {.forwards = false, .minSpeed = 95}, false);
        chassis.moveToPose(-7, 44, 150, 2000, {.forwards = false}, false);
        mogovalue = true;
        mogomech.set_value(mogovalue);
        delay(100);
        intaking = 1;
        chassis.turnTo(-10, 30, 2000, true, 127, false);
        chassis.moveToPoint(-10, 26, 400, true, 127, false);
        intaking = 0;
        mogovalue = false;
        mogomech.set_value(mogovalue);
        chassis.moveToPoint(-10, 25, 400, true, 127, false);

        chassis.turnTo(-28, 32, 2000, false, 127, false);
        chassis.moveToPose(-28, 32, chassis.getPose().theta , 2000, {.forwards = false}, false);
        mogovalue = true;
        mogomech.set_value(mogovalue);
        delay(100);
        
        intaking = 1;
        chassis.moveToPose(-50, 10, 270, 2000, {}, false);
        chassis.moveToPose(-60, 10, 270, 2000, {.maxSpeed=30}, false);
        chassis.turnTo(-60, 100, 2000, true, 127, false);
        chassis.moveToPose(-60, 50, 270, 2000, {.maxSpeed=30}, false);


        /*
        doinkerValue = !doinkerValue;
		doinker.set_value(doinkerValue);
        delay(200);
        chassis.turnTo(-50, 25, 2000, true, 25, false);
        chassis.moveToPoint(-50,25,2000,true,127,false);
        doinkerValue = !doinkerValue;
		doinker.set_value(doinkerValue);
        chassis.moveToPose(-60, 15, 270, 1000, {.minSpeed = 80}, false);
        int end = millis();
        printf("auton took %d seconds\n", (end - start) / 1000);
        */
    }
}

//goal rush
void auton_1() {

}

void auton_2() {

}

void auton_3() {

}

void auton_4() {//crosby's skills auton

    mogomech.set_value(!mogovalue);
mogovalue = !mogovalue;
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
chassis.turnTo(-65,15,2000, true);
chassis.moveToPoint(-60,15,11000,true,40);// y tf is this not happening?????? im literaly about to crash out:

 intake.move(127);
 delay(2800);
 //

chassis.moveToPoint(-75,-5,4000, false);//go to corner 
delay(3000);
mogomech.set_value(!mogovalue);
mogovalue = !mogovalue;
delay(1000);
chassis.moveToPoint(-18,16,5000,true,90);
chassis.turnTo(18,16,2000,false,90);
chassis.moveToPoint(16,14,6000,false,70);
delay(3000);
mogomech.set_value(!mogovalue);
mogovalue = !mogovalue;
delay(2000);
chassis.turnTo(50,14,2000,true,90);

chassis.moveToPoint(56,14,8000,true,40);

//intake.brake();
}

void autonselect(int auton) {
    if (auton == 0){auton_0();}
    if (auton == 1){auton_1();}
    if (auton == 2){auton_2();}
    if (auton == 3){auton_3();}
    if (auton == 4){auton_4();}
    
}