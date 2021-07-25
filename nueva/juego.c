#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "test_de_personalidad.h"
#include "osos_contra_reloj.h"
#include "utiles.h"

const double TIEMPO_LIMITE = 120;

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
    iniciar_cronometro();
    while (estado_juego(juego) != -1){
        char jugada;
        printf("Ingrese su siguiente jugada: ");
        scanf(" %c", &jugada);
        system("clear");
        if (es_jugada_valida(jugada)){
            realizar_jugada(&juego, jugada);
            mostrar_juego(juego);
        } else {
            printf("Ingrese una tecla valida\n");
        }
    }
    
    double tiempo_final = detener_cronometro();
    double tiempo_total = tiempo_final + juego.personaje.tiempo_perdido;
    if (tiempo_total <= TIEMPO_LIMITE){
        printf("Felicidades encontraste a Chloe antes de que empiece el toque de queda\nGanaste el juego!!!\n");
    } else{
        printf("No lograste encontrar a Chloe antes de que empiece el toque de queda, vas a ir preso *carita triste*\n");
    }
    
      
    return 0;
}
