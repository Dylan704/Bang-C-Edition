//
// Created by Dylan on 03/03/2023.
//

//inclusione header
#include "Mazzi.h"
#include "Struct_enum.h"
#include "Funz_sistema.h"

/**
 * Funzione che apre un file di testo e legge i dati di ogni carta riga per riga fino a quando non finiscono i dati da leggere
 *
 * @param mazzo_pesca = mazzo da cui si pescano le carte
 */
void caricamento_mazzo_file(Mazzo *mazzo_pesca)
{
    //dichiarazione puntatore a file
    FILE *cards=NULL;
    int read=0;
    int i=0;


    //apertura file
    cards = fopen("mazzo_bang.txt", "r" );

    //verifica apertura corretta
    if (cards == NULL)
    {
        printf("\nError");
        exit(-1);
    }

    //ciclo di lettura
    do{
        mazzo_pesca->numero_carte++;

        if(mazzo_pesca->numero_carte==1)
        {
            mazzo_pesca->array_carte = (Carta*) calloc(mazzo_pesca->numero_carte, sizeof(Carta));
        }
        else
        {
            mazzo_pesca->array_carte= (Carta*) realloc (mazzo_pesca->array_carte, (mazzo_pesca->numero_carte) * sizeof(Carta));
        }

        read = fscanf(cards, "%d %d %d %[^\n]s\n", &mazzo_pesca->array_carte[i].tipo, &mazzo_pesca->array_carte[i].numero_carta, &mazzo_pesca->array_carte[i].seme, mazzo_pesca->array_carte[i].nome_carta);


        if( read != 4 )
        {
            mazzo_pesca->numero_carte--;
            mazzo_pesca->array_carte= (Carta*) realloc (mazzo_pesca->array_carte, (mazzo_pesca->numero_carte) * sizeof(Carta));
        }

        i++;

    }while (read == 4);

    printf(GREEN_TEXT);
    printf("\n\t\tCaricamento mazzo... COMPLETATO!");
    printf(RIPRISTINA);

    printf("\n\t\tNumero di carte caricate: %d", mazzo_pesca->numero_carte);

    //printf("\n\nMAZZO ORIGINALE LETTO DA FILE:");       //check mazzo caricato
    //visualizzatore_di_mazzi(mazzo_pesca);

    //chiusura file
    fclose(cards);
}

/**
 * Funzione che verifica che tutti i valori delle carte del mazzo rispettino i parametri.
 * Se viene trovato qualche errore si visualizza un messaggio di errore e si fa un exit dal programma
 * dopo aver liberato lo spazio allocato.
 *
 * @param giocatori = array di giocatori
 * @param mazzo_generico = mazzo che viene controllato
 * @param mazzo_pesca = mazzo da cui si pescano le carte
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param num_giocatori = numero di giocatori in partita
 */
void check_correttezza_mazzo(Giocatore *giocatori, Mazzo *mazzo_generico, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int num_giocatori)
{
    int errore=0;

    for (int i = 0; i < mazzo_generico->numero_carte; ++i)
    {
        if(mazzo_generico->array_carte[i].seme < CUORI || mazzo_generico->array_carte[i].seme > PICCHE)
        {
            printf("\nError, seme della carta %d corrotto!", i+1);

            errore=1;
        }
        if(mazzo_generico->array_carte[i].tipo < ISTANTANEA || mazzo_generico->array_carte[i].tipo > EFFETTO)
        {
            printf("\nError, tipo della carta %d corrotto!", i+1);

            errore=1;
        }
        if(mazzo_generico->array_carte[i].numero_carta < 1 || mazzo_generico->array_carte[i].numero_carta > 13)
        {
            printf("\nError, numero della carta %d corrotto!", i+1);

            errore=1;
        }
        if(strlen(mazzo_generico->array_carte[i].nome_carta) > (DIM_NOME_CARTA+1))
        {
            printf("\nError, nome della carta %d corrotto!", i+1);

            errore=1;
        }
    }

    if(errore==1)
    {
        printf("\nErrore presente nel mazzo di tipo: ");
        visualizza_tipo_mazzo(*mazzo_generico);

        total_free(giocatori, num_giocatori, mazzo_pesca, mazzo_scarta);

        exit(-69);
    }
}

