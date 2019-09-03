#include <math.h>
#include <Servo.h> 

float pi=3.1416;

// Servos de la pata
Servo servoM3;
Servo servoM2;
Servo servoM1;

// Pines a los que están conectados los servos
int M3 = 10;
int M2 = 9;
int M1 = 8;

// Para la posicion de la pata
int SHOULDER = 1;
int ELBOW = 1;

// Dimensiones del robot en cm
float L0 = 4;           //Distancia del centro del robot al hombro   <-  FALTA MEDIR EN FISICO     CAMBIAR
float L1 = 4.7;         //Distancia del hombro al codo
float L2 = 10.461;      //Distancia del codo a la muneca
float L3 = 11.363;      //Distancia de la muneca a la punta

float gama = pi/4;      //Angulo al que se encuentra la pata con respecto al universal

// Variables for position  
float PUx;
float PUy;
float PUz;

float Px;
float Py;
float Pz;

// Variables para trayectoria pata 1                                          SE PUEDEN MODIFICAR
float ry=3; //Radio en y (a lo largo del piso)
float rz=3; //Radio en z (maxima distancia entre la pata y el piso)

float a0=pi/2;   //Angulo inicial de la rotacion
float af=5*pi/2;  //Angulo final de la rotacion

float b=3*pi/2;  //Rotación de la trayectoria con respecto al unviersal    0->Adelante    pi/2->izquierda    pi->atras    3pi/2->derecha
float a;    //Angulo del círculo de la trayectoria
float w;

//Distamcia al origen de la trayectoria con respecto al universal             SE PUEDEN MODIFICAR
float dx=5;
float dy=5;
float dz=-18;

//Delay entre cada movimiento
float delayM = 100;

float t0 = 0;
float tf = 3000;  // Duración de un ciclo de la pata andando en milisegundos
float delta_t = delayM;
  
void setup() {
  Serial.begin(9600);
  servoM3.attach(M3);
  servoM2.attach(M2);
  servoM1.attach(M1);
}

void loop() {
  for(int t=t0; t<=tf; t=t+delta_t) {
    w=(t-t0)/(tf-t0);
    a=(af-a0)*w+a0+pi;   //Offset de pi
    
    // Calculo de posición pata
    //(Lugar desdeado universal)-traslación
    PUx=(-1*ry*cos(a)*sin(b) + dx) - L0*cos(gama);
    PUy= (ry*cos(a)*cos(b) + dy) - L0*sin(gama);
    PUz=(max2(0,rz*sin(a)) + dz);
    //Lugar desdeado pata    sin rotación
    Px=PUx*cos(-gama)-PUy*sin(-gama);
    Py=PUx*sin(-gama)+PUy*cos(-gama);
    Pz=PUz;
    /*Serial.print("Px: ");
    Serial.println(PUx);
    Serial.print("Py: ");
    Serial.println(PUy);
    Serial.print("Pz: ");
    Serial.println(PUz);*/
    // Cinematica inversa pata
    float q1 = findQ1(Px, Py);
    float q3 = findQ3(Px, Py, Pz, q1);
    float q2 = findQ2(Px, Py, Pz, q1, q3);
  
    // Posiciona la pata
    servoM1.writeMicroseconds(msM1(q3));
    servoM2.writeMicroseconds(msM2(q2));
    servoM3.writeMicroseconds(msM3(q1));
    /*Serial.print("Px: ");
    Serial.println(Px);
    Serial.print("Py: ");
    Serial.println(Py);
    Serial.print("Pz: ");
    Serial.println(Pz);
    Serial.print("q1: ");
    Serial.println(q1);
    Serial.print("q2: ");
    Serial.println(q2);
    Serial.print("q3: ");
    Serial.println(q3);*/
    delay(delayM);
  }
}

//  Funciones para milisegundos del servo
//M
float msM1(float grados) {
  return 539.61*grados + 1647.1;
}

float msM2(float grados) {
  return 474.19*grados + 709.5;
}

float msM3(float grados) {
  return 634.86*grados + 1925.4;
}

float findQ1(float Px, float Py) {
  float q1=atan2(SHOULDER*Py, SHOULDER*Px);
  return q1;
}

float findQ3(float Px, float Py, float Pz, float q1) {
  float s1=sin(q1); float c1=cos(q1);

  float s3=(L2*L2+L3*L3-(c1*Px+s1*Py-L1)*(c1*Px+s1*Py-L1)-Pz*Pz)/(2*L2*L3);
  if (1-s3*s3>=0) {
    float q3=atan2(s3,ELBOW*sqrt(1-s3*s3));
    return q3;
  } else {
    Serial.println("angulo imposible");
    return;   //No se puede sacar ese ángulo
  }
}

float findQ2(float Px, float Py, float Pz, float q1, float q3) {
  float s1=sin(q1); float c1=cos(q1);
  float s3=sin(q3); float c3=cos(q3);
  
  float q2 = atan2(Pz,c1*Px+s1*Py-L1)-atan2(-L3*c3,L2-L3*s3);
  q2 = atan2(sin(q2),cos(q2));
  return -1*q2;
}

float max2(float n1, float n2) {
  if(n2>n1){return n2;}
  else{return n1;}
}
