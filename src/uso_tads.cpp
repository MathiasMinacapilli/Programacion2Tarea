/* 4976552 - 4927155 */

#include "../include/uso_tads.h"

#include "../include/pila.h"
#include "../include/cola_binarios.h"
#include "../include/cola_prioridad.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>

/* **************  NUEVO  ************** */
/*
  Devuelve una cadena_t ordenada de manera creciente según los valores numéricos
  de sus elementos con los hasta `k' elementos de `cad' cuyos valores numéricos
  sean mayores entre los que están en el rango 0 .. `max'.
  Si en `cad' hay más de un elemento con el mismo valor numérico el único de
  ellos que se debe incluir en la cadena_t resultado es el que aparece primero
  en `cad'.
  La cadena_t resultado no comparte memoria con `cad'.
  El tiempo de ejecución es O(n.log k), donde `n' es la cantidad de elementos de
  `cad'.
  Precondición: k > 0.
 */
cadena_t mayores(cadena_t cad, unsigned int k, unsigned int max) {
    cadena_t cad_rango = crear_cadena();
    unsigned int n = longitud(cad);
    if (n > 0){
        cola_prioridad_t cp = crear_cp(n,max);
        unsigned int tope = 0;//cuenta la cantidad de elems de cp
        localizador_t loc = inicio_cadena(cad);
        /* Copio los elementos que estan dentro del rango a cp*/
        while (es_localizador(loc)) {
            unsigned int num = numero_info(info_cadena(loc,cad));
            if (0 <= num && num <= max && !hay_prioridad(num,cp)) { //Inserto solo los elementos que estan dentro del rango
                info_t i = copia_info(info_cadena(loc,cad));
                insertar_en_cp(i,num,cp);
                tope++;
            }
            loc = siguiente(loc, cad);
        }
        /* Ordeno mi cadena */
        /* Elimino los primeros tope-k elementos menores, y quedan los k(o menos) mayores */
        int i=1;
        int resta = tope - k + 1;
        while (i < resta){
            eliminar_prioritario(cp);
            i++;
        };
        while (!es_vacia_cp(cp)) {
            info_t info = copia_info(prioritario(cp));
            insertar_al_final(info,cad_rango);
            eliminar_prioritario(cp);
        }
        liberar_cp(cp);
    };
    return cad_rango;
}

/* ----------------- IMPLEMENTACIONES SOFI ----------------- */

static void inverso_de_iter_aux(int a, int b, iterador_t it, iterador_t &inverso){
	if (esta_definida_actual(it)) {
		if (a <= 1 && b >= 1){
			char *copia_frase = new char[strlen(actual_iter(it)) + 1]; //creo frase nueva para cad
        	strcpy(copia_frase, actual_iter(it));
			avanzar_iter(it);
			inverso_de_iter_aux(a-1, b-1, it, inverso);
			agregar_a_iterador(copia_frase, inverso);
		}
		else{
			avanzar_iter(it);
			inverso_de_iter_aux(a-1, b-1, it, inverso);
		}
    }	
}

iterador_t inverso_de_iter(nat a, nat b, iterador_t &it){
	reiniciar_iter(it);
	iterador_t inverso = crear_iterador();
	inverso_de_iter_aux(a, b, it, inverso);
	reiniciar_iter(it);
	reiniciar_iter(inverso);
	return inverso;
}

bool esta_ordenada_por_frase(cadena_t cad){
    bool ordenada = true;
    if (!es_vacia_cadena(cad)) {
    	localizador_t aux = inicio_cadena(cad);
    	while( ordenada && siguiente(aux, cad) != NULL){
    		if (strcmp(frase_info(info_cadena(aux, cad)), frase_info(info_cadena(siguiente(aux, cad), cad))) >= 0 )
    			ordenada = false;
    		aux = siguiente(aux, cad);
    	}
    }
	return ordenada;
}

bool pertenece(int i, cadena_t cad){
	localizador_t aux = inicio_cadena(cad);
	bool pert = false;
	while( !pert && aux != NULL){
		if (numero_info(info_cadena(aux, cad)) == i)
			pert = true;
		aux = siguiente(aux, cad);
	}
	return pert;
}

/* ----------------- IMPLEMENTACIONES MATHI ----------------- */

