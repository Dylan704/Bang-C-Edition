//
// Created by Dylan on 02/03/2023.
//

#ifndef PROGETTO_FINALE_PR1_GIOCATORI_H
#define PROGETTO_FINALE_PR1_GIOCATORI_H

//inclusione header
#include "Struct_enum.h"

///DICHIARAZIONI PROTOTIPI FUNZIONI PREPARAZIONE
int inserisci_numero_giocatori();
void inserisci_nomi(Giocatore * giocatori, int num_giocatori, int dim_nome_gioc);
int dim_nome_maggiore(Giocatore *giocatori, int num_giocatori);
int dim_nome_minore(Giocatore *giocatori, int num_giocatori);
void assegnamento_ruoli(Giocatore * giocatori, int num_giocatori);
void inizializzazione_mazzi_gittata_players(Giocatore * giocatori, int num_giocatori);

Giocatore *preparazione_partita(Giocatore * giocatori, int *num_giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta);

void visualizza_nomi(Giocatore * giocatori, int num_giocatori);
void visualizzatore_ruoli(Giocatore * giocatori, int num_giocatori);
void visualizzatore_ruolo_singolo(Giocatore * giocatori, int i);
char *visualizzatore_termine_ruolo(Giocatore * giocatori, int player);
void distribuzione_carte_iniziale(Giocatore * giocatori, Mazzo *mazzo_pesca, int num_giocatori);
int find_posizione_sceriffo(Giocatore *giocatori, int num_giocatori);

void check_correttezza_giocatori(Giocatore *giocatori, int num_giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int hp_max_sceffo, int hp_mazzo_ruoli_base);

///FUNZIONI GIOCO
void output_avvio_gioco();
void output_ogni_turno(Giocatore *giocatori, int turno, int player);
void output_ogni_mossa(Giocatore *giocatori, int num_giocatori);
void output_libro();
int scelta_pagina_libro();

int distanza_tra_due_player(Giocatore *giocatori, int num_giocatori, int giocatore1, int giocatore2);
void output_distanze_tutti_giocatori(Giocatore* giocatori, int num_giocatori, int giocatore);

int scelta_mosse_player(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int player, int *num_giocatori, int *numero_bang, int *chiusura_gioco, FILE *log);
void gioca_una_carta(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int player, int *num_giocatori, int *numero_bang, FILE *log);
void pescata_ogni_turno(Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, Giocatore *giocatori, int player, FILE*log);
void scarto_fine_turno( Mazzo *mazzo_scarta, Giocatore *giocatori, int player, FILE *log);

void scarta_mazzo(Mazzo *mazzo_target, Mazzo *mazzo_scarta);

void check_morte_giocatore(Giocatore *giocatori, Mazzo *pesca, Mazzo *mazzo_scarta, int *num_giocatori, int *player_attuale, int *turno_in_corso, FILE *log);
void bonus_malus_kill(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int player_attuale, int player_morto, FILE *log);
int check_bersagli_da_colpire(Giocatore *giocatori, int player ,int *num_giocatori);
int check_distanza_1(Giocatore *giocatori, int player ,int *num_giocatori);

///FUNZIONI VITTORIA
int vittoria(Giocatore *giocatori, int num_giocatori);
void incremento_counter_vittoria(Giocatore *giocatori, int num_giocatori, int *cont_sceriffo, int *cont_fuorilegge, int *cont_rinnegato, int *cont_vice);
void output_vittoria(Giocatore *giocatori, int num_giocatori, FILE *LOG);

#endif //PROGETTO_FINALE_PR1_GIOCATORI_H
