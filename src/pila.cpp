/* 4976552 - 4927155 */

#include "../include/pila.h"

#include <string.h> // strlen, strcpy, strcmp, strcat
#include <stdio.h>  // sprintf, scanf, ungetc, stdin
#include <limits.h> // INT_MAX

struct rep_pila {
	char **string;
	int tamanio;
	int tope;
};

/*
  Devuelve una pila_t vacía (sin elementos) que puede tener hasta `tamanio'
  elementos.
 */
pila_t crear_pila(int tamanio){
    pila_t pila = new rep_pila;
    pila->string = new char*[tamanio];
    pila->tope = 0;
    pila->tamanio = tamanio;
    return pila;
}

/*
  Apila`t' en `p'.
  Si es_llena_pila(p) no hace nada.
 */
void apilar(char *t, pila_t &p){
    if (!es_llena_pila(p)){
        p->string[p->tope] = t;
        p->tope++;
    }
}

/*
  Remueve de `p' el elemento que está en la cima.
  Libera la memoria del elemento removido.
  Si es_vacia_pila(p) no hace nada.
 */
void desapilar(pila_t &p){
    if (!es_vacia_pila(p)){
        delete[] p->string[p->tope - 1];
        p->tope--;
    }    
}

/* Libera la memoria asignada a `p' y la de todos sus elementos. */
void liberar_pila(pila_t &p){
    while (p->tope > 0){
        desapilar(p);
        p->tope--;        
    }
    delete[] p->string;
    delete p;
}

/* Devuelve `true' si y sólo si `p' es vacía (no tiene elementos). */
bool es_vacia_pila(pila_t p){
  return p->tope == 0; }

/*
  Devuelve `true' si y sólo si la cantidad de elementos en `p' es `tamanio'
  (siendo `tamanio' el valor del parámetro con que fue creada `p').
 */
bool es_llena_pila(pila_t p){ 
  if (es_vacia_pila(p)) { return false; }
  else return p->tope == p->tamanio;
}

/*
  Devuelve el elemento que está en la cima de `p'.
  Precondición: ! es_vacia_pila(p);
 */
char *cima(pila_t p){ return p->string[p->tope - 1]; }