/**
 * Funzione di output
 *
 * @param mazzo_generico = mazzo generico della quale viene letto il tipo
 */
void visualizza_tipo_mazzo(Mazzo mazzo_generico)
{
    switch(mazzo_generico.tipo_mazzo)
    {
        case MAZZO_PESCA:
            printf("MAZZO PESCA;");
            break;
        case MAZZO_SCARTO:
            printf("MAZZO SCARTO;");
            break;
        case GIOC_GIOCO:
            printf("MAZZO CAMPO;");
            break;
        case GIOC_MANO:
            printf("MAZZO MANO;");
            break;
        default:
            printf("\nErrore, tipo di mazzo non esistente!");
            break;
    }
}

/**
 * Funzione di output che visualizza tutte le carte di un mazzo
 *
 * @param mazzo_generico = mazzo che viene visualizzato
 */
void visualizzatore_di_mazzi(Mazzo *mazzo_generico)
{
    if(mazzo_generico->numero_carte>0)
    {
        //printf("\nNUMERO CARTE DA STAMPARE->%d", mazzo_generico->numero_carte);
        printf("\n");

        for (int i = 0; i < mazzo_generico->numero_carte; ++i) {
            visualizzatore_di_carta(mazzo_generico, i);
        }
    }
    else
    {
        printf("\n---------");
        printf("\n---------");
    }
}

/**
 * Funzione che visualizza una carta singola di un mazzo
 *
 * @param mazzo_generico = mazzo dove e' presente la carta da visualizzare
 * @param carta = carta da visualizzare
 */
void visualizzatore_di_carta(Mazzo *mazzo_generico, int carta)
{
    printf("\ncarta %d: tipo = %d; numero = %d;", carta + 1, mazzo_generico->array_carte[carta].tipo, mazzo_generico->array_carte[carta].numero_carta);

    switch(mazzo_generico->array_carte[carta].seme)
    {
        case CUORI:
            printf(" seme = CUORI;");
            break;
        case PICCHE:
            printf(" seme = PICCHE;");
            break;
        case QUADRI:
            printf(" seme = QUADRI;");
            break;
        case FIORI:
            printf(" seme = FIORI;");
            break;
    }

    printf(" nome = %s", mazzo_generico->array_carte[carta].nome_carta);

}

/**
 * Funzione che permette di mischiare un mazzo effettuando ripetuti scambi tra carte di posizioni randomiche
 *
 * @param mazzo_generico
 * @param numero_scambi
 */
void mischiatore_di_carte(Mazzo *mazzo_generico, int numero_scambi)
{
    int carta1;
    int carta2;

    //variabile di tipo carta ausiliare
    Carta aux;

    for (int i = 0; i < numero_scambi; ++i)                             //tramite questo for scambio ripetutamente carte in posizioni generate casualmente,
    {                                                                   //numero_scambi e' un numero scelto arbitrariamente elevato, in questo caso e' 300
        carta1=rand()%mazzo_generico->numero_carte;
        carta2=rand()%mazzo_generico->numero_carte;

        //scambio di valori tramite variabile ausiliare
        aux = mazzo_generico->array_carte[carta1];
        mazzo_generico->array_carte[carta1] = mazzo_generico->array_carte[carta2];
        mazzo_generico->array_carte[carta2] = aux;
    }
}
/**
 * Funzione che permette di pescare una carta dalla cima di un mazzo_pesca, ingrandire un mazzo target
 * e inserire nel nuovo spazio la carta pescata. Entrambi i mazzi vengono riallocati dinamicamente a ogni pescata
 *
 * @param pesca =  mazzo da cui vengono pescate le carte
 * @param target =  mazzo in cui finiscono le carte pescate
 */
void pesca_carta(Mazzo *pesca, Mazzo *target)
{
    (target->numero_carte)++;
    if(target->numero_carte==1)
    {
        target->array_carte = (Carta*) calloc(target->numero_carte, sizeof(Carta));
    }
    else
    {
        target->array_carte= (Carta*) realloc (target->array_carte, (target->numero_carte) * sizeof(Carta));
    }

    target->array_carte[(target->numero_carte)-1] = pesca->array_carte[(pesca->numero_carte)-1];

    (pesca->numero_carte)--;
    pesca->array_carte = (Carta*) realloc (pesca->array_carte, (pesca->numero_carte) * sizeof(Carta));
}

