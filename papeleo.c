#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "utiles.h"
#include "papeleo.h"
#include "controlador.h"
#include "dia_en_la_uni.h"

#define MAX_OBSTACULOS 100
#define MAX_HERRAMIENTAS 100
#define MAX_OBJETOS 100
#define MAX_PAPELEOS 10
#define MAX_NIVELES 3

#define MAX_ELEMENTOS 100
#define MAX_FILAS_MATRIZ 25
#define MAX_COLUMNAS_MATRIZ 25

#define EMOJI_MIKE "\U0001f47e"
#define EMOJI_PAREDES "\U0001f9f1"
#define EMOJI_FUEGOS "\U0001f525"
#define EMOJI_MEDIAS "\U0001f9e6"
#define EMOJI_BOTELLAS "\U0001f376"

static const char OLAF = 'O';
static const char STITCH = 'S';
static const char RAYO_MCQUEEN = 'R';
static const char JASMIN = 'J';

const int TOPE_LV1 = 22;
const int TOPE_LV2 = 17;
const int TOPE_LV3 = 12;

const char PAREDES = 'P', MIKE = 'W', FUEGOS = 'F', MEDIAS = 'M', BOTELLAS_DE_GRITOS = 'G', INTERRUPTORES_AHUYENTA_RANDALL = 'I';
const char PAPELEO = 'X', ESPACIO = ' ';

const int PAPELEO_LV1 = 2, PAPELEO_LV2 = 3, PAPELEO_LV3 = 4;

const int FUEGOS_LV1 = 10, FUEGOS_LV2 = 5, FUEGOS_LV3 = 3; //10 FUEGOS LV1; 5; 3;
const int MEDIAS_LV1 = 5, MEDIAS_LV2 = 4, MEDIAS_LV3 = 3;
const int BOTELLAS_LV1 = 4, BOTELLAS_LV2 = 3, BOTELLAS_LV3 = 2;
const int INTERRUPTORES_LV1 = 1, INTERRUPTORES_LV2 = 1, INTERRUPTORES_LV3 = 0;

const int MARTILLOS_LV1 = 4, MARTILLOS_LV2 = 5, MARTILLOS_LV3 = 6;
const int EXTINTORES_LV1 = 4, EXTINTORES_LV2 = 2, EXTINTORES_LV3 = 2;
const int MOVIMIENTOS_LV1 = 40, MOVIMIENTOS_LV2 = 30, MOVIMIENTOS_LV3 = 20;

const int CONDICION_FALLIDA = -1;
const int CONDICION_CUMPLIDA = 0;
const int NO_SE_REPITEN = 0, SE_REPITEN = -1;
const int NO_SE_SUPERPONEN = 0, SE_SUPERPONEN = -1;
const int NO_SON_ADYACENTES = -1, SON_ADYACENTES = 0, NO_NECESITAN_SER_ADYACENTES = 0;

const int OLAF_ELIMINA_FUEGOS_LV1 = 2, OLAF_ELIMINA_FUEGOS_LV2 = 1; 
const int STITCH_ELIMINA_PAPELELO_LV3 = 1;

const char DERECHA = 'D', IZQUIERDA = 'A', ARRIBA = 'W', ABAJO = 'S';
const char ROTACION_IZQUIERDA = 'Q', ROTACION_DERECHA = 'E';
const char MARTILLO = 'Z', EXTINTOR = 'C';

static const int GANASTE = 1;
static const int JUGANDO = 0;
static const int PERDISTE = -1;

//Pre: El personaje obtenido del TP1 debe ser jasmin.
//Post: Otorga un martillo extra por cada nivel en el que estemos.
void obsequio_jasmin(int nivel_actual, char personaje_tp1, int* martillos){
    if(personaje_tp1 == JASMIN){
        *martillos = *martillos + 1;
    }
}

//Pre: El personaje obtenido del TP1 debe ser el rayo mcqueen.
//Post: Otorga diez movimientos iniciales extras en el primer nivel.
void obsequio_rayo_mcqueen(int nivel_actual, char personaje_tp1, int* movimientos){
    if(nivel_actual == 1 && personaje_tp1 == RAYO_MCQUEEN){
        *movimientos = *movimientos + 10;
    }
}

//Pre: El personaje obtenido del TP1 debe ser stitch.
//Post: Otorga un papeleo menos a recolectar al estar en el nivel tres.
void obsequio_stitch(int* tope_papeleos, int nivel_actual, char personaje_tp1){
    
    if(nivel_actual == 3 && personaje_tp1 == STITCH){
        *tope_papeleos = *tope_papeleos - STITCH_ELIMINA_PAPELELO_LV3;
    }
}

//Pre: El personaje obtenido del TP1 debe ser olaf.
//Post: Otorga una reduccion de dos y de uno, de los fuegos de los primeros dos niveles, respectivamente.
void obsequio_olaf(int* cantidad_fuegos, int nivel_actual, char personaje_tp1){
    
    if(nivel_actual == 1 && personaje_tp1 == OLAF){
        *cantidad_fuegos = *cantidad_fuegos - OLAF_ELIMINA_FUEGOS_LV1;
    }else if(nivel_actual == 2 && personaje_tp1 == OLAF){
        *cantidad_fuegos = *cantidad_fuegos - OLAF_ELIMINA_FUEGOS_LV2;
    }
}

//Pre: -
//Post: Verifica que la posicion de un elemento no sea la misma que la de otro.
bool esta_la_posicion_ocupada( coordenada_t objeto_uno, coordenada_t objeto_dos){
    
    return(objeto_uno.fil == objeto_dos.fil && objeto_uno.col == objeto_dos.col);
}

//Pre: -
//Post: Verifica que los fuegos se enecuentren de forma adyacente a las paredes.
bool son_fuegos_adyacentes( coordenada_t objeto_uno, coordenada_t paredes){
    
    return( abs(objeto_uno.fil - paredes.fil) + abs(objeto_uno.col - paredes.col) == 1);
}

//Pre: -
//Post: Verifica que los diferentes elementos cumplan las condiciones necesarias.
bool se_cumplen_las_condiciones( int verificar_paredes, int verificar_obstaculos, int verificar_herramientas, int verificar_papeleos, coordenada_t posicion_actual, 
    coordenada_t posicion_jugador){
    
    return(verificar_paredes == CONDICION_CUMPLIDA && verificar_obstaculos == CONDICION_CUMPLIDA && 
    verificar_herramientas == CONDICION_CUMPLIDA && verificar_papeleos == CONDICION_CUMPLIDA && !esta_la_posicion_ocupada( posicion_actual, posicion_jugador));
}

