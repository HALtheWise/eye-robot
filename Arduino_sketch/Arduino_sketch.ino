//Reads Serial and controls Eye-Robot
#include <Servo.h>
const int leftmotorpin = 3;//left motor pin 3 PWM
const int leftmotordir = 2;// left motor direction on pin 4
const int leftmotordir2 = 4; //left motor direction 2 on pint 2
const int rightmotorpin = 6;//right motor on pin 5 PWM
const int rightmotordir = 5;//right motor on pin 6 PWM
const int rightmotordir2 = 7;//right motor 2 on pin 6 PWM
const int leftservopin = 9;//leftservopin on pin 9
const int rightservopin = 10;//rightservopin on pin 10
const int panservopin = 11;//panservo on pin 11

const int ledpin = 13;



float fwdspeed = 0; 
float turnspeed = 0;
float pan = 0;
float tilt = 0;

Servo leftservo;
Servo rightservo;
Servo panservo;

void setup()
{
  Serial.begin(115200);//Begin serial at 115000 baud
  leftservo.attach(9);//attaches left servo to pin 9
  rightservo.attach(10);//attaches right servo to pin 10
  panservo.attach(11);//attaches pan servo to pin 11
  pinMode(rightmotorpin, OUTPUT);
  pinMode(leftmotorpin, OUTPUT);
  pinMode(rightmotordir, OUTPUT);
  pinMode(leftmotordir, OUTPUT);
  pinMode(rightmotordir2, OUTPUT);
  pinMode(leftmotordir2, OUTPUT);
  pinMode(ledpin, OUTPUT);
}

void loop()
{
  
  //if there's any serial avaliable , read it

  digitalWrite(ledpin, LOW);
  while(Serial.available() > 1){
    digitalWrite(ledpin, HIGH);
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
    Serial.println("Recieved values");
  }
  //Clear the serial buffer
  while (Serial.available()>0){
    Serial.read();
  }
  delay(10);
}

void motorcontroller(float fwd, float turn){
  //given a forward speed and turn speed controls motors
  float leftmotor = fwd + turn;
  float rightmotor = fwd - turn;
  normalizePowers(&leftmotor, &rightmotor, 1.0);
  if (leftmotor <= 0){
    digitalWrite(leftmotordir, HIGH);
    digitalWrite(leftmotordir2, LOW);
    analogWrite(leftmotorpin, abs(leftmotor*255));
  }else{
    digitalWrite(leftmotordir, LOW);
    digitalWrite(leftmotordir2, HIGH);
    analogWrite(leftmotorpin, abs(leftmotor*255));
  }

  if (rightmotor <= 0){
    digitalWrite(rightmotordir, HIGH);
    digitalWrite(rightmotordir2, LOW);
    analogWrite(rightmotorpin, abs(rightmotor*255));
  }else{
    digitalWrite(rightmotordir, LOW);
    digitalWrite(rightmotordir2, HIGH);
    analogWrite(rightmotorpin, abs(rightmotor*255));
  }
}

void servocontroller(float panpos, float tiltpos){
  // given a pan and tilt value controls the differential
  int leftpos;
  int rightpos;
  //do any math you need to do here
  panpos = pan;
  rightpos = tilt;
  leftpos = -tilt;
  //write values to servos
  leftservo.write(leftpos);
  rightservo.write(rightpos);
  panservo.write(panpos);
}  


void normalizePowers(float *left, float *right, float limit){
	int maxabs = max(abs(*left), abs(*right));
	if(maxabs > limit)
	{
	    *left = (*left * limit) / maxabs;
	    *right = (*right * limit) / maxabs;
	}
}

