#pragma config(Sensor, in1,    light,          sensorReflection)
#pragma config(Sensor, dgtl1,  encoder,        sensorQuadEncoder)
#pragma config(Motor,  port2,           LM,            tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           RM,            tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           ARM,           tmotorVex269_MC29, openLoop)
#pragma config(Motor,  port5,           ARM2,          tmotorVex269_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/**
* Determines which functions the robot should run
* manual control listens for joystick control, infinite loop
* avatar checks wind speed, algorithm that runs once
* praise the sun checks light, algorithm that runs once
*
* bools are checked and executed in order listed here when the robot starts; thus setting mc to true
would cause all other bools to be irrelevant because it runs forever.
*/
bool mc = false; //manualControl
bool ava = false; //avatar
bool allHail = false; //praiseTheSun
bool programmedRove = true;

/**
* Executes the automatic, programmed rove that takes place at the beginning of the entire rove.
* Drives to the lake, takes temperature, drives back to starting, finds the sun, then switches over 
* to manual rove
*/
void noHands(int speed){
	
  //starts facing the lake, so go towards it (bottom right)
	motor[LM] = -1 * speed;
	motor[RM] = speed;
	wait(1.5);

	//take temperature
	//collectTemperature();
	wait(1); //uncomment this if temp running, vice versa

	//turn around
	motor[LM] = speed;
	motor[RM] = speed;
	wait(1);
	
	//go to the pond (upper right)
	motor[LM] = -1 * speed;
	motor[RM] = speed;
	wait(1);
	
	
}

/**
* Finds the brightest spot
* Rotates to the right at x speed for n seconds, taking y measurements total, one every m milliseconds
* Then rotates to the left back to the brightest spot (numChecks - indexOfLowestValue to find how much we need
to rotate backwards)
*/
void sunRoutine(int speed, int numChecks, int checkInterval, bool debug){
	//turn to the right while saving the lowest value (brightest)

	int lightValue = 5000;
	motor[LM] = -1 * speed;
	motor[RM] = -1 * speed;
	int count = 0;
	int indexOfLowestValue = 0;
	while(count < numChecks){
		if(SensorValue[in1] < lightValue){
			lightValue = SensorValue[in1];
			indexOfLowestValue = count;
		}
		wait1Msec(checkInterval);
		count++;
	}



	//Turn the appropriate amount backwards to get to the brightest spot

	motor[LM] = speed;
	motor[RM] = speed;

	//how many checks to turn back
	int turnBack = numChecks - indexOfLowestValue;
	wait1Msec(100 * turnBack);

	//stop moving
	motor[LM] = 0;
	motor[RM] = 0;

	if(debug){
 //Move forward, then back, then forward, of which the total amount of times corresponds to turnBack at runtime
		for(int i = 0; i < turnBack; i++){
			if(i%2 == 0){
					motor[LM] = speed;
					motor[RM] = -1 * speed;
					wait1Msec(500);
			}
			else {
				motor[LM] = -1 * speed;
				motor[RM] = speed;
				wait1Msec(500);
			}
		}
	}
}

/**
* Finds the brightest spot
* Rotates to the right at x speed for n seconds, taking y measurements total, one every m milliseconds.
* Then rotates to the left back to the brightest spot (numChecks - indexOfLowestValue to find how much we need
to rotate backwards)
* Repeat arbitrary amount of times with half the numChecks every time
*/
void praiseTheSun(){

	bool debug = true; //If the routine jerks back and forth after each run

	int speed = 67;
	int checkInterval = 200; // The interval, in milliseconds, to check the light
	int numChecks = 25; //How many times to check the light
	//Total time the first pass takes is checkInterval * numChecks / 1000

	while(true){
		if(numChecks < 4){ //stop when it takes a fairly small amount of time to run the routine, <4 is arbitrary
			break;
		}
		numChecks /= 2; //make the radius it checks smaller every time
		sunRoutine(speed, numChecks, checkInterval, debug); //run the routine
	}
}


void avatar(){ //you had to be there

	//reset all values
	motor[LM] = 0;
	motor[RM] = 0;
	SensorValue[encoder] = 0;

	wait(1);
	int deg = SensorValue[encoder];
	int rev = (int) (deg / 360);

}

/**
* Responds to joystick input
* Channel 2 goes forward
* Channel 4 goes backwards
* Channel 8R pulls arm up
* Channel 8L puts arm down
*/
void manualControl(int wheelMult, int armMult){

	while(true){

	  //forwards
		motor[LM] = wheelMult * -1 * vexRT[Ch2];
		motor[RM] = wheelMult * vexRT[Ch2];

		//backwards
	  	motor[RM] = wheelMult * vexRT[Ch4];
		motor[LM] = wheelMult * vexRT[Ch4];

		// Arm control
		motor[ARM] = armMult/2 * -2 * vexRT[Btn8R];
		motor[ARM] = armMult * vexRT[Btn8L];
		motor[ARM2] = armMult/2 * -1 * vexRT[Btn8R];
		motor[ARM2] = armMult *2* vexRT[Btn8L];

	}

}

//Run when the robot starts/is turned on
task main()
{
	if(mc){
		manualControl(5, 67);
	}
	if(ava){
		avatar();
	}
	if(allHail){
		praiseTheSun();
	}
	if(programmedRove){
		noHands(40);
	}
}