//Pre: -
//Post: Verifica que los movimientos introducidos por el usuario correspondan a los establecidos.
bool es_caracter_valido( char movimiento_realizado){
    return(movimiento_realizado == DERECHA || movimiento_realizado == IZQUIERDA || movimiento_realizado ==ROTACION_DERECHA
     || movimiento_realizado == ROTACION_IZQUIERDA || movimiento_realizado == MARTILLO || movimiento_realizado == EXTINTOR);
}

//Pre: -
//Posta: Verifica si un papeleo ya fue recolectado.
bool esta_papeleo_recolectado( papeleo_t papeleo){
    return papeleo.recolectado;
}

//Pre: -
//Post: Verifica si lo introducido es una herramienta o no.
bool selecciona_herramienta( char movimiento_realizado){
    return (movimiento_realizado == MARTILLO || movimiento_realizado == EXTINTOR);
}

//Pre: -
//Post: Indica si la cantidad de movimiento realizados es multiplo de otro numero.
bool es_multiplo( int movimientos_realizados, int multiplo){
    return(movimientos_realizados % multiplo == 0);
}

//Pre: -
//Post: Verifica si la direccion introducida es valida.
bool no_es_direccion_valida( char direccion_seleccionada){
    return (direccion_seleccionada != ARRIBA && direccion_seleccionada != ABAJO && direccion_seleccionada != DERECHA && direccion_seleccionada != IZQUIERDA);
}

//Pre: -
//Post: Indica si es posible romper un pared o si esta se encuentra en los limites del tablero.
bool se_puede_romper( coordenada_t coordenada, int tope){
    return( coordenada.fil == 0 || coordenada.fil == tope || coordenada.col == 0 || coordenada.col == tope);
}

//Pre: -
//Post: Devuelve una coordenada que se genera aleatoriamente entre determinados numeros segun el nivel actual.
coordenada_t asignar_coordenadas(coordenada_t coordenada, int nivel_actual){
    
    if(nivel_actual == 1){
        coordenada.fil = rand()%20 + 1; 
        coordenada.col = rand()%20 + 1;

    }else if(nivel_actual == 2){
        coordenada.fil = rand()%15 + 1; 
        coordenada.col = rand()%15 + 1;

    }else{
        coordenada.fil = rand()%10 + 1; 
        coordenada.col = rand()%10 + 1;
    }
    return coordenada;
}

//Pre: -
//Post: Verifica que los objetos no se superpongan con las paredes.
void verificar_si_coinciden_paredes( coordenada_t objeto_uno, char tipo_objeto_uno, coordenada_t paredes[MAX_ELEMENTOS], int tope_paredes, int* verificar_paredes){
    
    int superposicion = NO_SE_SUPERPONEN;
    int locacion = NO_SON_ADYACENTES;
    int j = 0;
    
    while(j < tope_paredes && (superposicion == NO_SE_SUPERPONEN || locacion == NO_SON_ADYACENTES) ){
        
        if( esta_la_posicion_ocupada(objeto_uno, paredes[j])){
            superposicion = SE_SUPERPONEN;
        }

        if(tipo_objeto_uno == FUEGOS && son_fuegos_adyacentes( objeto_uno, paredes[j])){
            locacion = SON_ADYACENTES;
        
        }else if(tipo_objeto_uno != FUEGOS){
            locacion = NO_NECESITAN_SER_ADYACENTES;
        }
        j++;
    }
   *verificar_paredes = superposicion + locacion;
}

//Pre: -
//Post: Verifica que diferentes objetos no se superpongan entre ellos.
void verificar_si_coinciden_objetos( coordenada_t coordenada, objeto_t objetos[MAX_ELEMENTOS], int tope_objeto, char* objeto_encontrado, int* verificar){
    
    int superposicion = NO_SE_SUPERPONEN;
    int j = 0;
    
    while(j < tope_objeto && superposicion == NO_SE_SUPERPONEN){
        
        if( esta_la_posicion_ocupada( coordenada, objetos[j].posicion)){
            superposicion = SE_SUPERPONEN;
            *objeto_encontrado = objetos[j].tipo;
        }
        j++;
    }
    *verificar = superposicion;
}

//Pre: -
//Post: Verifica que al recorrer el vector al que pertenece cada papeleo no se encuentren dos papeleos en el mismo lugar) 
void verificar_si_coinciden_papeleos( coordenada_t coordenada, papeleo_t papeleos[MAX_PAPELEOS], int tope_papeleos, int* papeleo_encontrado, int* verificar_papeleos){
    
    int repeticiones = NO_SE_REPITEN;
    int j = 0;

    while(j < tope_papeleos && repeticiones == NO_SE_REPITEN){
        
        if( esta_la_posicion_ocupada( coordenada, papeleos[j].posicion)){
            repeticiones = SE_REPITEN;
            *papeleo_encontrado = papeleos[j].id_papeleo;
        }
        j++;
    }
    *verificar_papeleos = repeticiones;
}

//Pre: -
//Post: Devuelve valores segun se vaya corroborando que las coordenadas dadas no se superpongan con los demas elementos.
void verificaciones(coordenada_t posicion, nivel_t nivel, char tipo, int* verificar_paredes, int* verificar_obstaculos, int* verificar_herramientas, int* verificar_papeleos){

    char objeto_encontrado = ' ';
    int papeleo_encontrado = 0;

    verificar_si_coinciden_paredes( posicion, tipo, nivel.paredes, nivel.tope_paredes, verificar_paredes);
    
    verificar_si_coinciden_objetos( posicion, nivel.obstaculos, nivel.tope_obstaculos, &objeto_encontrado, verificar_obstaculos);
    
    verificar_si_coinciden_objetos( posicion, nivel.herramientas, nivel.tope_herramientas, &objeto_encontrado, verificar_herramientas);

    verificar_si_coinciden_papeleos( posicion, nivel.papeleos, nivel.tope_papeleos, &papeleo_encontrado, verificar_papeleos);
}

