#include <stdio.h>
#include "dia_en_la_uni.h"

static const char OLAF = 'O';
static const char STITCH = 'S';
static const char RAYO_MCQUEEN = 'R';
static const char JASMIN = 'J';

const char TRANSPORTE_OLAF = 'T';
const char TRANSPORTE_STITCH = 'N';
const char TRANSPORTE_RAYO = 'C';

const char ARMA_OLAF = 'B';
const char ARMA_STITCH = 'P';
const char ARMA_RAYO = 'K';

const int valor_arma_olaf = 3;
const int valor_arma_stitch = 6;
const int valor_arma_rayo = 9;
const int valor_arma_jasmin = 12;

const int valor_transporte_olaf = 10;
const int valor_transporte_stitch = 10;
const int valor_transporte_rayo = 20;
const int valor_transporte_jasmin = 20;

const int hora_correcta = 1;
const int hora_incorrecta = 2;

const int mutiplicador_nota_uno = 1;
const int mutiplicador_nota_dos = 2;
const int mutiplicador_nota_tres = 3;
const int mutiplicador_nota_cuatro = 4;

void introduccion_uno(){

  printf("Para seleccionar el personaje mas adecuado para ti, te presetaremos el siguiente problema.\n");
  printf("Tenes que rendir un examen en tu universidad pero no tenes forma de llegar debido a un paro de transportes, pero por suerte");
  printf(" uno de cuatro amigos lograra ayudarte. Segun tus respuestas averiguaremos quien es el mas indicado para ti.\n");
}

//Pre condicion: -
//Post condicion: -
int valor_transporte(int respuesta){

  if(respuesta == TRANSPORTE_OLAF){

    return valor_transporte_olaf;
  } else if(respuesta == TRANSPORTE_STITCH){

    return valor_transporte_stitch;
  } else if(respuesta == TRANSPORTE_RAYO){

    return valor_transporte_rayo;
  }  else{

    return valor_transporte_jasmin;
  }
  return -1;
}

//Pre condicion: -
//Post condicion: -
int valor_arma(char respuesta){

  if(respuesta == ARMA_OLAF){

    return valor_arma_olaf;
  } else if(respuesta == ARMA_STITCH){

    return  valor_arma_stitch;
  } else if(respuesta == ARMA_RAYO){

    return valor_arma_rayo;
  } else{

    return valor_arma_jasmin;
  }
  return -1;
}

//Pre condicion: -
//Post condicion: -
int valor_hora (float hora){

  if(hora >= 0.0 && hora <= 15.30){
    return hora_correcta;

  }else{
  return hora_incorrecta;
   
  }
  return -1;
}

//Pre condicion: -
//Post condicion: -
int valor_nota(int respuesta){

  if(respuesta >= 1 && respuesta <= 3){
    return mutiplicador_nota_uno;
  } else if(respuesta >= 4 && respuesta <= 6){
    return mutiplicador_nota_dos;
  } else if(respuesta >= 7 && respuesta <= 8){
    return mutiplicador_nota_tres;
  } else{
    return mutiplicador_nota_cuatro;
  }
  return -1;
}

//Pre condicion: -
//Post condicion: Le asigna un valor a TRANSPORTE segun la letra que introduzca el usuario.
void pedir_valor_transporte(char* TRANSPORTE){

  printf("PREGUNTA NUMERO UNO: Que transporte preferirias?\n");
  printf("Coloque (T) para trineo, (N) para nave espacial, (C) para camion o (A) para alfombra\n");
  scanf(" %c", TRANSPORTE); 
  while(*TRANSPORTE != 'T' && *TRANSPORTE != 'N' && *TRANSPORTE != 'C' && *TRANSPORTE != 'A'){
    printf("Respuesta invalida, intente de nuevo.\n");
    scanf(" %c", TRANSPORTE);
  }
}

