#pragma config(Motor,  port2,           LM,            tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           RM,            tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void forward(int power, int time){
	motor[LM] = power;
	motor[RM] = power;
	wait(time);

}

void back(int power, int time){
	motor[LM] = -power;
	motor[RM] = -power;
	wait(time);
}

void left(){
	turnLeft(360, degrees, 50);
}






task main()
{
	forward(67, 2);
	back(67, 2);
	left();
}