//Pre: -
//Post: Genera una coordenada aleatoria para los elementos del vector papeleos.
void asignar_posiciones_papeleos( nivel_t* nivel, int nivel_actual){

    for(int i = 0; i < (*nivel).tope_papeleos ; i++){

        int verificar = CONDICION_FALLIDA;
        int verificar_paredes = 0, verificar_obstaculos = 0, verificar_herramientas = 0, verificar_papeleos = 0;
        
        do{
            
            coordenada_t coordenada = asignar_coordenadas((*nivel).papeleos[i].posicion, nivel_actual);
            verificaciones(coordenada, *nivel, PAPELEO, &verificar_paredes, &verificar_obstaculos, &verificar_herramientas, &verificar_papeleos);

            if(se_cumplen_las_condiciones(verificar_paredes, verificar_obstaculos, verificar_herramientas, verificar_papeleos, coordenada, (*nivel).pos_inicial_jugador)){
                
                verificar = CONDICION_CUMPLIDA;
                (*nivel).papeleos[i].posicion.fil = coordenada.fil;
                (*nivel).papeleos[i].posicion.col = coordenada.col;
            }
        
        }while(verificar == CONDICION_FALLIDA);
    }
}

//Pre: -
//Post: Genera una coordenada alaetoria para los elementos de un vector de objetos.
void asignar_posiciones_objetos( nivel_t* nivel, objeto_t objetos[MAX_ELEMENTOS], int tope, int nivel_actual){

    for(int i = 0; i < tope ; i++){
        
        int verificar = CONDICION_FALLIDA;
        int verificar_paredes = 0, verificar_obstaculos = 0, verificar_herramientas = 0, verificar_papeleos = 0;

        do{
            coordenada_t coordenada = asignar_coordenadas( objetos[i].posicion, nivel_actual);
            verificaciones(coordenada, *nivel, objetos[i].tipo, &verificar_paredes, &verificar_obstaculos, &verificar_herramientas, &verificar_papeleos);

            if(se_cumplen_las_condiciones(verificar_paredes, verificar_obstaculos, verificar_herramientas, verificar_papeleos, coordenada, (*nivel).pos_inicial_jugador)){
                
                verificar = CONDICION_CUMPLIDA;
                objetos[i].posicion.fil = coordenada.fil;
                objetos[i].posicion.col = coordenada.col;
            }

        }while( verificar == CONDICION_FALLIDA);
    }
}

//Pre: -
//Post: Define la posicion del jugador.
void definir_posicion_jugador(jugador_t* jugador, nivel_t nivel){

    (*jugador).posicion.fil = nivel.pos_inicial_jugador.fil;
    (*jugador).posicion.col = nivel.pos_inicial_jugador.col;
}

//Pre: -
//Post: Devuelve el valor de las filas y columnas de mi matriz segun el nivel actual.
int determinar_tope(int nivel_actual){

     if(nivel_actual == 1){
        return TOPE_LV1;
    }else if(nivel_actual == 2){
        return TOPE_LV2;
    }else{
        return TOPE_LV3;
    }
}

//Pre: -
//Post: Convierte el tipo de dato de un numero en una letra.
int convertir_int_en_char( int id_papeleo){
    return( id_papeleo + '0');
}

//Pre: -
//Post: Posiciona las diferentes coordenadas de cada elemento en una matriz.
void posicionar_coordenadas(juego_t juego, int nivel_actual, char primer_terreno[MAX_FILAS_MATRIZ][MAX_COLUMNAS_MATRIZ]){

    int numero_nivel = nivel_actual - 1;
    
    for(int i = 0; i < juego.niveles[numero_nivel].tope_paredes ; i++){
        primer_terreno[juego.niveles[numero_nivel].paredes[i].fil][juego.niveles[numero_nivel].paredes[i].col] = PAREDES; 
    }

    for(int i = 0; i < juego.niveles[numero_nivel].tope_obstaculos ; i++){
        primer_terreno[juego.niveles[numero_nivel].obstaculos[i].posicion.fil][juego.niveles[numero_nivel].obstaculos[i].posicion.col] = 
        juego.niveles[numero_nivel].obstaculos[i].tipo; 
    }

    for(int i = 0; i < juego.niveles[numero_nivel].tope_herramientas ; i++){
        primer_terreno[juego.niveles[numero_nivel].herramientas[i].posicion.fil][juego.niveles[numero_nivel].herramientas[i].posicion.col] = 
        juego.niveles[numero_nivel].herramientas[i].tipo; 
    }

    for(int i = 0; i < juego.niveles[numero_nivel].tope_papeleos ; i++){
        
        int id_papeleo = convertir_int_en_char(juego.niveles[numero_nivel].papeleos[i].id_papeleo);
        primer_terreno[juego.niveles[numero_nivel].papeleos[i].posicion.fil][juego.niveles[numero_nivel].papeleos[i].posicion.col] = (char)id_papeleo;
    }

    primer_terreno[juego.jugador.posicion.fil][juego.jugador.posicion.col] = MIKE;
}

//Pre: -
//Post: Le asigna a las variables de jugador un valor segun el nivel en el que estemos.
void definir_nivel_jugador( jugador_t* jugador, int nivel_actual, int movimientos_sobrantes){
    
    if( nivel_actual == 1){

        (*jugador).movimientos = MOVIMIENTOS_LV1;
        (*jugador).martillos = MARTILLOS_LV1;
        (*jugador).extintores = EXTINTORES_LV1;

    }else if( nivel_actual == 2){

        (*jugador).movimientos = MOVIMIENTOS_LV2 + movimientos_sobrantes;
        (*jugador).martillos = MARTILLOS_LV2;
        (*jugador).extintores = EXTINTORES_LV2;

    }else{
        
        (*jugador).movimientos = MOVIMIENTOS_LV3 + movimientos_sobrantes;
        (*jugador).martillos = MARTILLOS_LV3;
        (*jugador).extintores = EXTINTORES_LV3;
    }
}

//Pre: -
//Post: Carga todos los valores necesarios en lo que respecta al juegador.
void cargar_jugador(jugador_t* jugador, nivel_t nivel, int nivel_actual, char personaje_tp1){
    
    (*jugador).ahuyenta_randall = false;
    (*jugador).movimientos_realizados = 0;

    definir_posicion_jugador( jugador, nivel);
    definir_nivel_jugador( jugador, nivel_actual, (*jugador).movimientos_realizados);

    obsequio_jasmin( nivel_actual, personaje_tp1, &(*jugador).martillos);
    obsequio_rayo_mcqueen( nivel_actual, personaje_tp1, &(*jugador).movimientos);
}

