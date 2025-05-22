//
// Created by Dylan on 03/03/2023.
//

//inclusione header
#include "Carte.h"
#include "Mazzi.h"
#include "Giocatori.h"
#include "Struct_enum.h"

/**
 * Funzione che controlla se e' presente una particolare carta nella mano del giocatore tramite uno string compare.
 * Nel caso questa carta sia presente restituisce la posizione della carta nella mano (l'indice nell' array di carte).
 *
 * @param giocatori = array di giocatori
 * @param target_giocatore = giocatore di cui si controllano le carte in mano
 * @param carta = stringa che rappresenta il nome di una carta
 * @return indice della carta, se la carta non viene trovata restituisce -1
 */
int check_carta_mano(Giocatore *giocatori, int target_giocatore, char carta [])
{
    int posizione_carta = -1;

    for (int i = 0; i < giocatori[target_giocatore].carte_in_mano.numero_carte; ++i)
    {
        if(strcmp(giocatori[target_giocatore].carte_in_mano.array_carte[i].nome_carta, carta)==0)
        {
            posizione_carta=i;
            return posizione_carta;
        }
    }

    return posizione_carta;
}

/**
 * Funzione analoga alla precedente che pero' controlla le carte in campo del giocatore.
 *
 * @param giocatori = array di giocatori
 * @param target_giocatore = giocatore di cui si controllano le carte in campo
 * @param carta = stringa che rappresenta il nome di una carta
 * @return indice della carta, se la carta non viene trovata restituisce -1
 */
int check_carta_campo(Giocatore *giocatori, int target_giocatore, char carta [])
{
    int posizione_carta = -1;

    for (int i = 0; i < giocatori[target_giocatore].carte_in_gioco.numero_carte; ++i)
    {
        if(strcmp(giocatori[target_giocatore].carte_in_gioco.array_carte[i].nome_carta, carta)==0)
        {
            posizione_carta=i;
            return posizione_carta;
        }
    }

    return posizione_carta;
}

/**
 * Funzione che diminuisce di 1 gli hp del bersaglio dello sparo,
 * Se il bersaglio ha in campo un barile allora prima si estra per vedere se riesce a schivare o no il colpo.
 * Nel caso non abbia il barile allora si controlla se abbia un mancato in mano per schivare il colpo.
 * Se il bersaglio non ha nessun barile e nessun mancato (oppure decide di non usarlo) allora perde un hp.
 *
 * @param giocatori = array di giocatori
 * @param target_bang = bersaglio del bang
 * @param mazzo_pesca = mazzo da cui si pesca
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param player = giocatore che gioca il bang
 * @param log = puntatore al file di testo del log
 */
void bang(Giocatore *giocatori, int target_bang, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int player, FILE *log)
{

    int posizione_mancato = check_carta_mano(giocatori, target_bang, "Mancato");
    int scelta_schivata;

    if(check_carta_campo(giocatori, target_bang, "Barile")>=0)
    {
        printf("\n<==========ATTENZIONE! Giocatore %d (%s), hai un Barile in campo! Se estrai una carta di cuori potresti schivare un colpo!==========>", target_bang+1, giocatori[target_bang].nome);

        if(estrai(giocatori, mazzo_pesca, mazzo_scarta, CUORI, player, log)==1)
        {
            printf("\n\n\t\t+++++++++++++Hai estratto una carta con seme CUORI, hai schivato il proiettile!!!+++++++++++++");

            fprintf(log, "\t Il giocatore %d (%s) schiva il proiettile nascondendosi dietro ad un barile;\n", target_bang+1, giocatori[target_bang].nome);
        }
        else
        {
            printf("\n\n\t\t+++++++++++++Hai estratto una carta con seme diverso da cuori, non hai schivato proprio un bel niente!!!+++++++++++++");

            if(posizione_mancato>=0)
            {
                printf("\nGiocatore %d! Sei bersaglio di uno sparo e hai un mancato in mano, vuoi utilizzarlo? [1=SI / 0=NO]", target_bang+1);
                do
                {
                    printf("\nInserisci risposta:");
                    scanf("%d", &scelta_schivata);
                    getchar();
                }while(scelta_schivata<0 || scelta_schivata>1);

                if(scelta_schivata==1)
                {
                    printf("\nHai schivato il colpo!, i tuoi punti vita rimangono intatti.\n\n");
                    scarta_carta_specifica_mano(giocatori, mazzo_scarta, target_bang, posizione_mancato);

                    fprintf(log, "\t Il giocatore %d (%s) usa un MANCATO e schiva il proiettile;\n", target_bang+1, giocatori[target_bang].nome);
                }
                else if(scelta_schivata==0)
                {
                    printf("\nGiocatore %d, sei stato colpito in pieno!! I tuoi punti vita diminuiscono di uno!\n\n", target_bang+1);
                    giocatori[target_bang].hp-=1;

                    fprintf(log, "\t Giocatore %d (%s) perde 1 HP\n", target_bang+1, giocatori[target_bang].nome);
                }
            }

            //nel caso il target non abbia un bang in mano
            else
            {
                printf("\nGiocatore %d! Qualche burlone ha deciso di spararti!! putroppo non hai modi per difenderti, perdi 1 HP!!\n\n", target_bang+1);
                giocatori[target_bang].hp-=1;

                fprintf(log, "\t Giocatore %d (%s) perde 1 HP\n", target_bang+1, giocatori[target_bang].nome);
            }
        }
    }

    //nel caso il giocatore non abbia un barile in campo si controlla solamente la parte relativa ai mancati
    else
    {
        if(posizione_mancato>=0)
        {
            printf("\nGiocatore %d! Sei bersaglio di uno sparo e hai un mancato in mano, vuoi utilizzarlo? [1=SI / 0=NO]", target_bang+1);
            do
            {
                printf("\nInserisci risposta:");
                scanf("%d", &scelta_schivata);
                getchar();
            }while(scelta_schivata<0 || scelta_schivata>1);

            if(scelta_schivata==1)
            {
                printf("\nHai schivato il colpo!, i tuoi punti vita rimangono intatti.\n\n");
                scarta_carta_specifica_mano(giocatori, mazzo_scarta, target_bang, posizione_mancato);

                fprintf(log, "\t Il giocatore %d (%s) usa un MANCATO e schiva il proiettile;\n", target_bang+1, giocatori[target_bang].nome);
            }
            else if(scelta_schivata==0)
            {
                printf("\nGiocatore %d, sei stato colpito in pieno!! I tuoi punti vita diminuiscono di uno\n\n", target_bang+1);
                giocatori[target_bang].hp-=1;

                fprintf(log, "\t Giocatore %d (%s) perde 1 HP\n", target_bang+1, giocatori[target_bang].nome);
            }
        }

        //nel caso il target non abbia un bang in mano
        else
        {
            printf("\nGiocatore %d! Qualche burlone ha deciso di spararti!! putroppo non hai modi per difenderti, perdi 1 HP!!\n\n", target_bang+1);
            giocatori[target_bang].hp-=1;

            fprintf(log, "\t Giocatore %d (%s) perde 1 HP\n", target_bang+1, giocatori[target_bang].nome);
        }
    }
}

