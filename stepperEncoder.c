//Copyright Evan Johnson, 2015
//GPL 3.0 license. Feel free to redistribute
//Please cite me in your project. 
//Email me for questions:  erjohnson227@gmail.com
  
#include <AFMotor.h>  
#include <AccelStepper.h>  
  
// Connect a stepper motor with 48 steps per revolution (7.5 degree)  
// to motor port #2 (M3 and M4)  
  
int clockPin = 10;   
int inPin    = 11;   
int CSPin    = 12;   
  
uint16_t count = 0;   
uint16_t data = 0;   
uint16_t posLast = 0;   
int16_t revs = 0;   
uint16_t contPos = 0;   
  
  
AF_Stepper motor(100, 2);  
  
void stepForward2()  
{  
  motor.onestep(FORWARD, SINGLE);   
}  
void stepBackward2()  
{  
  motor.onestep(BACKWARD, SINGLE);   
}  
  
AccelStepper stepper2(stepForward2, stepBackward2);   
  
  
  
void setup() {  
  Serial.begin(9600);           // set up Serial library at 9600 bps  
  Serial.println("Stepper test!");  
    
  pinMode(CSPin, OUTPUT);   
  pinMode(clockPin, OUTPUT);   
    
  digitalWrite(clockPin, LOW);   
  digitalWrite(CSPin, HIGH);   
  
  motor.setSpeed(10);  // 10 rpm    
  stepper2.setMaxSpeed(600.0);   
  stepper2.setAcceleration(10000.0);    
  stepper2.setCurrentPosition(0.0);   
  stepper2.moveTo(0);   
}  
  
void loop() {  
    
   if((count % 500) == 0)  
   {  
    digitalWrite(CSPin, LOW);   
    //delayMicroseconds(10);   
    uint8_t dataHigh = shiftIn(inPin, clockPin, MSBFIRST);   
    uint8_t dataLow = shiftIn(inPin, clockPin, MSBFIRST);   
    digitalWrite(CSPin, HIGH);   
      
    data = (dataHigh << 8) | dataLow; //Bring 16 bit data into one byte.   
    
    uint16_t pos = data >> 6;   
      
    int delPos = pos - posLast;   
      
    if(delPos < 0)  
      delPos = - delPos;   
        
    if((posLast >= 900) && (pos <= 100))  
      revs ++;   
    else if((pos >= 900) && (posLast <= 100))  
      revs --;   
        
      
    if(delPos > 0)  
    {  
      stepper2.moveTo((float)(pos+revs*1024.0f)*(200.0f/1024.0f));   
      //Serial.print(revs, DEC);   
      //Serial.print(", ");   
      //Serial.println(pos, DEC);   
    }  
      
    posLast = pos;   
       
   }  
     
    count ++;    
      
   stepper2.run();  
     
} 