//Pre: -
//Post: Segun el nivel en el que estemos y el personaje elegido, les asigna un determiando valor a las variables de papeleos.
void definir_cantidad_papeleos(int* tope_papeleos, int nivel_actual, char personaje_tp1){
    
    if(nivel_actual == 1){
        *tope_papeleos = PAPELEO_LV1;
    
    }else if(nivel_actual == 2){
        *tope_papeleos = PAPELEO_LV2;
    
    }else if(nivel_actual == 3){
        *tope_papeleos = PAPELEO_LV3;
    }

    obsequio_stitch( tope_papeleos, nivel_actual, personaje_tp1);
}

//Pre: -
//Post: Define los valores de los diferentes elementos del vector papeleos.
void definir_id_papeleos( papeleo_t papeleos[MAX_PAPELEOS], int tope_papeleos){

    int tope_papeleos_total = PAPELEO_LV1 + PAPELEO_LV2 + PAPELEO_LV3;

    for(int i = 0; i <= tope_papeleos_total; i++){
        (papeleos[i]).id_papeleo = i + 1;
        (papeleos[i]).recolectado = false;
    }    
}

//Pre: -
//Post:  Carga todos los valores necesarios en lo que respecta a papeleos.
void cargar_papeleos( nivel_t* nivel, int nivel_actual, char personaje_tp1){

    definir_cantidad_papeleos( &(*nivel).tope_papeleos, nivel_actual, personaje_tp1);
    definir_id_papeleos( (*nivel).papeleos, (*nivel).tope_papeleos);
    asignar_posiciones_papeleos( nivel, nivel_actual);
}

//Pre: -
//Post: Segun el nivel en el que estemos, les asigna un determinado valor a las variables de herramientas.
void definir_cantidad_herramientas(int* tope_botellas, int* tope_interruptores, int* tope_herramientas, int nivel_actual){

    if(nivel_actual == 1){
        
        *tope_botellas = BOTELLAS_LV1;
        *tope_interruptores = INTERRUPTORES_LV1;
    
    }else if(nivel_actual == 2){
        
        *tope_botellas = BOTELLAS_LV2;
        *tope_interruptores = INTERRUPTORES_LV2;
    
    }else if(nivel_actual == 3){
        
        *tope_botellas = BOTELLAS_LV3;
        *tope_interruptores = INTERRUPTORES_LV3;
    }
    *tope_herramientas = *tope_botellas + *tope_interruptores;
}

//Pre: -
//Post:  Le asigna un tipo de caracter (en vector herramientas) a dos tipos diferentes de obstaculos en el juego.
void definir_tipo_herramientas( objeto_t herramientas[MAX_HERRAMIENTAS], int tope_herramientas, int tope_botellas){

    for(int i = 0; i < tope_botellas; i++){
        (herramientas[i]).tipo = BOTELLAS_DE_GRITOS;    
    }
    for(int i = tope_botellas; i < tope_herramientas; i++){
        (herramientas[i]).tipo = INTERRUPTORES_AHUYENTA_RANDALL;
    }
}

//Pre: -
//Post: Carga todos los valores necesarios en lo que respecta a las herramientas.
void cargar_herramientas( nivel_t* nivel, int nivel_actual){

    int tope_botellas = 0;
    int tope_interruptores = 0;

    definir_cantidad_herramientas( &tope_botellas, &tope_interruptores, &(*nivel).tope_herramientas, nivel_actual);
    definir_tipo_herramientas( (*nivel).herramientas, (*nivel).tope_herramientas, tope_botellas);
    asignar_posiciones_objetos( nivel, (*nivel).herramientas, (*nivel).tope_herramientas, nivel_actual);
}

//Pre: -
//Post: Segun el nivel en el que estemos y el personaje elegido, les asigna un determinado valor a las variables de obstaculos.
void definir_cantidad_obstaculos(int* cantidad_fuegos, int* cantidad_medias, int* tope_obstaculos, int nivel_actual, char personaje_tp1){
    
    if(nivel_actual == 1){
        
        *cantidad_fuegos = FUEGOS_LV1;
        *cantidad_medias = MEDIAS_LV1;
    
    }else if(nivel_actual == 2){
        
        *cantidad_fuegos = FUEGOS_LV2;
        *cantidad_medias = MEDIAS_LV2;
    
    }else if(nivel_actual == 3){
        
        *cantidad_fuegos = FUEGOS_LV3;
        *cantidad_medias = MEDIAS_LV3;
    }
    obsequio_olaf( cantidad_fuegos, nivel_actual, personaje_tp1);
    *tope_obstaculos = *cantidad_fuegos + *cantidad_medias;
}

//Pre: -
//Post: Le asigna un tipo de caracter (en vector obstaculos) a dos tipos diferentes de obstaculos en el juego.
void definir_tipo_obstaculos( objeto_t obstaculos[MAX_OBSTACULOS], int tope_obstaculos, int cantidad_fuegos){

    for(int i = 0; i < cantidad_fuegos; i++){
        (obstaculos[i]).tipo = FUEGOS;
    }
    
    for(int i = cantidad_fuegos; i < tope_obstaculos; i++){
        (obstaculos[i]).tipo = MEDIAS; 
    }
}

//Pre: -
//Post: Carga todos los necesarios en lo que respecta a los obstaculos.
void cargar_obstaculos( nivel_t* nivel, int nivel_actual, char personaje_tp1){
    
    int cantidad_fuegos = 0;
    int cantidad_medias = 0; 
    
    definir_cantidad_obstaculos( &cantidad_fuegos, &cantidad_medias, &(*nivel).tope_obstaculos, nivel_actual, personaje_tp1);
    definir_tipo_obstaculos( (*nivel).obstaculos, (*nivel).tope_obstaculos, cantidad_fuegos);
    asignar_posiciones_objetos( nivel, (*nivel).obstaculos, (*nivel).tope_obstaculos, nivel_actual);
}

