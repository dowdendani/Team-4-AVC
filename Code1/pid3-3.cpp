// PID4.0
#include <stdio.h>
#include <time.h>
# include "E101.h"
#include <math.h>
#include <ctype.h>
//Assign variables here
//num
float whiteProportion=0;
float numWhiteLeft=0;
float numWhiteRight=0;
float numWhiteTop=0;


	//Motor speeds
	float left = 0;
	float right = 0;
	float leftSt =0;
	float rightSt = 0;
  
//Algorithm change
	int quadrant = 1 ;	
	int intersection = 0;
        int leftTurn=0;
         
	 

//Method to control motors.
void move(int left, int right){
    set_motor(1,(int)right);   
    set_motor(2,(int)left);
    sleep1(0,1000);
} 

void moveSt(int leftSt, int rightSt){
    set_motor(1,(int)right);   
    set_motor(2,(int)left);
    sleep1(0,1000);


}
void stop(){
    set_motor(1,0);
    set_motor(2,0);
}

//Turn left
void turnLeft(){ //change
    set_motor(1, 30);   
    set_motor(2, -30);
    sleep1(0,500000);
    set_motor(1, 0);   
    set_motor(2, 0);
} 

//Turn right
void turnRight(){ //change
	set_motor(1, -30);   
    set_motor(2, 30);
    sleep1(0,500000);
    set_motor(1,0);   
    set_motor(2,0);
} 

//Networking Code
//void openGate(){
		//insert IP
//    connect_to_server(" ",1024);
    //insert password
//    send_to_server(" ");


//    char message[24];
//    receive_from_server(message);
//   send_to_server(message);

//}
int cameraRead(){
	
	//Camera control
	
	int whiteThreshold = 100;	
	int p = 0; // p is the actual number of how white the pixel is, 
	int s = 0; //s is the converted number of p to  1(white) or 0(black)
	
	//Error control
	float  error;
        float current_error =0;
	float previous_error =0;
	float revError =0;
	float  numWhite=0;
	float  whiteProportion = 0; //proportion of white pixels in image
//	float numWhiteLeft = 0;
//	float numWhiteRight=0;
//	float numWhiteTop=0;
       
	
	//PID constants
	float kp = 0.02;
	float ki =0.001;
	float kd = 0.35;
	
	//PID signals
	float  proportional_signal;
	float derivative_signal;
	//int integral_signal;
	float final_signal;
	
	

	take_picture();
numWhiteLeft=0;
numWhiteRight=0;
numWhiteTop=0;
	 //display_picture(0,5000000);
				
				for(int i=0; i<240; i++){				     //for left	verticle
					p = get_pixel(i,80,3);						
					if(p>whiteThreshold){   
						
						numWhiteLeft++;																	
					}
				}					
							
				for(int i=0; i<240; i++){					//for Right verticle
					p = get_pixel(i,240,3);							
					if(p>whiteThreshold){   
						  

						numWhiteRight++;
					}	
				}	 	 
				
				for(int i=0; i<320; i++){					//for top  middle horizontal 					
					p = get_pixel(210,i,3);			
					if(p>whiteThreshold){   
						
						numWhiteTop++;												
					}
                                }
						
				for(int i=0; i<320; i++){					//for middle horizontal 					
					p = get_pixel(120,i,3);			
					if(p>whiteThreshold){   
						s = 1;
						numWhite++;												
					}
					else{
						s = 0;												
					}
					//printf("%d\n", s); 
					error = (i-160)*s; // if white is on left, error < 0, if on right, error > 0, if in center, error = 0
					current_error = (current_error + error);
					
				}
//			 if(!isnan(current_error)){
//				revError = current_error;
//			}
				current_error = current_error/numWhite;
				printf("number of white%.2f \n",numWhite);
                               // printf("current error %.2f \n",current_error);
printf("number of white left %.2f \n",numWhiteLeft);
printf("number of white right %.2f \n", numWhiteRight);
printf("number of white top %.2f \n", numWhiteTop);
				whiteProportion = (numWhite/320.0)*100;
printf("white prop: %2.f",whiteProportion);
				if(quadrant ==1){
				if(whiteProportion >=80 && numWhiteLeft >=30 && numWhiteRight >=30 ){ // Subject to change
				quadrant = 3;
                               // stop();
				moveSt(28,28);
                                sleep1(0,500000);	
				printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

				}		
		                }
  
				sleep1(0,10000);

				//Proportional
				proportional_signal = current_error*kp;  
 
				
				//Derivative
				derivative_signal = (current_error-previous_error/0.0125)*kd;

				previous_error = current_error;
				printf("Derivative signal is: %.2f", derivative_signal );
				
				//final signal
				//final_signal = (((proportional_signal/(160*1*kp)) + (derivative_signal/((320/0.1)*1*kd)))/2)*255;
				final_signal = ((proportional_signal+derivative_signal)/160)*45;
				printf("final signal is: %.2f \n", final_signal);
				if(numWhite>0){
					left = 30 - (final_signal);
					right = 30 +(final_signal);
		                }
               			 else{
					printf("--------------------------------------------- ----------------------");
					left= -30;
					right =-30;
					sleep1(0,500);
				//	if(revError <0){
				//	left=-30;
				//	right=-28;
		              	  //      sleep1(0,500);
			 //	}else if( revError > 0){
			//		left = -28;
			//		right =-30;
			//		sleep1(0,500);
			//		}
         		        }
				printf("left is: %.2f \n",left);
				printf("right is: %.2f \n", right);
						
return 0;  
}


