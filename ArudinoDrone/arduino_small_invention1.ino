#include <LiquidCrystal.h>
#include <IRremote.h>
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);


//Switch
int switchPin = 3;
bool on = false;
int switchState = 0;

//Motor Speed
int normalSpeed = 255/2;

//IR Remote
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;


//Distance Sensor 
int cm = 0;


//Motor pins
int topLeftMotorPin = 5;
int topRightMotorPin = 10;
int bottomLeftMotorPin = 9;
int bottomRightMotorPin = 6;

//Lights pin
int lightsPin = 12;



//Tilt sensor pins
int topTiltPin = 2;
int leftTiltPin = 13;
int rightTiltPin = 4;
int bottomTiltPin = 8;


void setup()
{  
  Serial.begin(9600);  
  irrecv.enableIRIn(); // Start the receiver

  //Switch
  pinMode(switchPin, INPUT);

  //Motors setup
  pinMode(topLeftMotorPin, OUTPUT);
  pinMode(topRightMotorPin, OUTPUT);
  pinMode(bottomLeftMotorPin, OUTPUT);
  pinMode(bottomRightMotorPin, OUTPUT);
  
  //Lights
  pinMode(lightsPin, OUTPUT);
 
  
  //LCD Screen
  lcd.begin(16, 2); 
  lcd.print("Turning ON");

 //Setting up tiltPins 
  pinMode(topTiltPin, INPUT);  
  pinMode(leftTiltPin, INPUT);  
  pinMode(rightTiltPin, INPUT);  
  pinMode(bottomTiltPin, INPUT);
 
  

}

void loop()
{
  //Setting up tilt sensors
   int bottomTilt = digitalRead(8);
   int forwardsTilt = digitalRead(2);
   int rightTilt = digitalRead(4);
   int leftTilt = digitalRead(13);
  
  
   switchState = digitalRead(3);
   cm = 0.01723 * readUltrasonicDistance(7, 7);

  
  
  if (irrecv.decode(&results)) {
  	  irrecv.resume(); // Waiting to receive the next value 
  }
  
  
  //Checks to see if the switch is on
  if (switchState == HIGH){
      lcd.clear(); 
   
    
    if(cm < 30){ // object/wall detection
		moveBackwards(); // moves backwards if it comes close
      	lcd.clear();
       	lcd.print("Move Backward"); 
    } else{
      hover(); // returns to normal hover speed
    }
    
    //Checks for tilt position and adjusts motor speed accordingly
   	  if(results.value == 0xFD807F || forwardsTilt == 1){
        moveForward();
        lcd.clear();
	    lcd.print("Move Forward");
      
      } else if(results.value == 0xFD906F || bottomTilt == 1 ){
        moveBackwards();
        lcd.clear();
       	lcd.print("Move Backward"); 
      
      
      } else if(results.value == 0xFD609F || rightTilt == 1 ){
        moveRight();
        lcd.clear();
        lcd.print("Turn Right"); 

    } else if(results.value == 0xFD20DF || leftTilt == 1 ){
        moveLeft();
        lcd.clear();
        lcd.print("Turn Left");
      }

  }else{
      lcd.clear(); 
      lcd.print("Drone inactive");
      droneOff();
  	}
}


long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}


void hover(){ // Sets normal hover for the drone
  analogWrite(topLeftMotorPin, normalSpeed);
  analogWrite(topRightMotorPin, normalSpeed);
  analogWrite(bottomLeftMotorPin, normalSpeed);
  analogWrite(bottomRightMotorPin, normalSpeed);
	
  digitalWrite(lightsPin, HIGH);
}




void droneOff(){ // Shuts the drone off
  analogWrite(topLeftMotorPin, 0);
  analogWrite(topRightMotorPin, 0);
  analogWrite(bottomLeftMotorPin, 0);
  analogWrite(bottomRightMotorPin, 0);
	
  digitalWrite(lightsPin, LOW);
}


void moveLeft(){ // Adjusts motors and moves leftwards
  analogWrite(topLeftMotorPin, normalSpeed);  
  analogWrite(topRightMotorPin, normalSpeed*2);
  analogWrite(bottomLeftMotorPin, normalSpeed*2);
  analogWrite(bottomRightMotorPin, normalSpeed);  
}

void moveRight(){ // Adjusts motors and moves rightwards
  analogWrite(topLeftMotorPin, normalSpeed*2); 
  analogWrite(topRightMotorPin, normalSpeed);  
  analogWrite(bottomLeftMotorPin, normalSpeed); 
  analogWrite(bottomRightMotorPin, normalSpeed*2);  
  
}

void moveForward(){ // Adjusts motors and moves forwards
  analogWrite(topLeftMotorPin, normalSpeed);  
  analogWrite(topRightMotorPin, normalSpeed);  
  analogWrite(bottomLeftMotorPin, normalSpeed*2); 
  analogWrite(bottomRightMotorPin, normalSpeed*2);  
}

void moveBackwards(){ // Adjusts motors and moves backwards
  analogWrite(topLeftMotorPin, normalSpeed*2); 
  analogWrite(topRightMotorPin, normalSpeed*2); 
  analogWrite(bottomLeftMotorPin, normalSpeed);// same
  analogWrite(bottomRightMotorPin, normalSpeed);  // same
}


