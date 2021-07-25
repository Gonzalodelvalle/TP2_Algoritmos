#include "osos_contra_reloj.h"
#include <time.h>
#include "utiles.h"

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

const double MEDIO_SEG_PERDIDO = 0.5;
const double SEGUNDO_PERDIDO = 1;
const double DOS_SEG_PERDIDOS = 2;

const int POSICION_LINTERNA = 0;
const int BATERIA_AGREGADA = 5;

const int PRIMER_FILA = 0;
const int ULTIMA_FILA = 19;
const int PRIMER_COLUMNA = 0;
const int ULTIMA_COLUMNA = 29;

const int MAXIMA_DISTANCIA_M = 3;

const int SIN_MOVIMIENTOS = 0;

const double TIEMPO_GPS = 30;

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
#define MOSTRAR_TIEMPO 'T'

//Antes de inciar el juego
/*
.PRE: tope deben ser > 0, fila debe estar entre (PRIMER_FILA y ULTIMA_FILA) y col debe estar entre (PRIMER_COLUMNA y ULTIMA_FILA) 
.POST: Busca si fila y col coinciden con algun elemento del vector en ese caso devuelve true
*/
bool comparar_con_vector(elemento_del_mapa_t vector[], int tope_vector, int fila, int col){
    int i = 0;
    bool ocupado = false;
    while ((i < tope_vector) && (!ocupado)){
        if ((vector[i].posicion.fil == fila) && (vector[i].posicion.col == col)){
            ocupado = true;
        }  
        i++;
    }
    return ocupado;
}

/*
.PRE: la posicion en vector debe ser >= 0, la fila debe estar entre PRIMER_FILA Y ULTIMA_FILA y la col entre PRIMER_COLUMNA y ULTIMA_COLUMNA
.POST: Devuelve true si las fila y la columna del elemento del vector en "posicion_vector" coincide con las del personaje
*/
bool posicion_ocupada(elemento_del_mapa_t vector[], int posicion_vector, int fil_personaje, int col_personaje){
    bool posicion_coincide = false;
    if ((vector[posicion_vector].posicion.fil == fil_personaje) && (vector[posicion_vector].posicion.col == col_personaje)){
        posicion_coincide = true;
    }
    return posicion_coincide;
}

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

        (*tope_vector)++;
         
    }
    
}

/*
.PRE:
.POST: Asigna una fila y una columna random a la posicion de Chloe 
*/
void posicionar_chloe(int *fila_chloe, int *columna_chloe){
    (*fila_chloe) = rand() % 20;
    (*columna_chloe) = rand() % 30;
}

/*
.PRE: 
.POST: Asigna al personaje una fila random
*/
void fila_inicial_personaje(int *fila_personaje){
    (*fila_personaje) = rand() % 20;
}

/*
.PRE: posicion_repetida debe estar entre 0 y el tope del vector 
.POST: devuelve el vector  con una fila y una columna nuevas
*/
void cambiar_repetidas(elemento_del_mapa_t vector[], int posicion_repetida){
    vector[posicion_repetida].posicion.fil = rand() % 20;
    vector[posicion_repetida].posicion.col = rand() % 30;
}

/*
.PRE: fin debe ser el tope del vector recibido
.POST: Devuelve true en caso de que coincidan las posiciones del vector en el fin y en el inicio
*/
bool repetidas_en_vectores(elemento_del_mapa_t vector[], int inicio, int fin){
    bool hay_repetida = false;
    if (inicio < fin){
        if (posicion_ocupada(vector, fin, vector[inicio].posicion.fil, vector[inicio].posicion.col)){
            hay_repetida = true;
        }
    }
    return hay_repetida;
}