iterador_t rango_en_conjunto(char *primero, char *ultimo, conjunto_t c) {
    if(!es_vacio_conjunto(c)) {
        /* Pasamos el conjunto al tipo iterador */
        iterador_t res_iter = iterador_conjunto(c); //No comparte memoria con 'c'
        /* Pasamos de iterador a cadena */
        cadena_t res_cad = iterador_a_cadena(res_iter); //No comparte memoria con 'res_iter'
        liberar_iterador(res_iter);
        /* Busco el localizador_t en la cadena que sea el primero */
        localizador_t prim = inicio_cadena(res_cad);
        while(es_localizador(prim) && strcmp(frase_info(info_cadena(prim,res_cad)),primero) < 0) 
            prim = siguiente(prim,res_cad);
        /* Busco el ultimo */
        localizador_t ult = prim;
        while(es_localizador(ult) && strcmp(frase_info(info_cadena(ult,res_cad)),ultimo) < 0 && es_localizador(siguiente(ult,res_cad))) 
            ult = siguiente(ult,res_cad);
        /* Saco el segmento que quiero de la cadena y libero la memoria */
        cadena_t segmento = crear_cadena();
        if (es_localizador(prim)) {
            liberar_cadena(segmento); //Porque habia creado un cabezal, pero con segmento cadena me devuelve un cabezal
            segmento = segmento_cadena(prim,ult,res_cad);
        }
        liberar_cadena(res_cad);
        /* Paso la cadena (la cual es el rango que quiero) a un iterador_t */
        res_iter = crear_iterador();
        localizador_t iter_segmento = final_cadena(segmento);
        while(es_localizador(iter_segmento)){
            char* fras = new char[strlen(frase_info(info_cadena(iter_segmento,segmento))) + 1];
            strcpy(fras, frase_info(info_cadena(iter_segmento,segmento)));
            agregar_a_iterador(fras,res_iter);
            iter_segmento = anterior(iter_segmento,segmento);
        }
        /* Libero la memoria del segmento */
        liberar_cadena(segmento);
        return res_iter;
    }
    else {
        iterador_t res_iter = crear_iterador();
        return res_iter;
    }
}

static void imprimir_por_niveles_aux(binario_t b, cola_binarios_t &cola, pila_t &pila) {
    binario_t nodo_actual;
    encolar(b,cola); //Agrego primero la raiz del arbol a mi cola.
    /* Voy a agregar elementos a la cola mientras no se me hayan ido todos */
    while (!es_vacia_cola_binarios(cola)) {
        nodo_actual = frente(cola);
        desencolar(cola);
        /* SI LO IMPRIMO ME QUEDA DE ARRIBA HACIA ABAJO, 
        TENGO QUE PONERLO EN UNA PILA Y LUEGO IMPRIMIR LA PILA*/
        char *frase = new char[strlen(frase_info(raiz(nodo_actual))) + 1];
        strcpy (frase,frase_info(raiz(nodo_actual)));
        apilar(frase,pila); //No comparte memoria.
        /* Agrego "el siguiente nivel" a la cola */
        if (!es_vacio_binario(derecho(nodo_actual))) {
            encolar(derecho(nodo_actual),cola);
        }
        if (!es_vacio_binario(izquierdo(nodo_actual))) {
            encolar(izquierdo(nodo_actual),cola);
        }
    } //while
    /* Imprimo la pila */
    while (!es_vacia_pila(pila)) {
            printf("%s ", cima(pila));
            char *ex_cima = new char[strlen(cima(pila)) + 1];
            strcpy(ex_cima,cima(pila));
            desapilar(pila); //Libera la memoria del string.
            if (!es_vacia_pila(pila) && strcmp(ex_cima,cima(pila)) > 0)
                printf("\n");
            else if (!es_vacia_pila(pila)){
                    binario_t izq_menor_niv = buscar_subarbol(cima(pila),b);
                    if ( !es_vacio_binario(izquierdo(izq_menor_niv))){
                        izq_menor_niv = izquierdo(buscar_subarbol(cima(pila),b));
                        if(strcmp(frase_info(raiz(izq_menor_niv)),ex_cima) == 0){ printf("\n"); }
                    }
            }
            delete[] ex_cima;
    }//while
}

/*
  Imprime las frases de los nodos de `b' en orden decreciente de
  niveles (desde las hojas hasta la raíz), un nivel por línea.
  En cada nivel los textos se imprimen en orden lexicográfico creciente.
  Después de cada texto se imprime un espacio en blanco.
  Si `b' es vacío no imprime nada.
 */
