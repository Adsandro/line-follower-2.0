#include "Pilha.h"
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

#define SDA 10
#define SCK 13
#define MOSI 11
#define MISO 12
#define RST 9

LiquidCrystal_I2C lcd(0x27, 16, 2); // Um display lcd de 16x2, 0x27 é um endereço padrão do módulo I2C, pode falhar como não testei

Pilha minhaPilha;

MFRC522 mfrc522(SDA, RST);
MFRC522::MIFARE_Key key;


void setup() 
{

  Serial.begin(9600);
  while(!Serial);

  Serial.println("Monitor ligado");

  SPI.begin();

  mfrc522.PCD_Init();

  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(0,0); lcd.print("Arduino iniciado");
  
  minhaPilha = NULL;

  for(byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  
  dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);

  
}

void loop() 
{

  if(minhaPilha != NULL && tamanhoAtual == 3)
  {
    while(minhaPilha != NULL)
    {
      //dump_byte_array_as_char(remover(&minhaPilha), 16);
      escreve_lcd_dinamico(remover(&minhaPilha), 16);
      delay(5000);
    }
  }

  leRFID();
  
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  Serial.println("DUMPING TO SERIAL: ");
  for (byte i = 0; i < bufferSize; i++) {
      Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      Serial.print(buffer[i], HEX);
  }
  Serial.println();
}

void dump_byte_array_as_char(byte *buffer, byte bufferSize) {
  Serial.println();
  for (byte i = 0; i < bufferSize; i++) {
      Serial.print((char) buffer[i]);
  }
  Serial.println();
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
    Serial.println("Falha ao autenticar");
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("Falha ao");
    lcd.setCursor(0,1); lcd.print("autenticar");
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return NULL;
  }

  lcd.clear();
  Serial.println("Autenticou");
  lcd.setCursor(0,0); lcd.print("Autenticado");


  byte buffer[18];
  byte tamanhoBuffer = sizeof(buffer);

  status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(bloco, buffer, &tamanhoBuffer);
  if(status != MFRC522::STATUS_OK)
  {
    Serial.println("Falha na leitura");
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("Falha na");
    lcd.setCursor(0,1); lcd.print("leitura");
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return NULL;
  }

  Serial.println();
  dump_byte_array(buffer, tamanhoBuffer);
  Serial.println();

  inserir(&minhaPilha, buffer, &tamanhoBuffer);

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
}