/*
.PRE: Se recibe el juego en un estado valido
.POST: Verifica si hay posiciones repetidas
*/
void encontrar_repetidas(juego_t *juego){
    int i = 0;
    while (i < juego->cantidad_obstaculos){
        if (repetidas_en_vectores(juego->obstaculos, i, juego->cantidad_obstaculos)){
            cambiar_repetidas(juego->obstaculos, i);
        } else if (repetidas_en_vectores(juego->herramientas, i, juego->cantidad_herramientas)){
            cambiar_repetidas(juego->herramientas, i);
        } else if (comparar_con_vector(juego->herramientas, juego->cantidad_herramientas, juego->obstaculos[i].posicion.fil, juego->obstaculos[i].posicion.col)){
            cambiar_repetidas(juego->obstaculos, i);
        } else if (posicion_ocupada(juego->obstaculos, i, juego->personaje.posicion.fil, juego->personaje.posicion.col)){
            cambiar_repetidas(juego->obstaculos, i);
        } else if (posicion_ocupada(juego->obstaculos, i, juego->amiga_chloe.fil, juego->amiga_chloe.col)){
            cambiar_repetidas(juego->obstaculos, i);
        } else {
            i++;
        }  
    }
}

/*
.PRE: Cantidad_elementos debe ser > o igual a cero
.POST: Completa los elementos en la mochila
*/
void cargar_elemento_en_mochila(elemento_mochila_t mochila[MAX_HERRAMIENTAS], int posicion_en_mochila,  char tipo_elemento, int movimientos_elemento, int *cantidad_elementos){
    mochila[posicion_en_mochila].tipo = tipo_elemento;
    mochila[posicion_en_mochila].movimientos_restantes = movimientos_elemento;
    (*cantidad_elementos)++;
}

