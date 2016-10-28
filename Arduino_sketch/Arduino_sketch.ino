//Reads Serial and controls Eye-Robot
#include <Servo.h>
const int leftmotor = 3;//left motor on pin 3 PWM
const int leftmotordir = 4;// left motor direction on pin 4
const int rightmotor = 5;//right motor on pin 5 PWM
const int rightmotordir = 6;//right motor on pin 6 PWMz


float fwdspeed = 0; 
float turnspeed = 0;
float pan = 0;
float tilt = 0;

void setup()
{
  Serial.begin(115200);//Begin serial at 115000 baud
  leftservo.attach(9);//attaches left servo to pin 9
  rightservo.attach(10);//attaches right servo to pin 10
  pinMode(rightmotor, OUTPUT);
  pinMode(leftmotor, OUTPUT);
}

void loop()
{
  //if there's any serial avaliable , read it
  while(Serial.available() > 0){
    //look for each valid float in the serial stream
    fwdspeed = Serial.parseFloat();
    turnspeed = Serial.parseFloat();
    pan = Serial.parseFloat();
    tilt = Serial.parseFloat();
  }
  //on newline, control hardware
  if(Serial.read() == '\n') {  
  motorcontroller(fwdspeed,turnspeed);
  servocontroller(pan, tilt);
  delay(15);
  }
}

void motorcontroller(float fwd, float turn){
  //given a forward speed and turn speed controls motors
}

void servocontroller(float panpos, float tiltpos){
  // given a pan and tilt value controls the differential
  int leftpos
  int rightpos
  leftpos = pan - tilt
  rightpos = pan + tilt
  //converts from -90 to 90 to 0 to 180
  leftpos = leftpos + 90
  rightpos = rightpos + 90
  //write values to servos
  leftservo.write(leftpos);
  rightservo.write(rightpos);
}
  
 