//Pre: -
//Post: Procedimiento que recibe el juego e imprime toda su información por pantalla.
void imprimir_terreno(juego_t juego){
    
    char primer_terreno[MAX_FILAS_MATRIZ][MAX_COLUMNAS_MATRIZ];
    int tope_matriz = determinar_tope(juego.nivel_actual);

    for(int i = 0; i < tope_matriz; i++){
        for(int j = 0; j < tope_matriz; j++){

            primer_terreno[i][j] = ESPACIO; 
        }
    }

    posicionar_coordenadas( juego, juego.nivel_actual, primer_terreno);
    
    printf("\n");
    for(int i = 0; i < tope_matriz; i++){
        
        for(int j = 0; j < tope_matriz; j++){
            
            if( primer_terreno[i][j] == PAREDES){
                printf(" %s ", EMOJI_PAREDES);
            }else if(  primer_terreno[i][j] == FUEGOS){
                printf(" %s", EMOJI_FUEGOS);
            }else if( primer_terreno[i][j] == MEDIAS){
                printf(" %s", EMOJI_MEDIAS);
            }else if( primer_terreno[i][j] == BOTELLAS_DE_GRITOS){
                printf(" %s", EMOJI_BOTELLAS);
            }else if( primer_terreno[i][j] == MIKE){
                printf(" %s", EMOJI_MIKE);
            }else{
                printf( " %c ", primer_terreno[i][j]);
            }
        }
        printf("\n");
    }   
}

// Pre: -
// Post: Asigna todos los valores correspondientes a cada elemento.
void cargar_valores( nivel_t* nivel, int nivel_actual, char personaje_tp1){

    cargar_obstaculos( nivel, nivel_actual, personaje_tp1);
    
    cargar_herramientas( nivel, nivel_actual);

    cargar_papeleos( nivel, nivel_actual, personaje_tp1);
}

//Pre: -
/* Post: Inicializará un nivel, cargando toda la información inicial, las paredes,los objetos, 
         los papeleos y la posición inicial del jugador en dicho nivel. */
void inicializar_nivel( nivel_t* nivel, int nivel_actual, char personaje_tp1){

    (*nivel).tope_paredes = 0;
   
    (*nivel).tope_obstaculos = 0;

    (*nivel).tope_herramientas = 0;

    (*nivel).tope_papeleos = 0;
    
    obtener_paredes(nivel_actual, (*nivel).paredes, &(*nivel).tope_paredes, &(*nivel).pos_inicial_jugador);

    cargar_valores( nivel, nivel_actual, personaje_tp1);
}

// Pre: -
/* Post: Inicializará el juego, cargando toda la información inicial, los datos del jugador, 
    el personaje resultado del tp anterior, y los 3 niveles. El campo "nivel_actual"
    comienza en 1. */
void inicializar_juego(juego_t* juego, char personaje_tp1){

    (*juego).nivel_actual = 1;
    (*juego).personaje_tp1 = personaje_tp1;

    for(int i = 0; i < 3; i++){

        int numero_nivel = i + 1;
        
        inicializar_nivel( &(*juego).niveles[i], numero_nivel , personaje_tp1);
    }

    cargar_jugador( &(*juego).jugador, (*juego).niveles[(*juego).nivel_actual - 1], (*juego).nivel_actual, personaje_tp1);
}

//Pre: -
//Post: Define el multiplo por el cual se cambiaran de forma aleatoria la posicion de los papeleos.
int definir_multiplo( int nivel_actual){
    
    int multiplo = 0;

    if( nivel_actual == 1){
        multiplo = 7;
    
    }else if( nivel_actual == 2){
        multiplo = 5;
    
    }else if( nivel_actual == 3){
        multiplo = 3;
    }
    return multiplo;
}

//Pre: -
//Post: Define el maximo de paredes que van apareciendo en cada nivel a medida que el personaje realiza un movimiento valido.
int definir_paredes_adicionales( int nivel_actual){

    int paredes_adicionales = 0;

    if(nivel_actual == 1){
        paredes_adicionales = 40;
    
    }else if(nivel_actual == 2){
        paredes_adicionales = 30;
    
    }else if(nivel_actual == 3){
        paredes_adicionales = 20;
    }
    return paredes_adicionales;
}

//Pre: -
//Post: Genera la cantidad de paredes aleatorias necesarias segun el nivel en el que se este.
void generar_nuevas_paredes_aleatorias(juego_t* juego, nivel_t* nivel, int verificar_paredes, int verificar_obstaculos, int verificar_herramientas, int verificar_papeleos){

    int verificar = CONDICION_FALLIDA;
    int movimientos_hechos = definir_paredes_adicionales((*juego).nivel_actual);

    while( (*juego).jugador.movimientos_realizados <= movimientos_hechos && verificar == CONDICION_FALLIDA){
        
        coordenada_t coordenada = asignar_coordenadas((*nivel).paredes[(*nivel).tope_paredes - 1], (*juego).nivel_actual);
        verificaciones(coordenada, *nivel, PAREDES, &verificar_paredes, &verificar_obstaculos, &verificar_herramientas, &verificar_papeleos);

        if(se_cumplen_las_condiciones(verificar_paredes, verificar_obstaculos, verificar_herramientas, verificar_papeleos, coordenada, (*juego).jugador.posicion)){
                
            (*nivel).tope_paredes++;
            verificar = CONDICION_CUMPLIDA;
            (*nivel).paredes[(*nivel).tope_paredes - 1].fil = coordenada.fil;
            (*nivel).paredes[(*nivel).tope_paredes - 1].col = coordenada.col;
        }
    }
}

//Pre: -
//Post: Segun la cantidad de movimientos realizados, se generen coordenadas aleatorias para los papeleos mientras el interruptor ahuyenta randall esta desactivado.
void cambiar_posicion_papeleos( juego_t* juego, nivel_t* nivel){

    int i = 0;
    int verificar = CONDICION_FALLIDA; //NO_CAMBIARON_LUGARES;
    int multiplo = definir_multiplo((*juego).nivel_actual);
    int verificar_paredes = 0, verificar_obstaculos = 0, verificar_herramientas = 0, verificar_papeleos = 0;
    
    while(i < (*nivel).tope_papeleos && verificar == CONDICION_FALLIDA){

        coordenada_t coordenada = asignar_coordenadas((*nivel).papeleos[i].posicion, (*juego).nivel_actual);
        verificaciones(coordenada, *nivel, PAPELEO, &verificar_paredes, &verificar_obstaculos, &verificar_herramientas, &verificar_papeleos);

        if( se_cumplen_las_condiciones(verificar_paredes, verificar_obstaculos, verificar_herramientas, verificar_papeleos, coordenada, (*juego).jugador.posicion)
            && !esta_papeleo_recolectado((*nivel).papeleos[i]) && es_multiplo((*juego).jugador.movimientos_realizados, multiplo)){

            verificar = CONDICION_CUMPLIDA; //CAMBIARON_LUGARES;
            (*nivel).papeleos[i].posicion.fil = coordenada.fil;
            (*nivel).papeleos[i].posicion.col = coordenada.col;
        }
        i++;
    }
}

