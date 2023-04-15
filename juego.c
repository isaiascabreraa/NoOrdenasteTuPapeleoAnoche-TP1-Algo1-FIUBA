#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "utiles.h"
#include "papeleo.h"
#include "controlador.h"
#include "dia_en_la_uni.h"

static const int JUGANDO = 0;
static const int GANASTE = 1;


//Pre: -
//Post: Corrobora si el nivel fue completado o si el estado del juego es JUGANDO, PERDIDO o GANADO.
void centro_de_control( juego_t* juego){

    int juego_en_marcha = JUGANDO;
    int nivel_en_marcha = JUGANDO;

    while(juego_en_marcha == JUGANDO){
    do{
        nivel_en_marcha = estado_nivel( (*juego).niveles[(*juego).nivel_actual - 1].papeleos, (*juego).niveles[(*juego).nivel_actual - 1].tope_papeleos);
        
        if(nivel_en_marcha == JUGANDO && (*juego).jugador.movimientos > 0){
            realizar_jugada( juego);
            imprimir_terreno( *juego);
        
        }else{
            nivel_en_marcha = GANASTE;
        }

    }while(nivel_en_marcha == JUGANDO);

    juego_en_marcha = estado_juego( *juego);
    
    (*juego).nivel_actual++;
    
    }
}

int main(){

    srand((unsigned)time(NULL));
    system("clear");

    juego_t juego;
    char personaje_tp1 = ' ';

    seleccionar_personaje( &personaje_tp1);

    inicializar_juego( &juego, personaje_tp1);
    imprimir_terreno( juego);
    inicializar_controlador();

    centro_de_control( &juego);

    terminar_controlador();

    return 0;
}
