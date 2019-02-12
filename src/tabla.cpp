/* 4927155 - 4976552 */

#include "../include/tabla.h"
#include "../include/info.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <cmath>

struct rep_tabla {
    cadena_t* tabla;
    nat tope;
    nat tamanio;
};

/*
 Crea una tabla_t de asociaciones numero->texto.
 Podrá haber hasta `tamanio' asociaciones.
 */
tabla_t crear_tabla(nat tamanio) {
    tabla_t t = new rep_tabla;
    t->tabla = new cadena_t[tamanio];
    for (nat i=0; i<tamanio; i++) 
        t->tabla[i] = NULL;
    t->tope = 0;
    t->tamanio = tamanio;
    return t;
}

/*
  Inserta en `t' la asociación entre `clave' y `valor'.
  Si ya existía una asociación para `clave' es reemplazada por la
  nueva.
  Precondición: ! esta_llena_tabla(t).
  El tiempo de ejecución es O(1).
 */
void asociar_en_tabla(int clave, char *valor, tabla_t &t) {
    info_t insertar = crear_info(clave,valor); //La frase 'valor' comparte memoria con la info
    nat indice = clave % t->tamanio;
    /* Si hay colisiones, agregamos a la lista en el bucket */
    if (t->tabla[indice] == NULL) {
        t->tabla[indice] = crear_cadena();
        t->tope++;  
    }
    else {
        localizador_t loc = inicio_cadena(t->tabla[indice]);
        while (es_localizador(loc) && numero_info(info_cadena(loc,t->tabla[indice])) != clave)
            loc = siguiente(loc,t->tabla[indice]);
        if (es_localizador(loc)) //Encontre un elemento con mi clave, entonces lo borro para insertar el nuevo
            remover_de_cadena(loc,t->tabla[indice]);
        else //Si no encontre mi elemento, aumento el tope, pues lo voy a insertar como un nuevo
            t->tope++;
    }
    //Agrego mi elemento al final de la cadena
    insertar_al_final(insertar,t->tabla[indice]);
}

/*
  Elimina de `t' la asociación entre `clave' y algún valor y libera la menoria
  asignada a mantener esa asociación.
  Precondición: existe_asociacion(clave, t).
  El tiempo de ejecución es O(1) en promedio.
 */
void eliminar_de_tabla(int clave, tabla_t &t) {
    t->tope--;
    nat indice = clave % t->tamanio;
    localizador_t loc = inicio_cadena(t->tabla[indice]);
    /* Busco la clave en mi bucket */
    while (es_localizador(loc) && numero_info(info_cadena(loc,t->tabla[indice])) != clave)
            loc = siguiente(loc,t->tabla[indice]);
    remover_de_cadena(loc,t->tabla[indice]);
}

/*
  Libera la memoria asignada a `t' y todos sus elementos.
 */
void liberar_tabla(tabla_t &t) {
    for(unsigned int i=0; i<t->tamanio; i++) {
        if (t->tabla[i] != NULL)
            liberar_cadena(t->tabla[i]);
    }
    delete[] t->tabla;
    delete t;
}

/*
  Devuelve `true' si y sólo si en `t' hay una asociación entre `clave'
  y algún valor.
  El tiempo de ejecución es O(1) en promedio.
 */
bool existe_asociacion(int clave, tabla_t t) {
    nat indice = clave % t->tamanio;
    if (t->tabla[indice] != NULL) {
        localizador_t loc = inicio_cadena(t->tabla[indice]);
        /* Busco mi elemento en el bucket */
        while (es_localizador(loc) && numero_info(info_cadena(loc,t->tabla[indice])) != clave)
                loc = siguiente(loc,t->tabla[indice]);
        return es_localizador(loc);
    }
    else //Porque ni siquiera esta creada la cadena
        return false;
}

/*
  Devuelve el valor correspondiente a la asociacion de `clave' en `t'.
  Precondición: existe_asociacion(clave, t).
  El tiempo de ejecución es O(1) en promedio.
 */
char *valor_en_tabla(int clave, tabla_t t) {
    nat indice = clave % t->tamanio;
    localizador_t loc = inicio_cadena(t->tabla[indice]);
    /* Busco mi elemento en el bucket */
    while (es_localizador(loc) && numero_info(info_cadena(loc,t->tabla[indice])) != clave)
        loc = siguiente(loc,t->tabla[indice]);
    return frase_info(info_cadena(loc,t->tabla[indice]));
}

/*
  Devuelve `true' si y sólo si `t' tiene `tamanio' elementos.
 */
bool esta_llena_tabla(tabla_t t) { return t->tope == t->tamanio; }


