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

const int LINTERNA_EN_MOCHILA = 1;
const int VELA_EN_MOCHILA = 4;
const int VELA_EN_MOCHILA_POLAR = 6;
const int BENGALA_EN_MOCHILA_PANDA = 2;

const int MOVIMIENTOS_LINTERNA = 10;
const int MOVIMIENTOS_LINTERNA_PARDO = 15;
const int MOVIMIENTOS_VELA = 5;
const int MOVIMIENTOS_BENGALA = 3;

const int DESACTIVADO = -1;

#define OBSTACULO 'O'
#define HERRAMIENTA 'H'

#define ARBOL 'A'
#define PIEDRA 'R'
#define KOALA 'K'

#define LINTERNA 'L'
#define PILA 'B'
#define VELA 'V'
#define BENGALA 'E'

#define POLAR 'I'
#define PANDA 'P'
#define PARDO 'G'
#define CHLOE 'C'

#define MOVER_ARRIBA 'W'
#define MOVER_IZQUIERDA 'A'
#define MOVER_ABAJO 'S'
#define MOVER_DERECHA 'D'
#define MOSTRA_TIEMPO 'T'

//Antes de inciar el juego
/*
.PRE: Recibe un vector vacio y el tipo de elemento (OBSTACULO o HERRAMIENTA) determina como va a ser completado 
.POST: Devuelve el vector con todos sus elementos inicializados 
*/
void completar_vectores(elemento_del_mapa_t vector[], int *tope_vector, int cantidad_inicial, char tipo_elemento){

    while ((*tope_vector) <  cantidad_inicial){
        int fila = rand() % 20;
        int columna = rand() % 30;             

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

/*
.PRE:
.POST: Asigna una fila y una columna random a la posicion de Chloe 
*/
void posicionar_chloe(int *fila_chloe, int *columna_chloe){
    *fila_chloe = rand() % 20;
    *columna_chloe = rand() % 30;
}

/*
.PRE: 
.POST: Asigna al personaje una fila random y en la columna 0
*/
void fila_inicial_personaje(int *fila_personaje){
    *fila_personaje = rand() % 20;
}

/*
.PRE: Se reciben los 2 vectores ya inicializados y se verifican que no hayan posiciones repetidas entre ellos
.POST: Devuelve la posicion del primer vector en el cual se encontro una posicion repetida
*/
int encontrar_repetidas(juego_t *juego){
    int i = 0;
    int j = 0;
    int posicion_repetida = -1;
    while ((posicion_repetida == -1) && (i < juego->cantidad_obstaculos)){
        while ((posicion_repetida == -1) && (j < juego->cantidad_herramientas)){
            if ((juego->obstaculos[i].posicion.fil == juego->herramientas[j].posicion.fil) && (juego->obstaculos[i].posicion.col == juego->herramientas[j].posicion.col)){
                posicion_repetida = i;
            } else if ((juego->obstaculos[i].posicion.fil == juego->amiga_chloe.fil) && (juego->obstaculos[i].posicion.col == juego->amiga_chloe.col)){
                posicion_repetida = i;
            } else if ((juego->herramientas[j].posicion.fil == juego->amiga_chloe.fil) && (juego->herramientas[j].posicion.col == juego->amiga_chloe.col)){
                posicionar_chloe(&(juego->amiga_chloe.fil), &(juego->amiga_chloe.col));
            } else if ((juego->obstaculos[i].posicion.fil == juego->personaje.posicion.fil) && (juego->obstaculos[i].posicion.col == juego->personaje.posicion.col)){
                posicion_repetida = i;
            } else if ((juego->herramientas[j].posicion.fil == juego->personaje.posicion.fil) && (juego->herramientas[j].posicion.col == juego->personaje.posicion.col)){
                fila_inicial_personaje(&(juego->personaje.posicion.fil));
            }            
            j++;                
        }
        i++;
        j = 0;
    }

    return posicion_repetida;  
}

/*
.PRE: recibe la posicion del vector donde hubo una coincidencia  
.POST: devuelve el vector  con una fila y una columna nuevas
*/
void cambiar_repetidas(elemento_del_mapa_t obstaculos[MAX_OBSTACULOS], int posicion_repetida){
    obstaculos[posicion_repetida].posicion.fil = rand() % 20;
    obstaculos[posicion_repetida].posicion.col = rand() % 30;
}

/*
.PRE: Recibe el vector mochila vacio
.POST: Devuelve el vector completado con los elementor dependiendo de la variable "personalidad"
*/
void completar_mochila(elemento_mochila_t mochila[MAX_HERRAMIENTAS], int *cantidad_elementos, char personalidad){
    *cantidad_elementos = 1;
    if (personalidad == PARDO){
        mochila[0].tipo = LINTERNA;
        mochila[0].movimientos_restantes = MOVIMIENTOS_LINTERNA_PARDO;
        for (int i = 1; i < VELA_EN_MOCHILA; i++){
            mochila[i].tipo = VELA;
            mochila[i].movimientos_restantes = MOVIMIENTOS_VELA;
            *cantidad_elementos += 1;
        }
    } else {
        mochila[0].tipo = LINTERNA;
        mochila[0].movimientos_restantes = MOVIMIENTOS_LINTERNA;
        if (personalidad == POLAR){
            for (int i = 1; i < VELA_EN_MOCHILA_POLAR; i++){
                mochila[i].tipo = VELA;
                mochila[i].movimientos_restantes = MOVIMIENTOS_VELA;
                *cantidad_elementos += 1;
            }
        } else if (personalidad == PANDA){
            for (int i = 1; i < (VELA_EN_MOCHILA + BENGALA_EN_MOCHILA_PANDA); i++){
                if (i <= VELA_EN_MOCHILA){
                    mochila[i].tipo = VELA;
                    mochila[i].movimientos_restantes = MOVIMIENTOS_VELA;
                } else {
                    mochila[i].tipo = BENGALA;
                    mochila[i].movimientos_restantes = MOVIMIENTOS_BENGALA;
                }
                *cantidad_elementos += 1;
            }
        }
    }
}

/*
.PRE:
.POST: Imprime por pantalla los distintos comandos que tiene el juego
*/
void mensajes_al_usuario(){
    printf("Pulsa para activar o moverte\n");

    printf("Herramientas: Linterna (%c), Vela (%c), Bengala (%c)\n", LINTERNA, VELA, BENGALA);
    
    printf("Movimientos: Arriba (%c), Abajo (%c), Izquierda (%c), Derecha (%c)\n", MOVER_ARRIBA, MOVER_ABAJO, MOVER_IZQUIERDA, MOVER_DERECHA);

    printf("Ver tiempo actual: Tiempo (%c)\n", MOSTRA_TIEMPO);
}

void inicializar_personaje(personaje_t* personaje, char personalidad){
    personaje->tipo = personalidad;
    fila_inicial_personaje(&(personaje->posicion.fil));
    personaje->posicion.col = 0;
    personaje->cantidad_elementos = 0;
    completar_mochila(personaje->mochila, &(personaje->cantidad_elementos), personalidad);
    personaje->elemento_en_uso = -1;
    personaje->tiempo_perdido = 0;
}

void inicializar_chloe(int *fila_chloe, int *col_chloe, bool *chloe_visible){
    posicionar_chloe(fila_chloe, col_chloe);
    *chloe_visible = true;
}

void inicializar_juego(juego_t* juego, char personalidad){
    int obstaculos_iniciales = (ARBOLES_INICIO + PIEDRAS_INICIO + KOALA_INICIO);
    int herramientas_iniciales = (PILAS_INICIO + VELAS_INICIO + BENGALAS_INICIO);

    completar_vectores(juego->obstaculos, &(juego->cantidad_obstaculos), obstaculos_iniciales, OBSTACULO);
    completar_vectores(juego->herramientas, &(juego->cantidad_herramientas), herramientas_iniciales, HERRAMIENTA);
    inicializar_chloe(&(juego->amiga_chloe.fil), &(juego->amiga_chloe.col), &(juego->chloe_visible));
    inicializar_personaje(&(juego->personaje), personalidad);
    printf("El personaje es: %c\n", personalidad);
    
    int posicion_repetida = encontrar_repetidas(juego);
    while (posicion_repetida != -1){
        cambiar_repetidas(juego->obstaculos, posicion_repetida);
        posicion_repetida = encontrar_repetidas(juego);
    }    
    printf("Personaje en la fila: %i y col: %i\n", juego->personaje.posicion.fil + 1, juego->personaje.posicion.col + 1);
    printf("Chloe en la fila: %i y col: %i\n", juego->amiga_chloe.fil + 1, juego->amiga_chloe.col + 1);
}



//Bosque
/*
.PRE:
.POST: Creo la matriz "Bosque"
*/
void generar_bosque(char bosque[MAX_FILAS][MAX_COLUMNAS]){
    for (int i = 0; i < MAX_FILAS; i++){
        for (int j = 0; j < MAX_COLUMNAS; j++){
            bosque[i][j] = '-';
        }
    }
}

/*
.PRE: La matriz bosque tiene que tener elementos cargados
.POST: Se imprime la matriz por pantalla
*/
void imprimir_bosque(char bosque[MAX_FILAS][MAX_COLUMNAS]){
    printf("\n  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30\n");
    for (int i = 0; i < MAX_FILAS; i++){
        printf("%i", i + 1);
        for (int j = 0; j < MAX_COLUMNAS; j++){
            printf(" %c", bosque[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

/*
.PRE: Recibe la matriz "bosque" vacia y el juego con sus elementos inicializado para posicionarlos en la matriz
.POST: Dependiendo el tipo de elemento que se pase, se ubican en la matriz
*/
void cargar_elementos_al_bosque(juego_t juego, char bosque[MAX_FILAS][MAX_COLUMNAS]){
    
    for (int i = 0; i < (juego.cantidad_obstaculos); i++){
        if (juego.obstaculos[i].visible){
            bosque[juego.obstaculos[i].posicion.fil][juego.obstaculos[i].posicion.col] = juego.obstaculos[i].tipo;
        }
    }
    
    for (int i = 0; i < (juego.cantidad_herramientas); i++){
        if (juego.herramientas[i].visible){
            bosque[juego.herramientas[i].posicion.fil][juego.herramientas[i].posicion.col] = juego.herramientas[i].tipo;
        }
    }

    if (juego.chloe_visible){
        bosque[juego.amiga_chloe.fil][juego.amiga_chloe.col] = CHLOE;
    }
    
    bosque[juego.personaje.posicion.fil][juego.personaje.posicion.col] = juego.personaje.tipo;
}



//Una vez iniciado el juego
void mostrar_juego(juego_t juego){
    
    char bosque[MAX_FILAS][MAX_COLUMNAS];
    generar_bosque(bosque);

    cargar_elementos_al_bosque(juego, bosque);

    imprimir_bosque(bosque);

    mensajes_al_usuario();

    printf("Personaje en la fila: %i y col: %i\n", juego.personaje.posicion.fil + 1, juego.personaje.posicion.col + 1);
    
}

int estado_juego(juego_t juego){

    int chloe_encontrada = 0;
    if ((juego.personaje.posicion.fil == juego.amiga_chloe.fil) && (juego.personaje.posicion.col == juego.amiga_chloe.col)){
        return chloe_encontrada = -1;
    } else {
        return chloe_encontrada;
    }
}

void mover_arriba(personaje_t *personaje){
    personaje->posicion.fil--;
    personaje->ultimo_movimiento = MOVER_ARRIBA;
}

void mover_abajo(personaje_t *personaje){
    personaje->posicion.fil++;
    personaje->ultimo_movimiento = MOVER_ABAJO;
}

void mover_izquierda(personaje_t *personaje){
    personaje->posicion.col--;
    personaje->ultimo_movimiento = MOVER_IZQUIERDA;
}

void mover_derecha(personaje_t *personaje){
    personaje->posicion.col++;
    personaje->ultimo_movimiento = MOVER_DERECHA;
}

void buscar_herramienta_en_mochila(elemento_mochila_t mochila[MAX_HERRAMIENTAS], int elementos_en_mochila, char elemento_buscado, int *posicion_elemento){
    bool encontrado = false;
    int i = 0;
    while ((!encontrado) && (i < elementos_en_mochila)){
        if ((mochila[i].tipo == elemento_buscado) && (mochila[i].movimientos_restantes != 0)){
            encontrado = true;
            *posicion_elemento = i;
        }
        i++;
    }
}

void iluminar_elemento(elemento_del_mapa_t vector[], personaje_t personaje, int cantidad_elementos){
    int i = 0;
    while (i < cantidad_elementos){
        if ((vector[i].posicion.fil == personaje.posicion.fil) && (vector[i].posicion.col == personaje.posicion.col - 1)){
            vector[i].visible = true;
        } else if ((vector[i].posicion.fil == personaje.posicion.fil) && (vector[i].posicion.col == personaje.posicion.col + 1)){
            vector[i].visible = true;
        } else if ((vector[i].posicion.fil == personaje.posicion.fil - 1) && (vector[i].posicion.col == personaje.posicion.col + 1)){
            vector[i].visible = true;
        } else if ((vector[i].posicion.fil == personaje.posicion.fil - 1) && (vector[i].posicion.col == personaje.posicion.col - 1)){
            vector[i].visible = true;
        } else if ((vector[i].posicion.fil == personaje.posicion.fil - 1) && (vector[i].posicion.col == personaje.posicion.col)){
            vector[i].visible = true;
        } else if ((vector[i].posicion.fil == personaje.posicion.fil + 1) && (vector[i].posicion.col == personaje.posicion.col)){
            vector[i].visible = true;
        } else if ((vector[i].posicion.fil == personaje.posicion.fil + 1) && (vector[i].posicion.col == personaje.posicion.col - 1)){
            vector[i].visible = true;
        } else if ((vector[i].posicion.fil == personaje.posicion.fil + 1) && (vector[i].posicion.col == personaje.posicion.col + 1)){
            vector[i].visible = true;
        } else {
            vector[i].visible = false;
        }
        i++;
    }
}

void iluminar_posicion(juego_t *juego){
    iluminar_elemento(juego->obstaculos, juego->personaje, juego->cantidad_obstaculos);
    iluminar_elemento(juego->herramientas, juego->personaje, juego->cantidad_herramientas);
}

void ocultar_elementos(elemento_del_mapa_t vector[], int cantidad_elementos){
    for (int i = 0; i < cantidad_elementos; i++){
        vector[i].visible = false;
    }
    
}

void usar_vela(juego_t *juego){
    if (juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes != 0){
    juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes -= 1;
    iluminar_posicion(juego);
    } else {
        juego->personaje.elemento_en_uso = -1;
        ocultar_elementos(juego->obstaculos, juego->cantidad_obstaculos);
        ocultar_elementos(juego->herramientas, juego->cantidad_herramientas);
    }
    
}

void encender_vela(juego_t *juego){
    buscar_herramienta_en_mochila(juego->personaje.mochila, juego->personaje.cantidad_elementos, VELA, &(juego->personaje.elemento_en_uso));
    if (juego->personaje.elemento_en_uso != -1){    
        usar_vela(juego);
    } else {
        printf("No hay mas velas en la mochila");
    }    
}

void realizar_jugada(juego_t *juego, char jugada){
    
    if (jugada == MOVER_ARRIBA){
        mover_arriba(&(juego->personaje));
        if (juego->personaje.elemento_en_uso != -1){
            if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == VELA){
                usar_vela(juego);        
            }  
        }
    } else if (jugada == MOVER_ABAJO){
        mover_abajo(&(juego->personaje));
        if (juego->personaje.elemento_en_uso != -1){
            if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == VELA){
                usar_vela(juego);        
            }  
        }
    } else if (jugada == MOVER_IZQUIERDA){
        mover_izquierda(&(juego->personaje));
        if (juego->personaje.elemento_en_uso != -1){
            if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == VELA){
                usar_vela(juego);        
            }  
        }
    }  else if (jugada == MOVER_DERECHA){
        mover_derecha(&(juego->personaje));
        if (juego->personaje.elemento_en_uso != -1){
            if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == VELA){
                usar_vela(juego);        
            }  
        }
    } else if (jugada == VELA){
        encender_vela(juego);
    }
    
}

/*
.PRE:
.POST:
*/