//
// Created by Dylan on 09/03/2023.
//
#include "Giocatori.h"
#include "Carte.h"
#include "Mazzi.h"
#include "Struct_enum.h"

/**
 * Funzione che pulisce il terminale.
 */
void clear_output()
{
    #ifdef _WIN64
        system("cls");
    #else
        system("clear");
    #endif
}

/**
 * Funzione che concatena l'estensione .txt all'input del nome file inserito dall'utente.
 * @param nomefile = stringa inserita dall'utente del file di salvataggio
 * @return stringa con aggiunto il suffisso
 */
char *aggiunta_suffisso_salvataggi(char *nomefile)
{
    return strcat(nomefile, SUFFISSO_FILE);
}

/**
 * Funzione che legge tutti i dati salvati in un file di gioco con estensione .sav per riprendere una partita in corso.
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori in partita
 * @param player = indice giocatore che inizia a giocare la partita (o che riprende la partita in corso)
 * @param mazzo_pesca = mazzo da cui si pescano le carte
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param file_gioco = puntatore al file di salvataggio
 */
void caricamento_salvataggio_partita(Giocatore * giocatori, int *num_giocatori, int *player, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta,  FILE *file_gioco)
{
    ///CARICAMENTO ARRAY GIOCATORI
    for (int i = 0; i < (*num_giocatori); ++i)
    {
        fread(&giocatori[i], sizeof(Giocatore), 1, file_gioco);

        //allocazione array di carte in mano
        giocatori[i].carte_in_mano.array_carte= (Carta*) calloc(giocatori[i].carte_in_mano.numero_carte, sizeof(Carta));
        for (int j = 0; j < giocatori[i].carte_in_mano.numero_carte; ++j)
        {
            fread(&giocatori[i].carte_in_mano.array_carte[j], sizeof(Carta), 1, file_gioco);
        }

        //allocazione array di carte in gioco
        giocatori[i].carte_in_gioco.array_carte= (Carta*) calloc(giocatori[i].carte_in_gioco.numero_carte, sizeof(Carta));
        for (int j = 0; j < giocatori[i].carte_in_gioco.numero_carte; ++j)
        {
            fread(&giocatori[i].carte_in_gioco.array_carte[j], sizeof(Carta), 1, file_gioco);
        }

    }

    ///caricamento giocatore che inizia a giocare
    fread(player, sizeof(int), 1, file_gioco);

    ///LETTURA MAZZO PESCA
    fread(mazzo_pesca, sizeof(Mazzo), 1, file_gioco);
    mazzo_pesca->array_carte= (Carta*) calloc(mazzo_pesca->numero_carte, sizeof(Carta));
    fread(mazzo_pesca->array_carte, sizeof(Carta), mazzo_pesca->numero_carte, file_gioco);

    ///LETTURA MAZZO SCARTA
    fread(mazzo_scarta, sizeof(Mazzo), 1, file_gioco);
    mazzo_scarta->array_carte= (Carta*) calloc(mazzo_scarta->numero_carte, sizeof(Carta));
    fread(mazzo_scarta->array_carte, sizeof(Carta), mazzo_scarta->numero_carte, file_gioco);

}

/**
 * Funzione analoga alla precedente, solo che anziche' leggere un file di salvataggio, ne crea uno nuovo scrivendo tutti
 * i dati della partita in corso.
 * La partita viene salvata all'inizio di ogni turno
 *
 * @param nome_file = nome del file dove vengono scritti o sovrascritti i dati di salvataggio
 * @param num_giocatori = numero di giocatori in partita
 * @param giocatori = array di giocatori
 * @param player = indice giocatore che deve riprendere il turno al prossimo caricamento
 * @param mazzo_pesca = mazzo da cui si pescano le carte
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 */
void salva_partita(char *nome_file, int *num_giocatori, Giocatore *giocatori, int *player, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta)
{
    FILE *file_gioco=NULL;

    file_gioco = fopen(nome_file, "wb");
    if(file_gioco==NULL)
    {
        printf("\nERROR, file non creato correttamente!");
    }

    //salvataggio numero di giocatori
    fwrite(num_giocatori, sizeof(int), 1, file_gioco);

    //salvataggio dati giocatori
    for (int i = 0; i < (*num_giocatori); ++i)
    {
        fwrite(&giocatori[i], sizeof(Giocatore), 1, file_gioco);

        //salvataggio carte in mano giocatore i-esimo
        for (int j = 0; j < giocatori[i].carte_in_mano.numero_carte; ++j)
        {
            fwrite(&giocatori[i].carte_in_mano.array_carte[j], sizeof(Carta), 1, file_gioco);
        }

        //salvataggio carte in gioco giocatore i-esimo
        for (int j = 0; j < giocatori[i].carte_in_gioco.numero_carte; ++j)
        {
            fwrite(&giocatori[i].carte_in_gioco.array_carte[j], sizeof(Carta), 1, file_gioco);
        }
    }

    //salvataggio giocatore che sta giocando il turno in corso
    fwrite(player, sizeof(int), 1, file_gioco);

    //salvataggio dati mazzo pesca
    fwrite(mazzo_pesca, sizeof(Mazzo), 1, file_gioco);
    //salvataggio carte mazzo pesca
    fwrite(mazzo_pesca->array_carte, sizeof(Carta), mazzo_pesca->numero_carte, file_gioco);

    //salvataggio dati mazzo scarta
    fwrite(mazzo_scarta, sizeof(Mazzo), 1, file_gioco);
    //salvataggio carte mazzo scarta
    fwrite(mazzo_scarta->array_carte, sizeof(Carta), mazzo_scarta->numero_carte, file_gioco);


    ///fine salvataggio
    fclose(file_gioco);
}

/**
 * Funzione che libera tutto lo spazio allocato durante la partita
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori in partita
 * @param mazzo_pesca = mazzo da cui si pescano le carte
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 */
void total_free(Giocatore *giocatori, int num_giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta)
{
    //free degli array di carte di ogni giocatore
    for (int i = 0; i < num_giocatori; ++i)
    {
        if(giocatori[i].carte_in_mano.numero_carte>0)
        {
            free(giocatori[i].carte_in_mano.array_carte);
        }

        if(giocatori[i].carte_in_gioco.numero_carte>0)
        {
            free(giocatori[i].carte_in_gioco.array_carte);
        }
    }

    //free array di carte dei mazzi
    if(mazzo_pesca->numero_carte>0)
    {
        free(mazzo_pesca->array_carte);
    }

    if(mazzo_scarta->numero_carte>0)
    {
        free(mazzo_scarta->array_carte);
    }

    //free dell'array dinamico di giocatori
    free(giocatori);
}
