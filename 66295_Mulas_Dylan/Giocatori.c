//
// Created by Dylan on 03/03/2023.
//

//inclusione header
#include "Giocatori.h"
#include "Struct_enum.h"
#include "Mazzi.h"
#include "Carte.h"
#include "Funz_sistema.h"
/**
 * Funzione che permette di inserire in input la quantita' di giocatori che dovranno
 * partecipare alla partita [min=4/max=7].
 *
 * @return  numero di giocatori che giocheranno la partita
 */
int inserisci_numero_giocatori()
{
    int num_giocatori;
    do {
        printf("\nInserire il numero di partecipanti [4-7]: ");
        scanf("%d", &num_giocatori);
        getchar();
    }while(num_giocatori<QUATTRO_GIOCATORI || num_giocatori>SETTE_GIOCATORI);

    return num_giocatori;
}

/**
 * Funzione di inserimento di del nome di ogni giocatore.
 * Un ciclo for scorre l'indice dell'array di giocatori mentre un while controlla che la stringa sia
 * della dimensione consentita.
 *
 * Per ogni nome inserito controllo tutti i nomi inseriti precedentemente e controllo se e' stato gia inserito, e se
 * questo e' vero faccio reinserire il nome in questione.
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori in partita
 * @param dim_nome_gioc = dimensione massima che puo' avere il nome di un giocatore
 */
void inserisci_nomi(Giocatore * giocatori, int num_giocatori, int dim_nome_gioc)
{
    for (int i = 0; i < num_giocatori; ++i)
    {
        do {
            printf("\nInserisci nome giocatore %d:", i + 1);
            scanf("%[^\n]s", giocatori[i].nome);
            getchar();

            if(strlen(giocatori[i].nome) > (dim_nome_gioc+1))
            {
                printf("\nTroppi caratteri inseriti, inserire di nuovo l'input!");
            }
        }while(strlen(giocatori[i].nome) > (dim_nome_gioc+1));

        //utilizzo un ciclo annidato per verificare che il nome inserito sia diverso da tutti quelli precedenti e in caso lo faccio reinserire...
        for (int j = 0; j < i; ++j)
        {
            if( strcmp( giocatori[j].nome , giocatori[i].nome)==0 )
            {
                //...utilizzando un do-while per verificare che non venga reinserito lo stesso nome ripetutamente
                do {
                    printf("\nNome uguale ad uno precedentemente inserito, reinserire nome giocatore %d", i + 1);
                    scanf("%[^\n]s", giocatori[i].nome);
                    getchar();
                }while( strcmp( giocatori[j].nome , giocatori[i].nome)==0 );
            }
        }
    }
}

/**
 * Funzione che identifica il nome piu' lungo tra tutti i giocatori e ne restituisce la dimensione.
 *
 * Questa funzione e' utilizzata nella funzione output_ogni_turno.
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori in partita
 * @return dimensione nome piu' grande
 */
int dim_nome_maggiore(Giocatore *giocatori, int num_giocatori)
{
    int dim_nome_piu_lungo = strlen(giocatori[0].nome);

    for (int i = 0; i < num_giocatori; ++i)
    {
        if (strlen(giocatori[i].nome) > dim_nome_piu_lungo)
        {
            dim_nome_piu_lungo=strlen(giocatori[i].nome);
        }
    }

    return dim_nome_piu_lungo;
}

/**
 * Funzione analoga alla precedente, ma che calcola la dimensione del nome piu' corto
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori in partita
 * @return dimensione nome piu' piccolo
 */
int dim_nome_minore(Giocatore *giocatori, int num_giocatori)
{
    int dim_nome_piu_corto = strlen(giocatori[0].nome);

    for (int i = 0; i < num_giocatori; ++i)
    {
        if (strlen(giocatori[i].nome) < dim_nome_piu_corto)
        {
            dim_nome_piu_corto=strlen(giocatori[i].nome);
        }
    }

    return dim_nome_piu_corto;
}

/**
 * Funzione che visualizza tutti i nomi dei giocatori di seguito
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori in partita
 */
void visualizza_nomi(Giocatore * giocatori, int num_giocatori)
{
    for (int i = 0; i < num_giocatori; ++i)
    {
        printf("\nNome giocatore %d: %s", i+1, (giocatori+i)->nome);
    }
    printf("\n\n");
}

/**
 * Funzione che assegna casualmente i ruoli a tutti i giocatori in gioco
 *
 * Assegno i ruoli secondo il seguente schema:
 *  4 giocatori: 1 sceriffo, 1 rinnegato, 2 fuorilegge
 *  5 giocatori: 1 sceriffo, 1 rinnegato, 2 fuorilegge, 1 vice sceriffo
 *  6 giocatori: 1 sceriffo, 1 rinnegato, 3 fuorilegge, 1 vice sceriffo
 *  7 giocatori: 1 sceriffo, 1 rinnegato, 3 fuorilegge, 2 vice sceriffo
 *
 *  Assegno sfruttando uno switch case, un vettore di booleani per evitare ripetizioni e 4  variabili counter.
 *  Lo switch considera quanti giocatori stanno giocando e incrementa i counter in maniera opportuna
 *
 *  Subito dopo aver assegnato il ruolo assegno anche i rispettivi punti vita
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori in partita
 */
void assegnamento_ruoli(Giocatore * giocatori, int num_giocatori)
{
    srand(time(NULL));

    int v[SETTE_GIOCATORI] = {0, 0, 0, 0, 0, 0, 0};
    int counter_scer = 0;
    int counter_rin = 0;
    int counter_fuori = 0;
    int counter_vice = 0;

    int max_sceriffo=1;
    int max_rinnegato=1;
    int max_vice_sceriffo=0;
    int max_fuorilegge=2;

    int target;

    //switch incrementa i counter in base al numero di giocatori in gioco inserito precedentemente in input
    switch(num_giocatori)
    {
        case SETTE_GIOCATORI:
            max_vice_sceriffo++;

        case SEI_GIOCATORI:
            max_fuorilegge++;

        case CINQUE_GIOCATORI:
            max_vice_sceriffo++;

        case QUATTRO_GIOCATORI:
            break;

        default:
            printf("\nERRORE, numero di giocatori non contemplato");
            break;
    }

    //4 do-while assegnano i rispettivi ruoli ai giocatori. Ogni ruolo viene assegnato a un tot di giocatori,
    //precisamente max_ruolo giocatori. Un vettore booleano non permette di assegnare un ruolo ad un giocatore
    //che gia possiede lo stesso oppure un'altro ruolo.

    do {
        target = rand() % num_giocatori;

        if (v[target] == 0)
        {
            giocatori[target].ruolo = SCERIFFO;
            giocatori[target].hp=HP_SCERIFFO;
            v[target] = 1;
            counter_scer++;
        }

    } while (counter_scer < max_sceriffo);

    do {
        target = rand() % num_giocatori;

        if (v[target] == 0)
        {
            giocatori[target].ruolo = RINNEGATO;
            giocatori[target].hp=HP_RUOLI_BASE;
            v[target] = 1;
            counter_rin++;
        }
    } while (counter_rin < max_rinnegato);

    do {
        target = rand() % num_giocatori;
        if (v[target] == 0)
        {
            giocatori[target].ruolo = FUORILEGGE;
            giocatori[target].hp=HP_RUOLI_BASE;
            v[target] = 1;
            counter_fuori++;
        }
    } while (counter_fuori < max_fuorilegge);

    do {
        target = rand() % num_giocatori;
        if (v[target] == 0)
        {
            giocatori[target].ruolo = VICESCERIFFO;
            giocatori[target].hp=HP_RUOLI_BASE;
            v[target] = 1;
            counter_vice++;
        }
    } while (counter_vice < max_vice_sceriffo);
}

/**
 * Funzione che permette di:
 * allocare dinamicamente e inizializzare l'array di giocatori;
 * inizializzare i 2 mazzi : mazzo pesca e mazzo scarta;
 * caricare il mazzo pesca dal file .txt;
 * mischiare il mazzo pesca e distribuire la mano iniziale ai giocatori;
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori in partita
 * @param mazzo_pesca = mazzo da cui vengono pescate le carte
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @return  array di giocatori
 */
Giocatore *preparazione_partita(Giocatore * giocatori, int *num_giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta)
{
    ///INPUT NUM_GIOCATORI E ALLOCAZIONE DINAMICA VETTORE GIOCATORI
    *num_giocatori = inserisci_numero_giocatori();
    giocatori = (Giocatore*) calloc(*num_giocatori, sizeof(Giocatore));
    if(giocatori==NULL)
    {
        printf("\nERROR, allocazione non riuscita!");
    }

    ///INSERIMENTO NOMI GIOCATORI
    inserisci_nomi(giocatori, *num_giocatori, DIM_NOME_GIOC+1);
    //visualizza_nomi(giocatori, *num_giocatori);

    ///INIZIALIZZAZIONE TIPO E NUMERO DI CARTE DEI MAZZI
    mazzo_pesca->tipo_mazzo = MAZZO_PESCA;
    mazzo_pesca->numero_carte = 0;

    mazzo_scarta->tipo_mazzo=MAZZO_SCARTO;
    mazzo_scarta->numero_carte=0;

    ///ASSEGNAMENTO RUOLI, MAZZI, HP E GITTATA AI VARI GIOCATORI
    assegnamento_ruoli(giocatori, *num_giocatori);
    visualizzatore_ruoli(giocatori, *num_giocatori);
    inizializzazione_mazzi_gittata_players(giocatori, *num_giocatori);


    ///CARICAMENTO MAZZO DA FILE E MISCHIATURA
    caricamento_mazzo_file(mazzo_pesca);
    mischiatore_di_carte(mazzo_pesca, mazzo_pesca->numero_carte);


    //printf("\n\n\nMAZZO MISCHIATO:");                   //check mazzo mischiato
    //visualizzatore_di_mazzi(mazzo_pesca);

    ///DISTRIBUZIONE MANO INIZIALE
    distribuzione_carte_iniziale(giocatori, mazzo_pesca, *num_giocatori);

    /*
    printf("\n\n\tMANO INIZIALE\n");
    for (int i = 0; i < (*num_giocatori); ++i)
    {
        printf("\nMazzo in mano giocatore %d:", i+1);
        visualizzatore_di_mazzi(&giocatori[i].carte_in_mano);
        printf("\n");
    }
    */
    printf("\n\n");

    /*
    visualizzatore_di_mazzi(&mazzo_pesca);              //check mazzo pesca dopo la distribuzione (dimensione ridotta)
    */

    return giocatori;
}