/**
 * Funzione che permette di sparare con un bang a tutti i giocatori tramite un ciclo for.
 * Consiste fondamentalmente in una ripetizione di bang, quindi verranno fatti tutti i controlli sul barile e sui mancati.
 *
 * @param giocatori = array di giocatori
 * @param mazzo_pesca = mazzo da cui si pesca
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param player = giocatore che gioca il gatling
 * @param num_giocatori = numero di giocatori in partita
 * @param log = puntatore al file di testo del log
 */
void gatling(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int player, int *num_giocatori, FILE *log)
{
    for (int i = 0; i < *num_giocatori; ++i)
    {
        if(i!=player)
        {
            bang(giocatori, i, mazzo_pesca, mazzo_scarta, player, log);
        }
    }
}

/**
 * Funzione che permette di far pescare 2 carte al player che sta giocando il turno.
 * Prima di ogni pescata si controlla che il mazzo pesca non sia vuoto.
 *
 * @param giocatori = array di giocatori
 * @param player = giocatore che sta giocando il turno attuale
 * @param mazzo_pesca = mazzo da cui si pesca
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param log = puntatore al file di testo del log
 */
void diligenza(Giocatore *giocatori, int player, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, FILE *log)
{
    for (int i = 0; i < 2; ++i)
    {
        check_mazzo_pesca_vuoto(mazzo_pesca, mazzo_scarta);
        pesca_carta(mazzo_pesca, &giocatori[player].carte_in_mano);

        fprintf(log, "\t Giocatore %d (%s) pesca %s;\n", player+1, giocatori[player].nome, giocatori[player].carte_in_mano.array_carte[giocatori[player].carte_in_mano.numero_carte-1].nome_carta);
    }
}

/**
 * Funzione che permette di far pescare 3 carte al player che sta giocando il turno.
 * Prima di ogni pescata si controlla che il mazzo pesca non sia vuoto.
 *
 * @param giocatori = array di giocatori
 * @param player = giocatore che sta giocando il turno attuale
 * @param mazzo_pesca = mazzo da cui si pesca
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param log = puntatore al file di testo del log
 */
void WellsFargo(Giocatore *giocatori, int player, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, FILE *log)
{
    for (int i = 0; i < 3; ++i)
    {
        check_mazzo_pesca_vuoto(mazzo_pesca, mazzo_scarta);
        pesca_carta(mazzo_pesca, &giocatori[player].carte_in_mano);

        fprintf(log, "\t Giocatore %d (%s) pesca %s;\n", player+1, giocatori[player].nome, giocatori[player].carte_in_mano.array_carte[giocatori[player].carte_in_mano.numero_carte-1].nome_carta);
    }
}

/**
 * Funzione che permette di curare un giocatore aumentando gli hp di 1
 *
 * @param giocatori = array di giocatori
 * @param player = giocatore che sta giocando la birra
 * @param log = puntatore al file di testo del log
 */
void birra(Giocatore *giocatori, int player, FILE *log)
{
    fprintf(log, "\t Giocatore %d (%s) gioca una BIRRA, si cura di 1 HP;\n", player+1, giocatori[player].nome);

    printf(GREEN_TEXT);
    printf("\nGiocatore %d si e' curato di 1 HP!\n\n", player+1);
    printf(RIPRISTINA);

    giocatori[player].hp++;
}

/**
 * Funzione che viene utilizzata in caso di morte di un giocatore.
 * Controlla se il giocatore morto ha una birra in mano e in caso questo sia vero la puo' utillizzare in extremis per salvarsi.
 * Se il giocatore si salva allora la funzione restituisce 1, altrimenti 0.
 *
 * @param giocatori = array di giocatori
 * @param target_giocatore = giocatore che si deve salvare
 * @param carta = stringa rappresentante il nome della carta, in questo caso "birra"
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param log = puntatore al file di testo del log
 * @return = intero trattato come booleano
 */