//Pre: -
//Post: Indica si el personaje pasa por el papeleo de menor denominacion actual sin recoletar, y en ese caso, lo marca como recolectado.
void recolectar_papeleos( juego_t* juego , nivel_t* nivel, int verificar_papeleos, int* papeleos_recogidos){

    int i = 0;
    int papeleo_encontrado = 0;

    while( i < (*nivel).tope_papeleos && verificar_papeleos == NO_SE_SUPERPONEN){

        if((*nivel).papeleos[i].recolectado == false ){
            verificar_si_coinciden_papeleos( (*juego).jugador.posicion , (*nivel).papeleos, (*nivel).tope_papeleos, &papeleo_encontrado, &verificar_papeleos);

            if( papeleo_encontrado == i + 1){
                (*nivel).papeleos[i].recolectado = true;
                (*papeleos_recogidos)++;
            }
            i++;
        
        }else{
            i++;
        }
    }
}

//Pre: -
//Post: Indica si el personaje pasa sobre un objeto, y de hacerlo, realiza la accion correspondiente.
void pasar_sobre_un_objeto( juego_t* juego, nivel_t* nivel, int verificar_obstaculos, int verificar_herramientas, char* tipo){

    verificar_si_coinciden_objetos( (*juego).jugador.posicion, (*nivel).obstaculos, (*nivel).tope_obstaculos, tipo, &verificar_obstaculos);
    verificar_si_coinciden_objetos( (*juego).jugador.posicion, (*nivel).herramientas, (*nivel).tope_herramientas, tipo, &verificar_herramientas);

    if( verificar_obstaculos == SE_SUPERPONEN){
    
        if( *tipo == FUEGOS){
            (*juego).jugador.movimientos = 0;
        
        }else if( *tipo == MEDIAS){
            (*juego).jugador.movimientos = (*juego).jugador.movimientos - 10;
        }
    
    }else if( verificar_herramientas == CONDICION_FALLIDA){
    
        if( *tipo == BOTELLAS_DE_GRITOS){
            (*juego).jugador.movimientos = (*juego).jugador.movimientos + 7;
        
        }else if( *tipo == INTERRUPTORES_AHUYENTA_RANDALL){
            
            if((*juego).jugador.ahuyenta_randall == false){
                (*juego).jugador.ahuyenta_randall = true;   
            
            }else if((*juego).jugador.ahuyenta_randall == true){
                (*juego).jugador.ahuyenta_randall = false;
            }
        }
    }
}

//Pre: -
//Post: Provoca que el personaje siempre se encuentre por encima de una pared y de no estarlo, que este caiga hasta encontrar una.
void gravedad_simulada(juego_t* juego, nivel_t* nivel){

    int papeleos_recogidos = 0;
    int i = (*juego).jugador.posicion.fil;
    int tope = determinar_tope((*juego).nivel_actual);
    
    int verificar_paredes = 0, verificar_obstaculos = 0, verificar_herramientas = 0, verificar_papeleos = 0;
    int verificar = verificar_paredes;
    
    while( i < tope && verificar == NO_SE_SUPERPONEN){
            
        coordenada_t otra_coordenada;
        otra_coordenada.fil = i + 1;
        otra_coordenada.col = (*juego).jugador.posicion.col;
        
        recolectar_papeleos( juego, nivel, verificar_papeleos, &papeleos_recogidos);
        int nivel_en_marcha = estado_nivel( (*nivel).papeleos, (*nivel).tope_papeleos);

        char tipo = ' ';
        pasar_sobre_un_objeto( juego, nivel, verificar_obstaculos, verificar_herramientas, &tipo);
        
        verificar_si_coinciden_paredes( otra_coordenada, MIKE, (*nivel).paredes, (*nivel).tope_paredes, &verificar_paredes);
        
        if( verificar_paredes == NO_SE_SUPERPONEN && nivel_en_marcha == JUGANDO && tipo != FUEGOS && (*juego).jugador.movimientos > 0){
            system("clear");
            imprimir_terreno(*juego);
            (*juego).jugador.posicion.fil++;
            detener_el_tiempo(0.25);
            system("clear");

        }else if( verificar_paredes == SE_SUPERPONEN || nivel_en_marcha == GANASTE){
            verificar = SE_SUPERPONEN;
        }
        i++;
    }
}

//Pre: -
//Post: Rota una coordenada de forma horaria.
void rotacion_horaria(coordenada_t* posicion, int nivel_actual){

    int tope = determinar_tope(nivel_actual);
    coordenada_t rotar = {(*posicion).fil, (*posicion).col};
    coordenada_t rotacion_horaria = {rotar.col, abs(rotar.fil - (tope - 1))};

    *posicion = rotacion_horaria;
}

//Pre: -
//Post: Rota una coordenada de forma antihoraria.
void rotacion_anithoraria(coordenada_t* posicion, int nivel_actual){

    int tope = determinar_tope(nivel_actual);
    coordenada_t rotar = {(*posicion).fil, (*posicion).col};
    coordenada_t rotacion_anithoraria = {abs(rotar.col - (tope - 1)), rotar.fil};

    *posicion = rotacion_anithoraria;
}

//Pre: -
//Post: Envia las coordenadas de todo el vector paredes para que rote segun el movimiento realizado.
void rotacion_paredes( nivel_t* nivel, int tope_paredes, int nivel_actual, char movimiento_realizado){

    for(int i = 0; i < tope_paredes; i++){

        if(movimiento_realizado == ROTACION_IZQUIERDA){
            rotacion_anithoraria( &(*nivel).paredes[i], nivel_actual);

        }else if(movimiento_realizado == ROTACION_DERECHA){
            rotacion_horaria( &(*nivel).paredes[i], nivel_actual);
        }
    }
}

