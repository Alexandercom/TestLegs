#include <Servo.h>    //to define and control servos
#include <math.h>

Servo servo1;
int x = 90;           //grados de 0 a 180 para poner el servo
//define servos' ports

void setup() {
  // put your setup code here, to run once:
  servo1.attach (2);
}

int grados_reales(int gf){
  int a = 1;      //Valores de pendiente ax + b = f(x)
  int b = 0;
  
  return a*gf + b;
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i <= 180; i += 45) {
    servo1.write(grados_reales(i));  
    delay(1000);
  }
}
