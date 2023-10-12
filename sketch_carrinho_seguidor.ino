#include <MFRC522.h>
#include <SPI.h>

//Pinagem do Módulo MFRC522 / Leitor ondas de rádio 13.56 MHz de frequência
#define SDA 10
#define SCK 13
#define MOSI 11
#define MISO 12
#define RST 9

MFRC522 leitor_mfrc522(SDA, RST);
MFRC522::MIFARE_Key chave;
byte buffer[18];
byte tamanhoDoBuffer = sizeof(buffer);

//Pinagem de output Módulo Ponte-H L298N
#define ENA 6
#define IN1 8
#define IN2 7
#define IN3 4
#define IN4 2
#define ENB 5

//Pinagem de input Sensores Infravermelho
#define SENSOR_ESQ 15
#define SENSOR_MID 16
#define SENSOR_DIR 17

// Variáveis para controlar os estados dos Sensores IR a fim de identificar presença ou ausência de cor
int estadoEsq = 0, estadoDir = 0, estadoMeio = 0;


// enum que define as velocidades padrões
typedef enum {
  LENTO = 50,
  NORMAL = 150,
  RAPIDO = 250
} Velocidade;

class Motor{

  public:
    int _inPrincipal, _inSecundario, _en; // O nPrincipal equivale ao pino nX da ponte-h que gira as rodas para a frente quando há voltagem (HIGH)

    // Construtor do motor, set inicial dos pins, "Motor::" é só uma forma de referenciar escopo explicitamente a fim de evitar erros de compilação
    Motor(int inPrincipal, int inSecundario, int en){
      Motor::_inPrincipal = inPrincipal;
      Motor::_inSecundario = inSecundario;
      Motor::_en = en;
    }

    // Inicializa o modo dos pins, use no setup();
    inicializa(){
      pinMode(_inPrincipal, OUTPUT);
      pinMode(_inSecundario, OUTPUT);
      pinMode(_en, OUTPUT);
    }

    giraFrente(Velocidade v){
      digitalWrite(_inPrincipal, HIGH);
      digitalWrite(_inSecundario, LOW);
      analogWrite(_en, v);
    }

    giraFrente(int v){
      digitalWrite(_inPrincipal, HIGH);
      digitalWrite(_inSecundario, LOW);
      analogWrite(_en, v);
    }

    giraTras(Velocidade v){
      digitalWrite(_inPrincipal, LOW);
      digitalWrite(_inSecundario, HIGH);
      analogWrite(_en, v);
    }

    giraTras(int v){
      digitalWrite(_inPrincipal, LOW);
      digitalWrite(_inSecundario, HIGH);
      analogWrite(_en, v);
    }

    paraMotor(){
      digitalWrite(_inSecundario, LOW);
      digitalWrite(_inPrincipal, LOW);
      analogWrite(_en, 255);
    }

};

Motor mtDir(IN1, IN2, ENA), mtEsq(IN4, IN3, ENB);

void frente(){
  mtDir.giraFrente(RAPIDO);
  mtEsq.giraFrente(RAPIDO);
}

void tras(){
  mtDir.giraTras(RAPIDO);
  mtEsq.giraTras(RAPIDO);
}

void para(){
  mtDir.paraMotor();
  mtEsq.paraMotor();
}

void direita(){
  mtDir.giraFrente(NORMAL);
  mtEsq.giraFrente(RAPIDO);
}

void esquerda(){
  mtDir.giraFrente(RAPIDO);
  mtEsq.giraFrente(NORMAL);
}


// 0 - PRESENÇA DE COR / TEM LINHA
// 1 - AUSÊNCIA DE COR / NÃO TEM LINHA