//Pre: -
//Post: Envia las coordenadas de todo el vector objetos para que rote segun el movimiento realizado.
void rotacion_objetos(objeto_t objetos[MAX_ELEMENTOS], int tope_objetos, int nivel_actual, char movimiento_realizado){

    for(int i = 0; i < tope_objetos; i++){
        
        if(movimiento_realizado == ROTACION_IZQUIERDA){
            rotacion_anithoraria(&(objetos[i]).posicion, nivel_actual);

        }else if(movimiento_realizado == ROTACION_DERECHA){
            rotacion_horaria(&(objetos[i]).posicion, nivel_actual);
        }
    }
}

//Pre: -
//Post: Envia las coordenadas de todo el vector papeleos para que rote segun el movimiento realizado.
void rotacion_papeleos(papeleo_t papeleos[MAX_PAPELEOS], int tope_papeleos, int nivel_actual, char movimiento_realizado){

    for(int i = 0; i < tope_papeleos; i++){
        
        if(movimiento_realizado == ROTACION_IZQUIERDA){
            rotacion_anithoraria(&(papeleos[i]).posicion, nivel_actual);

        }else if(movimiento_realizado == ROTACION_DERECHA){
            rotacion_horaria(&(papeleos[i]).posicion, nivel_actual);
        }
    }
}

//Pre: -
//Post: Envia la coordenada del personaje para que rote segun el movimiento realizado.
void rotacion_personaje( coordenada_t* posicion_jugador, int nivel_actual, char movimiento_realizado){

    if(movimiento_realizado == ROTACION_IZQUIERDA){
        rotacion_anithoraria( posicion_jugador, nivel_actual);

    }else if(movimiento_realizado == ROTACION_DERECHA){
        rotacion_horaria( posicion_jugador, nivel_actual);
    }
}

//Pre: -
//Post: Controla todas las operaciones relacionadas con rotaciones.
void rotaciones(juego_t* juego, nivel_t* nivel, int numero_nivel, char movimiento_realizado){

    int verificar_paredes = 0, verificar_obstaculos = 0, verificar_herramientas = 0, verificar_papeleos = 0;

    rotacion_paredes( nivel, (*nivel).tope_paredes, (*juego).nivel_actual ,movimiento_realizado);
    rotacion_personaje( &(*juego).jugador.posicion, (*juego).nivel_actual, movimiento_realizado);
    rotacion_papeleos( (*nivel).papeleos, (*nivel).tope_papeleos, (*juego).nivel_actual, movimiento_realizado);
    rotacion_objetos( (*nivel).obstaculos, (*nivel).tope_obstaculos, (*juego).nivel_actual, movimiento_realizado);
    rotacion_objetos( (*nivel).herramientas, (*nivel).tope_herramientas, (*juego).nivel_actual, movimiento_realizado);

    generar_nuevas_paredes_aleatorias( juego, nivel, verificar_paredes, verificar_obstaculos, verificar_herramientas, verificar_papeleos);
}

//Pre: -
//Post: No permite al jugador avanzar si delante de el se encuentra una pared.
void movimiento_lateral(juego_t* juego, nivel_t* nivel, char movimiento_realizado){

    int verificar_paredes = 0, verificar_obstaculos = 0, verificar_herramientas = 0, verificar_papeleos = 0;

    coordenada_t coordenada;
    coordenada.fil = (*juego).jugador.posicion.fil;
    coordenada.col = (*juego).jugador.posicion.col;

    if(movimiento_realizado == DERECHA){
        coordenada.col++;
        verificar_si_coinciden_paredes( coordenada, MIKE, (*nivel).paredes, (*nivel).tope_paredes, &verificar_paredes);

    }else if(movimiento_realizado == IZQUIERDA){
        coordenada.col--;
        verificar_si_coinciden_paredes( coordenada, MIKE, (*nivel).paredes, (*nivel).tope_paredes, &verificar_paredes);
    }

    if(verificar_paredes == CONDICION_CUMPLIDA){

        if( movimiento_realizado == DERECHA){
            (*juego).jugador.posicion.col++;
        
        }else if( movimiento_realizado == IZQUIERDA){
            (*juego).jugador.posicion.col--;
        }
        
        (*juego).jugador.movimientos_realizados++;
        (*juego).jugador.movimientos--;
        generar_nuevas_paredes_aleatorias( juego, nivel, verificar_paredes, verificar_obstaculos, verificar_herramientas, verificar_papeleos);
    
    }else{
        printf("No puedes moverte en esa direccion\n");
    }
}

//Pre: -
//Post: Segun la direccion introducida, indica la posicion relativa respecto al juador de donde se debe usar determinada herramienta.
coordenada_t determinar_direccion( coordenada_t coordenada, char direccion_seleccionada){

    if(direccion_seleccionada == ARRIBA){
        coordenada.fil--;

    }else if(direccion_seleccionada == ABAJO){
        coordenada.fil++;

    }else if(direccion_seleccionada == DERECHA){
        coordenada.col++;

    }else if(direccion_seleccionada == IZQUIERDA){
        coordenada.col--;
    }
    return coordenada;
}

//Pre: -
//Post: Se encarga de eliminar un fuego del vector obstaculos si esta se encuentra alli.
void eliminar_fuego( juego_t* juego, nivel_t* nivel, coordenada_t  coordenada){

    int i = 0;
    int verificar = CONDICION_FALLIDA;

    while( i < (*nivel).tope_obstaculos && (*juego).jugador.extintores > 0 && verificar == CONDICION_FALLIDA){

        if( esta_la_posicion_ocupada((*nivel).obstaculos[i].posicion, coordenada) && (*nivel).obstaculos[i].tipo == FUEGOS){

            for(int j = i; j < (*nivel).tope_obstaculos - 1; j++){
                (*nivel).obstaculos[j] = (*nivel).obstaculos[j + 1];
            }
            verificar = CONDICION_CUMPLIDA;
            (*nivel).tope_obstaculos--;
            (*juego).jugador.extintores--;
        }
        i++;
    }
}

//Pre: -
//Post: Se encarga de eliminar una pared del vector de paredes si esta se encuentra alli.
void eliminar_pared( juego_t* juego, nivel_t* nivel, coordenada_t coordenada){

    int i = 0;
    int tope = determinar_tope( (*juego).nivel_actual);

    while( i < (*nivel).tope_paredes && (*juego).jugador.martillos > 0){

        if(esta_la_posicion_ocupada((*nivel).paredes[i], coordenada) && !se_puede_romper(coordenada, tope - 1)){

            (*nivel).paredes[i] = (*nivel).paredes[(*nivel).tope_paredes - 1];
            (*nivel).tope_paredes--;
            (*juego).jugador.martillos--;
        }
        i++;
    }
}