/**
 * Funzionamento pressoche' uguale alla funzione pesca_carta, esiste per motivi di leggibilita' e di comprensione del codice
 * Scarta l'ultima carta del mazzo in uso, non e' possibile scegliere quale carta viene scartata
 *
 * @param mazzo_in_uso = mazzo che contiene la carta che deve essere scartata
 * @param scarta = mazzo in cui verra' posta la carta precedentemente usata
 */
void scarta_carta(Mazzo *mazzo_in_uso, Mazzo *mazzo_scarta)
{
    (mazzo_scarta->numero_carte)++;

    if(mazzo_scarta->numero_carte==1)
    {
        mazzo_scarta->array_carte = (Carta*) calloc(mazzo_scarta->numero_carte, sizeof(Carta));
    }
    else
    {
        mazzo_scarta->array_carte= (Carta*) realloc (mazzo_scarta->array_carte, (mazzo_scarta->numero_carte) * sizeof(Carta));
    }

    mazzo_scarta->array_carte[(mazzo_scarta->numero_carte)-1] = mazzo_in_uso->array_carte[(mazzo_in_uso->numero_carte)-1];

    (mazzo_in_uso->numero_carte)--;
    mazzo_in_uso->array_carte = (Carta*) realloc (mazzo_in_uso->array_carte, (mazzo_in_uso->numero_carte) * sizeof(Carta));
}

/**
 * Funzione che permette di scartare una carta in qualsiasi posizione dalla mano di un giocatore shiftando di uno
 * la posizione di tutte le carte successive
 *
 * @param giocatori = array di giocatori
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param player = giocatore che scarta la carta dalla sua mano
 * @param carta_scarto = indice carta scartata
 */
void scarta_carta_specifica_mano(Giocatore *giocatori, Mazzo *mazzo_scarta, int player, int carta_scarto)
{
    //incremento il mazzo scarta
    mazzo_scarta->numero_carte++;
    if(mazzo_scarta->numero_carte==1)
    {
        mazzo_scarta->array_carte=(Carta *) calloc(mazzo_scarta->numero_carte, sizeof(Carta));
    }
    else
    {
        mazzo_scarta->array_carte= (Carta *)realloc(mazzo_scarta->array_carte , (mazzo_scarta->numero_carte) * sizeof(Carta));
    }

    //copio la carta da scartare nell'ultima posizione del mazzo scarta
    mazzo_scarta->array_carte[(mazzo_scarta->numero_carte)-1] = giocatori[player].carte_in_mano.array_carte[carta_scarto];

    //shifto l'elenco di carte di una posizione a sinistra a partire dalla carta scelta che verra' sovrascritta
    for (int j = 0; j < ((giocatori[player].carte_in_mano.numero_carte) - (carta_scarto + 1)); ++j)
    {
        //copio la carta successiva nella carta da scartare ripetutamente
        giocatori[player].carte_in_mano.array_carte[(carta_scarto) +j] = giocatori[player].carte_in_mano.array_carte[(carta_scarto) + j + 1];
    }

    //infine decremento e rialloco il mazzo da cui ho preso la carta da scartare
    (giocatori[player].carte_in_mano.numero_carte)--;
    giocatori[player].carte_in_mano.array_carte = (Carta*) realloc(giocatori[player].carte_in_mano.array_carte, (giocatori[player].carte_in_mano.numero_carte)* sizeof(Carta));

    //visualizzatore_di_mazzi(mazzo_scarta);
}

/**
 * Funzione che permette di scartare una carta in qualsiasi posizione dal terreno di un giocatore shiftando di uno
 * la posizione di tutte le carte successive
 *
 * @param giocatori = array di giocatori
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param player = giocatore che scarta la carta dal suo terreno
 * @param carta_scarto = indice carta scartata
 */
