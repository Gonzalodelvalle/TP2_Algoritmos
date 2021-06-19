#include "osos_contra_reloj.h"
#include <time.h>


const int ARBOLES_INICIO = 350;
const int PIEDRAS_INICIO = 80;
const int KOALA_INICIO = 1;

const int PILAS_INICIO = 30;
const int VELAS_INICIO = 30;
const int BENGALAS_INICIO = 10;

const int MAX_FILAS = 20;
const int MAX_COLUMNAS = 30;

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
    for (int i = 0; i < MAX_FILAS; i++){
        printf("%i", i + 1);
        for (int j = 0; j < MAX_COLUMNAS; j++){
            printf(" %c", bosque[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void cargar_elementos_al_bosque(char bosque[MAX_FILAS][MAX_COLUMNAS], elemento_del_mapa_t obstaculos[MAX_OBSTACULOS] , int cantidad_obstaculos){
    for (int i = 0; i < cantidad_obstaculos; i++){
       if (i < ARBOLES_INICIO){
           bosque[obstaculos[i].posicion.fil][obstaculos[i].posicion.col] = ARBOL;
       } else if (i >= ARBOLES_INICIO && i < (ARBOLES_INICIO + PIEDRAS_INICIO)){
           bosque[obstaculos[i].posicion.fil][obstaculos[i].posicion.col] = PIEDRA;
       } else {
           bosque[obstaculos[i].posicion.fil][obstaculos[i].posicion.col] = KOALA;
       }
    }
}

void mostrar_juego(juego_t juego){
    
    char bosque[MAX_FILAS][MAX_COLUMNAS];
    generar_bosque(bosque);

    cargar_elementos_al_bosque(bosque, juego.obstaculos, juego.cantidad_obstaculos);

    imprimir_bosque(bosque);
}

bool verificar_posicion(elemento_del_mapa_t vector[], int tope_vector, int fila, int columna){
    bool ocupado = false;
    for (int i = 0; i < tope_vector; i++){
        if ((vector[i].posicion.fil) == fila && (vector[i].posicion.col == columna)){
            ocupado = true;
        } else {
            ocupado = false;
        }
    }
    return ocupado;
    
}

void agregar_obstaculos(elemento_del_mapa_t obstaculos[MAX_OBSTACULOS], int *cantidad_obstaculos, int obstaculos_iniciales){
    for ((*cantidad_obstaculos) = 0; (*cantidad_obstaculos) < obstaculos_iniciales; (*cantidad_obstaculos)++){
        
        int fila = rand() % 20;
        int columna = rand() % 30;
        while (verificar_posicion(obstaculos, *cantidad_obstaculos, fila, columna)){
            int fila = rand() % 20;
            int columna = rand() % 30;
        }

        if ((*cantidad_obstaculos) < ARBOLES_INICIO){
            obstaculos[(*cantidad_obstaculos)].tipo = ARBOL;
        } else if ((*cantidad_obstaculos) >= ARBOLES_INICIO && (*cantidad_obstaculos) <= (obstaculos_iniciales - 1)){
            obstaculos[(*cantidad_obstaculos)].tipo = PIEDRA;
        } else {
            obstaculos[(*cantidad_obstaculos)].tipo = KOALA;
        }

        obstaculos[(*cantidad_obstaculos)].posicion.col = columna;
        obstaculos[(*cantidad_obstaculos)].posicion.fil = fila;
        obstaculos[(*cantidad_obstaculos)].visible = false;

    }
}

void agregar_recolectables(elemento_del_mapa_t herramientas[MAX_HERRAMIENTAS], int *cantidad_herramientas, int herramientas_iniciales){
    for ((*cantidad_herramientas) = 0; (*cantidad_herramientas) < herramientas_iniciales; (*cantidad_herramientas)++){
        int fila = rand() % 20;
        int columna = rand() % 30;
        while (verificar_posicion(herramientas, *cantidad_herramientas, fila, columna)){
            int fila = rand() % 20;
            int columna = rand() % 30;
        }
        
        if ((*cantidad_herramientas) < ARBOLES_INICIO){
            herramientas[(*cantidad_herramientas)].tipo = ARBOL;
        } else if ((*cantidad_herramientas) >= ARBOLES_INICIO && (*cantidad_herramientas) <= (herramientas_iniciales - 1)){
            herramientas[(*cantidad_herramientas)].tipo = PIEDRA;
        } else {
            herramientas[(*cantidad_herramientas)].tipo = KOALA;
        }

        herramientas[(*cantidad_herramientas)].posicion.col = columna;
        herramientas[(*cantidad_herramientas)].posicion.fil = fila;
        herramientas[(*cantidad_herramientas)].visible = false;
    }
    
}

/*
void completar_vectores(juego_t *juego, int tope_vector){}
*/

void inicializar_juego(juego_t* juego, char tipo_personaje){
    int obstaculos_iniciales = (ARBOLES_INICIO + PIEDRAS_INICIO + KOALA_INICIO);
    int herramientas_iniciales = (PILAS_INICIO + VELAS_INICIO + BENGALAS_INICIO);
    agregar_obstaculos(juego->obstaculos, &(juego->cantidad_obstaculos), obstaculos_iniciales);
    //agrega_recolectables(juego->herramientas, &(juego->cantidad_herramientas), herramientas_iniciales);
}
