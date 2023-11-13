#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define SIZE 18

int tamanhoAtual = 0;

typedef struct Node
{
    uint8_t data[SIZE];
    struct Node * proximo;
}Node;

typedef Node * Pilha;

extern bool inserir(Pilha * minhaPilha, uint8_t * buffer, uint8_t * tamBuffer)
{
    Node * novoNode = (Node*) malloc(sizeof(Node));
    if(novoNode == NULL) return false;

    for(uint8_t i = 0; i < *tamBuffer; i++)
    {
        (novoNode->data)[i] = buffer[i];
    }

    novoNode->proximo = *minhaPilha;
    * minhaPilha = novoNode;
    tamanhoAtual++;
    return true;
}

extern uint8_t * remover(Pilha * minhaPilha)
{
    if(minhaPilha == NULL) return NULL;
    uint8_t * valor = (uint8_t*) malloc(sizeof(uint8_t) * SIZE);

    for(uint8_t i = 0; i < SIZE; i++)
    {
        valor[i] = ((*minhaPilha)->data)[i];
    }

    Node * tmp = * minhaPilha;
    (*minhaPilha) = (*minhaPilha)->proximo;
    free(tmp);
    tamanhoAtual--;
    return valor;
}