/**
 * Funzione che visualizza in sequenza i ruoli di ogni giocatore e una breve descrizione del loro obiettivo.
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori in partita
 */
void visualizzatore_ruoli(Giocatore * giocatori, int num_giocatori)
{
    printf("\n|----------------------------------RUOLI----------------------------------|");
    for (int i = 0; i < num_giocatori; ++i)
    {
        switch(giocatori[i].ruolo)
        {
            case SCERIFFO:
                printf("\n\t    |%s ha ruolo SCERIFFO e possiede %d punti vita!", giocatori[i].nome, giocatori[i].hp);
                break;
            case VICESCERIFFO:
                printf("\n\t    |%s ha ruolo VICESCERIFFO e possiede %d punti vita!", giocatori[i].nome, giocatori[i].hp);
                break;
            case FUORILEGGE:
                printf("\n\t    |%s ha ruolo FUORILEGGE e possiede %d punti vita!", giocatori[i].nome, giocatori[i].hp);
                break;
            case RINNEGATO:
                printf("\n\t    |%s ha ruolo RINNEGATO e possiede %d punti vita!", giocatori[i].nome, giocatori[i].hp);
                break;
            default:
                printf("\nRuolo non previsto, ERRORE");
                break;
        }
    }
    printf("\n|-------------------------------------------------------------------------|");
    printf("\n");
}

/**
 * Funzione che visualizza il ruolo di uno specifico giocatore.
 * Utilizzata nella funzione output_ogni_turno
 *
 * @param giocatori = array di giocatori
 * @param i = indice giocatore di cui si vuole visualizzare il ruolo
 */
void visualizzatore_ruolo_singolo(Giocatore * giocatori, int i)
{
    switch(giocatori[i].ruolo)
    {
        case SCERIFFO:
            printf("\n\t|RUOLO: SCERIFFO------>cerca di eliminare i criminali, fai fuori il rinnegato e i fuorilegge!!" );
            break;
        case VICESCERIFFO:
            printf("\n\t|RUOLO: VICESCERIFFO-------> supporta lo sceriffo, anche a costo della tua vita!!!");
            break;
        case FUORILEGGE:
            printf("\n\t|RUOLO : FUORILEGGE-------> uccidi lo sceriffo, sacrifica anche i tuoi compagni se necessario!!");
            break;
        case RINNEGATO:
            printf("\n\t|RUOLO : RINNEGATO-------> uccidi chiunque si ponga davanti al tuo obiettivo, il tuo scopo e' rimanere l'unico in vita!!!", i + 1, giocatori[i].hp);
            break;
        default:
            printf("\n\tERROR, ruolo non contemplato");
            break;
    }
}

/**
 * Funzione che visualizza per ogni ruolo la corrispettiva stringa
 *
 * @param giocatori = array di giocatori
 * @param player = indice giocatore
 * @return stringa contenente il ruolo
 */
char *visualizzatore_termine_ruolo(Giocatore * giocatori, int player)
{
    switch(giocatori[player].ruolo)
    {
        case SCERIFFO:
            return "SCERIFFO";
            break;
        case VICESCERIFFO:
            return "VICESCERIFFO";
            break;
        case FUORILEGGE:
            return "FUORILEGGE";
            break;
        case RINNEGATO:
            return "RINNEGATO";
            break;
        default:
            return "ERROR";
            break;
    }
}

/**
 *Funzione che inizializza il tipo e il numero di carte del mazzo in mano e del mazzo in campo di ogni giocatore.
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori in partita
 */
void inizializzazione_mazzi_gittata_players(Giocatore *giocatori, int num_giocatori)
{
    for (int i = 0; i < num_giocatori; ++i)
    {
        giocatori[i].carte_in_mano.tipo_mazzo= GIOC_MANO;
        giocatori[i].carte_in_mano.numero_carte=0;

        giocatori[i].carte_in_gioco.tipo_mazzo= GIOC_GIOCO;
        giocatori[i].gittata=GITTATA_BASE;
    }
}

/**
 * Funzione che sfrutta la pesca_carta per pescare le carte della mano iniziale per ogni giocatore.
 * 5 carte se il ruolo e' sceriffo, altrimenti 4.
 *
 *
 * @param giocatori = array di giocatori
 * @param mazzo_pesca = mazzo da cui si pescano le carte
 * @param num_giocatori = numero di giocatori in partita
 */
void distribuzione_carte_iniziale(Giocatore *giocatori, Mazzo *mazzo_pesca, int num_giocatori)
{
    for (int i = 0; i < num_giocatori; ++i)
    {
        if(giocatori[i].ruolo==SCERIFFO)
        {
            for (int j = 0; j < HP_SCERIFFO; ++j)
            {
                pesca_carta(mazzo_pesca, &giocatori[i].carte_in_mano);
            }
        }
        else
        {
            for (int j = 0; j < HP_RUOLI_BASE; ++j)
            {
                pesca_carta(mazzo_pesca, &giocatori[i].carte_in_mano);
            }
        }
    }
}

void check_correttezza_giocatori(Giocatore *giocatori, int num_giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int hp_max_sceffo, int hp_mazzo_ruoli_base)
{
    int errore=0;

    for (int i = 0; i < num_giocatori; ++i)
    {
        if(strlen(giocatori[i].nome)>DIM_NOME_GIOC)
        {
            printf("\nERROR, nome del giocatore %d corrotto!", i+1);

            errore=1;
        }
        if(giocatori[i].ruolo<SCERIFFO || giocatori[i].ruolo>RINNEGATO)
        {
            printf("\nERROR, ruolo del giocatore %d corrotto!", i+1);

            errore=1;
        }
        if(giocatori[i].hp<0 || ((giocatori[i].ruolo==SCERIFFO) && giocatori[i].hp>hp_max_sceffo) || ((giocatori[i].ruolo!=SCERIFFO) && (giocatori[i].hp>hp_mazzo_ruoli_base)))
        {
            printf("\nERROR, HP del giocatore %d corrotti!", i+1);

            errore=1;
        }
        if(giocatori[i].gittata<0)
        {
            printf("\nERROR, gittata del giocatore %d corrotto!", i+1);

            errore=1;
        }

        check_correttezza_mazzo(giocatori, &giocatori[i].carte_in_mano, mazzo_pesca, mazzo_scarta, num_giocatori);
        check_correttezza_mazzo(giocatori, &giocatori[i].carte_in_gioco, mazzo_pesca, mazzo_scarta, num_giocatori);

    }
    if(errore==1)
    {
        total_free(giocatori, num_giocatori, mazzo_pesca, mazzo_scarta);
        exit(-1);
    }
}

/**
 * Funzione che scorre l'array di giocatori e restituisce l'indice del giocatore che ha ruolo sceriffo.
 * Serve per far iniziare la partita allo sceriffo.
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori in partita
 * @return posizione dello sceriffo
 */
int find_posizione_sceriffo(Giocatore *giocatori, int num_giocatori)
{
    for (int i = 0; i < num_giocatori; ++i)
    {
        if(giocatori[i].ruolo==SCERIFFO)
        {
            return i;
        }
    }
}

/**
 * Funzione di output
 */
void output_avvio_gioco()
{
    printf(YELLOW_TEXT);
    printf("\n");
    printf(" .----------------.  .----------------.  .-----------------. .----------------.  .----------------. \n");
    printf("| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |\n");
    printf("| |   %s______%s     | || |      %s__%s      | || | %s____  _____%s  | || |    %s______%s    | || |              | |\n", LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT);
    printf("| |  %s|_   _ \\%s    | || |     %s/  \\%s     | || |%s|_   \\|_   _|%s | || |  %s.' ___  |%s   | || |      %s_%s       | |\n", LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT);
    printf("| |    %s| |_) |%s   | || |    %s/ /\\ \\%s    | || |  %s|   \\ | |%s   | || | %s/ .'   \\_|%s   | || |     %s| |%s      | |\n", LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT);
    printf("| |    %s|  __'.%s   | || |   %s/ ____ \\%s   | || |  %s| |\\ \\| |%s   | || | %s| |    ____%s  | || |     %s| |%s      | |\n", LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT);
    printf("| |   %s_| |__) |%s  | || | %s_/ /    \\ \\_%s | || | %s_| |_\\   |_%s  | || | %s\\ `.___]  _|%s | || |     %s| |%s      | |\n", LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT);
    printf("| |  %s|_______/%s   | || |%s|____|  |____|%s| || |%s|_____|\\____|%s | || |  %s`._____.'%s   | || |     %s|_|%s      | |\n", LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT);
    printf("| |              | || |              | || |              | || |              | || |     %s(_)%s      | |\n", LIGHT_YELLOW, YELLOW_TEXT);
    printf("| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |\n");
    printf(" '----------------'  '----------------'  '----------------'  '----------------'  '----------------'\n");
    printf(RIPRISTINA);

    printf(LIGHT_RED_TEXT);
    printf("\t\t\t\t   ___       __            __  ___     __                  ___ __  _             \n"
          "\t\t\t\t  / _ \\__ __/ /__ ____    /  |/  /_ __/ /__ ____   ___ ___/ (_) /_(_)__  ___     \n"
          "\t\t\t\t / // / // / / _ `/ _ \\  / /|_/ / // / / _ `(_-<  / -_) _  / / __/ / _ \\/ _ \\    \n"
          "\t\t\t\t/____/\\_, /_/\\_,_/_//_/ /_/  /_/\\_,_/_/\\_,_/___/  \\__/\\_,_/_/\\__/_/\\___/_//_/    \n"
          "\t\t\t\t     /___/\n\n");
    printf(RIPRISTINA);
}