/*
.PRE: Recibe el vector mochila vacio
.POST: Devuelve el vector completado con los elementor dependiendo de la variable "personalidad"
*/
void completar_mochila(elemento_mochila_t mochila[MAX_HERRAMIENTAS], int *cantidad_elementos, char personalidad){
    if (personalidad == PARDO){
        cargar_elemento_en_mochila(mochila, POSICION_LINTERNA, LINTERNA, MOVIMIENTOS_LINTERNA_PARDO, cantidad_elementos);
        for (int i = 1; i < VELA_EN_MOCHILA; i++){
            cargar_elemento_en_mochila(mochila, i, VELA, MOVIMIENTOS_VELA, cantidad_elementos);
        }
    } else {
        cargar_elemento_en_mochila(mochila, POSICION_LINTERNA, LINTERNA, MOVIMIENTOS_LINTERNA, cantidad_elementos);
        if (personalidad == POLAR){
            for (int i = 1; i <= VELA_EN_MOCHILA_POLAR; i++){
                cargar_elemento_en_mochila(mochila, i, VELA, MOVIMIENTOS_VELA, cantidad_elementos);
            }
        } else if (personalidad == PANDA){
            for (int i = 1; i <= (VELA_EN_MOCHILA + BENGALA_EN_MOCHILA_PANDA); i++){
                if (i <= VELA_EN_MOCHILA){
                    cargar_elemento_en_mochila(mochila, i, VELA, MOVIMIENTOS_VELA, cantidad_elementos);
                } else {
                    cargar_elemento_en_mochila(mochila, i, BENGALA, MOVIMIENTOS_BENGALA, cantidad_elementos);
                }
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

    printf("Ver tiempo actual: Tiempo (%c)\n", MOSTRAR_TIEMPO);
}

/*
.PRE: El struct personaje esta sin inicializar, personalidad puede ser 'I', 'P' o 'G' 
.POST: Carga todos los datos iniciales al struct personaje
*/
void inicializar_personaje(personaje_t* personaje, char personalidad){
    personaje->tipo = personalidad;
    fila_inicial_personaje(&(personaje->posicion.fil));
    personaje->posicion.col = 0;
    personaje->cantidad_elementos = 0;
    completar_mochila(personaje->mochila, &(personaje->cantidad_elementos), personalidad);
    personaje->elemento_en_uso = -1;
    personaje->tiempo_perdido = 0;
    personaje->ultimo_movimiento = MOVER_DERECHA;
}

/*
.PRE: 
.POST: Establecemos la posicion de Chloe y su estado (no visible)
*/
void inicializar_chloe(int *fila_chloe, int *col_chloe, bool *chloe_visible){
    posicionar_chloe(fila_chloe, col_chloe);
    *chloe_visible = false;
}

/*
.PRE: Personalidad debe ser valida (PANDA, PARDO O POLAR)
.POST: Completa todos los elementos del struct juego con sus datos iniciales y los deja en un estado valido paracomenzar el juego
*/
void inicializar_juego(juego_t* juego, char personalidad){
    int obstaculos_iniciales = (ARBOLES_INICIO + PIEDRAS_INICIO + KOALA_INICIO);
    int herramientas_iniciales = (PILAS_INICIO + VELAS_INICIO + BENGALAS_INICIO);

    completar_vectores(juego->obstaculos, &(juego->cantidad_obstaculos), obstaculos_iniciales, OBSTACULO);
    completar_vectores(juego->herramientas, &(juego->cantidad_herramientas), herramientas_iniciales, HERRAMIENTA);
    inicializar_chloe(&(juego->amiga_chloe.fil), &(juego->amiga_chloe.col), &(juego->chloe_visible));
    inicializar_personaje(&(juego->personaje), personalidad);
    
    encontrar_repetidas(juego); 
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
    for (int i = 0; i < MAX_FILAS; i++){
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
}

int estado_juego(juego_t juego){

    int chloe_encontrada = 0;
    if ((juego.personaje.posicion.fil == juego.amiga_chloe.fil) && (juego.personaje.posicion.col == juego.amiga_chloe.col)){
        return chloe_encontrada = -1;
    } else {
        return chloe_encontrada;
    }
}

/*
.PRE: 
.POST: Cambia la posicion y el ultimo movimieno del personaje
*/
void mover_arriba(personaje_t *personaje){
    personaje->posicion.fil--;
    personaje->ultimo_movimiento = MOVER_ARRIBA;
}

/*
.PRE: 
.POST: Cambia la posicion y el ultimo movimieno del personaje
*/
void mover_abajo(personaje_t *personaje){
    personaje->posicion.fil++;
    personaje->ultimo_movimiento = MOVER_ABAJO;
}

/*
.PRE: 
.POST: Cambia la posicion y el ultimo movimieno del personaje
*/
void mover_izquierda(personaje_t *personaje){
    personaje->posicion.col--;
    personaje->ultimo_movimiento = MOVER_IZQUIERDA;
}

/*
.PRE: 
.POST: Cambia la posicion y el ultimo movimieno del personaje
*/
void mover_derecha(personaje_t *personaje){
    personaje->posicion.col++;
    personaje->ultimo_movimiento = MOVER_DERECHA;
}



//Usar herramientas en la mochila
/*
.PRE: elementos en mochila debe ser mayor a 0, elemento buscado debe ser (VELA, LINTERNA o BENGALA)
.POST: Pone la posicion en el vector del elemento que se busco, en uso
*/
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

/*
.PRE: cantidad de elementos debe ser mayor a 0 
.POST: Hace visibles las posiciones donde ilumina la vela
*/
void iluminar_con_vela(elemento_del_mapa_t vector[], personaje_t personaje, int cantidad_elementos){
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

/*
.PRE: Cantidad de elementos debe ser mayor a 0 y la fila_actual debe estar entre PRIMER_FILA y ULTIMA_FILA 
.POST: hace visibles los elementos que estan en las filas anteriores a la del personaje
*/
void linterna_arriba(elemento_del_mapa_t vector[], personaje_t personaje, int cantidad_elementos, int fila_actual){
    while (fila_actual >= 0){
        for (int i = 0; i < cantidad_elementos; i++){
            if ((vector[i].posicion.fil == fila_actual) && (vector[i].posicion.col == personaje.posicion.col)){
                vector[i].visible = true;
            }
        }
        fila_actual -= 1;
    }                
}

/*
.PRE: Cantidad de elementos debe ser mayor a 0 y la fila_actual debe estar entre PRIMER_FILA y ULTIMA_FILA 
.POST: hace visibles los elementos que estan en las filas siguientes a la del personaje
*/
void linterna_abajo(elemento_del_mapa_t vector[], personaje_t personaje, int cantidad_elementos, int fila_actual){
    while (fila_actual < MAX_FILAS){
        for (int i = 0; i < cantidad_elementos; i++){
            if ((vector[i].posicion.fil == fila_actual) && (vector[i].posicion.col == personaje.posicion.col)){
                vector[i].visible = true;
            }
        }
        fila_actual += 1;
    }
}

/*
.PRE: Cantidad de elementos debe ser mayor a 0 y la col_actual debe estar entre PRIMER_COLUMNA y ULTIMA_COLUMNA 
.POST: hace visibles los elementos que estan en las columnas siguientes a la del personaje
*/
void linterna_derecha(elemento_del_mapa_t vector[], personaje_t personaje, int cantidad_elementos, int col_actual){
    while (col_actual < MAX_COLUMNAS){
        for (int i = 0; i < cantidad_elementos; i++){
            if ((vector[i].posicion.fil == personaje.posicion.fil) && (vector[i].posicion.col == col_actual)){
                vector[i].visible = true;
            }
        }
        col_actual += 1;   
    }
}

/*
.PRE: Cantidad de elementos debe ser mayor a 0 y la col_actual debe estar entre PRIMER_COLUMNA y ULTIMA_COLUMNA 
.POST: hace visibles los elementos que estan en las columnas anteriores a la del personaje
*/
void linterna_izquierda(elemento_del_mapa_t vector[], personaje_t personaje, int cantidad_elementos, int col_actual){
    while (col_actual >= 0){
        for (int i = 0; i < cantidad_elementos; i++){
            if ((vector[i].posicion.fil == personaje.posicion.fil) && (vector[i].posicion.col == col_actual)){
                vector[i].visible = true;
            }
        }
        col_actual -= 1;   
    }
}

/*
.PRE: cantidad_elementos debe ser mayor a 0  
.POST: Ilumina el mapa dependiendo nuestro ultimo movimiento
*/
void iluminar_con_linterna(elemento_del_mapa_t vector[], personaje_t personaje, int cantidad_elementos){
    int fila_actual = personaje.posicion.fil;
    int col_actual = personaje.posicion.col;
    if (personaje.ultimo_movimiento == MOVER_ARRIBA){
        linterna_arriba(vector, personaje, cantidad_elementos, fila_actual);
    } else if (personaje.ultimo_movimiento == MOVER_ABAJO){
        linterna_abajo(vector, personaje, cantidad_elementos, fila_actual);
    } else if (personaje.ultimo_movimiento == MOVER_DERECHA){
        linterna_derecha(vector, personaje, cantidad_elementos, col_actual);
    } else if (personaje.ultimo_movimiento == MOVER_IZQUIERDA){
        linterna_izquierda(vector, personaje, cantidad_elementos, col_actual);
    }    
}

/*
.PRE: fila_a_iluminar y fila_bengala deben estar entre (PRIMER_FILA y ULTIMA_FILA) y col_a_iluminar y col_bengala deben estar entre (PRIMER_COLUMNA y ULTIMA_COLUMNA) 
.POST: Devuelve la distancia Manhattan entre ellas
*/
int calcular_distancia_m(int fila_a_iluminar, int col_a_iluminar, int fila_bengala, int col_bengala){
    int valor_filas = abs((fila_bengala - fila_a_iluminar));
    int valor_cols = abs((col_bengala - col_a_iluminar));
    return valor_filas + valor_cols;
}

/*
.PRE:  fila_a_iluminar y fila_bengala deben estar entre (PRIMER_FILA y ULTIMA_FILA) y col_a_iluminar y col_bengala deben estar entre (PRIMER_COLUMNA y ULTIMA_COLUMNA) 
.POST: Devuelve true si la distancia calculada es menor o igual a MAXIMA_DISTANCIA_M
*/
bool dentro_del_rango(int fila_a_iluminar, int col_a_iluminar, int fila_bengala, int col_bengala){
    int distancia = calcular_distancia_m(fila_a_iluminar, col_a_iluminar, fila_bengala, col_bengala);
    return (distancia <= MAXIMA_DISTANCIA_M);   
}

/*
.PRE: fila_bengala debe estar entre (PRIMER_FILA y ULTIMA_FILA) y col_bengala debe estar entre (PRIMER_COLUMNA y ULTIMA_COLUMNA), cantidad_elementos debe ser mayor a 0
.POST: Hace visible los elementos que esten dentro del rango
*/
void iluminar_con_bengala(elemento_del_mapa_t vector[], int cantidad_elementos, int fila_bengala, int col_bengala){    
    for (int i = 0; i < cantidad_elementos; i++){
        if (dentro_del_rango(vector[i].posicion.fil, vector[i].posicion.col, fila_bengala, col_bengala)){
            vector[i].visible = true;
        }
    }
}

/*
.PRE: cantidad_elementos debe ser mayor a 0 y y herramienta_usada debe ser (VELA o LINTERNA)
.POST: Ilumina dependiendo de la herramienta usada
*/
void iluminar_elemento(elemento_del_mapa_t vector[], personaje_t personaje, int cantidad_elementos, char herramienta_usada){
    if (herramienta_usada == VELA){
        iluminar_con_vela(vector, personaje, cantidad_elementos);
    } else if (herramienta_usada == LINTERNA){
        iluminar_con_linterna(vector, personaje, cantidad_elementos);
    }
}

/*
.PRE: herramienta debe ser VELA o LINTERNA
.POST: Ilumnina los elementos de los vectores obstaculos y herramientas
*/
void iluminar_posicion(juego_t *juego, char herramienta){
    iluminar_elemento(juego->obstaculos, juego->personaje, juego->cantidad_obstaculos, herramienta);
    iluminar_elemento(juego->herramientas, juego->personaje, juego->cantidad_herramientas, herramienta);
}

/*
.PRE: cantidad_elementos debe ser mayor a 0
.POST: Pone todos los elementos en no visibles
*/
void ocultar_elementos(elemento_del_mapa_t vector[], int cantidad_elementos){
    for (int i = 0; i < cantidad_elementos; i++){
        vector[i].visible = false;
    }
    
}

/*
.PRE:
.POST: Deja en un estado desactivado a elemento_en_uso y oculata los elementos del mapa
*/
void apagar_herramienta(juego_t *juego){
    juego->personaje.elemento_en_uso = DESACTIVADO;
    ocultar_elementos(juego->obstaculos, juego->cantidad_obstaculos);
    ocultar_elementos(juego->herramientas, juego->cantidad_herramientas);
}

/*
.PRE: herramienta debe ser (VELA, LINTERNA o BENGALA)
.POST: Si quedan movimientos al elemento usado ilumina dependiendo cual sea, sino lo apaga
*/
void usar_herramienta(juego_t *juego, char herramienta){
    if (juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes != SIN_MOVIMIENTOS){
        juego->personaje.mochila[juego->personaje.elemento_en_uso].movimientos_restantes--;
        if (herramienta == BENGALA){
            int fila_bengala = rand() % 20;
            int col_bengala = rand() % 30;
            iluminar_con_bengala(juego->obstaculos, juego->cantidad_obstaculos, fila_bengala, col_bengala);
            iluminar_con_bengala(juego->herramientas, juego->cantidad_herramientas, fila_bengala, col_bengala);
        } else {
            iluminar_posicion(juego, herramienta);
        }
    } else {
        apagar_herramienta(juego);
    }
    
}

/*
.PRE: herramienta debe ser (VELA, LINTERNA o BENGALA)
.POST: Si esta la herramienta en la mochila la enciende sino nos imprimira por pantalla que dicha herramienta no esta en la mochila o no tiene mas movimientos
*/
void encender_herramienta(juego_t *juego, char herramienta){
    buscar_herramienta_en_mochila(juego->personaje.mochila, juego->personaje.cantidad_elementos, herramienta, &(juego->personaje.elemento_en_uso));
    if (juego->personaje.elemento_en_uso != -1){    
        usar_herramienta(juego, herramienta);
    } else if (juego->personaje.elemento_en_uso == DESACTIVADO && herramienta == VELA){
        printf("No hay velas en la mochila\n");
    } else if (juego->personaje.elemento_en_uso == DESACTIVADO && herramienta == LINTERNA){
        printf("La linterna no tiene pila\n");
    } else if (juego->personaje.elemento_en_uso == DESACTIVADO && herramienta == BENGALA){
        printf("No hay bengalas en la mochila\n");
    }    
}

/*
.PRE:
.POST: Imprime por pantalla el tiempo actual
*/
void mostrar_tiempo_actual(){
    double tiempo = tiempo_actual();
    if (tiempo >= 0){
        printf("El tiempo actual es %f\n", tiempo);
    }
}

/*
.PRE: cantida_elementos debe ser mayor a 0, fila_personaje debe estar entre (PRIMER_FILA y ULTIMA_FILA) y col_personaje debe estar entre (PRIMER_COLUMNA y ULTIMA_COLUMNA)
.POST: Si el personaje esta sobre un elemento del mapa devuelve la posicion en el vector de este ultimo
*/
int personaje_sobre_elemento(elemento_del_mapa_t vector[], int cantidad_elementos, int fil_personaje, int col_personaje){
    int posicion_encontrada = -1;
    for (int i = 0; i < cantidad_elementos; i++){
        if (posicion_ocupada(vector, i, fil_personaje, col_personaje)){
            posicion_encontrada = i;
        }        
    }
    return posicion_encontrada;
}

/*
.PRE: personalidad debe ser (PARDO, PANDA o POLAR) y el tiempo perdido deb ser mayor o igual a 0
.POST: Agrega el tiempo perdido por chochar con un arbol segun la personalidad
*/
void tiempo_perdido_arbol(char personalidad, double *tiempo_perdido){
    if (personalidad == PARDO){
        *tiempo_perdido += MEDIO_SEG_PERDIDO;
    } else {
        *tiempo_perdido += SEGUNDO_PERDIDO;
    }   
}

/*
.PRE: personalidad debe ser (PARDO, PANDA o POLAR) y el tiempo perdido deb ser mayor o igual a 0
.POST: Agrega el tiempo perdido por chochar con una piedra segun la personalidad
*/
void tiempo_perdido_piedra(char personalidad, double *tiempo_perdido){
    if (personalidad == PARDO || personalidad == PANDA){
        *tiempo_perdido += DOS_SEG_PERDIDOS;
    }
}

/*
.PRE: 
.POST: Devuelve al personaje a la columna 0 y a una fila random
*/
void devolver_al_principio(personaje_t *personaje){
    personaje->posicion.col = 0;
    personaje->posicion.fil = rand() % 20;
}

/*
.PRE: bateria_restante debe ser mayor o igual a 0
.POST: Añade bateria a la linterna
*/
void agregar_pilas(int *bateria_restante){
    *bateria_restante += BATERIA_AGREGADA;
}

/*
.PRE: cantidad_elementos debe ser mayor a 0 y posicion_elemento debe estar entre 0 y cantidad_elementos
.POST: Borra un elemento del vector y reduce en uno a cantidad_elementos
*/
void borrar_elemento(elemento_del_mapa_t elementos[MAX_HERRAMIENTAS], int *cantidad_elementos, int posicion_elemento){
    elementos[posicion_elemento] = elementos[(*cantidad_elementos) - 1];
    (*cantidad_elementos)--;
}

/*
.PRE: posicion_en_vector debe estar entre 0 y cantidad_herramientas, y cantidad_herramientas debe ser mayor a 0
.POST: Recoge el elemento lo pone en la mochila y lo borra del mapa
*/
void recoger_herramienta(int posicion_en_vector, elemento_del_mapa_t herramientas[MAX_HERRAMIENTAS], int cantidad_herramientas ,personaje_t *personaje){
    if (herramientas[posicion_en_vector].tipo == PILA){
        agregar_pilas(&(personaje->mochila[POSICION_LINTERNA].movimientos_restantes));
    } else if (herramientas[posicion_en_vector].tipo == VELA){
        cargar_elemento_en_mochila(personaje->mochila, personaje->cantidad_elementos, VELA, MOVIMIENTOS_VELA, &(personaje->cantidad_elementos));
    } else if (herramientas[posicion_en_vector].tipo == BENGALA){
        cargar_elemento_en_mochila(personaje->mochila, personaje->cantidad_elementos, BENGALA, MOVIMIENTOS_BENGALA, &(personaje->cantidad_elementos));
    }            
    borrar_elemento(herramientas, &(cantidad_herramientas), posicion_en_vector);
}

/*
.PRE: posicion_en_vector debe estar entre 0 y cantida_obstaculos
.POST: Agrega el tiempo perdido segun contra que obstaculo choco el personaje
*/
void prenda_obstaculo(int posicion_en_vector, elemento_del_mapa_t obstaculos[MAX_OBSTACULOS], personaje_t *personaje){
    if (obstaculos[posicion_en_vector].tipo == ARBOL){
        tiempo_perdido_arbol(personaje->tipo, &(personaje->tiempo_perdido));
    } else if (obstaculos[posicion_en_vector].tipo == PIEDRA){
        tiempo_perdido_piedra(personaje->tipo, &(personaje->tiempo_perdido));
    } else if (obstaculos[posicion_en_vector].tipo == KOALA){
        devolver_al_principio(personaje);
    }       

}

/*
.PRE:
.POST: Verifica si el personaje choco con un obstaculo o sobre una herramienta
*/
void verificar_posicion_actual(juego_t *juego){
    int posicion_en_vector = personaje_sobre_elemento(juego->obstaculos, juego->cantidad_obstaculos, juego->personaje.posicion.fil, juego->personaje.posicion.col);
    if (posicion_en_vector != -1){
        prenda_obstaculo(posicion_en_vector, juego->obstaculos, &(juego->personaje));
    } else {
        posicion_en_vector = personaje_sobre_elemento(juego->herramientas, juego->cantidad_herramientas, juego->personaje.posicion.fil, juego->personaje.posicion.col);
        if (posicion_en_vector != -1){
            recoger_herramienta(posicion_en_vector, juego->herramientas, juego->cantidad_herramientas, &(juego->personaje));
        }
    }   
}

bool es_jugada_valida(char jugada){
    bool es_valido = false;
    switch (jugada){
    case MOVER_ARRIBA:
        es_valido = true;
        break;
    case MOVER_ABAJO:
        es_valido = true;
        break;
    case MOVER_DERECHA:
        es_valido = true;
        break;
    case MOVER_IZQUIERDA:
        es_valido = true;
        break;
    case LINTERNA:
        es_valido = true;
        break;
    case VELA:
        es_valido = true;
        break;
    case BENGALA:
        es_valido = true;
        break;            
    case MOSTRAR_TIEMPO:
        es_valido = true;
        break;    
    default:
        break;
    }
    return es_valido;
}

/*
.PRE: movimiento debe ser (MOVER_ARRIBA, MOVER_ABAJO, MOVER_IZQUIERDA, MOVER_DERECHA) y borde (PRIMER_FILA, PRIMER_COLUMNA, ULTIMA_FILA, ULTIMA_COLUMNA)
.POST: Devuelve true si posicion_actual esta en uno de los bordes 
*/
bool movimiento_valido(char movimiento, int posicion_actual, int borde){
    if (posicion_actual == borde){
        return false;
    } else {
        return true;
    }
}

/*
.PRE: cantidad_obstaculos y cantidad_herramientas deben ser mayores a 0, y fila y col deben estar entre cantidad_obstaculos y cantidad_herramientas 
.POST: Busca fila y col libres 
*/
void encontrar_lugar_libre(elemento_del_mapa_t obstaculos[MAX_OBSTACULOS], int cantidad_obstaculos, elemento_del_mapa_t herramientas[MAX_HERRAMIENTAS], int cantidad_herramientas, int *fila, int *col){
    
    int posicion_ocupada_en_obstaculos = personaje_sobre_elemento(obstaculos, cantidad_obstaculos, *fila, *col); 
    int posicion_ocupada_en_herramientas = personaje_sobre_elemento(herramientas, cantidad_herramientas, *fila, *col);
    while ((posicion_ocupada_en_obstaculos != -1) && (posicion_ocupada_en_herramientas != -1)){
        *fila = rand() % 20;
        *col = rand() % 30;
        posicion_ocupada_en_obstaculos = personaje_sobre_elemento(obstaculos, cantidad_obstaculos, *fila, *col);
        posicion_ocupada_en_herramientas = personaje_sobre_elemento(herramientas, cantidad_herramientas, *fila, *col);
    }  
}

/*
.PRE: cantidad_obstaculos y cantidad_herramientas deben ser mayores a 0, koala debe ser igual a KOALA
.POST: Agrega un nuevo koala al vector obstaculos
*/
void agregar_koala(elemento_del_mapa_t obstaculos[MAX_OBSTACULOS], int *cantidad_obstaculos, char koala, elemento_del_mapa_t herramientas[MAX_HERRAMIENTAS], int cantidad_herramientas){
    obstaculos[*cantidad_obstaculos].tipo = koala;
    int fila = rand() % 20;
    int col = rand() % 30;

    encontrar_lugar_libre(obstaculos, *cantidad_obstaculos, herramientas, cantidad_herramientas, &fila, &col);

    obstaculos[*cantidad_obstaculos].posicion.fil = fila;
    obstaculos[*cantidad_obstaculos].posicion.col = col;
    obstaculos[*cantidad_obstaculos].visible = false;
    (*cantidad_obstaculos)++;

}

void realizar_jugada(juego_t *juego, char jugada){
    ocultar_elementos(juego->obstaculos, juego->cantidad_obstaculos);
    ocultar_elementos(juego->herramientas, juego->cantidad_herramientas);
    if (jugada == MOVER_ARRIBA){
        if (movimiento_valido(jugada, juego->personaje.posicion.fil, PRIMER_FILA)){
            mover_arriba(&(juego->personaje));
            if (juego->personaje.elemento_en_uso != DESACTIVADO){
                if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == VELA){
                    usar_herramienta(juego, VELA);        
                } else if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == LINTERNA){
                    usar_herramienta(juego, LINTERNA);
                } else if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == BENGALA){
                    usar_herramienta(juego, BENGALA);
                } 
            }
            verificar_posicion_actual(juego);
        } else {
            printf("No podes moverte en esta direccion\n");
        }
    } else if (jugada == MOVER_ABAJO){
        if (movimiento_valido(jugada, juego->personaje.posicion.fil, ULTIMA_FILA)){
            mover_abajo(&(juego->personaje));
            if (juego->personaje.elemento_en_uso != DESACTIVADO){
                if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == VELA){
                    usar_herramienta(juego, VELA);        
                } else if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == LINTERNA){
                    usar_herramienta(juego, LINTERNA);        
                } else if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == BENGALA){
                    usar_herramienta(juego, BENGALA);
                }  
            }
            verificar_posicion_actual(juego);
        } else {
            printf("No podes moverte en esta direccion\n");
        }
    } else if (jugada == MOVER_IZQUIERDA){
        if (movimiento_valido(jugada, juego->personaje.posicion.col, PRIMER_COLUMNA)){
            mover_izquierda(&(juego->personaje));
            if (juego->personaje.elemento_en_uso != DESACTIVADO){
                if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == VELA){
                    usar_herramienta(juego, VELA);        
                }else if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == LINTERNA){
                    usar_herramienta(juego, LINTERNA);        
                } else if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == BENGALA){
                    usar_herramienta(juego, BENGALA);
                }  
            }
            verificar_posicion_actual(juego);
        } else {
            printf("No podes moverte en esta direccion\n");
        }
    }  else if (jugada == MOVER_DERECHA){
        if (movimiento_valido(jugada, juego->personaje.posicion.col, ULTIMA_COLUMNA)){
            mover_derecha(&(juego->personaje));
            if (juego->personaje.elemento_en_uso != DESACTIVADO){
                if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == VELA){
                    usar_herramienta(juego, VELA);        
                } else if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == LINTERNA){
                    usar_herramienta(juego, LINTERNA);        
                } else if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo == BENGALA){
                    usar_herramienta(juego, BENGALA);
                }  
            }
            verificar_posicion_actual(juego);
        } else {
            printf("No podes moverte en esta direccion\n");
        }
    } else if (jugada == VELA){
        if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo != VELA){
            encender_herramienta(juego, VELA);
        } else {
            apagar_herramienta(juego);
        }
        
    } else if (jugada == LINTERNA){
        if (juego->personaje.mochila[juego->personaje.elemento_en_uso].tipo != LINTERNA){
            encender_herramienta(juego, LINTERNA);
            agregar_koala(juego->obstaculos, &(juego->cantidad_obstaculos), KOALA, juego->herramientas, juego->cantidad_herramientas);
        } else {
            apagar_herramienta(juego);
        }
    } else if (jugada == BENGALA){
        encender_herramienta(juego, BENGALA);
    }else if (jugada == MOSTRAR_TIEMPO){
        mostrar_tiempo_actual();
    }
    if ((juego->personaje.tiempo_perdido >= TIEMPO_GPS) && (juego->personaje.tipo == PANDA)){
        if (juego->chloe_visible != true){
            juego->chloe_visible = true;
        }
    }
    
    
}
