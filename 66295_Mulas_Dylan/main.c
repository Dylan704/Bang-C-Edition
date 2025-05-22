//NOME: Dylan Mulas (d.mulas32@studenti.unica.it)
//Matricola: 60/61//66295
//Tipologia progetto:medio

//inclusione header
#include "Giocatori.h"
#include "Funz_sistema.h"

int main()
{
    ///pulisci schermo per permettere agli escape codes di funzionare (non so perche' funzionano se faccio cosi' sinceramente, pero' era l'unico modo)
    clear_output();

    srand(time(NULL));              //generazione seme casuale

    ///DICHIARAZIONI VARIABILI
    int num_giocatori;              //numero di giocatori in partita, si aggiorna ogni morte
    int turno=0;                    //numero turno in corso
    int numero_bang=0;              //numero bang giocati in un turno


    int player;                   //indice giocatore che sta giocando il turno in corso
    int passa_turno;              //variabile che serve per il passaggio del turno al giocatore successivo

    int turno_in_corso = true;     //variabile booleana, se falsa il turno attuale viene skippato e si passa al successivo
    int partita_finita = false;    //variabile che se vera termina la partita e permette di visualizzare l'output dei vincitori
    int chiusura_gioco = false;

    /// GIOCATORI E MAZZI BASE
    Giocatore *giocatori = NULL;          //array di giocatori
    Mazzo mazzo_pesca;              //array mazzo da cui si pesca
    Mazzo mazzo_scarta;             //array mazzo in cui si mettono le carte scartate

    ///FILE E STRINGHE
    char *nome_file;    //nome file da caricare oppure da creare in caso di nuova partita
    nome_file=(char*) malloc((DIM_NOME_FILE+DIM_SUFFISSO_FILE+1) * sizeof(char));

    FILE *file_gioco=NULL;              //puntatore a file
    int scelta_nuova_partita;
    
    char nome_log[DIM_NOME_FILE+1]="Log.txt";       //nome file log e puntatore a log
    FILE *log=NULL;

    output_avvio_gioco();

    do {
        printf("\nVuoi creare una nuova partita? [1=SI/0=NO] ");
        scanf("%d", &scelta_nuova_partita);
        getchar();
    }while(scelta_nuova_partita!=0 && scelta_nuova_partita!=1);


    ///--------------------------------------------------------------------------------------------------------------///
    ///---------------------------------------------PREPARAZIONE GIOCO-----------------------------------------------///
    ///--------------------------------------------------------------------------------------------------------------///

    ///CREAZIONE PARTITA NUOVA
    if(scelta_nuova_partita==1)
    {
        printf("\nInserisci nome file da creare:");
        scanf("%[^\n]s", nome_file);
        getchar();

        nome_file = aggiunta_suffisso_salvataggi(nome_file);

        ///PREPARAZIONE NUOVA PARTITA
        giocatori = preparazione_partita(giocatori, &num_giocatori, &mazzo_pesca, &mazzo_scarta);
        player = find_posizione_sceriffo(giocatori, num_giocatori);
    }

        ///CARICAMENTO FILE BINARIO DI SALVATAGGIO
    else
    {
        ///inserimento nome file da caricare
        printf("\nInserisci nome file da caricare:");
        scanf("%[^\n]s", nome_file);
        getchar();
        nome_file = aggiunta_suffisso_salvataggi(nome_file);

        ///APERTURA FILE...
        file_gioco = fopen(nome_file, "rb");
        if(file_gioco==NULL)
        {
            printf("\nERRORE, apertura corrotta");
            exit(-69);
        }

        ///lettura numero di giocatori
        fread(&num_giocatori, sizeof(int), 1, file_gioco);

        ///allocazione array giocatori
        giocatori = (Giocatore*) calloc((num_giocatori), sizeof(Giocatore));
        if(giocatori==NULL)
        {
            exit(-1);
        }

        ///caricamento dati giocatori e mazzi
        caricamento_salvataggio_partita(giocatori, &num_giocatori, &player, &mazzo_pesca, &mazzo_scarta, file_gioco);

        ///...CHIUSURA FILE
        fclose(file_gioco);

        printf("\n%s...CARICAMENTI COMPLETATI!%s\n",GREEN_TEXT, RIPRISTINA);
    }

    ///----------------------------------------------FINE PREPARAZIONE-----------------------------------------------///

    ///CONTROLLO CORRETTEZZA DATI LETTI/CREATI
    check_correttezza_giocatori(giocatori, num_giocatori, &mazzo_pesca, &mazzo_scarta, HP_SCERIFFO, HP_RUOLI_BASE);
    check_correttezza_mazzo(giocatori, &mazzo_pesca, &mazzo_pesca, &mazzo_scarta, num_giocatori);
    check_correttezza_mazzo(giocatori, &mazzo_scarta, &mazzo_pesca, &mazzo_scarta, num_giocatori);

    ///--------------------------------------------------------------------------------------------------------------///
    ///------------------------------------------------INIZIO GIOCO--------------------------------------------------///
    ///--------------------------------------------------------------------------------------------------------------///
    /*
    printf("\n Vuoi resettare l'interfaccia [0=no, 1=si]?");
    scanf("%d", &choose_clean);

    if(choose_clean==1)
        clear_output();

    //FUNZIONA SOLO DA CONSOLE
    */

    ///APERTURA LOG
    log=fopen(nome_log, "a");
    if(log==NULL)
    {
        exit(-1);
    }

    fprintf(log, "\nAPERTURA FILE DI SALVATAGGIO: %s\n\n", nome_file);

    ///WHILE PARTITA...
    do
    {

        ///SALVATAGGIO AD OGNI TURNO
        salva_partita(nome_file, &num_giocatori, giocatori, &player, &mazzo_pesca, &mazzo_scarta);

        turno_in_corso=true;
        turno++;
        output_ogni_turno(giocatori, turno, player);

        fprintf(log, "\nTURNO %d: tocca al giocatore %d, %s!\n\n", turno, player+1, giocatori[player].nome);

        ///CONTROLLO ESPLOZIONE DINAMITE
        check_esplosione_dinamite(giocatori, &mazzo_pesca, &mazzo_scarta, player, num_giocatori, log);
        check_morte_giocatore(giocatori, &mazzo_pesca, &mazzo_scarta, &num_giocatori, &player, &turno_in_corso, log);

        ///CONTROLLO VITTORIA
        partita_finita=vittoria(giocatori, num_giocatori);

        ///CONTROLLO PRIGIONE
        check_prigione(giocatori, &mazzo_pesca, &mazzo_scarta, &turno, player, &turno_in_corso, log);


        if(turno_in_corso==true  && partita_finita==false)
        {
            ///PESCA DI 2 CARTE OGNI TURNO
            pescata_ogni_turno(&mazzo_pesca, &mazzo_scarta, giocatori, player, log);
            do
            {
                output_ogni_mossa(giocatori, num_giocatori);

                ///SCELTA MOSSE OGNI TURNO (quando passa_turno == true, il turno passa al giocatore successivo)
                passa_turno = scelta_mosse_player(giocatori, &mazzo_pesca, &mazzo_scarta, player, &num_giocatori, &numero_bang, &chiusura_gioco, log);

                ///CONTROLLO MORTE DOPO OGNI MOSSA
                check_morte_giocatore(giocatori, &mazzo_pesca, &mazzo_scarta, &num_giocatori, &player, &turno_in_corso, log);

                ///CONTROLLO VITTORIA DOPO OGNI MORTE
                partita_finita = vittoria(giocatori, num_giocatori);

            } while (passa_turno == false && turno_in_corso == true && partita_finita==false && chiusura_gioco==false);


            ///se il giocatore passa il turno ma la partita e' ancora in corso, allora il giocatore scarta le carte alla fine del turno
            if(passa_turno == true && turno_in_corso==true && partita_finita==false && chiusura_gioco==false)
            {
                scarto_fine_turno(&mazzo_scarta, giocatori, player, log);
            }

        }

        ///se la partita e' ancora in corso, gioca il giocatore successivo e il counter bang e' resettato
        if(partita_finita==false && chiusura_gioco==false)
        {
            player++;
            if(player >= num_giocatori)
            {
                player = 0;
            }
            numero_bang=0;
        }

    ///...TERMINA QUANDO SUBROUTINE VITTORIA RESTITUISCE FALSE
    }while(partita_finita==false && chiusura_gioco==false);

    output_vittoria(giocatori, num_giocatori, log);

    printf("\nInserisci un carattere per chiudere l'interfaccia!");
    getchar();

    ///CHIUSURA LOG
    fclose(log);
    ///------------------------------------------------FINE GIOCO----------------------------------------------------///


    ///------------------------------------------------ENDING FREE---------------------------------------------------///
    total_free(giocatori, num_giocatori, &mazzo_pesca, &mazzo_scarta);
    return 0;
}
