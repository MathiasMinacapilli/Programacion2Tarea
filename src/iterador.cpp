/* 4976552 - 4927155 */

#include "../include/iterador.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>

struct nodo_iter{
	char *frase;
	nodo_iter *sig;
};

struct rep_iterador {
	nodo_iter *inicio, *actual;
};

/* Implementaciones Mathi */

iterador_t crear_iterador() {
    iterador_t aux = new rep_iterador;
    aux->inicio = aux->actual = NULL;
    return aux;
}

void reiniciar_iter(iterador_t &i) {
    if (i->inicio != NULL) {
        i->actual = i->inicio;
    }
}

char *actual_iter(iterador_t &i) {
    return i->actual->frase;
}

bool esta_definida_actual(iterador_t i) {
    return i->actual != NULL;
}

static void liberar_iterador_aux (iterador_t &i) {
    if (esta_definida_actual (i)) {
        nodo_iter *elim = i->actual;
        avanzar_iter (i);
        //Libero la memoria
        delete[] elim->frase;
        delete elim;
        liberar_iterador_aux (i);
    }
}

void liberar_iterador(iterador_t &i) {
    //Reinicio 'actual'
    reiniciar_iter (i);
    //Si hay elementos los elimino
    if (esta_definida_actual (i)) { 
        liberar_iterador_aux (i);
    }
    //Reinicio punteros
    i->actual = i->inicio = NULL;
    delete i;
}

/* Implementaciones Sofi */

/*
  Agrega `t' al final de `i'. No se modifica la posición actual.
 */
void agregar_a_iterador(char *t, iterador_t &i){
    nodo_iter *nuevo = new nodo_iter;
    nuevo->frase = t;
    nuevo->sig = NULL;
    if (i->inicio == NULL)
        i->inicio = nuevo;
    else{
        nodo_iter *aux = i->inicio;
        while (aux->sig != NULL)
            aux = aux->sig;
        aux->sig = nuevo;
    }
}

/*
  Avanza la posición actual hacia el siguiente.
  Si ! hay_siguiente_en_iter(i) la posición actual queda indefinida;
  Si la posición actual no está definida no hace nada.
 */
void avanzar_iter(iterador_t &i){
    if (esta_definida_actual(i))
        i->actual = i->actual->sig; //Si ! hay_siguiente_en_iter(i) queda en NULL
}

/*
  Devuelve `true' si y sólo si en `i' hay un elemento siguiente al de la
  posición actual.
  Precondición: esta_definida_actual(i).
 */
bool hay_siguiente_en_iter(iterador_t i){
    return i->actual->sig != NULL;
}

/*
  Devuelve una cadena con todos los elementos que se pueden recorrer con `i'.
  El dato numérico de los elementos debe ser 0.
  Al terminar, la posicion actual debe permanecer donde estaba al empezar la
  operación.
  La cadena resultado no debe compartir memoria con `i'.
 */
cadena_t iterador_a_cadena(iterador_t i){
    cadena_t cad = crear_cadena();
    nodo_iter *aux = i->inicio;
    while(aux != NULL){
        char *copia_frase = new char[strlen(aux->frase) + 1]; //creo frase nueva para cad
        strcpy(copia_frase, aux->frase);
        info_t inf = crear_info(0,copia_frase);
        insertar_al_final(inf, cad);
        aux = aux->sig;
    }
    return cad;
}
