//
// Created by Dylan on 02/03/2023.
//

#ifndef PROGETTO_FINALE_PR1_CARTE_H
#define PROGETTO_FINALE_PR1_CARTE_H

//inclusione header
#include "Struct_enum.h"

///DICHIARAZIONI PROTOTIPI FUNZIONI
int check_carta_mano(Giocatore *giocatori, int target_giocatore, char carta []);
int check_carta_campo(Giocatore *giocatori, int target_giocatore, char carta []);

void bang(Giocatore *giocatori, int target_bang, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int player, FILE *log);
void gatling(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int player, int *num_giocatori, FILE *log);

void diligenza(Giocatore *giocatori, int player, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, FILE *log);
void WellsFargo(Giocatore *giocatori, int player, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, FILE *log);

void birra(Giocatore *giocatori, int player, FILE *log);
int birra_in_extremis(Giocatore *giocatori, int target_giocatore, char carta [], Mazzo *mazzo_scarta, FILE *log);

int check_panico(Giocatore *giocatori, int player, int num_giocatori);

void panico(Giocatore *giocatori, int player, int num_giocatori, FILE *log);
void pesca_carta_casuale_da_mano(Giocatore *giocatori, int target, int player);

void saloon(Giocatore *giocatori, int num_giocatori, FILE *log);
int check_saloon(Giocatore *giocatori, int num_giocatori);

int check_catbalou(Giocatore *giocatori, int target);
int gioco_catbalou(Giocatore *giocatori, int player, int num_giocatori);
void catbalou(Giocatore *giocatori, Mazzo *mazzo_scarta, int player, int num_giocatori, FILE *log);
void scarta_carta_casuale_da_mano(Giocatore *giocatori, Mazzo *mazzo_scarta, int target);

void duello(Giocatore *giocatori, int player, int num_giocatori, Mazzo *mazzo_scarta, FILE *log);

void indiani(Giocatore *giocatori, Mazzo *mazzo_scarta, int player, int num_giocatori, FILE *log);

void emporio(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int player, int num_giocatori, FILE *log);

void gioca_arma(Giocatore *giocatori, int player, Mazzo *mazzo_scarta, int scelta, FILE *log);
int check_volcanic(Giocatore *giocatori, int player);

void mirino(Giocatore *giocatori, int player, int scelta, FILE *log);
void mustang(Giocatore *giocatori, int player, int scelta, FILE *log);

void barile(Giocatore *giocatori, int player, int scelta, FILE *log);

void dinamite(Giocatore *giocatori, int player, int scelta, FILE *log);
void check_esplosione_dinamite(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int player, int num_giocatori, FILE *log);

void prigione(Giocatore *giocatori, int player, int num_giocatori, int scelta, FILE *log);
void check_prigione(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int *turno, int player, int *turno_in_corso, FILE *log);

#endif //PROGETTO_FINALE_PR1_CARTE_H