void imprimir_por_niveles(binario_t b) {
    /* Recorrida postOrder de derecha a izquierda (Derecha-Izquierda-Raiz) */
    if (!es_vacio_binario(b)) { //Si `b' es vacío no imprime nada.
        cola_binarios_t cola = crear_cola_binarios(); //Creo la cola
        pila_t pila = crear_pila(cantidad_binario(b)); //Creo la pila de tamaño igual al del arbol
        imprimir_por_niveles_aux(b,cola,pila);
        liberar_cola_binarios(cola);
        liberar_pila(pila);
    }

}

void ordenar(cadena_t &cad) {
    if(!es_vacia_cadena(cad)){ //Si es vacia la cadena no hago nada
        localizador_t loc = inicio_cadena(cad);
        localizador_t menor;
        while(es_localizador(siguiente(loc,cad))){
            //Busco el menor elemento
            menor = menor_en_cadena(loc, cad);
            /* Cambio el menor elemento con loc que esta en la primer 
            posicion despues de los que estan ordenados */
            intercambiar(loc,menor,cad);
            loc = siguiente(loc, cad);
        }//while
    }//if
}

nat longitud(cadena_t cad) {
    nat longi = 0;
	localizador_t iter = inicio_cadena(cad);
	while (es_localizador(iter)){
		longi++;
		iter = siguiente(iter,cad);
	};
	return longi;
}

bool son_iguales(cadena_t c1, cadena_t c2) {
    localizador_t loc1,loc2;
    loc1 = inicio_cadena(c1);
    loc2 = inicio_cadena(c2);
    //Si cualquiera de los dos se hace NULL es que una de las dos es mas chica que la otra
    while( es_localizador(loc1) && es_localizador(loc2) && son_iguales(info_cadena(loc1, c1), info_cadena(loc2, c2))){
        loc1 = siguiente(loc1, c1);
        loc2 = siguiente(loc2, c2);
    }
    /* Si los dos son NULL es porque llegaron al final de las dos listas y son iguales todos sus info_t */
    if (!es_localizador(loc1) && !es_localizador(loc2)){
        return true;
    }
    else{
        return false;
    }
}

cadena_t concatenar(cadena_t c1, cadena_t c2) {
    cadena_t nueva = crear_cadena();
	localizador_t iter = inicio_cadena(c1);
	while (es_localizador(iter)){
		insertar_al_final(copia_info(info_cadena(iter,c1)),nueva);
		iter = siguiente(iter,c1);
	};//copio la primer cadena
	iter = inicio_cadena(c2);
		while (es_localizador(iter)){
			insertar_al_final(copia_info(info_cadena(iter,c2)),nueva);
			iter = siguiente(iter,c2);
		};//copio la segunda cadena
	return nueva;
}

void cambiar_todos(int original, int nuevo, cadena_t &cad) {
    localizador_t iter = inicio_cadena(cad);
	while (es_localizador(iter)){
		if (numero_info(info_cadena(iter, cad)) == original){
            info_t eliminar = info_cadena(iter, cad); //Guardo el que voy a eliminar
            char *frase = new char[strlen(frase_info(info_cadena(iter, cad))) + 1];
            strcpy(frase, frase_info(info_cadena(iter, cad)));
            info_t cambio = crear_info(nuevo,frase);
            /* la frase de 'cambio' NO tiene la misma direccion que la de 'iter->dato->frase') */
            cambiar_en_cadena(cambio,iter,cad);
            liberar_info(eliminar);
        };
		iter = siguiente(iter,cad);
	};
}

cadena_t subcadena(int menor, int mayor, cadena_t cad) {
    //Encuentro el primer elemento de la subcadena
    localizador_t desde = inicio_cadena(cad);
    while(numero_info(info_cadena(desde, cad)) < menor){
        desde = siguiente(desde, cad);
    }
    //Encuentro el ultimo elemento de la subcadena
    localizador_t hasta = final_cadena(cad);
    while(numero_info(info_cadena(hasta, cad)) > mayor){
        hasta = anterior(hasta, cad);
    }
    //Retorno el segmento de la cadena desde 'desde' hasta 'hasta'(incluidos)
    return segmento_cadena(desde, hasta, cad);
}









