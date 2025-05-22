//
// Created by Dylan on 13/03/2023.
//

#ifndef PROGETTO_FINALE_PR1_STRUCT_ENUM_H
#define PROGETTO_FINALE_PR1_STRUCT_ENUM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define DIM_NOME_CARTA 14
#define SCAMBI_CARTE_DA_FILE 300
#define N_SCAMBI_CARTE 100

#define DIM_NOME_GIOC 49
#define QUATTRO_GIOCATORI 4
#define CINQUE_GIOCATORI 5
#define SEI_GIOCATORI 6
#define SETTE_GIOCATORI 7
#define HP_SCERIFFO 5
#define HP_RUOLI_BASE 4
#define GITTATA_BASE 1
#define DIMENSIONE_ORIGINALE_MAZZO_PESCA 80
#define DIM_NOME_FILE 19
#define SUFFISSO_FILE ".sav"
#define DIM_SUFFISSO_FILE 4

#define RED_TEXT "\x1b[31m"
#define LIGHT_RED_TEXT "\x1b[91m"
#define BLUE_TEXT "\x1b[94m"
#define GREEN_TEXT "\x1b[32m"
#define WHITE_TEXT "\x1b[37m"
#define MAGENTA_TEXT "\x1b[35m"
#define CYAN_TEXT "\x1b[36m"
#define YELLOW_TEXT "\x1b[33m"
#define LIGHT_YELLOW "\x1b[93m"
#define LIGHT_BLACK "\x1b[90m"

#define RIPRISTINA "\x1b[0m"


typedef enum{SCERIFFO, VICESCERIFFO, FUORILEGGE, RINNEGATO} Ruoli;

typedef enum {MAZZO_PESCA, MAZZO_SCARTO, GIOC_MANO, GIOC_GIOCO} Tipologia_mazzo;

typedef enum {ISTANTANEA=1, ISTANTANEA_SPECIAL, ARMA, EFFETTO} Tipologia_carta;

typedef enum {CUORI=0, QUADRI, FIORI, PICCHE}Seme;

typedef enum {VOLCANIC=1, SCHOFIELD, REMINGTON, CARABINA, WINCHESTER}Gittata_armi;

typedef struct
{
    char nome_carta[DIM_NOME_CARTA+1];
    Tipologia_carta tipo;
    int numero_carta;                       //compreso tra 1 e 13
    Seme seme;

}Carta;

typedef struct
{
    Tipologia_mazzo tipo_mazzo;
    int numero_carte;
    Carta * array_carte;                //array dinamico di tipo Carte

}Mazzo;

typedef struct
{
    char nome [DIM_NOME_GIOC+1];
    Ruoli ruolo;
    int hp;
    Mazzo carte_in_mano;
    Mazzo carte_in_gioco;
    int gittata;

} Giocatore;



#endif //PROGETTO_FINALE_PR1_STRUCT_ENUM_H
