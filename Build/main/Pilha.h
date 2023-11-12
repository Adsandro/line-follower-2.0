#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

typedef struct node {
  char buffer[18];
  struct node *proximo;
} Node;

typedef struct pilha {
  Node *primeiro;
  Node *ultimo;
} Pilha;

Node *iniciaNode() {
  return NULL;
}

Node *criaNode(Node *proximoNode, char charBffr[18]) {
  Node *novoNode = (Node *)malloc(sizeof(Node));
  strcpy(novoNode->buffer, charBffr);
  novoNode->proximo = proximoNode;
  return novoNode;
}

Pilha *criaPilha() {
  Pilha *novaPilha = (Pilha *)malloc(sizeof(Pilha));
  novaPilha->primeiro = NULL;
  novaPilha->ultimo = NULL;
  return novaPilha;
}

Pilha *empilha(Pilha *pilha, char charBffr[18]) {
  if (!pilha->primeiro) {
    pilha->primeiro = criaNode(pilha->ultimo, charBffr);
    pilha->ultimo = pilha->primeiro;
    return pilha;
  }

  Node *novo = criaNode(pilha->primeiro, charBffr);
  pilha->primeiro = novo;
  return pilha;
}

void desempilha(Pilha *pilha, char *buffer) {
  if (!pilha->primeiro) {
    return;
  }

  char conteudo[18];
  strcpy(conteudo, pilha->primeiro->buffer);
  Node *nodeLimpar = pilha->primeiro;

  if (pilha->primeiro == pilha->ultimo) {
    pilha->primeiro = NULL;
    pilha->ultimo = NULL;
  } else {
    pilha->primeiro = pilha->primeiro->proximo;
  }

  free(nodeLimpar);
  strcpy(buffer, conteudo);
  return;
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  Wire.begin();
}

void loop() {
  Pilha *minhaPilha = criaPilha();
  char buffer[18];

  while (1) {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      String uid = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
        uid += String(mfrc522.uid.uidByte[i], HEX);
      }
      mfrc522.PICC_HaltA();
      Serial.println("Cartão lido: " + uid);

      empilha(minhaPilha, uid.c_str());

      if (minhaPilha->primeiro) {
        Serial.println("----- Dados dos Cartões na Pilha -----");
        while (minhaPilha->primeiro) {
          desempilha(minhaPilha, buffer);
          Serial.println("UID: " + String(buffer));
        }
        Serial.println("--------------------------------------");
      }
    }
  }
}
