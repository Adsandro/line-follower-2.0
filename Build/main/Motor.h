#include <Arduino.h>


typedef enum {
  LENTO = 50,
  NORMAL = 120,
  RAPIDO = 160
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