void movimentaCarro(){
  estadoEsq = digitalRead(SENSOR_ESQ);
  estadoDir = digitalRead(SENSOR_DIR);
  estadoMeio = digitalRead(SENSOR_MID);

  if(estadoEsq == 1 & estadoMeio == 1 & estadoDir == 1){
    tras();
  }
  else if(estadoEsq == 1 &  estadoMeio == 1 &  estadoDir == 0){
    esquerda();
  }
  else if(estadoEsq == 1 &  estadoMeio == 0 &  estadoDir == 1){
    frente();  
  }
  else if(estadoEsq == 1 &  estadoMeio == 0 &  estadoDir == 0){
    frente();
  }
  else if(estadoEsq == 0 &  estadoMeio == 1 &  estadoDir == 1){
    direita();
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

// Printa os bytes colocados no buffer pela leitura do PICC (cartão rfid)
void dumpBytesParaSerial(byte *buffer, byte tamanhoBuffer){
  for (byte i = 0; i < tamanhoBuffer; i++){
      Serial.print(" "); Serial.print(buffer[i], HEX);
  }
}

void dumpSetor(byte setor){
  //Faz dumping de todo os blocos de bytes de UM SETOR para o monitor
  Serial.println("Bytes do setor " + setor);
  leitor_mfrc522.PICC_DumpMifareClassicSectorToSerial(&(leitor_mfrc522.uid), &chave, setor);
  Serial.println();
}

MFRC522::StatusCode autenticaSetor(byte enderecoTrailerBlock){
  return (MFRC522::StatusCode) leitor_mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, enderecoTrailerBlock, &chave, &(leitor_mfrc522.uid));
}

MFRC522::StatusCode leCartaoRFID(byte setor, byte enderecoTrailerBlock, byte enderecoDoBloco){
  MFRC522::StatusCode status;

  status = autenticaSetor(enderecoTrailerBlock);
  if(status != MFRC522::STATUS_OK){
    Serial.println("Falha ao autenticar o setor " + setor);
    Serial.println(leitor_mfrc522.GetStatusCodeName(status));
    return status;
  }

  status = (MFRC522::StatusCode) leitor_mfrc522.MIFARE_Read(enderecoDoBloco, buffer, &tamanhoDoBuffer);
  if(status != MFRC522::STATUS_OK){
    Serial.println("Falha ao autenticar o setor " + setor);
    return status;
  }

  return status;
}

void setup(){

  Serial.begin(9600);
  while(!Serial);

  pinMode(SENSOR_ESQ, INPUT);
  pinMode(SENSOR_MID, INPUT);
  pinMode(SENSOR_DIR, INPUT);

  mtDir.inicializa();
  mtEsq.inicializa();

  SPI.begin();
  leitor_mfrc522.PCD_Init();

  // Aqui é definida que a chave possui o valor FFFFFFFFFFFF, chave padrão para acesso de um setor da memória do cartão
  for (byte i = 0; i < 6; i++) {
        chave.keyByte[i] = 0xFF;
    }
}


void loop(){
  movimentaCarro();

  if ( ! leitor_mfrc522.PICC_IsNewCardPresent())
        return;

  if ( ! leitor_mfrc522.PICC_ReadCardSerial())
      return;

  /*

    UM CHIP CLASSICO POSSUI 1MB DE ARMAZENAMENTO = 1024 BYTES (NEM TODOS PODEM SER ACESSADOS)

    1024 BYTES / 64 BLOCOS = 16 BYTES/BLOCO (CADA BLOCO POSSUI UM "ARRAY" DE 16 BYTES)

    OS BLOCOS SÃO SEPARADOS POR SETORES DE 4 BLOCOS DE MEMORIA

    64 BLOCOS / 4 BLOCOS/SETOR = 16 SETORES

    O ULTIMO BLOCO DE CADA SETOR É UM BLOCO DE AUTENTICAÇÃO (BLOCOS: 3, 7, 11, 15) (LEMBRANDO QUE AS CONTAGENS COMEÇAM EM 0)
    O ULTIMO BLOCO É CONHECIDO COMO TRAILER SECTOR E TAMBÉM DEFINE AS OPERAÇÕES POSSIVEIS DENTRO DOS BLOCOS DE SEU SETOR, PODENDO INCLUSIVE IMPEDIR A LEITURA E ESCRITA;


  */
  byte enderecoBlocoDeTrailer = 7; // O bloco que possui as chaves de autenticação para todos os outros 3 blocos do setor;
  byte setor = 1;
  byte enderecoBloco = 4;
  MFRC522::StatusCode status;


  
}