void scarta_carta_specifica_gioco(Giocatore *giocatori, Mazzo *mazzo_scarta, int player, int carta_scarto)
{
    mazzo_scarta->numero_carte++;
    if(mazzo_scarta->numero_carte==1)
    {
        mazzo_scarta->array_carte=(Carta *) calloc(mazzo_scarta->numero_carte, sizeof(Carta));
    }
    else
    {
        mazzo_scarta->array_carte= (Carta *)realloc(mazzo_scarta->array_carte , (mazzo_scarta->numero_carte) * sizeof(Carta));
    }

    //se la carta scartata e' un'arma si resetta la gittata
    if(giocatori[player].carte_in_gioco.array_carte[carta_scarto].tipo == ARMA)
    {
        giocatori[player].gittata = 1;
    }

    mazzo_scarta->array_carte[(mazzo_scarta->numero_carte)-1] = giocatori[player].carte_in_gioco.array_carte[carta_scarto];

    //shifto l'elenco di carte di una posizione a sinistra a partire dalla carta scelta che verra' sovrascritta
    for (int j = 0; j < ((giocatori[player].carte_in_gioco.numero_carte) - (carta_scarto + 1)); ++j)
    {
        //copio la carta successiva nella carta precedente
        giocatori[player].carte_in_gioco.array_carte[(carta_scarto) +j] = giocatori[player].carte_in_gioco.array_carte[(carta_scarto) + j + 1];
    }

    (giocatori[player].carte_in_gioco.numero_carte)--;
    giocatori[player].carte_in_gioco.array_carte = (Carta*) realloc(giocatori[player].carte_in_gioco.array_carte, (giocatori[player].carte_in_gioco.numero_carte)* sizeof(Carta));

    //visualizzatore_di_mazzi(mazzo_scarta);
}

/**
 * Funzione analoga alle precedenti, ma che mi permette di scartare una carta specifica da un qualsiasi mazzo, non necessariamente
 * un mazzo di un giocatore.
 *
 * @param mazzo_target = mazzo da cui viene presa la carta da scartare
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param carta_scarto = indice carta da scartare
 */
void scarta_carta_specifica_mazzo_generico(Mazzo *mazzo_target,  Mazzo *mazzo_scarta, int carta_scarto)
{

    //rialloco mazzo scarta ingrandendolo
    mazzo_scarta->numero_carte++;
    if(mazzo_scarta->numero_carte==1)
    {
        mazzo_scarta->array_carte=(Carta *) calloc(mazzo_scarta->numero_carte, sizeof(Carta));
    }
    else
    {
        mazzo_scarta->array_carte= (Carta *)realloc(mazzo_scarta->array_carte , (mazzo_scarta->numero_carte) * sizeof(Carta));
    }

    //copiatura elemento da scartare in cima al mazzo scarta
    mazzo_scarta->array_carte[(mazzo_scarta->numero_carte)-1] = mazzo_target->array_carte[carta_scarto];

    //shifto l'elenco di carte di una posizione a sinistra a partire dalla carta scelta che verra' sovrascritta
    for (int j = 0; j < ((mazzo_target->numero_carte) - (carta_scarto + 1)); ++j)
    {
        //copio la carta successiva nella carta precedente
        mazzo_target->array_carte[(carta_scarto) +j] = mazzo_target->array_carte[(carta_scarto) + j + 1];
    }

    //rialloco il mazzo target diminuendo la dimensione
    (mazzo_target->numero_carte)--;
    mazzo_target->array_carte = (Carta*) realloc(mazzo_target->array_carte, (mazzo_target->numero_carte)* sizeof(Carta));
}

