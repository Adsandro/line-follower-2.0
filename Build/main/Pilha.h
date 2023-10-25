#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

#include <stdlib.h>
#include <string.h>

typedef struct node
{
    char buffer[18];
    struct node * proximo;

}Node;

typedef struct pilha
{
    Node * primeiro;
    Node * ultimo;
}Pilha;

Node * iniciaNode()
{
    return NULL;
}

Node * criaNode(Node * proximoNode, char charBffr[18])
{
    Node * novoNode = (Node*) malloc(sizeof(Node));
    strcpy(novoNode->buffer, charBffr);
    novoNode -> proximo = proximoNode;
    return novoNode;
}

Pilha * criaPilha()
{
    Pilha * novaPilha = (Pilha *) malloc(sizeof(Pilha));
    novaPilha->primeiro = NULL;
    novaPilha->ultimo = NULL;
    return novaPilha;
}

Pilha * empilha(Pilha * pilha, char charBffr[18])
{
    if(!pilha->primeiro)
    {
        pilha->primeiro = criaNode(pilha->ultimo, charBffr);
        return pilha;
    }

    Node * novo = criaNode(pilha->primeiro, charBffr);
    pilha->primeiro = novo;
    return pilha;

}

void desempilha(Pilha * pilha, char * buffer)
{
    if(!pilha->primeiro)
    {
        return;
    }

    char conteudo[18];
    strcpy(conteudo, pilha->primeiro->buffer);
    Node * nodeLimpar = pilha ->primeiro;
    pilha->primeiro = pilha->primeiro->proximo;
    free(nodeLimpar);
    strcpy(buffer, conteudo);
    return;
}