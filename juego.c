#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "test_de_personalidad.h"
#include "osos_contra_reloj.h"

#define POLAR 'I'
#define PANDA 'P'
#define PARDO 'G'

//#include "utiles.h"

void mensaje_inicio(char personalidad){
    printf("\n\nBienvenido al juego el objetivo es encontrar a Clhoe, que esta escondida en el bosque antes de que comience el toque de queda, Podras lograrlo?\n\n");
}

int main(){
    
    char personalidad;
    juego_t juego;

    srand((unsigned) time (NULL));
    
    test_de_personalidad(&personalidad);
    mensaje_inicio(personalidad);
    inicializar_juego(&juego, personalidad);
    mostrar_juego(juego);

    

    

    return 0;
}