/**
 * Funzione che controlla se il mazzo pesca sia vuoto, e in caso questo sia vero copia il mazzo scarta nel mazzo pesca
 * e lo mischia per poterlo riutilizzare come in un reale gioco di carte. Infine il mazzo scarta viene azzerato.
 *
 * @param mazzo_pesca = mazzo da cui si pescano le carte
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 */
void check_mazzo_pesca_vuoto(Mazzo *mazzo_pesca, Mazzo *mazzo_scarta)
{
    if(mazzo_pesca->numero_carte==0)
    {
        //uguaglio le dimensioni dei due mazzi
        mazzo_pesca->numero_carte = mazzo_scarta->numero_carte;

        //realloco l'array di carte pesca
        mazzo_pesca->array_carte = (Carta*) realloc(mazzo_pesca->array_carte, mazzo_scarta->numero_carte * sizeof(Carta));

        //con un for copio tutte le carte del mazzo scarta nel mazzo pesca
        for (int i = 0; i < mazzo_scarta->numero_carte; ++i)
        {
            mazzo_pesca->array_carte[i] = mazzo_scarta->array_carte[i];
        }

        //azzero il numero di carte del mazzo scarta
        mazzo_scarta->numero_carte=0;

        //e azzero il suo array di carte
        mazzo_scarta->array_carte = (Carta*) realloc(mazzo_scarta->array_carte, mazzo_scarta->numero_carte*sizeof(Carta*));


        //infine mischio il mazzo_pesca appena copiato per renderlo giocabile randomicamente
        mischiatore_di_carte(mazzo_pesca, N_SCAMBI_CARTE);

        printf("\nMAZZO PESCA VUOTO, RIMESCOLAMENTO IN CORSO!\n");
    }
}

/**
 * Funzione che permette al giocatore di estrarre una carta: pescare una carta dal mazzo pesca, vedere le sue caratteristiche e poi scartarla.
 * Se la carta e' del seme corrispondente a quello da controllare, allora la funzione restituisce 1, altrimenti 0.
 *
 * @param giocatori = array di giocatori
 * @param mazzo_pesca = mazzo da cui si pescano le carte
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param seme = seme con cui si fa il confronto con la carta estratta
 * @param player = giocatore che estrae la carta
 * @param log = puntatore al file di testo del log
 * @return intero trattato come booleano
 */
int estrai(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int seme, int player, FILE *log)
{
    int estrazione=0;

    (mazzo_scarta->numero_carte)++;
    if(mazzo_scarta->numero_carte==1)
    {
        mazzo_scarta->array_carte = (Carta*) calloc(mazzo_scarta->numero_carte, sizeof(Carta));
    }
    else
    {
        mazzo_scarta->array_carte= (Carta*) realloc (mazzo_scarta->array_carte, (mazzo_scarta->numero_carte) * sizeof(Carta));
    }

    mazzo_scarta->array_carte[(mazzo_scarta->numero_carte)-1] = mazzo_pesca->array_carte[(mazzo_pesca->numero_carte)-1];


    printf("\n\n#######################ESTRAZIONE CARTA#######################\n");
    switch( mazzo_scarta->array_carte[(mazzo_scarta->numero_carte)-1].seme)
    {
        case CUORI:
            printf("\n<-----------SEME CARTA ESTRATTA: |CUORI|----------->\n");
            fprintf(log, "\t Giocatore %d (%s) estrae carta CUORI;\n", player+1, giocatori[player].nome);

            break;

        case QUADRI:
            printf("\n<-----------SEME CARTA ESTRATTA: |QUADRI|----------->\n");
            fprintf(log, "\t Giocatore %d (%s) estrae carta QUADRI;\n", player+1, giocatori[player].nome);

            break;

        case FIORI:
            printf("\n<-----------SEME CARTA ESTRATTA: |FIORI|----------->\n");
            fprintf(log, "\t Giocatore %d (%s) estrae carta FIORI;\n", player+1, giocatori[player].nome);

            break;

        case PICCHE:
            printf("\n<-----------SEME CARTA ESTRATTA: |PICCHE|----------->\n");
            fprintf(log, "\t Giocatore %d (%s) estrae carta PICCHE;\n", player+1, giocatori[player].nome);

            break;

        default:
        {
            printf("\nERROR, seme_not_found!");
        }
    }

    //se la carta estratta e' del seme giusto, la funzione restituisce 1, altrimenti il valore di "estrazione" rimane 0
    if(mazzo_scarta->array_carte[(mazzo_scarta->numero_carte)-1].seme == seme)
    {
        estrazione = 1;
    }

    printf("\nCarta estratta: ");
    visualizzatore_di_carta(mazzo_scarta, (mazzo_scarta->numero_carte)-1);

    (mazzo_pesca->numero_carte)--;
    mazzo_pesca->array_carte = (Carta*) realloc (mazzo_pesca->array_carte, (mazzo_pesca->numero_carte) * sizeof(Carta));

    return estrazione;
}
