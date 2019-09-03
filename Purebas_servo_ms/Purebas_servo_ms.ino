#include <Servo.h> 

Servo servo;
char val;
String readString;
int mseconds;

void setup() 
{ 
  Serial.begin(9600);
    servo.attach(9);
  servo.writeMicroseconds(1000);  
  
} 

void loop() {
  while(Serial.available()){
    val = Serial.read();
    readString += val;
    delay(2);
    }

  if(readString.length()>0){
  Serial.print(readString);
  mseconds = readString.toInt();
  if(mseconds>= 450 && mseconds <= 2500){
    Serial.print("Microseconds: ");
    Serial.println(mseconds);
    servo.writeMicroseconds(mseconds);
    }
    readString = "";
  }
}
