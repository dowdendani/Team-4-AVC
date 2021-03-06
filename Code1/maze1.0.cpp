#include <stdio.h>
#include <time.h>
#include “E101.h”

// Sensor variables
int near_wall = 550;
int sensorThresh = 550; 
int sensorMinThresh = 100;
int front_sensor;
int left_sensor;
int right_sensor;
float sensor_error;

//Sensor 
	
	float sensor_kp = 0.1;
	float sensor_ki =0.001;
	float sensor_kd = 0.13;
	float sensor_scale =50;

//Motor speeds
	float sensor_final_signal;
	float sensor_baseSpeed = 29;
	float sensor_min = 22;
	float sensor_max = 45;
	float left = 0;
	float right = 0;
  
//Method to control motors.
void move(int left, int right){
    set_motor(1,(int)right);   
    set_motor(2,(int)left);
    sleep1(0,500);
} 

void stop(){
    set_motor(1,0);
    set_motor(2,0);
}


//Turn left
void turnLeft(){ //change
  set_motor(1,35);
  set_motor(2,-35);
  sleep1(0,80000); 
} 


//Turn right
void turnRight(){ //change
  set_motor(1,-35);
  set_motor(2,35);
  sleep1(0,80000);
} 

int sensorRead(){
for(int i =0; i< 5;i++){
    front_sensor = front_sensor + read_analog(7);
    left_sensor = left_sensor + read_analog(6);
    right_sensor = right_sensor + read_analog(5);
    }
    // Calculates the average of 5 values 
    front_sensor = front_sensor/5;
    left_sensor = left_sensor/5;
    right_sensor = right_sensor/5;
    
    //Error control for the sensor. 0 if it is in the center.
    sensor_error = (float)left_sensor - (float)right_sensor;
  
    //final signal
		sensor_final_signal = (((float)sensor_error* sensor_kp)/160);

		//set speeds
		//left = sensor_baseSpeed - (sensor_final_signal);
	  //right = sensor_baseSpeed +(sensor_final_signal);
    
    		printf("Sensor error %.2f \n",sensor_error);
		printf("left %.2f \n",(float)left_sensor);
		printf("right %.2f \n", (float)right_sensor);
		printf("front %.2f \n", (float)front_sensor);
    		printf("final signal %.2f \n\n\n", sensor_final_signal);
    
  return 0;
}

int main(){

init();

while (true){
  sensorRead();
  move(left,right);

//if (((front_sensor < near_wall)) || ((sensor_error < 0) && (front_sensor < near_wall))){
  //robot moves forward if there are walls left and right, with space ahead OR wall on right with space ahead and left

// move(left, right);

//}

   //robot turns 90 degrees clockwise if there are walls ahead and left, with space right
  if ((front_sensor >= near_wall) && (left_sensor >= sensorThresh) && (right_sensor <= sensorMinThresh)){
    	turnRight();
	move(28,30);  
    
  }
  
  //robot turns 90 degrees anticlockwise if there are walls ahead and right, with space left
  if ((front_sensor >= near_wall) && (right_sensor >= sensorThresh) && (left_sensor <= sensorMinThresh)){
	  turnLeft();
	  move(28,30);  
  }
	
 if ((right_sensor >= sensorThresh) && (left_sensor <= sensorMinThresh)){
	 
	  move(30,32);
	  sleep1(0,500000);  
  }	
  return 0;
}
}







