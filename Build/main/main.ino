#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <HCSR04.h>
#include "Motor.h"

//Pinagem do Módulo MFRC522 / Leitor ondas de rádio 13.56 MHz de frequência
#define SDA 10
#define SCK 13
#define MOSI 11
#define MISO 12
#define RST 9

//Pinagem de output Módulo Ponte-H L298N
#define ENA 6
#define IN1 8
#define IN2 7
#define IN3 4
#define IN4 2
#define ENB 5

//Pinagem de input Sensores Infravermelho
#define SENSOR_ESQ 17 //A3
#define SENSOR_MID 16 //A2
#define SENSOR_DIR 15 // A1 

#define triggerPin 1
#define echoPin 3


// Variáveis para controlar os estados dos Sensores IR a fim de identificar presença ou ausência de cor
int estadoEsq = 0, estadoDir = 0, estadoMeio = 0;



UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

// Objetos dos motores
Motor mtEsq(IN1, IN2, ENA), mtDir(IN4, IN3, ENB);

void frente(){
  mtDir.giraFrente(90);
  mtEsq.giraFrente(90);
}

void tras(){
  mtDir.giraTras(90);
  mtEsq.giraTras(90);
}

void para(){
  mtDir.paraMotor();
  mtEsq.paraMotor();
}

void direita(){
  //mtDir.paraMotor();
  mtDir.giraTras(95);
  //mtDir.giraFrente(100);
  mtEsq.giraFrente(95);
}

void esquerda(){
  mtDir.giraFrente(95);
  //mtEsq.giraFrente(100);
  // mtEsq.paraMotor();
  mtEsq.giraTras(95);
}


// 0 - PRESENÇA DE COR / TEM LINHA
// 1 - AUSÊNCIA DE COR / NÃO TEM LINHA

void movimentaCarro(){
  
  estadoEsq = digitalRead(SENSOR_ESQ);
  estadoDir = digitalRead(SENSOR_DIR);
  estadoMeio = digitalRead(SENSOR_MID);

  if(estadoEsq == 1 & estadoMeio == 1 & estadoDir == 1){
    frente();
  }
  else if(estadoEsq == 1 &  estadoMeio == 1 &  estadoDir == 0){
    direita();
  }
  else if(estadoEsq == 1 &  estadoMeio == 0 &  estadoDir == 1){
    frente();  
  }
  else if(estadoEsq == 1 &  estadoMeio == 0 &  estadoDir == 0){
    frente();
  }
  else if(estadoEsq == 0 &  estadoMeio == 1 &  estadoDir == 1){
    esquerda();
  }
  else if(estadoEsq == 0 &  estadoMeio == 1 &  estadoDir == 0){
    frente();
  }
  else if(estadoEsq == 0 &  estadoMeio == 0 &  estadoDir == 1){
    frente();
  }
  else if(estadoEsq ==  0 &  estadoMeio == 0 &  estadoDir == 0){
    frente();
  }
  
}

void movimentaCarro2()
{
  int distancia = distanceSensor.measureDistanceCm();
  estadoEsq = digitalRead(SENSOR_ESQ);
  estadoDir = digitalRead(SENSOR_DIR);

  Serial.print("Esquerdo: "); Serial.println(estadoEsq);
  Serial.print("Direito: "); Serial.println(estadoDir);
  /*
  if(distancia <= 10.0)
  {
    para();
    Serial.println(distancia);
    return;
  }
  */

  
  if(estadoEsq == 0 & estadoDir == 0)
  {
    frente();
  }
  else if (estadoEsq == 1 & estadoDir == 0)
  {
   esquerda(); 
  }
  else if (estadoEsq == 0 & estadoDir == 1)
  {
    direita();
  }
  else if (estadoEsq == 1 & estadoDir == 1)
  {
  frente();
  }
  

}



void setup()
{

  Serial.begin(9600);
  while(!Serial);

  pinMode(SENSOR_ESQ, INPUT);
  pinMode(SENSOR_MID, INPUT);
  pinMode(SENSOR_DIR, INPUT);

  mtDir.inicializa();
  mtEsq.inicializa();

}


void loop()
{

  movimentaCarro2();
  
}