/**
 * Funzione di output.
 * Visualizza il turno attuale, il giocatore che deve giocare il turno attuale, il suo ruolo, i suoi HP e la sua gittata.
 *
 * @param giocatori = array di giocatori
 * @param turno = turno della partita
 * @param player = indice giocatore che sta giocando il turno attuale
 */
void output_ogni_turno(Giocatore *giocatori, int turno, int player)
{
    printf(YELLOW_TEXT);
    printf("\n\n");
    printf("\t\t        |############################################|\n", turno);
    printf("\t\t|##########################%sTURNO %2d%s##########################|\n", LIGHT_YELLOW, turno, YELLOW_TEXT);
    printf("\t\t        |############################################|\n", turno);
    printf(RIPRISTINA);

    printf("\n\t|Tocca a %s, il giocatore %d!", giocatori[player].nome, player+1);
    printf("\n");
    visualizzatore_ruolo_singolo(giocatori, player);
    printf("\n\t|PALLOTTOLE: %d, GITTATA: %d", giocatori[player].hp, giocatori[player].gittata);
    printf("\n\n");
}

/**
 * Funzione di output che ad ogni mossa visualizza ruolo, HP e range di ogni giocatore
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori in partita
 */
void output_ogni_mossa(Giocatore *giocatori, int num_giocatori)
{
    printf("\n");
    for (int i = 0; i < num_giocatori; ++i)
    {
        if(i==0)
        {
            printf(YELLOW_TEXT);
            printf("\n\t+-------------");
            for (int j = 0; j < dim_nome_maggiore(giocatori, num_giocatori) - dim_nome_minore(giocatori, num_giocatori) + dim_nome_minore(giocatori, num_giocatori)-1; ++j)
            {
                printf("-");
            }
            printf("-------------------------------------+");
            printf(RIPRISTINA);
        }

        printf("\n\t");
        printf(YELLOW_TEXT);printf("|");printf(RIPRISTINA);

        printf("GIOCATORE %d: ", i+1);

        for (int j = 0; j < dim_nome_maggiore(giocatori, num_giocatori) - strlen(giocatori[i].nome); ++j)
        {
            printf(" ");
        }

        if(giocatori[i].ruolo==SCERIFFO)
        {
            printf("%s%s%s--->ruolo: ", LIGHT_RED_TEXT, giocatori[i].nome, RIPRISTINA);
            printf(LIGHT_YELLOW);
            printf("SCERIFFO,");
            printf(RIPRISTINA);
        }
        else
        {
            printf("%s%s%s--->ruolo: ********,",  LIGHT_RED_TEXT, giocatori[i].nome, RIPRISTINA);
        }

        printf(GREEN_TEXT);
        printf(" HP: %d", giocatori[i].hp);
        printf(RIPRISTINA);
        printf(",");

        printf(BLUE_TEXT);
        printf(" RANGE: %d", giocatori[i].gittata);
        printf(RIPRISTINA);

        printf(YELLOW_TEXT);printf("|");printf(RIPRISTINA);

        if(i==num_giocatori-1)
        {
            printf(YELLOW_TEXT);
            printf("\n\t+-------------");
            for (int j = 0; j < dim_nome_maggiore(giocatori, num_giocatori) - dim_nome_minore(giocatori, num_giocatori) + dim_nome_minore(giocatori, num_giocatori)-1; ++j)
            {
                printf("-");
            }
            printf("-------------------------------------+");
            printf(RIPRISTINA);
        }
    }
    printf("\n");
}

/**
 * funzione di output che tramite un input e poi uno switch visualizza
 * le diverse pagine del libro di istruzioni per bang.
 * Sono riportate le regole generali e le spiegazioni relative ai ruoli e alle carte.
 */
