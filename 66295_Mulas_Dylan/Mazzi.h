//
// Created by Dylan on 02/03/2023.
//

#ifndef PROGETTO_FINALE_PR1_MAZZI_H
#define PROGETTO_FINALE_PR1_MAZZI_H

//inclusione header
#include "Struct_enum.h"

///DICHIARAZIONI PROTOTIPI FUNZIONI
void caricamento_mazzo_file(Mazzo *mazzo_pesca);
void check_correttezza_mazzo(Giocatore *giocatori, Mazzo *mazzo_generico, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int num_giocatori);
void visualizza_tipo_mazzo(Mazzo mazzo_generico);
void visualizzatore_di_mazzi(Mazzo *mazzo_generico);
void visualizzatore_di_carta(Mazzo *mazzo_generico, int carta);
void mischiatore_di_carte(Mazzo *mazzo_generico, int numero_scambi);

void pesca_carta(Mazzo *pesca, Mazzo *target);
void scarta_carta(Mazzo *mazzo_in_uso, Mazzo *scarta);

void scarta_carta_specifica_mano(Giocatore *giocatori, Mazzo *mazzo_scarta, int player, int carta_scarto);
void scarta_carta_specifica_gioco(Giocatore *giocatori, Mazzo *mazzo_scarta, int player, int carta_scarto);
void scarta_carta_specifica_mazzo_generico(Mazzo *mazzo_target,  Mazzo *mazzo_scarta, int carta_scarto);

void check_mazzo_pesca_vuoto(Mazzo *mazzo_pesca, Mazzo *mazzo_scarta);

int estrai(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int seme, int player, FILE *log);

#endif //PROGETTO_FINALE_PR1_MAZZI_H


