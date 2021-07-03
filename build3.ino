#define motorPin 3 //declare the DC motor pin
#define servoPin 5 //declare the servo motor pin
#define sensorPin 6 //declare the sound sensor pin
#include <Servo.h> //include the Servo library

Servo servomotor; //create a servo object 

unsigned long lastEvent = 0; //time of last sound event
unsigned long currEvent = 0; //time of current sound event
unsigned long rightanswerEvent = 0; //time of last right answer

int motorspeed = 100; //setting motor speed - must be between 0 and 255

void setup() {
  Serial.begin(115200); //setting up serial monitor
  Serial.println("STARTING"); //starting the program
  pinMode(motorPin, OUTPUT); //set DC motor pin as output
  pinMode(sensorPin, INPUT);  //set sound sensor pin as an INPUT
  servomotor.attach(servoPin); //attaching servo motor to the pin number
  servomotor.write(0); //move servo to start
  pinMode(servoPin, INPUT);  //to prevent jitter when not in use, setting servo pin to input
  servomotor.write(0); //move servo to starting position
}

void loop() {
  int sensorData = digitalRead(sensorPin); //read from sound sensor
  
  if (sensorData == LOW) { //if sensor is LOW, sound is detected
    //i am defining an event as any sound that is at least 25 ms past another sound
    //so it doesn't just keep detecting events constantly during a single chime
    if (millis() - lastEvent > 25) {
      currEvent = millis(); //storing the start time of the current sound event
      Serial.print("Sound! diff: ");
      Serial.println(currEvent-lastEvent);
      //checking time difference between sound events
      if (currEvent - lastEvent < 120 && currEvent - lastEvent > 110) { //if the chimes are within this range, then it is a right answer chime
        rightanswerEvent = millis(); //storing time of the right answer
        Serial.println("=====================");
        Serial.println("====RIGHT ANSWER!====");
        Serial.println("=====================");
        
        //using servo motor to deploy candy
        servomotor.attach(servoPin); //attaching servo motor to the pin number
        servomotor.write(90); //move servo 90 degrees
        delay(500); //delay to allow time for motor to move
        servomotor.write(0); //move servo back to start
        delay(500); //delay to allow time for motor to move
        pinMode(servoPin, INPUT);  //to prevent jitter when not in use, setting servo pin to input
      }
    }
    lastEvent = millis(); //storing when last event happened
  }

  //if it takes too long to get a right answer
  if (millis()-rightanswerEvent > 10000) {
    Serial.println("=====================");
    Serial.println("=====PUNISHMENT!=====");
    Serial.println("=====================");
    
    //spinning DC motor for the punishment
    analogWrite(motorPin, motorspeed); //starting the motor
    delay(500); //delay to allow time for motor to move
    analogWrite(motorPin, 0); //stopping the motor
    rightanswerEvent = millis(); //resetting the timer from the last right answer
  }
  
}
