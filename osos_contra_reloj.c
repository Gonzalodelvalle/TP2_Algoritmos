#include "osos_contra_reloj.h"
#include <time.h>


const int ARBOLES_INICIO = 350;
const int PIEDRAS_INICIO = 80;
const int KOALA_INICIO = 1;

const int PILAS_INICIO = 30;
const int VELAS_INICIO = 30;
const int BENGALAS_INICIO = 10;

const int MAX_FILAS = 21;
const int MAX_COLUMNAS = 31;

#define OBSTACULO 'O'
#define HERRAMIENTA 'H'

#define ARBOL 'A'
#define PIEDRA 'R'
#define KOALA 'K'

#define LINTERNA 'L'
#define PILA 'B'
#define VELA 'V'
#define BENGALA 'E'


void generar_bosque(char bosque[MAX_FILAS][MAX_COLUMNAS]){
    for (int i = 0; i < MAX_FILAS; i++){
        for (int j = 0; j < MAX_COLUMNAS; j++){
            bosque[i][j] = '-';
        }
    }
}

void imprimir_bosque(char bosque[MAX_FILAS][MAX_COLUMNAS]){
    printf("  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30\n");
    for (int i = 1; i < MAX_FILAS; i++){
        printf("%i", i);
        for (int j = 1; j < MAX_COLUMNAS; j++){
            printf(" %c", bosque[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void cargar_elementos_al_bosque(char bosque[MAX_FILAS][MAX_COLUMNAS], elemento_del_mapa_t vector[] , int tope_vector, char tipo_obstaculo){
    
    if (tipo_obstaculo == OBSTACULO){
        for (int i = 0; i < tope_vector; i++){
            bosque[vector[i].posicion.fil][vector[i].posicion.col] = vector[i].tipo;
        }
        
    } else if (tipo_obstaculo == HERRAMIENTA){
        for (int i = 0; i < tope_vector; i++){
            //printf("columna: %i, fila: %i del vector en la posicion: %i se pone letra %c\n", vector[i].posicion.col, vector[i].posicion.fil, i, vector[i].tipo);
            bosque[vector[i].posicion.fil][vector[i].posicion.col] = vector[i].tipo;
        }
    }
}


void completar_vectores(juego_t juego, elemento_del_mapa_t vector[], int *tope_vector, int cantidad_inicial, char tipo_elemento){

    while ((*tope_vector) <  cantidad_inicial){
        int fila = rand() % 21;
        int columna = rand() % 31;             

        if (tipo_elemento == OBSTACULO){
            if ((*tope_vector) < ARBOLES_INICIO){
                vector[(*tope_vector)].tipo = ARBOL;
            } else if ((*tope_vector) >= ARBOLES_INICIO && (*tope_vector) < (cantidad_inicial - KOALA_INICIO)){
                vector[(*tope_vector)].tipo = PIEDRA;
            } else {
                vector[(*tope_vector)].tipo = KOALA;
            }
        } else if (tipo_elemento == HERRAMIENTA){
            if ((*tope_vector) < PILAS_INICIO){
                vector[(*tope_vector)].tipo = PILA;
            } else if ((*tope_vector) >= PILAS_INICIO && (*tope_vector) < (cantidad_inicial - BENGALAS_INICIO)){
                vector[(*tope_vector)].tipo = VELA;
            } else {
                vector[(*tope_vector)].tipo = BENGALA;
            }
        }
        
        vector[(*tope_vector)].posicion.col = columna;
        vector[(*tope_vector)].posicion.fil = fila;
        vector[(*tope_vector)].visible = false;

        *tope_vector += 1;
         
    }
    
}

int encontrar_repetidas(juego_t juego){
    int i = 0;
    int j = 0;
    int posicion_repetida = -1;
    while ((posicion_repetida == -1) && (i < juego.cantidad_obstaculos)){
        while ((posicion_repetida == -1) && (j < juego.cantidad_herramientas)){
            if ((juego.obstaculos[i].posicion.fil) == (juego.herramientas[j].posicion.fil) && (juego.obstaculos[i].posicion.col) == (juego.herramientas[j].posicion.col)){
                int posicion_repetida = i;
            }
            j++;                
        }
        i++;
    }
    return posicion_repetida;  
}

void cambiar_repetidas(juego_t *juego, int posicion_repetida){
    juego->obstaculos[posicion_repetida].posicion.fil = rand() % 21;
    juego->obstaculos[posicion_repetida].posicion.col = rand() % 31;
}





void inicializar_juego(juego_t* juego, char tipo_personaje){
    int obstaculos_iniciales = (ARBOLES_INICIO + PIEDRAS_INICIO + KOALA_INICIO);
    int herramientas_iniciales = (PILAS_INICIO + VELAS_INICIO + BENGALAS_INICIO);
    printf("\n cantidad de obstaculos %i \n", juego->cantidad_obstaculos);
    completar_vectores(*juego, juego->obstaculos, &(juego->cantidad_obstaculos), obstaculos_iniciales, OBSTACULO);
    printf("\n cantidad de obstaculos despues de cargar %i \n", juego->cantidad_obstaculos);
    
    printf("\n cantidad de herramientas %i \n", juego->cantidad_herramientas);
    completar_vectores(*juego, juego->herramientas, &(juego->cantidad_herramientas), herramientas_iniciales, HERRAMIENTA);
    printf("\n cantidad de herramientas desp de cargar %i \n", juego->cantidad_herramientas);
    
    int posicion_repetida = encontrar_repetidas(*juego);
    while (posicion_repetida != -1){
        cambiar_repetidas(juego, posicion_repetida);
    } 
    
}

void mostrar_juego(juego_t juego){
    
    char bosque[MAX_FILAS][MAX_COLUMNAS];
    generar_bosque(bosque);

    cargar_elementos_al_bosque(bosque, juego.obstaculos, juego.cantidad_obstaculos, OBSTACULO);

    cargar_elementos_al_bosque(bosque, juego.herramientas, juego.cantidad_herramientas, HERRAMIENTA);

    imprimir_bosque(bosque);
}
