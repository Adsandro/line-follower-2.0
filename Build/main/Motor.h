#include <Arduino.h>

class Motor{

  public:
    int _inPrincipal, _inSecundario, _en; 

    Motor(int inPrincipal, int inSecundario, int en){
      Motor::_inPrincipal = inPrincipal;
      Motor::_inSecundario = inSecundario;
      Motor::_en = en;
    }

    inicializa(){
      pinMode(_inPrincipal, OUTPUT);
      pinMode(_inSecundario, OUTPUT);
      pinMode(_en, OUTPUT);
    }

    giraFrente(int v){
      digitalWrite(_inPrincipal, HIGH);
      digitalWrite(_inSecundario, LOW);
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