int birra_in_extremis(Giocatore *giocatori, int target_giocatore, char carta [], Mazzo *mazzo_scarta, FILE *log)
{
    int posizione_birra;
    posizione_birra = check_carta_mano(giocatori, target_giocatore, carta);

    if(posizione_birra > -1)
    {

        printf(GREEN_TEXT);
        printf("\nGiocatore %d (%s%s%s), stavi per morire ma hai una birra in mano e quindi ti sei BIRRATO IN EXTREMIS!!! HP+1!!", target_giocatore+1, LIGHT_YELLOW, giocatori[target_giocatore].nome, GREEN_TEXT);
        printf(RIPRISTINA);

        fprintf(log, "\t Giocatore %d (%s) si salva con una birra in extremis!\n", target_giocatore+1, giocatori[target_giocatore].nome);

        birra(giocatori, target_giocatore, log);
        scarta_carta_specifica_mano(giocatori, mazzo_scarta, target_giocatore, posizione_birra);

        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * Funzione che controlla la carta panico puo' essere giocata.
 * Per giocare panico deve esistere almeno un giocatore a distanza 1 con almeno 1 carta o in mano o in campo.
 * Restituisce 1 se panico puo' essere giocata, altrimenti 0.
 *
 * @param giocatori = array di giocatori
 * @param player = giocatore che sta giocando panico
 * @param num_giocatori = numero di giocatori in parita
 * @return intero trattato come booleano
 */
int check_panico(Giocatore *giocatori, int player, int num_giocatori){
    for (int i = 0; i < num_giocatori; ++i)
    {
        if(i!=player)
        {
            //la subroutine restituisce uno se esiste almeno un giocatore a distanza 1 dal player che abbia almeno una carta in mano o in campo
            if((giocatori[i].carte_in_mano.numero_carte>0 || giocatori[i].carte_in_gioco.numero_carte>0) && distanza_tra_due_player(giocatori, num_giocatori, player, i)==1)
            {
                return 1;
            }
        }

    }

    return 0;
}

/**
 * La funzione permette di scegliere un bersaglio (tra quelli possibili) che abbia carte o in mano, o in campo
 * oppure entrambe per rubargliene una.
 * Se si decide di rubare una carta dalla mano allora la carta sara' scelta casualmente,
 * altrimenti se si ruba dal terreno sara' il giocatore che ha giocato panico a scegliere.
 *
 * @param giocatori = array di giocatori
 * @param player = giocatore che gioca panico
 * @param num_giocatori = numero di giocatori in partita
 * @param log = puntatore a file di testo del log
 */
void panico(Giocatore *giocatori, int player, int num_giocatori, FILE *log)
{
    int target;
    int mano_campo;
    int carta_scarto;

    do{
        for (int i = 0; i < num_giocatori; ++i)                                                        //output persone colpibili
        {
            if ((distanza_tra_due_player(giocatori, num_giocatori, player, i) == 1 && i!=player))
            {
                printf("\nPuoi scegliere il giocatore %d", i + 1);
            }
        }

        printf("\nGiocatore %d, a che giocatore vuoi rubare una carta?", player + 1);
        scanf("%d", &target);
        getchar();
        target--;

        if((distanza_tra_due_player(giocatori, num_giocatori, player, target) > 1) || target<0 || target>num_giocatori-1)
        {
            printf("\nScegli tra uno dei giocatori possibili! Riprova\n");
        }
        else if(target == player)
        {
            printf("\nERRORE, non ti rubare carte da solo");
        }
        else if(giocatori[target].carte_in_mano.numero_carte==0 && giocatori[target].carte_in_gioco.numero_carte==0)
        {
            printf("\nIl giocatore non ha carte in mano o in campo!");
        }

    }while(target<0 || target == player || target > num_giocatori-1 || (distanza_tra_due_player(giocatori, num_giocatori, player, target) > 1) || giocatori[target].carte_in_mano.numero_carte==0 && giocatori[target].carte_in_gioco.numero_carte==0);

    fprintf(log, "\t Giocatore %d (%s) gioca PANICO contro il giocatore %d (%s);\n", player+1, giocatori[player].nome, target+1, giocatori[target].nome);

    //target ha sia carte in mano che in campo
    if(giocatori[target].carte_in_mano.numero_carte > 0 && giocatori[target].carte_in_gioco.numero_carte > 0)
    {
        //il giocatore sceglie se rubare una carta dalla mano o dal campo
        do{
            printf("\nVuoi rubare dalla mano o dal campo del giocatore %d? [1=MANO / 0=CAMPO]", target+1);
            scanf("%d", &mano_campo);
            getchar();
        }while(mano_campo != 0 && mano_campo != 1);

        if(mano_campo==1)
        {
            printf("\nCarte in mano da cui pescare:");
            visualizzatore_di_mazzi(&giocatori[target].carte_in_mano);
            pesca_carta_casuale_da_mano(giocatori, target, player);

            fprintf(log, "\t Giocatore %d (%s) ha rubato la carta %s;\n", player+1, giocatori[player].nome, giocatori[player].carte_in_mano.array_carte[giocatori[player].carte_in_mano.numero_carte-1].nome_carta);

        }
        else if(mano_campo==0)
        {
            printf("\nCarte in gioco da cui scegliere:");
            visualizzatore_di_mazzi(&giocatori[target].carte_in_gioco);

            do {
                printf("\nScegli che carta vuoi rubare:");
                scanf("%d", &carta_scarto);
                getchar();
                carta_scarto--;
            }while(carta_scarto < 0 || carta_scarto > giocatori[target].carte_in_gioco.numero_carte-1);

            fprintf(log, "\t Giocatore %d (%s) ha rubato la carta %s;\n", player+1, giocatori[player].nome, giocatori[target].carte_in_mano.array_carte[carta_scarto].nome_carta);

            scarta_carta_specifica_gioco(giocatori, &giocatori[player].carte_in_mano, target, carta_scarto);
        }

    }


    //target ha solo carte in mano
    else if(giocatori[target].carte_in_mano.numero_carte > 0 && giocatori[target].carte_in_gioco.numero_carte <= 0)
    {
        printf("\nCarte in mano da cui pescare:");
        visualizzatore_di_mazzi(&giocatori[target].carte_in_mano);
        pesca_carta_casuale_da_mano(giocatori, target, player);

        fprintf(log, "\t Giocatore %d (%s) ha rubato la carta %s;\n", player+1, giocatori[player].nome, giocatori[player].carte_in_mano.array_carte[giocatori[player].carte_in_mano.numero_carte-1].nome_carta);
    }


    //target ha solo carte in campo
    else if(giocatori[target].carte_in_mano.numero_carte <= 0 && giocatori[target].carte_in_gioco.numero_carte > 0)
    {
        printf("\n\nCarte in gioco da cui scegliere:");
        visualizzatore_di_mazzi(&giocatori[target].carte_in_gioco);

        do {
            printf("\nScegli che carta vuoi rubare:");
            scanf("%d", &carta_scarto);
            getchar();
        }while(carta_scarto < 1 || carta_scarto > giocatori[target].carte_in_gioco.numero_carte);
        carta_scarto--;

        fprintf(log, "\t Giocatore %d (%s) ha rubato la carta %s;\n", player+1, giocatori[player].nome, giocatori[target].carte_in_mano.array_carte[carta_scarto].nome_carta);

        scarta_carta_specifica_gioco(giocatori, &giocatori[player].carte_in_mano, target, carta_scarto);
    }
}

/**
 * Funzione che permette di pescare casualmente una carta dalla mano di un giocatore trasferendola
 * nella mano di un altro giocatore.
 *
 * @param giocatori = array di giocatori
 * @param target = bersaglio della pesca casuale
 * @param player = giocatore che pesca la carta
 */
void pesca_carta_casuale_da_mano(Giocatore *giocatori, int target, int player)
{
    int carta_random;

    carta_random= 1 + rand() % (giocatori[target].carte_in_mano.numero_carte - 1 + 1);
    carta_random--;

    printf("\n\nHai pescato la carta %s\n\n", giocatori[target].carte_in_mano.array_carte[carta_random].nome_carta);

    scarta_carta_specifica_mano(giocatori, &giocatori[player].carte_in_mano, target, carta_random);
}

/**
 * Funzione che cura di 1 hp tutti i giocatori che possono essere curati, quindi che non hanno full HP.
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori in partita
 * @param log = puntatore al file di testo del log
 */
void saloon(Giocatore *giocatori, int num_giocatori, FILE *log)
{
    //birro tutti i giocatori in gioco
    for (int i = 0; i < num_giocatori; ++i)
    {
        if(giocatori[i].ruolo==SCERIFFO && giocatori[i].hp < HP_SCERIFFO)
        {
            birra(giocatori, i, log);
        }
        else if(giocatori[i].ruolo!=SCERIFFO && giocatori[i].hp < HP_RUOLI_BASE)
        {
            birra(giocatori, i, log);
        }
    }
}

/**
 * Funzione che controlla che almeno un giocatore non sia full HP e restituisce 1 in caso questo sia vero, altrimenti 0;
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori
 * @return intero trattato come booleano
 */
int check_saloon(Giocatore *giocatori, int num_giocatori)
{
    //controllo se almeno un giocatore ha perso almeno un HP e ritorno 1 in caso cio' sia vero
    for (int i = 0; i < num_giocatori; ++i)
    {
        if(giocatori[i].ruolo==SCERIFFO && giocatori[i].hp < HP_SCERIFFO)
        {
            return 1;
        }
        else if(giocatori[i].ruolo!=SCERIFFO && giocatori[i].hp < HP_RUOLI_BASE)
        {
            return 1;
        }
        //altrimenti ritorno 0 se tutti sono full HP
    }
    return 0;
}

/**
 * Funzione che controlla che il bersaglio scelto per catbalou abbia almeno una carta in mano o in campo
 *
 * @param giocatori = array di giocatori
 * @param target = bersaglio di catbalou
 * @return intero trattato come booleano
 */
int check_catbalou(Giocatore *giocatori, int target)
{
    if(giocatori[target].carte_in_mano.numero_carte>0 || giocatori[target].carte_in_gioco.numero_carte>0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/**
 * Funzione che sfrutta check_catbalou per controllare che si possa giocare catbalou.
 * Un ciclo for ripete check_catbalou per tutti i giocatori che non siano quello che deve giocare la carta.
 * La funzione restituisce 1 se catbalou puo' essere giocata, altrimenti 0.
 *
 * @param giocatori = array di giocatori
 * @param player = giocatore che deve giocare catbalou
 * @param num_giocatori = numero di giocatori in partita
 * @return intero trattato come booleano
 */
int gioco_catbalou(Giocatore *giocatori, int player, int num_giocatori)
{
    for (int i = 0; i < num_giocatori; ++i)
    {
        if(i!=player)
        {
            if(check_catbalou(giocatori, i)==1)
            {
                return 1;
            }
        }

    }
    return 0;
}

/**
 * Funzione con struttura analoga a "panico", la differenza pero' consiste nel fatto che la carta anziche' essere
 * "rubata" dal campo o dalla mano del bersaglio viene semplicemente scartata.
 * Proprio come nel panico vengono effettuati tutti i controlli opportuni per la scelta del bersaglio
 *
 * @param giocatori = array di giocatori
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param player = giocatore che gioca catbalou
 * @param num_giocatori = numero di giocatori in partita
 * @param log = puntatore al file di testo del log
 */
void catbalou(Giocatore *giocatori, Mazzo *mazzo_scarta, int player, int num_giocatori, FILE *log)
{
    int target;
    int mano_campo;
    int carta_scarto;

    //scelta bersaglio catbalou
    do{
        for (int i = 0; i < num_giocatori; ++i)
        {
            if(i!=player)
            {
                if(check_catbalou(giocatori, i)==1)
                {
                    printf("\nPuoi segliere il giocatore %d!", i + 1);
                }
            }
        }

        printf("\nScegli bersaglio del tuo cat-balou:");
        scanf("%d", &target);
        target--;

        if(target==player)
        {
            printf("\nNon puoi scegliere te stesso come bersaglio, riprova!");
        }
        else if(check_catbalou(giocatori, target)==0)
        {
            if(target<0 || target > num_giocatori-1)
            {
                printf("\nError, bersaglio non esiste!");
            }
            else
            {
                printf("\nIl target (giocatore %d (%s)) che hai scelto non possiede carte in mano o carte in campo", target+1, giocatori[target].nome);
            }
        }
    }while( target==player || (check_catbalou(giocatori, target)==0) || target<0 || target > num_giocatori-1);

    fprintf(log, "\t Giocatore %d (%s) gioca CATBALOU contro il giocatore %d (%s);\n", player+1, giocatori[player].nome, target+1, giocatori[target].nome);

    //target ha sia carte in mano che in campo
    if(giocatori[target].carte_in_mano.numero_carte>0 && giocatori[target].carte_in_gioco.numero_carte>0)
    {
        //scelta mano o campo
        do{
            printf("\nGiocatore %d! Il tuo target ha carte sia in campo che in mano!"
                   "\nDa che mazzo vuoi fargli scartare una carta? [1=MANO / 0=CAMPO]", player+1);
            scanf("%d", &mano_campo);
        }while(mano_campo != 0 && mano_campo != 1);

        //Se il player obbliga il target a scartare una carta dalla mano, essa sara' scelta casualmente
        if(mano_campo==1)
        {
            printf("\nCarte in mano da cui scartare:");
            visualizzatore_di_mazzi(&giocatori[target].carte_in_mano);
            scarta_carta_casuale_da_mano(giocatori, mazzo_scarta, target);

            fprintf(log, "\t Giocatore %d (%s) scarta una carta %s dalla mano;\n", target+1, giocatori[target].nome, mazzo_scarta->array_carte[mazzo_scarta->numero_carte-1].nome_carta);
        }
        else if(mano_campo==0)
        {
            printf("\nGiocatore %d! Adesso dovrai scegliere una carta dal terreno avversario da scartare!!", player+1);
            printf("\n\nCarte in campo da cui scartare:");
            visualizzatore_di_mazzi(&giocatori[target].carte_in_gioco);

            //scelta carta da scartare
            do {
                printf("\nScegli che carta vuoi scartare:");
                scanf("%d", &carta_scarto);
            }while(carta_scarto < 1 || carta_scarto > giocatori[target].carte_in_gioco.numero_carte);
            carta_scarto--;

            scarta_carta_specifica_gioco(giocatori, mazzo_scarta, target, carta_scarto);

            fprintf(log, "\t Giocatore %d (%s) scarta una carta %s dal terreno;\n", target+1, giocatori[target].nome, mazzo_scarta->array_carte[mazzo_scarta->numero_carte-1].nome_carta);
        }
    }

    //target ha solo carte in mano
    else if(giocatori[target].carte_in_mano.numero_carte > 0 && giocatori[target].carte_in_gioco.numero_carte <= 0)
    {
        printf("\nCarte in mano da cui scartare:");
        visualizzatore_di_mazzi(&giocatori[target].carte_in_mano);
        scarta_carta_casuale_da_mano(giocatori, mazzo_scarta, target);

        fprintf(log, "\t Giocatore %d (%s) scarta una carta %s dalla mano;\n", target+1, giocatori[target].nome, mazzo_scarta->array_carte[mazzo_scarta->numero_carte-1].nome_carta);
    }

    //target ha solo carte in campo
    else if(giocatori[target].carte_in_mano.numero_carte <= 0 && giocatori[target].carte_in_gioco.numero_carte > 0)
    {
        printf("\nGiocatore %d (%s)! Adesso dovrai scegliere una carta dal terreno avversario da scartare!!", player+1, giocatori[player].nome);
        printf("\n\nCarte in campo da cui scartare:");
        visualizzatore_di_mazzi(&giocatori[target].carte_in_gioco);

        do {
            printf("\nScegli che carta vuoi scartare:");
            scanf("%d", &carta_scarto);
        }while(carta_scarto < 1 || carta_scarto > giocatori[target].carte_in_gioco.numero_carte);
        carta_scarto--;
        scarta_carta_specifica_gioco(giocatori, mazzo_scarta, target, carta_scarto);

        fprintf(log, "\t Giocatore %d (%s) scarta una carta %s dal terreno;\n", target+1, giocatori[target].nome, mazzo_scarta->array_carte[mazzo_scarta->numero_carte-1].nome_carta);
    }
}

/**
 * Funzione che permette di scartare casualmente una carta dalla mano di un giocatore target.
 * Funzionamento analogo a pesca_carta_casuale_da_mano.
 *
 * @param giocatori = array di giocatori
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param target = bersaglio a cui verra' scartata una carta
 */
void scarta_carta_casuale_da_mano(Giocatore *giocatori, Mazzo *mazzo_scarta, int target)
{
    int carta_random;

    carta_random= 1 + rand() % (giocatori[target].carte_in_mano.numero_carte - 1 + 1);
    carta_random--;

    printf("\n\nHai scartato la carta %s\n\n", giocatori[target].carte_in_mano.array_carte[carta_random].nome_carta);

    scarta_carta_specifica_mano(giocatori, mazzo_scarta, target, carta_random);
}

/**
 * Funzione che simula un duello a colpi di pistola dove ogni giocatore a turno deve scartare un bang, il primo che non ha piu'
 * bang da scartare perde un HP.
 * Per fare cio' uso un ciclo while con un contatore che incrementa di 1 ad ogni ciclo, se consideriamo il modulo per 2 di questo
 * contatore, ad ogni iterazione l'operazione restituira' 1 oppure 0, e questo mi permette di alternare gli scarti tra i 2 player.
 *
 * @param giocatori = array di giocatori
 * @param player = giocatore che gioca il duello
 * @param num_giocatori = numero di giocatori in partita
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param log = puntatore al file di testo del log
 */
void duello(Giocatore *giocatori, int player, int num_giocatori, Mazzo *mazzo_scarta, FILE *log)
{
    int    target;

    //scelta target duello
    do {
        printf("\nChi vuoi sfidare a duello?");
        scanf("%d", &target);
        getchar();
        target--;

    }while(target==player || target<0 || target > num_giocatori-1);

    fprintf(log, "\t Giocatore %d (%s) sfida il giocatore %d (%s) a duello!\n", player+1, giocatori[player].nome, target+1, giocatori[target].nome);

    int i=1;
    int duello_attivo=1;


    while(duello_attivo==1)
    {
        //se i%2==1 deve scartare un bang il target del duello
        if( i%2 == 1 )
        {
            printf("\nSPARO %d ---|Giocatore %d (%s) deve scartare una carta bang|", i, target+1, giocatori[target].nome);

            //se il giocatore ha un bang in mano lo scarta
            if(check_carta_mano(giocatori, target, "Bang!")>=0)
            {
                printf("\nGiocatore %d (%s) scarta una carta bang!!", target+1, giocatori[target].nome);
                i++;
                scarta_carta_specifica_mano(giocatori, mazzo_scarta, target, check_carta_mano(giocatori, target, "Bang!"));

                fprintf(log, "\t Giocatore %d (%s) scarta una carta BANG\n", target+1, giocatori[target].nome);
            }
            //altrimenti perde un hp
            else
            {
                printf("\nIl giocatore %d (%s) non ha piu' bang in mano e quindi PERDE IL DUELLO, perde 1 hp!", target+1, giocatori[target].nome);
                giocatori[target].hp--;
                duello_attivo=0;

                fprintf(log, "\t Giocatore %d (%s) perde il duello, perde 1 HP\n", target+1, giocatori[target].nome);
            }
        }
        //se i%2==0 deve scartare un bang il player che ha usato duello
        else if( i%2 == 0 )
        {
            printf("\nSPARO %d ---|Giocatore %d (%s) deve scartare una carta bang|", i, player+1, giocatori[player].nome);

            //se il giocatore ha un bang in mano lo scarta
            if(check_carta_mano(giocatori, player, "Bang!")>=0)
            {
                printf("\nGiocatore %d (%s) scarta una carta bang!!", player+1, giocatori[player].nome);
                i++;
                scarta_carta_specifica_mano(giocatori, mazzo_scarta, player, check_carta_mano(giocatori, player, "Bang!"));

                fprintf(log, "\t Giocatore %d (%s) scarta una carta BANG\n", player+1, giocatori[player].nome);
            }
            //altrimenti perde un hp
            else
            {
                printf("\nIl giocatore %d (%s) non ha piu' bang in mano e quindi PERDE IL DUELLO, perde 1 hp!", player+1, giocatori[player].nome);
                giocatori[player].hp--;
                duello_attivo=0;

                fprintf(log, "\t Giocatore %d (%s) perde il duello, perde 1 HP\n", player+1, giocatori[player].nome);
            }
        }
        printf("\n");
    }
}

/**
 * Questa funzione fa scartare a tutti i giocatori in partita (tranne chi gioca la carta) una carta bang.
 * Chi non possiede una carta bang perde un HP.
 *
 * @param giocatori
 * @param mazzo_scarta
 * @param player
 * @param num_giocatori
 * @param log
 */
void indiani(Giocatore *giocatori, Mazzo *mazzo_scarta, int player, int num_giocatori, FILE *log)
{
    fprintf(log, "\t Giocatore %d (%s) gioca INDIANI\n", player+1, giocatori[player].nome);

    for (int i = 0; i < num_giocatori; ++i)
    {
        if(i!=player)
        {
            //se possiede una carta bang, la scarta
            if(check_carta_mano(giocatori, i, "Bang!")>=0)
            {
                printf("\nGiocatore %d (%s), hai un bang in mano, lo scarti e non perdi nessun hp.", i+1, giocatori[i].nome);
                scarta_carta_specifica_mano(giocatori, mazzo_scarta, i, check_carta_mano(giocatori, i, "Bang!"));

                fprintf(log, "\t Giocatore %d (%s) scarta BANG\n", i+1, giocatori[i].nome);
            }
            //altrimenti perde un hp
            else
            {
                printf("\nGiocatore %d (%s), non hai nessun bang in mano, PERDI 1 HP.", i+1, giocatori[i].nome);
                giocatori[i].hp--;
                fprintf(log, "\t Giocatore %d (%s) perde 1 HP\n", i+1, giocatori[i].nome);
            }
        }
    }
}
/**
 * Funzione che crea un mazzo ausiliare con tante carte quante sono i giocatori in partita.
 * A turno, partendo da chi ha giocato emporio, tutti i giocatori scelgono una carta da tale mazzo.
 *
 * @param giocatori = array di giocatori
 * @param mazzo_pesca = mazzo da cui si pescano le carte
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param player = giocatore che sta giocando il turno attuale
 * @param num_giocatori = numero di giocatori in partita
 * @param log = puntatore al file di testo del log
 */
void emporio(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int player, int num_giocatori, FILE *log)
{
    Mazzo mazzo_emporio;
    mazzo_emporio.numero_carte=0;

    int i = player;
    int j = 0;

    int scelta;

    fprintf(log, "\t Giocatore %d (%s) gioca EMPORIO\n", player+1, giocatori[player].nome);

    //il mazzo emporio viene riempito di "num_giocatori" carte
    for (int k = 0; k < num_giocatori; ++k)
    {
        check_mazzo_pesca_vuoto(mazzo_pesca, mazzo_scarta);
        pesca_carta(mazzo_pesca, &mazzo_emporio);
    }

    //ciclo while che permette ad ogni giocatore di scegliere che carta pescare dal mazzo emporio
    while(j<num_giocatori)
    {
       printf("\nMAZZO EMPORIO:");
       visualizzatore_di_mazzi(&mazzo_emporio);

       do {
           printf("\nGiocatore %d: (%s), scegli una delle carte disponibili da pescare:", i + 1, giocatori[i].nome);
           scanf("%d", &scelta);
           scelta--;
       }while(scelta < 0 || scelta > (mazzo_emporio.numero_carte-1));

       fprintf(log, "\t Giocatore %d (%s) pesca %s dal mazzo Emporio\n", i+1, giocatori[i].nome, mazzo_emporio.array_carte[scelta].nome_carta);

       scarta_carta_specifica_mazzo_generico(&mazzo_emporio, &giocatori[i].carte_in_mano, scelta);

       i++;
       if(i==num_giocatori)
       {
           i=0;
       }
       j++;
    }
}

/**
 * Funzione che permette di schierare un arma nel proprio terreno, ogni arma aumenta la gittata del giocatore in maniera diversa,
 * tranne la volcanic che non aumenta la gittata ma permette al giocatore di giocare quanti bang vuole durante il suo turno.
 * Se un'arma e' gia presente nel terreno, viene scartata per fare spazio alla nuova.
 *
 * @param giocatori
 * @param player
 * @param mazzo_scarta
 * @param scelta
 * @param log
 */
void gioca_arma(Giocatore *giocatori, int player, Mazzo *mazzo_scarta, int scelta, FILE *log)
{
    int check_arma=-1;

    //controllo se tra le carte in campo del player ci sia gia un'arma
    if(giocatori[player].carte_in_gioco.numero_carte>0)
    {
        for (int i = 0; i < giocatori[player].carte_in_gioco.numero_carte; ++i)
        {
            if(giocatori[player].carte_in_gioco.array_carte[i].tipo == ARMA)
            {
                check_arma=i;
            }
        }
    }

    //se l'arma e' presente va scartata per lasciare spazio a quella da schierare
    if(check_arma>=0)
    {
        fprintf(log, "\t Giocatore %d (%s) scarta %s dal suo terreno;\n", player+1, giocatori[player].nome, giocatori[player].carte_in_gioco.array_carte[check_arma].nome_carta);
        scarta_carta_specifica_gioco(giocatori, mazzo_scarta, player, check_arma);
    }

    //ogni arma ha un incremento della gittata diverso, la volcanic non ha incremento ma ha un suo effetto unico
    if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Carabina")==0)
    {
        printf("\n/////////////////////////////////Hai giocato Carabina!!!/////////////////////////////////\n");

        fprintf(log, "\t Giocatore %d (%s) usa CARABINA\n", player+1, giocatori[player].nome);

        scarta_carta_specifica_mano(giocatori, &giocatori[player].carte_in_gioco, player, scelta);
        giocatori[player].gittata = CARABINA;

        printf("\nLa gittata del giocatore %d (%s) e' aumentata!! Adesso e' %d\n\n", player+1, giocatori[player].nome, CARABINA);
    }
    else if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Remington")==0)
    {
        printf("\n/////////////////////////////////Hai giocato Remington!!!/////////////////////////////////\n");

        fprintf(log, "\t Giocatore %d (%s) usa REMINGTON\n", player+1, giocatori[player].nome);

        scarta_carta_specifica_mano(giocatori, &giocatori[player].carte_in_gioco, player, scelta);
        giocatori[player].gittata = REMINGTON;

        printf("\nLa gittata del giocatore %d (%s) e' aumentata!! Adesso e' %d\n\n", player+1, giocatori[player].nome, REMINGTON);
    }
    else if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Schofield")==0)
    {
        printf("\n/////////////////////////////////Hai giocato Schofield!!!/////////////////////////////////\n");

        fprintf(log, "\t Giocatore %d (%s) usa SCHOFIELD\n", player+1, giocatori[player].nome);

        scarta_carta_specifica_mano(giocatori, &giocatori[player].carte_in_gioco, player, scelta);
        giocatori[player].gittata = SCHOFIELD;

        printf("\nLa gittata del giocatore %d (%s) e' aumentata!! Adesso e' %d\n\n", player+1, giocatori[player].nome, SCHOFIELD);
    }
    else if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Winchester")==0)
    {
        printf("\n/////////////////////////////////Hai giocato Winchester!!!/////////////////////////////////\n");

        fprintf(log, "\t Giocatore %d (%s) usa WINCHESTER\n", player+1, giocatori[player].nome);

        scarta_carta_specifica_mano(giocatori, &giocatori[player].carte_in_gioco, player, scelta);
        giocatori[player].gittata = WINCHESTER;

        printf("\nLa gittata del giocatore %d (%s) e' aumentata!! Adesso e' %d\n\n", player+1, giocatori[player].nome, WINCHESTER);
    }
    else if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Volcanic")==0)
    {
        printf("\n/////////////////////////////////Hai giocato Volcanic!!!/////////////////////////////////\n");

        fprintf(log, "\t Giocatore %d (%s) usa VOLCANIC\n", player+1, giocatori[player].nome);

        scarta_carta_specifica_mano(giocatori, &giocatori[player].carte_in_gioco, player, scelta);
        giocatori[player].gittata = VOLCANIC;

        printf("\nLa gittata del giocatore %d (%s) e' sempre %d, MA ADESSO PUOI SPARARE TUTTI I BANG CHE VUOI!!!\n\n", player+1, giocatori[player].nome, VOLCANIC);
    }
}

/**
 * Funzione che restituisce 1 nel caso il giocatore abbia una volcanic nel suo terreno, altrimento 0;
 *
 * @param giocatori = array di giocatori
 * @param player = giocatore di cui si controlla il terreno
 * @return intero trattato come booleano
 */
int check_volcanic(Giocatore *giocatori, int player)
{
    int volcanic=0;

    //ciclo che restituisce uno se trova la volcanic all'interno del campo del player
    for (int i = 0; i < giocatori[player].carte_in_gioco.numero_carte; ++i) {
        if(strcmp(giocatori[player].carte_in_gioco.array_carte[i].nome_carta, "Volcanic")==0)
        {
            volcanic=1;
        }
    }

    return volcanic;
}

/**
 * Funzione che piazza un mirino nel terreno del giocatore
 *
 * @param giocatori = array di giocatori
 * @param player = player che gioca mirino
 * @param scelta = carta scelta dalla mano
 * @param log = puntatore al file di testo del log
 */
void mirino(Giocatore *giocatori, int player, int scelta, FILE *log)
{
    fprintf(log, "\t Giocatore %d (%s) usa MIRINO;\n", player+1, giocatori[player].nome);

    scarta_carta_specifica_mano(giocatori, &giocatori[player].carte_in_gioco, player, scelta);
}

/**
 * Funzione che piazza un mustang nel terreno del giocatore
 *
 * @param giocatori = array di giocatori
 * @param player = player che gioca mustang
 * @param scelta = carta scelta dalla mano
 * @param log = puntatore al file di testo del log
 */
void mustang(Giocatore *giocatori, int player, int scelta, FILE *log)
{
    fprintf(log, "\t Giocatore %d (%s) usa MUSTANG;\n", player+1, giocatori[player].nome);

    scarta_carta_specifica_mano(giocatori, &giocatori[player].carte_in_gioco, player, scelta);
}

/**
 * Funzione che piazza un barile nel terreno del giocatore
 *
 * @param giocatori = array di giocatori
 * @param player = player che gioca barile
 * @param scelta = carta scelta dalla mano
 * @param log = puntatore al file di testo del log
 */
void barile(Giocatore *giocatori, int player, int scelta, FILE *log)
{
    fprintf(log, "\t Giocatore %d (%s) usa BARILE;\n", player+1, giocatori[player].nome);

    scarta_carta_specifica_mano(giocatori, &giocatori[player].carte_in_gioco, player, scelta);
}

/**
 * Funzione che piazza una dinamite nel terreno del giocatore
 *
 * @param giocatori = array di giocatori
 * @param player = player che gioca dinamite
 * @param scelta = carta scelta dalla mano
 * @param log = puntatore al file di testo del log
 */
void dinamite(Giocatore *giocatori, int player, int scelta, FILE *log)
{
    fprintf(log, "\t Giocatore %d (%s) usa DINAMITE;\n", player+1, giocatori[player].nome);

    scarta_carta_specifica_mano(giocatori, &giocatori[player].carte_in_gioco, player, scelta);
}

/**
 * La dinamite si attiva dopo un giro completo, se un giocatore ha una dinamite attiva in campo allora deve estrarre una carta,
 * se esce una carta con seme picche e numero compreso tra 2 e il nove allora la dinamite esplode togliendo 3 hp, altrimenti
 * la passa al giocatore successivo
 *
 * @param giocatori = array di giocatori
 * @param mazzo_pesca = mazzo da cui si pescano le carte
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param player = giocatore che ha la dinamite in campo
 * @param num_giocatori = numero di giocatori in partita
 * @param log = puntatore al file di testo del log
 */
void check_esplosione_dinamite(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int player, int num_giocatori, FILE *log)
{

    int posizione_dinamite = check_carta_campo(giocatori, player, "Dinamite");

    if (posizione_dinamite >= 0)
    {
        printf("\n|~~~~~~~~~~~~~~~~~~~Giocatore %d (%s), ATTENZIONE!! hai una dinamite accesa nel tuo terreno, devi estrarre una carta~~~~~~~~~~~~~~~~~~~|\n", player + 1, giocatori[player].nome);

        //se il giocatore estrae la carta giusta esplode
        if (estrai(giocatori, mazzo_pesca, mazzo_scarta, PICCHE, player, log) == 1 && (mazzo_scarta->array_carte[(mazzo_scarta->numero_carte) - 1].numero_carta >= 2 && mazzo_scarta->array_carte[(mazzo_scarta->numero_carte) - 1].numero_carta <= 9))
        {
            printf("\nBOOOOOM!!! il Giocatore %d (%s) ha fatto esplodere la dinamite, perde 3 punti vita!!\n", player + 1, giocatori[player].nome);
            if(giocatori[player].hp>=3)
            {
                giocatori[player].hp -= 3;
            }
            else
            {
                giocatori[player].hp=0;
            }


            scarta_carta_specifica_gioco(giocatori, mazzo_scarta, player, posizione_dinamite);

            fprintf(log, "\t Il giocatore %d (%s) HA fatto esplodere la dinamite, perde 3 HP;\n", player+1, giocatori[player].nome);
        }

        //altrimenti passa la carta al giocatore successivo
        else
        {
            printf("\nGiocatore %d (%s), sei stato fortunato, passi la dinamite al giocatore successivo!\n\n", player + 1, giocatori[player].nome);

            fprintf(log, "\t Il giocatore %d (%s) NON ha fatto esplodere la dinamite, la passa al giocatore successivo;\n", player+1, giocatori[player].nome);

            if (player < (num_giocatori - 1))
            {
                scarta_carta_specifica_gioco(giocatori, &giocatori[player + 1].carte_in_gioco, player, posizione_dinamite);
            }

            //se il giocatore che deve passare la dinamite e' l'ultimo dell'array, la passa al primo
            else if (player == (num_giocatori - 1))
            {
                scarta_carta_specifica_gioco(giocatori, &giocatori[0].carte_in_gioco, player, posizione_dinamite);
            }
        }
    }
}

/**
 * Funzione che permette di piazzare una prigione nel terreno di un altro giocatore (che non sia lo sceriffo)
 *
 * @param giocatori = array di giocatori
 * @param player = giocatore che gioca la prigione
 * @param num_giocatori = numero di giocatori in partita
 * @param scelta = carta scelta, in questo caso la prigione
 * @param log = puntatore al file di testo del log
 */
void prigione(Giocatore *giocatori, int player, int num_giocatori, int scelta, FILE *log)
{
    int target;

    do {
        printf("\nGiocatore %d (%s), scegli il giocatore che vuoi intrappolare:", player+1, giocatori[player].nome);
        scanf("%d", &target);
        target--;
        if(target==player)
        {
            printf("\nNon puoi intrappolarti da solo!");
        }
        if(target < 0 || target > (num_giocatori-1))
        {
            printf("\nError, inserisci bersaglio valido!!");
        }
        if(giocatori[target].ruolo==SCERIFFO)
        {
            printf("\nNon puoi imprigionare lo sceriffo");
        }
    }while(target==player || target < 0 || target > (num_giocatori-1) || giocatori[target].ruolo==SCERIFFO);

    fprintf(log, "\t Giocatore %d (%s) usa PRIGIONE contro il giocatore %d (%s);\n", player+1, giocatori[player].nome, target+1, giocatori[target].nome);

    //sfrutto la funzione scarta carta per piazzare la prigione nel terreno del target
    scarta_carta_specifica_mano(giocatori, &giocatori[target].carte_in_gioco, player, scelta);
}

/**
 * Questa funzione permette di controllare se un giocatore ha una prigione nel suo terreno.
 * Nel caso questo sia vero il giocatore deve estrarre una carta, se esce una carta con seme cuori allora scarta la prigione e gioca il suo
 * turno, altrimenti scarta la prigione ma salta il turno.
 *
 * @param giocatori = array di giocatori
 * @param mazzo_pesca = mazzo da cui si pescano le carte
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param turno = contatore che segna il turno in corso
 * @param player = giocatore che ha la prigione in campo
 * @param turno_in_corso = booleano che segna se il e' sia in corso o meno
 * @param log= puntatore al file di testo del log
 */
void check_prigione(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int *turno, int player, int *turno_in_corso, FILE *log)
{
    int posizione_prigione = check_carta_campo(giocatori, player, "Prigione");

    if (posizione_prigione >= 0)
    {
        printf("\n|~~~~Giocatore %d (%s), ATTENZIONE!! hai una prigione attiva sul tuo terreno, devi estrarre una carta per liberarti~~~~|\n", (player) + 1, giocatori[player].nome);

        //se il giocatore estrae una carta cuori
        if (estrai(giocatori, mazzo_pesca, mazzo_scarta, CUORI, player, log) == 1)
        {
            printf("\n\nATTENZIONE!!! il Giocatore %d (%s) ha estratto una carta con seme cuori, scarta la prigione e gioca il suo turno\n", (player) + 1, giocatori[player].nome);
            scarta_carta_specifica_gioco(giocatori, mazzo_scarta, player, posizione_prigione);

            fprintf(log, "\t Il giocatore %d (%s) scarta PRIGIONE e GIOCA il turno;\n", (player)+1, giocatori[player].nome);

        }
        //altrimenti
        else
        {
            printf("\n\nIl giocatore ha estratto una carta con seme sbagliato, salta questo turno e scarta la prigione!");

            fprintf(log, "\t Il giocatore %d (%s) scarta PRIGIONE e SALTA il turno;\n", (player)+1, giocatori[player].nome);

            scarta_carta_specifica_gioco(giocatori, mazzo_scarta, player, posizione_prigione);
            (*turno_in_corso)=false;
            //il giocatore scarta la prigione e skippa il turno
        }
    }
}