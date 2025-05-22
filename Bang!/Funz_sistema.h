//
// Created by Dylan on 09/03/2023.
//
#include "Giocatori.h"
#include "Carte.h"
#include "Mazzi.h"
#include "Struct_enum.h"

#ifndef PROGETTO_FINALE_PR1_FUNZ_SISTEMA_H
#define PROGETTO_FINALE_PR1_FUNZ_SISTEMA_H

void clear_output();
char *aggiunta_suffisso_salvataggi(char *nomefile);
void caricamento_salvataggio_partita(Giocatore * giocatori, int *num_giocatori, int *player, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, FILE *file_gioco);
void salva_partita(char *nome_file, int *num_giocatori, Giocatore *giocatori, int *player, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta);

///FUNZIONE FREE FINALE
void total_free(Giocatore *giocatori, int num_giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta);

#endif //PROGETTO_FINALE_PR1_FUNZ_SISTEMA_H
