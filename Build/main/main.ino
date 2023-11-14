#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <HCSR04.h>
#include "Motor.h"
#include "Pilha.h"

#define SDA 10
#define SCK 13
#define MOSI 11
#define MISO 12
#define RST 9

#define ENA 6
#define IN1 8
#define IN2 7
#define IN3 4
#define IN4 2
#define ENB 5

#define SENSOR_ESQ 17 
#define SENSOR_MID 16 
#define SENSOR_DIR 15 

#define triggerPin 1
#define echoPin 3

int estadoEsq = 0, estadoDir = 0, estadoMeio = 0;

UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

Motor mtEsq(IN1, IN2, ENA), mtDir(IN4, IN3, ENB);

LiquidCrystal_I2C lcd(0x27, 16, 2);

Pilha minhaPilha;

MFRC522 mfrc522(SDA, RST);
MFRC522::MIFARE_Key key;

void setup()
{

  Serial.begin(9600);
  while(!Serial);

  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(0,0); lcd.print("Arduino iniciado");

  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(SENSOR_ESQ, INPUT);
  pinMode(SENSOR_MID, INPUT);
  pinMode(SENSOR_DIR, INPUT);

  mtDir.inicializa();
  mtEsq.inicializa();

  minhaPilha = NULL;

  for(byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  
}

void loop()
{
  if(minhaPilha != NULL && tamanhoAtual == 5)
  {
    while(minhaPilha != NULL)
    {
      escreve_lcd_dinamico(remover(&minhaPilha), 16);
      delay(3000);
    }
  }

  movimentaCarro();

  leRFID();
}

void frente()
{
  mtDir.giraFrente(90);
  mtEsq.giraFrente(90);
}

void tras()
{
  mtDir.giraTras(90);
  mtEsq.giraTras(90);
}

void para()
{
  mtDir.paraMotor();
  mtEsq.paraMotor();
}

void direita()
{
  mtDir.giraTras(95);
  mtEsq.giraFrente(95);
}

void esquerda()
{
  mtDir.giraFrente(95);
  mtEsq.giraTras(95);
}


void movimentaCarro()
{
  int distancia = distanceSensor.measureDistanceCm();
  estadoEsq = digitalRead(SENSOR_ESQ);
  estadoDir = digitalRead(SENSOR_DIR);
  estadoMeio = digitalRead(SENSOR_MID);

  if(distancia <= 6) 
    return;

  if(estadoEsq == 1 & estadoMeio == 1 & estadoDir == 1)
  {
    frente();
  }
  if(estadoEsq == 1 &  estadoMeio == 1 &  estadoDir == 0)
  {
    direita();
  }
  if(estadoEsq == 1 &  estadoMeio == 0 &  estadoDir == 1)
  {
    frente();  
  }
  if(estadoEsq == 1 &  estadoMeio == 0 &  estadoDir == 0)
  {
    frente();
  }
  if(estadoEsq == 0 &  estadoMeio == 1 &  estadoDir == 1)
  {
    esquerda();
  }
  if(estadoEsq == 0 &  estadoMeio == 1 &  estadoDir == 0)
  {
    frente();
  }
  if(estadoEsq == 0 &  estadoMeio == 0 &  estadoDir == 1)
  {
    frente();
  }
  if(estadoEsq ==  0 &  estadoMeio == 0 &  estadoDir == 0)
  {
    frente();
  }
}

void movimentaCarro2()
{
  int distancia = distanceSensor.measureDistanceCm();
  estadoEsq = digitalRead(SENSOR_ESQ);
  estadoDir = digitalRead(SENSOR_DIR);

  if(distancia <= 6) 
    return;

  if(estadoEsq == 0 & estadoDir == 0)
  {
    frente();
  }
  if (estadoEsq == 1 & estadoDir == 0)
  {
   esquerda(); 
  }
  if (estadoEsq == 0 & estadoDir == 1)
  {
    direita();
  }
  if (estadoEsq == 1 & estadoDir == 1)
  {
    frente();
  }
  
}

void escreve_lcd_dinamico(char *buffer, byte * tamanhoBuffer)
{
  int posX = 0, posY = 0;
  lcd.clear();

  if(tamanhoBuffer > 32) return;

  for(byte i = 0; i < tamanhoBuffer; i++)
  {
    if(posX == 15) posY = 1;
    posX++;
    lcd.print((char)buffer[i]);
  }
  free(buffer);
}


void leRFID()
{

  if(!mfrc522.PICC_IsNewCardPresent())
    return;

  if(!mfrc522.PICC_ReadCardSerial())
    return;

  MFRC522::StatusCode status;
  byte bloco = 4;
  status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(mfrc522.PICC_CMD_MF_AUTH_KEY_A, bloco, &key, &(mfrc522.uid));

  if(status != MFRC522::STATUS_OK)
  {
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("Falha ao");
    lcd.setCursor(0,1); lcd.print("autenticar");
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return;
  }

  lcd.clear();
  Serial.println("Autenticou");
  lcd.setCursor(0,0); lcd.print("Autenticado");


  byte buffer[18];
  byte tamanhoBuffer = sizeof(buffer);

  status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(bloco, buffer, &tamanhoBuffer);
  if(status != MFRC522::STATUS_OK)
  {
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("Falha na");
    lcd.setCursor(0,1); lcd.print("leitura");
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return;
  }

  inserir(&minhaPilha, buffer, &tamanhoBuffer);

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
}