int main(){
	init();
			//openGate();
			while(quadrant == 1){
			//	turnRight();
			//	sleep1(2,0);
			//	turnLeft();
			//	break;
         			cameraRead();
                               move(left,right);
			}
			
		        while(quadrant == 3){
          		    cameraRead();

						if (whiteProportion>=80 && numWhiteLeft >= 30 && numWhiteRight >=30 && numWhiteTop >30 ){           //detects t intersection 
                                                    moveSt(28,28);
						      sleep1(1,0);
printf("ttttttttttttttttttttttttttttttttttttttttttt");
 						}

                                              if (numWhiteLeft > 15  && numWhiteRight> 15 && numWhiteTop <40){             //detects T intersection
                                                     sleep1(0,500000);
                                                     turnLeft();
							sleep1(0,300000);
printf("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL");
                                               }
						
						if(numWhiteLeft <1 && numWhiteRight >=30 && numWhiteTop <10){
							sleep1(0,500000);
							turnRight();
							sleep1(0,300000);

printf("RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR");
						}
						if(numWhiteLeft >= 30 && numWhiteRight <1 && numWhiteTop <10){
							sleep1(0,500000);
							turnRight();
							sleep1(0,300000);
						}
                                              else{
                                                   move(left,right);
printf("sssssssssssssssssssssssssssssssssssssssssssssssssssss");
                                                   }
                      }
//						if (numWhiteLeft >=30 && numWhiteRight<30 && numWhiteTop<30){           //detects left turn
//
//						        if(leftTurn == 0){ //go straight on
//								cameraRead();			
//						                move(left,right);
//						                leftTurn = 1;									
//							}
//							if(leftTurn == 1){  //go left	
//								sleep1(0,100000);
//							        turnLeft();	
//						                sleep1(0,100000);						
//							}
//						}
//										
//						if (numWhiteLeft <30 && numWhiteRight>=30  && numWhiteTop<30){           // detects right turn						
//							 //turn right every time
//								sleep1(0,100000);
//							        turnRight();
//								sleep1(0,100000);
//							}
//							
//						else{            // straight line		
//						    move(left,right);	
//						}														
//			}
//
//                      	while(quadrant == 4){
//
//			}
									
return 0;
}