void output_libro()
{
    int scelta_pagina=0;

    do{
        switch(scelta_pagina)
        {
            case 0:
                printf("\n\n");

                printf(LIGHT_YELLOW);

                printf("\t\t\t     +--------------------------------------------------------+\n");
                printf("\t\t\t     |________________________________________________________|\n");
                printf("\t\t\t     |  |                                                  |  |\n");
                printf("\t\t\t     |  |   %s$$$$$$$\\   $$$$$$\\  $$\\   $$\\  $$$$$$\\  $$\\%s    |  |\n", RED_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |  |   %s$$  __$$\\ $$  __$$\\ $$$\\  $$ |$$  __$$\\ $$ |%s   |  |\n", RED_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |  |   %s$$ |  $$ |$$ /  $$ |$$$$\\ $$ |$$ /  \\__|$$ |%s   |  |\n", RED_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |  |   %s$$$$$$$\\ |$$$$$$$$ |$$ $$\\$$ |$$ |$$$$\\ $$ |%s   |  |\n", RED_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |  |   %s$$  __$$\\ $$  __$$ |$$ \\$$$$ |$$ |\\_$$ |\\__|%s   |  |\n", RED_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |  |   %s$$ |  $$ |$$ |  $$ |$$ |\\$$$ |$$ |  $$ |%s       |  |\n", RED_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |  |   %s$$$$$$$  |$$ |  $$ |$$ | \\$$ |\\$$$$$$  |$$\\%s    |  |\n", RED_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |  |   %s\\_______/ \\__|  \\__|\\__|  \\__| \\______/ \\__|%s   |  |\n", RED_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |__|__________________________________________________|__|\n");
                printf("\t\t\t     |                                          %s(_)%s           |\n", YELLOW_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |                                          %s/\\\\%s           |\n", YELLOW_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |              %s_     _%s           %s(_)~~~~~~/  \\\\~~~~~~(_)%s |\n", LIGHT_RED_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |   %s__ _ _   _(_) __| | __ _%s        %s\\    /    \\\\    /%s    |\n", LIGHT_RED_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |  %s/ _` | | | | |/ _` |/ _` |%s        %s\\  /      \\\\  /%s     |\n", LIGHT_RED_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     | %s| (_| | |_| | | (_| | (_| |%s         %s\\/________\\\\/%s      |\n", LIGHT_RED_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |  %s\\__, |\\__,_|_|\\__,_|\\__,_|%s         %s/\\\\        /\\%s      |\n", LIGHT_RED_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |  %s|___/%s                             %s/  \\\\      /  \\%s     |\n", LIGHT_RED_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |                                   %s/    \\\\    /    \\%s    |\n", YELLOW_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |                                %s(_)~~~~~~\\\\  /~~~~~~(_)%s |\n", YELLOW_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |          %s__  __ _      _       _%s         %s\\\\/%s           |\n", LIGHT_RED_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |   %s_   _ / _|/ _(_) ___(_) __ _| | ___%s    %s(_)%s           |\n", LIGHT_RED_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |  %s| | | | |_| |_| |/ __| |/ _` | |/ _ \\%s                 |\n", LIGHT_RED_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |  %s| |_| |  _|  _| | (__| | (_| | |  __/%s                 |\n", LIGHT_RED_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |   %s\\__,_|_| |_| |_|\\___|_|\\__,_|_|\\___|%s                 |\n", LIGHT_RED_TEXT, LIGHT_YELLOW);
                printf("\t\t\t     |                                                        |\n");
                printf("\t\t\t     +--------------------------------------------------------+\n");

                printf(RIPRISTINA);

                printf("\n\n");

                break;

            case 1:
            case 2:
                printf("\n\n");

                printf(LIGHT_YELLOW);

                printf("             ________________________________                          ________________________________            \n");
                printf(" ____--------                                --------____  ____--------                                --------____\n");
                printf("|1|               %s___ _   _  ___  _    ___%s               ||        %s___  ___ ___ ___ _____ _____ _____   _____%s    |2|\n", YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("|               %s| _ \\ | | |/ _ \\| |  |_ _|%s               ||       %s/ _ \\| _ )_ _| __|_   _|_   _|_ _\\ \\ / / _ \\%s     |\n", YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("|               %s|   / |_| | (_) | |__ | |%s                ||      %s| (_) | _ \\| || _|  | |   | |  | | \\ V / (_) |%s    |\n", YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("|               %s|_|_\\\\___/ \\___/|____|___|%s               ||       %s\\___/|___/___|___| |_|   |_| |___| \\_/ \\___/%s     |\n", YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("|________________________________________________________||________________________________________________________|\n");
                printf("| %s-SCERIFFO:%s                                             ||  %s-BANG! e' un gioco di carte ambientato nel%s            |\n", LIGHT_RED_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sIl compito dello Sceriffo e' quello%s              ||   %sselvaggio WEST!! All'inizio del gioco vengono%s        |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sdi riportare l'ordine eliminando tutti%s           ||   %sassegnati dei ruoli, uno palese, gli altri%s           |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %si fuorilegge e il rinnegato.%s                     ||   %ssegreti.%s                                             |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sAll' inizio della partita lo sceriffo sara'%s      ||                                                        |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sl'unico ad avere il ruolo visibile agli%s          ||   %sLe pallottole iniziano a volare, con i Vice%s          |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %saltri, quindi fai attenzione!%s                    ||   %sche tentano di proteggere lo Sceriffo, i%s             |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("| ====================================================== ||   %sfuorilegge che lo vogliono uccidere e il%s             |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("| %s-FUORILEGGE:%s                                           ||   %srinnegato che fa un po' quello che gli pare.%s         |\n", LIGHT_RED_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sI Fuorilegge vogliono eliminare%s                  ||                                                        |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %slo sceriffo, ma non hanno scrupoli%s               || ====================================================== |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sad eliminarsi l'un l'altro.%s                      ||                                                        |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("| ====================================================== ||  %sIl gioco conta ben 25 carte, divise in istantanee%s     |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("| %s-VICESCERIFFO:%s                                         ||  %se carte in gioco, ognuna con un effetto diverso!%s      |\n", LIGHT_RED_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sI Vice aiutano e proteggono lo%s                   ||                                                        |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %ssceriffo, perseguono gli stessi%s                  ||  %sOgni giocatore spawna con 5 HP (pallottole) e%s         |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sobiettivi anche a costo della vita.%s              ||  %suna gittata base di 1.%s                                |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sPer esserci vice devono partecipare almeno%s       ||                                                        |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %s5 giocatori!%s                                     ||  %sIl gioco tiene conto della posizione in cui%s           |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("| ====================================================== ||  %ssono seduti i giocatori, che preferibilmente si%s       |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("| %s-RINNEGATO:%s                                            ||  %sdovrebbero disporre in cerchio, percio' stai%s          |\n", LIGHT_RED_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sI Fuorilegge vogliono eliminare%s                  ||  %sparticolarmente attento a chi hai affianco!!%s          |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %so sceriffo, ma non hanno scrupoli%s                ||                                                        |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sad eliminarsi l'un l'altro%s                       ||                                                        |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|            ________________________________            ||            ________________________________            |\n");
                printf("|____--------                                --------____||____--------                                --------____|\n");

                printf(RIPRISTINA);

                printf("\n\n");

                break;

            case 3:
            case 4:
                printf("\n\n");

                printf(LIGHT_YELLOW);

                printf("             ________________________________                          ________________________________            \n");
                printf(" ____--------                                --------____  ____--------                                --------____\n");
                printf("|3|                            %s_%s                         ||                              %s_%s                       |4|\n", YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("|                  %s__ __ _ _ _| |_ ___%s                   ||                  %s__ __ _ _ _| |_ ___%s                   |\n", YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("|                 %s/ _/ _` | '_|  _/ -_)%s                  ||                 %s/ _/ _` | '_|  _/ -_)%s                  |\n", YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("|                 %s\\__\\__,_|_|  \\__\\___|%s                  ||                 %s\\__\\__,_|_|  \\__\\___|%s                  |\n", YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("|________________________________________________________||________________________________________________________|\n");
                printf("|========================================================||                                                        |\n");
                printf("|                     %s|ISTANTANEE|%s                       ||   %s-Panico:%s                                             |\n", BLUE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|========================================================||        %sRuba una carta ad un giocatore distante 1.%s      |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|  %s-Bang!:%s                                               ||   %s-Catbalou:%s                                           |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sCarta che ti permette di sparare ad un%s           ||        %sFai scartare una carta ad un giocatore%s          |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %savversario alla tua portata.%s                     ||        %squalsiasi.%s                                      |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|  %s-Mancato:%s                                             ||                                                        |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sCarta counter, annulla l'effetto di uno%s          ||========================================================|\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %ssparo.%s                                           ||                   %s|ISTANTANEE SPECIAL|%s                 |\n", WHITE_TEXT, LIGHT_YELLOW, BLUE_TEXT, LIGHT_YELLOW);
                printf("|  %s-Birra:%s                                               ||========================================================|\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sCura di 1 HP, puo' essere usata  anche per%s       ||                                                        |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %ssalvarsi in extremis, un po come i tronchi,%s      ||  %s-Duello:%s                                              |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sper intenderci.%s                                  ||       %sscelto un giocatore qualsiasi da sfidare a%s       |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|  %s-Saloon:%s                                              ||       %squalsiasi distanza, i due giocatori iniziano%s     |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sCura tutti i giocatori di 1 HP.%s                  ||       %sa scartare bang, partendo dal giocatore sfidato.%s |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|  %s-Diligenza:%s                                           ||       %sIl primo che non ha piu' bang da scartare perde%s  |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sFa pescare 2 carte.%s                              ||       %s1 punto vita!%s                                    |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|  %s-Wellsfargo:%s                                          ||  %s-Emporio:%s                                             |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sFa pescare 3 carte.%s                              ||       %sviene rivelato dal mazzo di pesca un numero di%s   |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|  %s-Gatling:%s                                             ||       %scarte pari al numero di giocatori ancora in%s      |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sSpara a tutti i giocatori, a prescindere dalla%s   ||       %svita, ogni giocatore a turno sceglie una carta%s   |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sdistanza.%s                                        ||       %sda pescare partendo da chi ha giocato la carta%s   |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|                                                        ||       %sfino ad esaurire le carte rivelate.%s              |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|            ________________________________            ||            ________________________________            |\n");
                printf("|____--------                                --------____||____--------                                --------____|\n");

                printf(RIPRISTINA);
                printf("\n\n");

                break;

            case 5:
            case 6:

                printf(LIGHT_YELLOW);

                printf("             ________________________________                          ________________________________            \n");
                printf(" ____--------                                --------____  ____--------                                --------____\n");
                printf("|                              %s_%s                         ||                              %s_%s                         |\n", YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("|                  %s__ __ _ _ _| |_ ___%s                   ||                  %s__ __ _ _ _| |_ ___%s                   |\n", YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("|                 %s/ _/ _` | '_|  _/ -_)%s                  ||                 %s/ _/ _` | '_|  _/ -_)%s                  |\n", YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("|                 %s\\__\\__,_|_|  \\__\\___|%s                  ||                 %s\\__\\__,_|_|  \\__\\___|%s                  |\n", YELLOW_TEXT, LIGHT_YELLOW, YELLOW_TEXT, LIGHT_YELLOW);
                printf("|________________________________________________________||________________________________________________________|\n");
                printf("|                                                        ||                                                        |\n");
                printf("|  %s-Indiani:%s                                             ||  %s-Volcanic:%s                                            |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %stutti i giocatori (escluso chi ha giocato la%s     ||        %sla gittata rimane 1, ma quando si ha una%s        |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %scarta) deve scartare una carta bang! Se non%s      ||        %sVolcanic sul proprio terreno si possono%s         |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sla hanno perdono un HP.%s                          ||        %sgiocare piu' bang in un solo turno!%s             |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|                                                        ||                                                        |\n");
                printf("|========================================================||========================================================|\n");
                printf("|                        %s|ARMI|%s                          ||                      %s|CARTE IN GIOCO|%s                  |\n", BLUE_TEXT, LIGHT_YELLOW, BLUE_TEXT, LIGHT_YELLOW);
                printf("|========================================================||========================================================|\n");
                printf("|                                                        ||                                                        |\n");
                printf("|  %s-Carabina:%s                                            ||  %s-Barile:%s                                              |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %saumenta la gittata a 4.%s                          ||       %squesta carta consente di estrarre quando si e'%s   |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|                                                        ||       %sbersagli di un bang:%s                             |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|  %s-Remington:%s                                           ||         %s-se esce cuori, schivi il proiettile;%s          |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %saumenta la gittata a 3.%s                          ||         %s-in caso contrario, vieni colpito;%s             |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|                                                        ||                                                        |\n");
                printf("|  %s-Schofield:%s                                           ||  %s-Dinamite:%s                                            |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %saumenta la gittata a 2.%s                          ||       %sla dinamite viene giocata nel tuo terreno%s        |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|                                                        ||       %sdove restera' innocua per un intero giro.%s        |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|  %s-Winchester:%s                                          ||       %sQuando inizi il turno e la tua dinamite e'%s       |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %saumenta la gittata a 5.%s                          ||       %sin gioco devi estrarre una carta, se esce%s        |\n", WHITE_TEXT, LIGHT_YELLOW, WHITE_TEXT, LIGHT_YELLOW);
                printf("|                                                        ||       %suna carta tra il 2 e il 9 di picche la dinamite%s  |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|                                                        ||       %sesplode facendoti perdere 3 HP.%s                  |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|            ________________________________            ||            ________________________________            |\n");
                printf("|____--------                                --------____||____--------                                --------____|\n");

                printf(RIPRISTINA);
                printf("\n\n");

                break;

            case 7:
                printf(LIGHT_YELLOW);

                printf("             ________________________________\n");
                printf(" ____--------                                --------____\n");
                printf("|                              %s_%s                         |\n", YELLOW_TEXT, LIGHT_YELLOW);
                printf("|                  %s__ __ _ _ _| |_ ___%s                   |\n", YELLOW_TEXT, LIGHT_YELLOW);
                printf("|                 %s/ _/ _` | '_|  _/ -_)%s                  |\n", YELLOW_TEXT, LIGHT_YELLOW);
                printf("|                 %s\\__\\__,_|_|  \\__\\___|%s                  |\n", YELLOW_TEXT, LIGHT_YELLOW);
                printf("|________________________________________________________|\n");
                printf("|                                                        |\n");
                printf("|       %sIn caso contrario, passi la dinamite al%s          |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sgiocatore successivo, che all'inizio del%s         |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %ssuo turno fara' lo stesso controllo.%s             |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|                                                        |\n");
                printf("|  %s-Mirino:%s                                              |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %svedi gli altri giocatori ad una distanza%s         |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sdiminuita di 1.%s                                  |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|                                                        |\n");
                printf("|  %s-Mustang:%s                                             |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %ssei visto dagli altri ad una distanza aumentata%s  |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sdi 1.%s                                            |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|                                                        |\n");
                printf("|  %s-Prigione:%s                                            |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sgioca questa carta nel terreno di un giocatore%s   |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sa scelta, indipendentemente dalla distanta%s       |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %s(escluso SCERIFFO), e lo metti in prigione!%s      |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sSe sei in prigione all'inizio del turno devi%s     |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|       %sestrarre:%s                                        |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|          %s-Se esce cuori, evadi scartando la prigione%s   |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|           %se giocando il tuo turno.%s                     |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|          %s-in caso contrario, scarti Prigione e salti%s   |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|           %sl turno.%s                                    |\n", WHITE_TEXT, LIGHT_YELLOW);
                printf("|            ________________________________            |\n");
                printf("|____--------                                --------____|\n");

                printf(RIPRISTINA);
                break;
        }

        //ad ogni input compreso tra 0 e 7 corrisponde una pagina, nel caso si volesse chiudere il libro e continuare la
        //partita si inserisce il numero 8;
        scelta_pagina = scelta_pagina_libro();

    }while(scelta_pagina>=0 && scelta_pagina<=7);
}

/**
 * Funzione di input utilizzata in output_libro.
 * Possono essere inseriti numeri dall'uno all'otto.
 *
 * @return intero che corrisponde alla pagina del libro che si vuole visualizzare
 */
int scelta_pagina_libro()
{
    int scelta_pagina;

    do{
        printf("\nChe pagina vuoi visualizzare? [0 = COPERTINA / 1-7 = ISTRUZIONI / 8 = CHIUSURA LIBRO]" );
        scanf("%d", &scelta_pagina);

        if(scelta_pagina<0 || scelta_pagina>8)
        {
            printf("\nInserisci una pagina esistente!");
        }

    }while(scelta_pagina<0 || scelta_pagina>8);

    return scelta_pagina;
}

/**
 * Funzione che calcola la distanza tra due player, il giocatore 1 e il giocatore 2.
 *
 * La distanza viene diminuita di 1 se il giocatore 1 ha in campo un mirino.
 * La distanza viene aumentata di 1 se il giocatore 2 ha in campo Mustang.
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori in partita
 * @param giocatore1 = uno dei due giocatori di cui si calcola la distanza
 *                     (giocatore che sta giocando la carta o genericamente il turno attuale).
 * @param giocatore2 = uno dei due giocatori di cui si calcola la distanza
 * @return distanza tra i due giocatori
 */
int distanza_tra_due_player(Giocatore *giocatori, int num_giocatori, int giocatore1, int giocatore2)
{
    int distanza=0;

    //calcolo della distanza
    if(abs(giocatore2-giocatore1) < abs(num_giocatori-abs(giocatore2-giocatore1)))
    {
        distanza = abs(giocatore2-giocatore1);
    }
    else
    {
        distanza = abs(num_giocatori - abs(giocatore2 - giocatore1));
    }

    //controlli che incrementano o decrementano la distanza se sono in campo mirino e/o Mustang.
    if(check_carta_campo(giocatori, giocatore1, "Mirino")>=0)
    {
        if(distanza!=1)
        {
            distanza--;
        }
    }
    if(check_carta_campo(giocatori, giocatore2, "Mustang")>=0)
    {
        distanza++;
    }

    return distanza;
}

/**
 * Funzione di output che visualizza la distanza tra il giocatore e tutti gli altri giocatori in partita
 * tramite l'uso di un ciclo for e della funzione distanza_tra_due_player.
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori in partita
 * @param giocatore = giocatore che decide di visualizzare le distanze da tutti
 */
void output_distanze_tutti_giocatori(Giocatore* giocatori, int num_giocatori, int giocatore)
{
    for (int i = 0; i < num_giocatori; ++i)
    {
        if(i!=giocatore)
        {
            printf("\nDistanza da giocatore %d= %d", i+1, distanza_tra_due_player(giocatori, num_giocatori, giocatore, i));
        }
    }
    printf("\n\n");
}

/**
 * Funzione che permette ad un giocatore di scegliere la mossa da eseguire durante il suo turno, un giocatore puo' giocare quante mosse vuole
 * fino a che non decide di passare il turno al giocatore successivo con la mossa numero 6,
 * oppure puo' anche decidere di chiudere il gioco con la mossa 7.
 *
 * @param giocatori = array di giocatori
 * @param mazzo_pesca = mazzo da cui si pescano le carte per giocare
 * @param mazzo_scarta = mazzo di carte scartate
 * @param player = il giocatore che sta giocando il turno
 * @param num_giocatori = numero di giocatori ancora in gioco, si aggiorna ogni volta che un giocatore raggiunge 0 hp
 * @param numero_bang = puntatore al numero di bang giocati in questo turno
 * @param chiusura_gioco = puntatore al booleano che nel caso sia uguale a true chiuderebbe il gioco
 * @param log = puntatore al file di testo del log
 * @return intero trattato come booleano, se il valore di ritorno e' 0, il turno continua e posso giocare altre mosse, se e' 1 il turno passa e gioca il player successivo
 */
int scelta_mosse_player(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int player, int *num_giocatori, int *numero_bang, int *chiusura_gioco, FILE *log)
{
    int scelta;
    int passa_turno;
    int scelta_chiusura_gioco;

    printf("\nGiocatore %d (%s), che mossa vuoi effettuare?"
           "\n1) GIOCA UNA CARTA"
           "\n2) MOSTRA LA TUA MANO"
           "\n3) MOSTRA LE CARTE CHE HAI IN CAMPO"
           "\n4) CONTROLLA LA DISTANZA DAGLI ALTRI GIOCATORI"
           "\n5) CONTROLLA LE CARTE IN CAMPO DEGLI ALTRI GIOCATORI"
           "\n6) PASSA IL TURNO"
           "\n7) CHIUDI IL GIOCO"
           "\n8) PULISCI SCHERMO"
           "\n9) HELP", player+1, giocatori[player].nome);

    //input scelta che prende valori dall'uno al nove
    do {
        printf("\nMossa: ");
        scanf("%d", &scelta);
        getchar();
    }while(scelta < 1 || scelta > 9);

    switch(scelta)
    {
        //gioca una carta
        case 1:
            gioca_una_carta(giocatori, mazzo_pesca, mazzo_scarta, player, num_giocatori, numero_bang, log);
            return 0;
            break;


        case 2:                 //visualizza la tua mano
            printf("\nLa tua mano:");
            visualizzatore_di_mazzi(&giocatori[player].carte_in_mano);
            printf("\n\n");
            return 0;
            break;


        case 3:                 //mostra le tue carte in campo
            printf("\nLe tue carte in campo:");
            visualizzatore_di_mazzi(&giocatori[player].carte_in_gioco);
            printf("\n\n");
            return 0;
            break;

        case 4:                 //controlla la distanza degli altri players

            output_distanze_tutti_giocatori( giocatori, *num_giocatori, player);

            return 0;
            break;

        case 5:                 //controlla le carte in campo degli altri players

            //output di tutte le carte in gioco degli altri giocatori
            //viene specificata mano dello sceriffo
            for (int i = 0; i < *num_giocatori; ++i)
            {
                if(i!=player)
                {
                    if(giocatori[i].ruolo==SCERIFFO)
                    {
                        printf("\nCarte in gioco  %s, lo SCERIFFO:", giocatori[i].nome);
                    }
                    else
                    {
                        printf("\nCarte in gioco %s", giocatori[i].nome);
                    }

                    visualizzatore_di_mazzi(&giocatori[i].carte_in_gioco);       //ho messo le carte in mano, devo ricordarmi di cambiarlo porco dio
                    printf("\n");
                }
            }

            printf("\n\n");
            return 0;
            break;

        case 6:                 //scelta se passare il turno
            do {
                printf("\nSei sicuro che vuoi passare il turno?? [1=si / 0=no]:");
                scanf("%d", &passa_turno);
                getchar();
            }while(passa_turno < 0 || passa_turno > 1);

            return passa_turno;
            break;

        case 7:                 //chiusura gioco
            printf("\nIl salvataggio avviene ad inizio turno, sei sicuro di voler chiudere il gioco? Perderai i progressi del turno attuale!");
            do {
                printf("\nVuoi davvero chiudere il gioco? [1=SI / 0=NO]:");
                scanf("%d", &scelta_chiusura_gioco);
                getchar();
            }while(scelta_chiusura_gioco!=0 && scelta_chiusura_gioco!=1);

            if(scelta_chiusura_gioco==1)
            {
                printf("\nChiusura gioco!");
                fprintf(log, "\n\t Giocatore %d (%s) CHIUDE IL GIOCO;\n\n", player+1, giocatori[player].nome);

                *chiusura_gioco=true;

                passa_turno=1;
            }
            else
            {
                passa_turno=0;
            }

            return passa_turno;
            break;
        case 8:                 //pulisci il terminale
            clear_output();
            return 0;
            break;

        case 9:                 //visualizzazione libro di istruzioni
            output_libro();
            return 0;
            break;

        default:
            printf("\nERROR, scelta non contemplata!!");
            return 0;
            break;
    }
}

/**
 * Funzione che permette di giocare una delle carte che il giocatore ha in mano.
 * Prende in input un numero che corrisponde alla carta che si vuole giocare e uno switch chiama le corrispondenti
 * funzioni.
 *
 * @param giocatori = array di giocatori
 * @param mazzo_pesca = mazzo da cui si pesca
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param player = giocatore che sta giocando il turno attuale
 * @param num_giocatori = numero di giocatori in partita
 * @param numero_bang = numero di bang giocati nel turno
 * @param log = puntatore al file di testo del log
 */
void gioca_una_carta(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int player, int *num_giocatori, int *numero_bang, FILE *log)
{
    int scelta;
    int target_bang;

    //se il giocatore ha carte in mano...
    if(giocatori[player].carte_in_mano.numero_carte>0)
    {
        //... allora un ciclo gli permettera' di scegliere quale carta giocare
        do {
            printf("\nScegli la carta da giocare:");
            scanf("%d", &scelta);
            getchar();
        } while (scelta < 1 || scelta > giocatori[player].carte_in_mano.numero_carte);
        scelta--;

        if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Bang!")==0)
        {
            //in realta la carta scelta si trova a  [scelta-1], non a scelta!!
            //se in questo turno il player non ha ancora giocato nessun bang OPPURE se se possiede una volcanic in campo
            if(*numero_bang==0 || check_volcanic(giocatori, player)==1)
            {
                //se esiste almeno un bersaglio che si puo' colpire
                if( check_bersagli_da_colpire(giocatori, player , num_giocatori) )
                {
                    printf("\n/////////////////////////////////Hai giocato BANG!!!/////////////////////////////////\n");

                    //il giocatore sceglie il bersaglio tra quelli possibili
                    do {
                        for (int i = 0; i < *num_giocatori; ++i)                                                        //output persone colpibili
                        {
                            if ((distanza_tra_due_player(giocatori, *num_giocatori, player, i) <= giocatori[player].gittata) && i!=player)
                            {
                                printf("\nPuoi colpire il giocatore %d", i + 1);
                            }
                        }

                        printf("\n\nChe giocatore vuoi annichilire?");
                        scanf("%d", &target_bang);
                        getchar();
                        target_bang--;

                        if(giocatori[player].gittata < distanza_tra_due_player( giocatori, *num_giocatori, player, target_bang) || target_bang == player)
                        {
                            if(target_bang == player)
                            {
                                printf("\nERRORE, Non ti suicidare!!\n");
                            }
                            else
                            {
                                printf("\nGiocatore fuori portata, Scegli un bersaglio adeguato!\n");
                            }
                        }

                        //se il bersaglio inserito e' valido allora il bang viene giocato contro tale bersaglio e poi la carta viene scartata
                        else
                        {
                            fprintf(log, "\t Giocatore %d (%s) gioca un Bang contro il giocatore %d (%s);\n", player+1, giocatori[player].nome, target_bang+1, giocatori[target_bang].nome);

                            bang(giocatori, target_bang, mazzo_pesca, mazzo_scarta, player, log);

                            //incremento numero di bang
                            (*numero_bang)++;

                            scarta_carta_specifica_mano(giocatori, mazzo_scarta, player, scelta);
                        }
                    }while((target_bang<0 || target_bang>(*num_giocatori-1)) || giocatori[player].gittata < distanza_tra_due_player(giocatori, *num_giocatori, player, target_bang) || target_bang == player);
                }
                else
                {
                    printf("\nNon c'e' nessun bersaglio alla tua portata in questo momento, scegli un'altra mossa!\n\n");
                }
            }
            else
            {
                printf("\nHai gia giocato un bang in questo turno, scegli un altra mossa!!\n\n");
            }
        }

        else  if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Birra")==0)
        {
            //la birra puo' essere giocata solamente se il giocatore non e' full hp
            if(giocatori[player].ruolo==SCERIFFO && giocatori[player].hp==HP_SCERIFFO)
            {
                printf("\nSei full HP, non puoi essere birrato.\n\n");
            }
            else if( (giocatori[player].ruolo!=SCERIFFO) && (giocatori[player].hp==HP_RUOLI_BASE) )
            {
                printf("\nSei full HP, non puoi essere birrato.\n\n");
            }
            else
            {
                printf("\n/////////////////////////////////Hai giocato Birra!!!/////////////////////////////////\n");
                birra(giocatori, player, log);
                scarta_carta_specifica_mano(giocatori, mazzo_scarta, player, scelta);
            }
        }

        else  if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "CatBalou")==0)
        {
            //catbalou puo' essere giocato solo se almeno un giocatore che non sia chi sta giocando il turno
            //possiede almeno una carta in mano o in campo
            if(gioco_catbalou(giocatori, player, *num_giocatori) == 1)
            {
                printf("\n/////////////////////////////////Hai giocato CatBalou!!!/////////////////////////////////\n");

                catbalou(giocatori, mazzo_scarta, player, *num_giocatori, log);

                //dopo essere stata usata la carta viene scartata
                scarta_carta_specifica_mano(giocatori, mazzo_scarta, player, scelta);
            }
            else
            {
                printf("\nNon puoi giocare CatBalou, nessun giocatore ha carte in campo o in mano (cosa assai improbabile ma comunque)");
            }
        }
        else  if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Diligenza")==0)
        {
            printf("\nHai giocato Diligenza!! Peschi 2 carte\n\n");

            fprintf(log, "\t Giocatore %d (%s) gioca DILIGENZA;\n", player+1, giocatori[player].nome);

            diligenza(giocatori, player, mazzo_pesca, mazzo_scarta, log);
            scarta_carta_specifica_mano(giocatori, mazzo_scarta, player, scelta);
        }
        else  if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Gatling")==0)
        {
            printf("\n/////////////////////////////////Hai giocato Gatling!!!/////////////////////////////////\n");

            fprintf(log, "\t Giocatore %d (%s) usa GATLING;\n", player+1, giocatori[player].nome);

            //gatling e' una ripetizione di bang contro tutti i giocatori che non tiene conto della distanza
            gatling(giocatori, mazzo_pesca, mazzo_scarta, player, num_giocatori, log);
            scarta_carta_specifica_mano(giocatori, mazzo_scarta, player, scelta);
        }
        else  if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Mancato")==0)
        {
            //la carta mancato essendo una carta counter non puo' essere giocata
            printf("\nLa carta mancato non si puo' giocare!! E' una carta difensiva\n\n");
        }
        else  if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Panico!")==0)
        {
            //la carta panico puo' essere giocata solo se almeno un giocatore e' a distanza uno e almeno un giocatore a distanza uno possiede una carta in campo o in mano
            if((check_distanza_1(giocatori, player , num_giocatori)==1) && (check_panico(giocatori, player, *num_giocatori)==1))
            {
                printf("\n/////////////////////////////////Hai giocato Panico!!!/////////////////////////////////\n");

                panico(giocatori, player, *num_giocatori, log);
                scarta_carta_specifica_mano(giocatori, mazzo_scarta, player, scelta);
                ///non funziona quest cosa, non capisco il perche, entra nell else dopo essere entrato nell if
            }
            else
            {
                printf("\nNon puoi giocare la carta panico, nessun giocatore e' a distanza 1 da te\n");
            }

        }
        else  if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Saloon")==0)
        {
            //se nel tavolo e' presente almeno un giocatore non a max hp allora saloon viene giocato
            if(check_saloon(giocatori, *num_giocatori) == 1)
            {
                printf("\n/////////////////////////////////Hai giocato Saloon!!!/////////////////////////////////\n");

                fprintf(log, "\t Giocatore %d (%s) usa SALOON;\n", player+1, giocatori[player].nome);

                saloon(giocatori, *num_giocatori, log);

                //dopo aver giocato saloon la carta viene scartata
                scarta_carta_specifica_mano(giocatori, mazzo_scarta, player, scelta);
            }
            else
            {
                printf("\nSaloon non puo' essere usato, tutti i giocatori sono full HP\n");
            }

        }
        else  if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "WellsFargo")==0)
        {
            printf("\nHai giocato WellsFargo!! Peschi 3 carte\n\n");

            fprintf(log, "\t Giocatore %d (%s) gioca WELLSFARGO;\n", player+1, giocatori[player].nome);

            WellsFargo(giocatori, player, mazzo_pesca, mazzo_scarta, log);
            scarta_carta_specifica_mano(giocatori, mazzo_scarta, player, scelta);
        }
        else  if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Indiani")==0)
        {
            printf("\n/////////////////////////////////Hai giocato Indiani!!!/////////////////////////////////\n");

            indiani(giocatori, mazzo_scarta, player, *num_giocatori, log);

            scarta_carta_specifica_mano(giocatori, mazzo_scarta, player, scelta);

        }
        else  if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Duello")==0)
        {
            printf("\n/////////////////////////////////Hai giocato Duello!!!/////////////////////////////////\n");

            duello(giocatori, player, *num_giocatori, mazzo_scarta, log);
            scarta_carta_specifica_mano(giocatori, mazzo_scarta, player, scelta);
        }
        else  if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Emporio")==0)
        {
            printf("\n/////////////////////////////////Hai giocato EMPORIO!!!/////////////////////////////////\n");

            emporio(giocatori, mazzo_pesca, mazzo_scarta, player, *num_giocatori, log);
            scarta_carta_specifica_mano(giocatori, mazzo_scarta, player, scelta);
            printf("\n");
        }
        else  if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Barile")==0)
        {
            printf("\n/////////////////////////////////Hai piazzato un gigantesco BARILE!!!/////////////////////////////////\n");
            printf("\n-+-+-+-+-+-+ Adesso potrai schivare piu' proiettili!! ...o forse no! -+-+-+-+-+-+\n");
            barile(giocatori, player, scelta, log);
        }
        else  if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Dinamite")==0)
        {
            printf("\n/////////////////////////////////Hai piazzato una pazzesca DINAMITE!!!/////////////////////////////////\n");
            dinamite(giocatori, player, scelta, log);
            printf("\nLa dinamite per ora e' innocua, tra un giro verra' attivata! Dinamite viene passata al giocatore successivo!\n\n");
        }
        else  if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Mirino")==0)
        {
            printf("\n/////////////////////////////////Hai giocato Mirino!!!/////////////////////////////////\n");
            printf("\n\t-+-+-+-+-+-+ Adesso potrai vedere i tuoi nemici ad una distanza INFERIORE !! -+-+-+-+-+-+\n");

            mirino(giocatori, player, scelta, log);

        }
        else  if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Mustang")==0)
        {
            printf("\n/////////////////////////////////Hai giocato Mustang!!!/////////////////////////////////\n");
            printf("\n\t-+-+-+-+-+-+ Adesso i tuoi nemici ti vedranno ad una distanza MAGGIORE !! -+-+-+-+-+-+\n");

            mustang(giocatori, player, scelta, log);
        }
        else  if(strcmp(giocatori[player].carte_in_mano.array_carte[scelta].nome_carta, "Prigione")==0)
        {
            printf("\n/////////////////////////////////Hai piazzato la super mega PRIGIONE!!!/////////////////////////////////\n");
            printf("\n\t---------------- puoi intrappolare la gente!! ----------------\n");

            prigione(giocatori, player, *num_giocatori, scelta, log);
        }

        //per le armi ho una funzione unica con la selezione a parte
        else if(giocatori[player].carte_in_mano.array_carte[scelta].tipo==ARMA)
        {
            gioca_arma(giocatori, player, mazzo_scarta, scelta, log);
        }

    }
    else
    {
        printf("\nNon hai carte in mano, scegli un'altra mossa!");
    }
}

/**
 * Funzione che mi permette di diminuire la dimensione dell'array di giocatori nel caso in cui uno di essi muoia (raggionga gli 0 HP)
 *
 * @param giocatori ---->array di giocatori
 * @param num_giocatori ---->numero di giocatori ancora in partita
 */
void check_morte_giocatore(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int *num_giocatori, int *player_attuale, int *turno_in_corso, FILE *log)
{
    for (int i = 0; i < *num_giocatori; ++i)                    //con un for controllo se qualcuno dei giocatori abbia raggiunto HP 0
    {
        if(giocatori[i].hp<=0)
        {
            if(i == ((*num_giocatori)-1))                                       //se il giocatore morto e' l'ultimo dell array
            {
                //se il giocatore non puo' essere birrato
                if( birra_in_extremis(giocatori, i, "Birra", mazzo_scarta, log) == 0 )
                {
                    fprintf(log, "\t Il giocatore %d (%s) e' MORTO!\n", i+1, giocatori[i].nome);

                    //se un giocatore muore i suoi due mazzi vengono scartati
                    if(giocatori[i].carte_in_mano.numero_carte>0)
                    {
                        scarta_mazzo( &giocatori[i].carte_in_mano, mazzo_scarta);
                    }
                    if(giocatori[i].carte_in_gioco.numero_carte>0)
                    {
                        scarta_mazzo( &giocatori[i].carte_in_gioco, mazzo_scarta);
                    }

                    //decremento il numero di giocatori e rialloco
                    (*num_giocatori)--;
                    giocatori = (Giocatore *) realloc(giocatori, (*num_giocatori) * sizeof(Giocatore));

                    printf(RED_TEXT);
                    printf("\n<-+-+-+-+-+-+-+-+-+-+-+-+| IL GIOCATORE %d (%s%s%s) E' MORTO, Il suo ruolo era %12s!\n", i+1, LIGHT_YELLOW, giocatori[i].nome, RED_TEXT, visualizzatore_termine_ruolo(giocatori, i));
                    printf("<-+-+-+-+-+-+-+-+-+-+-+-+| I SUOI MAZZI SONO STATI SCARTATI SCARTATI");
                    printf(RIPRISTINA);

                    bonus_malus_kill(giocatori, mazzo_pesca, mazzo_scarta, *player_attuale, i, log);

                    printf("\n\nMazzo scarta:");
                    visualizzatore_di_mazzi(mazzo_scarta);
                    printf("\n\n");


                }
            }

                //se il giocatore invece non e' l'ultimo dell'array
            else if(i < ((*num_giocatori)-1))
            {
                //se il giocatore non puo' essere birrato
                if(birra_in_extremis(giocatori, i, "Birra", mazzo_scarta, log) == 0 )
                {
                    fprintf(log, "\t Il giocatore %d (%s) e' MORTO!\n", i+1, giocatori[i].nome);

                    //se un giocatore muore i suoi due mazzi vengono scartati
                    if(giocatori[i].carte_in_mano.numero_carte>0)
                    {
                        scarta_mazzo( &giocatori[i].carte_in_mano, mazzo_scarta);
                    }
                    if(giocatori[i].carte_in_gioco.numero_carte>0)
                    {
                        scarta_mazzo( &giocatori[i].carte_in_gioco, mazzo_scarta);
                    }

                    printf(RED_TEXT);
                    printf("\n<-+-+-+-+-+-+-+-+-+-+-+-+| IL GIOCATORE %d (%s%s%s) E' MORTO, Il suo ruolo era %12s!\n", i+1, LIGHT_YELLOW, giocatori[i].nome, RED_TEXT, visualizzatore_termine_ruolo(giocatori, i));
                    printf("<-+-+-+-+-+-+-+-+-+-+-+-+| I SUOI MAZZI SONO STATI SCARTATI SCARTATI");
                    printf(RIPRISTINA);

                    bonus_malus_kill(giocatori, mazzo_pesca, mazzo_scarta, *player_attuale, i, log);

                    printf("\n\nMazzo scarta:");
                    visualizzatore_di_mazzi(mazzo_scarta);
                    printf("\n\n");

                    //shifto i giocatori di uno a sinistra
                    for (int j = i; j <= ((*num_giocatori) - 2); ++j)
                    {
                        giocatori[j] = giocatori[j + 1];
                    }

                    //se muore un giocatore prima del player che sta giocando
                    if (i < (*player_attuale))
                    {
                        (*num_giocatori)--;
                        giocatori = (Giocatore *) realloc(giocatori, (*num_giocatori) * sizeof(Giocatore));
                        (*player_attuale)--;
                    }
                    //se muore il player che sta giocando il turno
                    else if (i == (*player_attuale))
                    {
                        (*num_giocatori)--;
                        giocatori = (Giocatore *) realloc(giocatori, (*num_giocatori) * sizeof(Giocatore));
                        (*player_attuale)--;
                        (*turno_in_corso)=false;

                    }
                    //se muore un un giocatore dopo il player che sta giocando ma comunque prima dell'ultima posizione
                    else
                    {
                        (*num_giocatori)--;
                        giocatori = (Giocatore *) realloc(giocatori, (*num_giocatori) * sizeof(Giocatore));
                    }
                }
            }
        }
    }
}

/**
 * Funzione che permette di assegnare un bonus o un malus ad un giocatore nel caso in cui uccida un'altro giocatore con
 * un particolare ruolo.
 *
 * Ad esempio, se un giocatore uccide uno sceriffo, allora pesca 3 carte aggiuntive.
 * Se lo sceriffo uccide un suo vice scarta tutta la mano.
 *
 * @param giocatori
 * @param mazzo_pesca
 * @param mazzo_scarta
 * @param player_attuale
 * @param player_morto
 * @param log
 */
void bonus_malus_kill(Giocatore *giocatori, Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, int player_attuale, int player_morto, FILE *log)
{
    if(player_attuale!=player_morto)
    {
        if(giocatori[player_morto].ruolo==FUORILEGGE)
        {
            printf("\nATTENZIONE! il giocatore %d (%s) ha ucciso un fuorilegge, pesca 3 carte!!", player_attuale+1, giocatori[player_attuale].nome);
            for (int i = 0; i < 3; ++i)
            {
                check_mazzo_pesca_vuoto(mazzo_pesca, mazzo_scarta);
                pesca_carta(mazzo_pesca, &giocatori[player_attuale].carte_in_mano);

                fprintf(log, "\t Il giocatore %d (%s) ha ucciso un fuorilegge, pesca %s;\n", player_attuale+1, giocatori[player_attuale].nome, giocatori[player_attuale].carte_in_mano.array_carte[giocatori[player_attuale].carte_in_mano.numero_carte-1].nome_carta);
            }
        }
        else if(giocatori[player_attuale].ruolo==SCERIFFO && giocatori[player_morto].ruolo==VICESCERIFFO)
        {
            fprintf(log, "\t Il giocatore %d (%s) ha ucciso un suo vice, scarta tutta la mano;\n", player_attuale+1, giocatori[player_attuale].nome);

            printf("\n ATTENZIONE! lo SCERIFFO ha ucciso un suo vice, scarta tutta la mano!!");
            scarta_mazzo( &giocatori[player_attuale].carte_in_mano, mazzo_scarta);

        }
    }

}
/**
 * Funzione che permette di scartare un intero mazzo tramite un ciclo while che scarta fin quando ci sono ancora
 * carte nel mazzo.
 *
 * @param mazzo_target = mazzo che si vuole scartare
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 */
void scarta_mazzo(Mazzo *mazzo_target, Mazzo *mazzo_scarta)
{

    do {
        scarta_carta(mazzo_target, mazzo_scarta);
    }while(mazzo_target->numero_carte>0);

}
/**
 * Funzione che permette di pescare 2 carte all'inizio di ogni turno.
 * Prima di ogni pescata si controlla che il mazzo pesca abbia almeno una carta, se questo non e' vero allora
 * si rimescola il mazzo scarta e si pesca da quello.
 *
 * @param mazzo_pesca = mazzo da cui si pescano le carte
 * @param mazzo_scarta = mazzo dove finiscono le carte scartate
 * @param giocatori = array di giocatori
 * @param player = giocatore che sta giocando il turno attuale
 * @param log = puntatore al file di testo del log
 */
void pescata_ogni_turno(Mazzo *mazzo_pesca, Mazzo *mazzo_scarta, Giocatore *giocatori, int player, FILE*log)
{
    for (int i = 0; i < 2; ++i) {
        check_mazzo_pesca_vuoto(mazzo_pesca, mazzo_scarta);
        pesca_carta(mazzo_pesca, &giocatori[player].carte_in_mano);

        fprintf(log, "\t Giocatore %d (%s) pesca %s!\n", player+1, giocatori[player].nome, giocatori[player].carte_in_mano.array_carte[giocatori[player].carte_in_mano.numero_carte-1].nome_carta);
    }
}

/**
 * Funzione che permette di scartare tante carte quante sono in piu' rispetto agli del player che sta giocando il turno
 * grazie all'utilizzo della funzione scarta_carta_specifica.
 * Se il giocatore ha meno carte del numero dei suoi hp (oppure le quantita' coincidono), non scartera' nessuna carta.
 *
 * @param mazzo_scarta = mazzo dove finiranno le carte scartate
 * @param giocatori = array di giocatori
 * @param player = indice del giocatore che sta giocando il turno in questo momento
 * @param log = puntatore al file di testo del log
 */
void scarto_fine_turno( Mazzo *mazzo_scarta, Giocatore *giocatori, int player, FILE *log)
{
    int n_scarti=0;

    int carta_scarto;

    if(giocatori[player].carte_in_mano.numero_carte > giocatori[player].hp)
    {

        n_scarti= (giocatori[player].carte_in_mano.numero_carte) - giocatori[player].hp;

        printf("\nscarta %d carte prima di passare il turno!!", n_scarti);

        for (int i = 0; i < n_scarti; ++i)
        {
            do
            {
                printf("\nScegli carta da scartare:");
                scanf("%d", &carta_scarto);
                getchar();

                if(carta_scarto<1 || carta_scarto > giocatori[player].carte_in_mano.numero_carte)
                {
                  printf("\nERROR, carta non consentita, reinserisci!");
                }
            }while(carta_scarto<1 || carta_scarto > giocatori[player].carte_in_mano.numero_carte);
            carta_scarto--;

            fprintf(log, "\t Il giocatore %d (%s) scarta %s;\n", player+1, giocatori[player].nome, giocatori[player].carte_in_mano.array_carte[carta_scarto].nome_carta);

            scarta_carta_specifica_mano(giocatori, mazzo_scarta, player, carta_scarto);

            printf("\nMazzo dopo lo scarto %d", i+1);
            visualizzatore_di_mazzi(&giocatori[player].carte_in_mano);

            printf("\n\nMazzo scarta dopo lo scarto:");
            visualizzatore_di_mazzi(mazzo_scarta);
            printf("\n");

        }
    }


}

/**
 * Funzione che verifica che ci sia almeno un giocatore nel raggio d'azione del player e restituisce 1 nel caso questo
 * sia vero, altrimenti 0;
 *
 * @param giocatori = array di giocatorei
 * @param player = giocatore di cui si controllano i bersagli
 * @param num_giocatori = numero di giocatori in partita
 * @return intero trattato come booleano
 */
int check_bersagli_da_colpire(Giocatore *giocatori, int player ,int *num_giocatori)
{
    int check=0;

    for (int i = 0; i < *num_giocatori; ++i)
    {
        if( (distanza_tra_due_player(giocatori, *num_giocatori, player, i) <= giocatori[player].gittata) && i!= player)
        {
            check=1;
        }
    }

    return check;
}

/**
 * Funzione che restituisce 1 nel caso ci sia almeno un giocatore a distanza 1 dal player.
 *
 * @param giocatori = array di giocatori
 * @param player = player di cui si controlla la distanza dagli altri giocatori
 * @param num_giocatori = numero di giocatori in partita
 * @return intero trattato come booleano
 */
int check_distanza_1(Giocatore *giocatori, int player ,int *num_giocatori)
{
    for (int i = 0; i < *num_giocatori; ++i)
    {
        if( (distanza_tra_due_player(giocatori, *num_giocatori, player, i) == 1) && i!= player)
        {
            return 1;
        }
    }

    return 0;
}
/**
 * Funzione che sfrutta dei counter per verificare quali giocatori sono ancora in vita e restituisce
 * true nel caso una condizione di vittoria venga soddisfatta
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori in partita
 * @return booleano
 */
int vittoria(Giocatore *giocatori, int num_giocatori)
{
    int cont_sceriffo=0, cont_fuorilegge=0, cont_rinnegato=0, cont_vice=0;

    incremento_counter_vittoria(giocatori, num_giocatori, &cont_sceriffo, &cont_fuorilegge, &cont_rinnegato, &cont_vice);

    //vittoria sceriffo (con sceriffo in vita e vicesceriffo vivi o anche morti)
    if(cont_sceriffo==1 && cont_vice>=0 && cont_fuorilegge==0 && cont_rinnegato==0)
    {
        return true;
    }
    //vittoria rinnegato
    else if(cont_rinnegato==1 && cont_sceriffo==0 && cont_fuorilegge==0 && cont_vice==0)
    {
        return true;
    }
    //vittoria fuorilegge
    else if(cont_fuorilegge>0 && cont_sceriffo==0 && cont_vice==0 && cont_rinnegato==0)
    {
        return true;
    }

    return false;
}

/**
 * Funzione che incrementa i counter dichiarati nelle altre funzioni vittoria tramite un ciclo for che scorre l'array
 * di giocatori e uno switch che incrementa il counter giusto.
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori
 * @param cont_sceriffo = counter di sceriffi
 * @param cont_fuorilegge = counter di fuorilegge
 * @param cont_rinnegato = counter di rinnegati
 * @param cont_vice = counter di vicesceriffi
 */
void incremento_counter_vittoria(Giocatore *giocatori, int num_giocatori, int *cont_sceriffo, int *cont_fuorilegge, int *cont_rinnegato, int *cont_vice)
{

    for (int i = 0; i < num_giocatori; ++i)
    {
        switch(giocatori[i].ruolo)
        {
            case SCERIFFO:
                (*cont_sceriffo)++;
                break;
            case VICESCERIFFO:
                (*cont_vice)++;
                break;
            case FUORILEGGE:
                (*cont_fuorilegge)++;
                break;
            case RINNEGATO:
                (*cont_rinnegato)++;
                break;
            default:
                printf("\nErrore INCREDIBILE, ruolo non contemplato!!");
        }
    }
}

/**
 * Funzione di output che visualizza un messaggio di vittoria diverso per ogni diverso caso.
 * In caso di chiusura gioco da parte di un utente non visualizzara' nessun messaggio di vittoria, ma solamente "PARTITA TERMINATA"
 *
 * @param giocatori = array di giocatori
 * @param num_giocatori = numero di giocatori
 * @param log = puntatore al file di testo del log
 */
void output_vittoria(Giocatore *giocatori, int num_giocatori, FILE *log)
{
    int cont_sceriffo=0, cont_fuorilegge=0, cont_rinnegato=0, cont_vice=0;

    incremento_counter_vittoria(giocatori, num_giocatori, &cont_sceriffo, &cont_fuorilegge, &cont_rinnegato, &cont_vice);

    clear_output();

    printf("\n"
           "    ____  ___    ____  ___________________       ________________  __  ________   _____  _________ \n"
           "   / __ \\/   |  / __ \\/_  __/  _/_  __/   |     /_  __/ ____/ __ \\/  |/  /  _/ | / /   |/_  __/   |\n"
           "  / /_/ / /| | / /_/ / / /  / /  / / / /| |      / / / __/ / /_/ / /|_/ // //  |/ / /| | / / / /| |\n"
           " / ____/ ___ |/ _, _/ / / _/ /  / / / ___ |     / / / /___/ _, _/ /  / // // /|  / ___ |/ / / ___ |\n"
           "/_/   /_/  |_/_/ |_| /_/ /___/ /_/ /_/  |_|    /_/ /_____/_/ |_/_/  /_/___/_/ |_/_/  |_/_/ /_/  |_|\n"
           "                                                                                                   ");


    if(cont_sceriffo==1 && cont_vice>=0 && cont_fuorilegge==0 && cont_rinnegato==0)
    {
        printf("\n"
               "\t\t         __                   _       __           __               _            __  _       _       __\n"
               "\t\t        / /_  ____ _   _   __(_)___  / /_____     / /___ _   ____ _(_)_  _______/ /_(_)___  (_)___ _/ /\n"
               "\t\t       / __ \\/ __ `/  | | / / / __ \\/ __/ __ \\   / / __ `/  / __ `/ / / / / ___/ __/ /_  / / / __ `/ / \n"
               "\t\t      / / / / /_/ /   | |/ / / / / / /_/ /_/ /  / / /_/ /  / /_/ / / /_/ (__  ) /_/ / / /_/ / /_/ /_/  \n"
               "\t\t     /_/ /_/\\__,_/    |___/_/_/ /_/\\__/\\____/  /_/\\__,_/   \\__, /_/\\__,_/____/\\__/_/ /___/_/\\__,_(_)   \n"
               "\t\t                                                          /____/                                       ");
        printf("\n\n");

        if(cont_vice>0)
        {
            fprintf(log, "\nPARTITA FINITA, HANNO VINTO SCERIFFO E VICESCERIFFI!\n");
        }
        else
        {
            fprintf(log, "\nPARTITA FINITA, HA VINTO LO SCERIFFO!\n");
        }

    }
        //vittoria rinnegato
    else if(cont_rinnegato==1 && cont_sceriffo==0 && cont_fuorilegge==0 && cont_vice==0)
    {
        printf("\n"
               "\t\t    __                   _       __           _ __        _                              __        __\n"
               "\t\t   / /_  ____ _   _   __(_)___  / /_____     (_) /  _____(_)___  ____  ___  ____ _____ _/ /_____  / /\n"
               "\t\t  / __ \\/ __ `/  | | / / / __ \\/ __/ __ \\   / / /  / ___/ / __ \\/ __ \\/ _ \\/ __ `/ __ `/ __/ __ \\/ / \n"
               "\t\t / / / / /_/ /   | |/ / / / / / /_/ /_/ /  / / /  / /  / / / / / / / /  __/ /_/ / /_/ / /_/ /_/ /_/  \n"
               "\t\t/_/ /_/\\__,_/    |___/_/_/ /_/\\__/\\____/  /_/_/  /_/  /_/_/ /_/_/ /_/\\___/\\__, /\\__,_/\\__/\\____(_)   \n"
               "\t\t                                                                         /____/                      ");
        printf("\n\n");

        fprintf(log, "\nPARTITA FINITA, HA VINTO IL RINNEGATO!\n");
    }
        //vittoria fuorilegge
    else if(cont_fuorilegge>0 && cont_sceriffo==0 && cont_vice==0 && cont_rinnegato==0)
    {
        printf("\n"
               "     __                                     _       __           _    ____                 _ __                      __\n"
               "    / /_  ____ _____  ____  ____     _   __(_)___  / /_____     (_)  / __/_  ______  _____(_) /__  ____ _____ ____  / /\n"
               "   / __ \\/ __ `/ __ \\/ __ \\/ __ \\   | | / / / __ \\/ __/ __ \\   / /  / /_/ / / / __ \\/ ___/ / / _ \\/ __ `/ __ `/ _ \\/ / \n"
               "  / / / / /_/ / / / / / / / /_/ /   | |/ / / / / / /_/ /_/ /  / /  / __/ /_/ / /_/ / /  / / /  __/ /_/ / /_/ /  __/_/  \n"
               " /_/ /_/\\__,_/_/ /_/_/ /_/\\____/    |___/_/_/ /_/\\__/\\____/  /_/  /_/  \\__,_/\\____/_/  /_/_/\\___/\\__, /\\__, /\\___(_)   \n"
               "                                                                                             /____//____/           ");

        fprintf(log, "\nPARTITA FINITA, HANNO VINTO I FUORILEGGE!\n");
    }
    else
    {
        printf("\nGioco chiuso correttamente!");
    }
}