//Pre condicion: -
//Post condicion: Le asigna un valor a ARMA segun la letra que introduzca el usuario.
void pedir_valor_arma(char* ARMA){

  printf("PREGUNTA NUMERO DOS: Te encontras una manifestacion en el camino, que elegirias para deshacerte de ella?\n");
  printf("Coloque (B) para bolas de nieve, (P) para pium pium, (K) para el kuchau o (G) para el genio\n");
  scanf(" %c", ARMA);
  while(*ARMA != 'B' && *ARMA != 'P' && *ARMA != 'K' && *ARMA != 'G'){
    printf("Respuesta invalida, intente de nuevo.\n");
    scanf(" %c", ARMA);
  }
}

//Pre condicion: La parte entera debe separarse de la decimal con un punto, ej: 15.30 y no 15,30
//Post condicion: Le asigna un valor a HORA segun el decimal que introduzca el usuario.
void pedir_valor_hora(float* HORA, float* DECIMAL){

  printf("PREGUNTA NUMERO TRES: Luego de superar la manifestacion, ¿A que hora conseguiste llegar a la universidad?\n");
  printf("Escriba primero las horas (un numero entre 0 y 23) y luego seguido de un punto los minutos (un numero entre 0 y 59)\n");
  
  scanf(" %f", HORA);
  int PASAJE = (int)*HORA;
  *DECIMAL = *HORA - (float)PASAJE;

  while(*HORA < 0 || *HORA >= 24 || *DECIMAL < 0 || *DECIMAL >= 0.60){
    printf("Numero invalido, si quiere escribir, por ejemplo, las 15:30, debe escribir 15.30\n");
    scanf(" %f", HORA);
    int PASAJE = (int)*HORA;
    *DECIMAL = *HORA - (float)PASAJE;
  }
  printf("La hora introducida es %4.2f. ", *HORA); //Mi intencion con el 4.2 es redondear los decimales para que nos muestre un valor util para identificar la hora.
}
  
//Pre condicion: -
//Post condicion: Le asigna un valor a NOTA segun el numero que introduzca el usuario.
void pedir_valor_nota(int* NOTA){

  printf("PREGUNTA NUMERO CUATRO: Por fin llego la nota del parcial,¿Que nota lograste sacarte?\n");
  printf("Debe ser un numero del 1 al 10\n");

  scanf(" %i", NOTA);
  
  while(*NOTA < 1 || *NOTA > 10){
    
    printf("El valor introducido es incorrecto, intente nuevamente\n");
    scanf(" %i", NOTA);
  }
}

//Pre condicion: -
//Post condicion: -
char definir_personaje(int asignar_valor_transporte,int asignar_valor_arma,int asignar_valor_hora,int asignar_valor_nota){
  
  int PUNTAJE = asignar_valor_transporte + ( asignar_valor_arma * asignar_valor_nota );
  printf("Has obtenido un puntaje de %i.\n", PUNTAJE);

  if(PUNTAJE <= 41 && PUNTAJE >= 13 && asignar_valor_hora == 1){
    printf("Tu personaje es -Stitch-\n");
    return STITCH;
  }else if(PUNTAJE > 41 && asignar_valor_hora == 1){
  printf("Tu personaje es -Jasmín-\n ");
  return JASMIN;
  }else if(PUNTAJE <= 41 && PUNTAJE >= 13 && asignar_valor_hora == 2){
  printf("Tu personaje es -Olaf- \n"); 
  return OLAF;
  }else{
  printf("Tu personaje es el -Rayo McQueen-0 \n");
  return RAYO_MCQUEEN;
  }
}

void seleccionar_personaje(char *personaje_tp1){

  char TRANSPORTE = ' ';
  char ARMA = ' ';
  float HORA = 0;
  float DECIMAL = 0;
  int NOTA = 0;

  introduccion_uno();
  pedir_valor_transporte(&TRANSPORTE);
  int asignar_valor_transporte = valor_transporte(TRANSPORTE);
  pedir_valor_arma(&ARMA);
  int asignar_valor_arma = valor_arma(ARMA);
  pedir_valor_hora(&HORA,&DECIMAL);
  int asignar_valor_hora = valor_hora(HORA);
  pedir_valor_nota(&NOTA);
  int asignar_valor_nota = valor_nota(NOTA);
  
  *personaje_tp1 = definir_personaje(asignar_valor_transporte,asignar_valor_arma,asignar_valor_hora,asignar_valor_nota);
}