//Pre: -
//Post: Controla que segun la herramienta utilizada, la misma se utilice de forma correcta.
void eliminar_elemento(juego_t* juego, nivel_t* nivel, char herramienta_utilizada){

    char direccion_seleccionada = ' ';

    printf("Seleccione hacia donde quiere utilizar su herramienta");
    scanf(" %c", &direccion_seleccionada);

    while( no_es_direccion_valida( direccion_seleccionada)){ 
        
        printf("Por favor, ingrese una direccion valida (W,A,S o D)\n");
        scanf(" %c", &direccion_seleccionada);
    }

    coordenada_t coordenada = determinar_direccion((*juego).jugador.posicion, direccion_seleccionada);

    if( herramienta_utilizada == MARTILLO){
        eliminar_pared( juego, nivel, coordenada);

    }else if( herramienta_utilizada == EXTINTOR){
        eliminar_fuego( juego, nivel, coordenada);
    }
    imprimir_terreno( *juego);
}

//Pre:
//Post: Devuelve 0 si todavia no se recolectaron los papeleos y 1 si se recolectaron todos.
int estado_nivel(papeleo_t papeleos[MAX_PAPELEOS], int tope_papeleos){

    int papeleos_recogidos = 0;

    for(int i = 0; i < tope_papeleos; i++){

        if(papeleos[i].recolectado == true){
            papeleos_recogidos++;
        }
    }

    if(papeleos_recogidos == tope_papeleos){
        return GANASTE;
    }else{
        return JUGANDO;
    }
}

//Pre: La estructura del juego debe estar validada previamente.
//Post: Devuelve 0 si todavia se esta jugando, 1 si se gano el ultimo nivel o 0 si no quedan mas movimientos.
int estado_juego( juego_t juego){
    
    int estado_juego = JUGANDO;
    int nivel = estado_nivel( juego.niveles[juego.nivel_actual - 1].papeleos, juego.niveles[juego.nivel_actual - 1].tope_papeleos);

    if( juego.jugador.movimientos <= 0){
        estado_juego = PERDISTE;
        printf(" PERDISTE ");

    }else if( juego.nivel_actual != 3 && nivel == GANASTE){
        estado_juego = JUGANDO;
    
    }else if( juego.nivel_actual == 3 && nivel == GANASTE){
        estado_juego = GANASTE;
        printf(" GANASTE ");
    }
    return estado_juego;
}

//Pre: -
//Post: Imprime las intrucciones correspondientes para que el jugador sepa los comandos.
void informacion_de_juego(juego_t juego){
    
    printf(" Presiones D para ir a la derecha.\n");
    printf(" Presione A para ir a la izquierda.\n");
    printf(" Presione E para rotar en sentido horario.\n");
    printf(" Presione Q para rotar en sentido antihorario.\n");
    printf(" Presion Z para utilizar un martillo\n");
    printf(" Presione C para utilizar un extintos\n");
    printf(" MARTILLOS RESTANTES: %i", juego.jugador.martillos);
    printf(" EXTINTORES RESTANTES: %i\n", juego.jugador.extintores);
    printf(" MOVIMIENTOS RESTANTES: %i\n", juego.jugador.movimientos);
}

//Pre: -
//Post: Al completar un nivel, actualiza los datos necesarios para el jugador (martillos, extintores, movimientos, etc).
void subir_de_nivel( juego_t* juego){
    
    int movimientos_sobrantes = (*juego).jugador.movimientos;
    int nivel_en_marcha = estado_nivel( (*juego).niveles[(*juego).nivel_actual - 1].papeleos, (*juego).niveles[(*juego).nivel_actual - 1].tope_papeleos);
    
    if( nivel_en_marcha == GANASTE){
        (*juego).jugador.movimientos_realizados = 0;
    }

    if( (*juego).nivel_actual == 2 && (*juego).jugador.movimientos_realizados == 1){
        definir_posicion_jugador( &(*juego).jugador, (*juego).niveles[(*juego).nivel_actual - 1]);
        definir_nivel_jugador( &(*juego).jugador, (*juego).nivel_actual, movimientos_sobrantes);
        (*juego).jugador.ahuyenta_randall = false;
    
    }else if((*juego).nivel_actual == 3 && (*juego).jugador.movimientos_realizados == 1){
        definir_posicion_jugador( &(*juego).jugador, (*juego).niveles[(*juego).nivel_actual - 1]);
        definir_nivel_jugador( &(*juego).jugador, (*juego).nivel_actual, movimientos_sobrantes);
        (*juego).jugador.ahuyenta_randall = false;

    }
}

//Pre: -
//Post: Mueve al personaje y realiza la accion necesaria en caso de chocar con un elemento.
void realizar_jugada(juego_t* juego){

    informacion_de_juego( *juego);
        
    char movimiento_realizado = ' ';
    scanf(" %c", &movimiento_realizado);

    while(!es_caracter_valido(movimiento_realizado)){

        printf("Por favor, ingrese uno de los comandos solicitados\n");
        scanf(" %c", &movimiento_realizado);
    }
    if( !selecciona_herramienta( movimiento_realizado)){

        if( movimiento_realizado == DERECHA){
            movimiento_lateral(juego, &(*juego).niveles[(*juego).nivel_actual - 1], movimiento_realizado);

        }else if( movimiento_realizado == IZQUIERDA){
            movimiento_lateral(juego, &(*juego).niveles[(*juego).nivel_actual - 1], movimiento_realizado);
        
        }else if( movimiento_realizado == ROTACION_DERECHA || movimiento_realizado == ROTACION_IZQUIERDA){
            rotaciones( juego, &(*juego).niveles[(*juego).nivel_actual - 1], (*juego).nivel_actual - 1, movimiento_realizado);

            (*juego).jugador.movimientos_realizados++;
            (*juego).jugador.movimientos--;
        }
        if((*juego).jugador.ahuyenta_randall == false){
            cambiar_posicion_papeleos( juego, &(*juego).niveles[(*juego).nivel_actual - 1]);
        }
    
    }else{
        eliminar_elemento( juego, &(*juego).niveles[(*juego).nivel_actual - 1], movimiento_realizado);
    }

    system("clear");
    gravedad_simulada( juego, &(*juego).niveles[(*juego).nivel_actual - 1]);
    subir_de_nivel( juego);
}
