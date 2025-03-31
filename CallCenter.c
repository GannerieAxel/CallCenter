#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>

//Horloge
typedef struct{
    int heure;
    int minute;
    int seconde;
}Temps;

//Création d'une file au moyen d'une liste doublement chaînée
typedef struct client_s{
    struct client_s* prev ;
    char nom[50] ;
    Temps heure_arr;
    struct client_s* next;
} client ;

typedef struct{
    int size ;
    client* head ;
    client* tail;
} queue ;

typedef struct{
    int tps_min;
    int tps_max;
    double tps_moy;
    int taille_min;
    int taille_max;
    double taille_moy;
    int nbclientstot;
    int taille_tot;
    int tps_rep_min;
    int tps_rep_max;
    double tps_rep_moy; 
} attente ; //Informations sur le temps d'attente en seconde

typedef struct 
{
    Temps debut_appel;
    Temps fin_appel;
    int duree_appel;
} operateurs;

queue createqueue();
void addtail(queue *q, char nom[], Temps horloge);
client* pophead(queue *q);
double explaw(double lambda);
Temps init_clock_deb(int heure_debut);
Temps init_clock_fin(int heure_fin);
void add_sec(Temps *clock);
bool est_libre(int num, operateurs* op);
void change_statut(operateurs op, Temps temps_max, Temps temps_min, Temps horloge);
void actualise_info_attente_clientsortant(attente* a, client c, queue q, Temps horloge);
void actualise_info_attente_cliententrant(attente* a, queue q);
double unilaw();

// Opération sur la structure opérateur

void init_op(operateurs* op, int nb_op){
    for (int i = 0 ; i<nb_op ; i++){
        op[i].duree_appel = 0;
        op[i].debut_appel.heure = 0;
        op[i].debut_appel.minute = 0;
        op[i].debut_appel.seconde = 0;
    }
}

bool est_libre(int num, operateurs* op){
    if(op[num].duree_appel==0){
        return(true);
    }
    else{
        return(false);
    }
}

void change_statut(operateurs op, Temps temps_max, Temps temps_min, Temps horloge){
    Temps intervalle;
    intervalle.heure = temps_max.heure - temps_min.heure;
    intervalle.minute = temps_max.minute - temps_min.minute;
    intervalle.seconde = temps_max.seconde - temps_min.seconde;
    intervalle.heure = intervalle.heure * unilaw();
    intervalle.minute = intervalle.minute * unilaw();
    intervalle.seconde = intervalle.seconde * unilaw();
    op.debut_appel.heure = horloge.heure;
    op.debut_appel.minute = horloge.minute;
    op.debut_appel.seconde = horloge.seconde;
    op.fin_appel.heure = horloge.heure + intervalle.heure;
    op.fin_appel.minute = (horloge.minute + intervalle.minute)%60;
    op.fin_appel.seconde = (horloge.seconde + intervalle.seconde)%60;
    if (horloge.seconde + intervalle.seconde>=60){
        op.fin_appel.minute ++;
    }
    if (horloge.minute + intervalle.minute>=60){
        op.fin_appel.heure ++;
    }
    op.duree_appel = intervalle.heure*3600+ intervalle.minute*60+ intervalle.seconde;
}

//Initialiser l'horloge pour la simulation
Temps init_clock_deb(int heure_debut){
    Temps clock = {.heure = heure_debut , .minute = 0, .seconde = 0};
    return clock ;
}

//Initialiser l'horloge pour la simulation
Temps init_clock_fin(int heure_fin){
    Temps clock = {.heure = heure_fin , .minute = 0, .seconde = 0};
    return clock ;
}

//Ajout d'une seconde à l'horloge
void add_sec(Temps* clock){
    clock->seconde++;
    if (clock->seconde == 60){
        clock->seconde=0;
        clock->minute++;
        if (clock->minute ==60){
            clock->minute=0;
            clock->heure++;
        }
    }
}

queue createqueue(){
    queue q = {.size = 0, .head = NULL, .tail = NULL} ;
    return q ;
}

void addtail(queue *q, char nom[], Temps horloge){
    //Ajoute un client en fin de file
    client* c = malloc(sizeof(client));
    if (q->size==0){
        q->head = c;
        q->tail = c;
        strcpy(c->nom,nom) ;
        q->size++;
        c->heure_arr.heure = horloge.heure;
        c->heure_arr.minute = horloge.minute;
        c->heure_arr.seconde = horloge.seconde;
    }
    else{
        c->prev = q->tail ;
        c->prev->next = c;
        q->tail = c ;
        strcpy(c->nom,nom) ;
        c->next = NULL;
        q->size+=1;
        c->heure_arr.heure = horloge.heure;
        c->heure_arr.minute = horloge.minute;
        c->heure_arr.seconde = horloge.seconde;
    }
}

client* pophead(queue* q){
    //Enleve le client en tête de file et le renvoie parce qu'on en a besoin en fait
    //assert(q->size>0)
    if(q->size>0){
        client* tmp = q->head;
        q->head = q->head->next;
        if (q->size==1){
            q->tail= NULL;
        }
        else{
            q->head->prev = NULL;
        }
        q->size--;
        return tmp;
    }
    else{
        //printf("Liste vide");
    }
}

double unilaw(){
    return ((double)rand()/(double)RAND_MAX);
}


double explaw(double lambda){
    double U = unilaw() ;
    return(-log(1-U)/(double)lambda);
}

void printlist(queue q){
    client* c = q.head;
    if(c==NULL){
        printf("Liste vide");
    }
    while(c!=NULL){
        printf("%s ", c->nom);
        c= c->next;
    }
    printf("\n");
}

void ajouteclient(client c, int jour, char* nomfichier){
    FILE* f = fopen(nomfichier, "a");
    if (f != NULL){
        fprintf(f, "Nom : %s, Jour : %d, Heure d'arrivée : %d:%d:%d\n", c.nom, jour, c.heure_arr.heure, c.heure_arr.minute, c.heure_arr.seconde);
        fclose(f);
    }
}

void ajoutedonneesjournee(int jour, attente a, char* nomfichier, int heure_deb, int heure_fin){
    FILE* f = fopen(nomfichier, "a");
    if (f != NULL){
        fprintf(f,"\n\n\nResume jour %d :\n", jour);
        fprintf(f,"Taille de la queue : min : %d, max : %d, moyenne : %lf\n", a.taille_min, a.taille_max, a.taille_moy);
        fprintf(f,"Temps d'attente : min : %d, max : %d, moyen : %lf\n", a.tps_min, a.tps_max, a.tps_moy);
        fprintf(f,"Debit journalier moyen : %lf/h\n", (double)(a.nbclientstot/(heure_fin-heure_deb)));
        fprintf(f,"Fin du jour %d", jour);
        fclose(f);
    }
}

//Fonctions pour les temps d'attente

attente init_attente(){
    attente a = {.tps_max=0, .tps_min=0, .tps_moy=0, .taille_max=0, .taille_min=0, .taille_moy=0, .nbclientstot = 0, .taille_tot = 0, .tps_rep_min = 0, .tps_rep_max = 0, .tps_rep_moy = 0};
    return a;
}

void actualise_info_attente_clientsortant(attente* a, client c, queue q, Temps horloge){
    int tps_att = (horloge.heure- c.heure_arr.heure)*3600 + (horloge.minute-c.heure_arr.minute)*60 + (horloge.seconde-c.heure_arr.seconde);
    if (a->tps_max < tps_att){
        a->tps_max = tps_att;
    }
    if (a->tps_min > tps_att){
        a->tps_min = tps_att;
    }
    a->tps_moy = (a->tps_moy*a->nbclientstot + tps_att)/(a->nbclientstot+1);
    a->nbclientstot++;
    if (q.size < a->taille_min){
        a->taille_min = q.size ;
    }
}

void actualise_info_attente_cliententrant(attente* a, queue q){
    if (q.size > a->taille_min){
        a->taille_max = q.size ;
    }
}

void actualise_tps_rep_attente(attente* a, Temps temps_max, Temps temps_min){
    
}

void actualise_taille_attente(attente* a, queue q, Temps horloge, int heure_deb, int heure_fin){
    a->taille_moy += q.size;
}

char* generenom(){
    int nb_char = rand()%10+5;
    char* nom = malloc(sizeof(char)*(nb_char+1)); 
    for(int i=0; i<nb_char;i++){
        char lettre = rand()%26+65;
        nom[i] = lettre;
    }
    nom[nb_char] = '\0';
    return nom;
}


void simulation(int heure_deb, int heure_fin, int nb_jours, int nb_op, double lambda, Temps temps_min, Temps temps_max, char* nomfichier){
    FILE* f = fopen(nomfichier, "w");
    fclose(f);
    Temps horloge = init_clock_deb(heure_deb);
    attente* tab_attente = malloc(sizeof(attente)*nb_jours);
    queue q = createqueue();
    operateurs* op = malloc(sizeof(operateurs)*nb_op);
    for(int i =0 ; i<nb_jours ; i++){
        init_op(op, nb_op);
        tab_attente[i] = init_attente();
        horloge = init_clock_deb(heure_deb);
        while (horloge.heure<heure_fin){
            //Ajoute un client selon la proba d'ajouter un client
            if(explaw(lambda)<1.0){
                char* nom = generenom();
                addtail(&q,nom,horloge);
                actualise_info_attente_cliententrant(&tab_attente[i], q);
                ajouteclient(*q.tail, i+1, nomfichier);
                free(nom);
            }
            for (int j = 0 ; j<nb_op ; j++){
                //Libère les opérateurs si un appel est fini
                if(op[j].fin_appel.heure==horloge.heure && op[j].fin_appel.minute==horloge.minute && op[j].fin_appel.seconde==horloge.seconde){
                    op[j].duree_appel=0;
                }
                if(est_libre(j, op) && q.size!=0){
                    change_statut(op[j], temps_max, temps_min, horloge);
                    client* c = pophead(&q);
                    actualise_info_attente_clientsortant(&tab_attente[i], *c, q, horloge);
                    free(c); //Libère le tas, sinon la mémoire explose sur une grosse simulation
                }
            }
            actualise_taille_attente(&tab_attente[i], q, horloge, heure_deb, heure_fin);
            add_sec(&horloge);
        }
        tab_attente[i].taille_moy = tab_attente[i].taille_moy/((heure_fin-heure_deb)*3600);
    }
    for (int i = 0; i<nb_jours ; i++){
        ajoutedonneesjournee(i+1, tab_attente[i], nomfichier, heure_deb, heure_fin);
    }
}

void menu()
{
    bool quitter=true;
    while (quitter)
    {
        printf("\n");
        printf("menu :\n");
        printf("\n");
        printf("tapez 1 pour modifier le nombre d'operateur\n");
        printf("tapez 2 pour modifier lamdba de la loi exponentielle\n");
        printf("tapez 3 pour modifier l'heure de debut de travail\n");
        printf("tapez 4 pour modifier l'heure de fin de travail\n");
        printf("tapez 5 pour modifier la duree minimale d'un appel\n");
        printf("tapez 6 pour modifier la duree maximale d'un appel\n");
        printf("tapez 7 pour modifier la duree en jours de la simulation\n");
        printf("tapez 8 pour demarrer la simulation\n");
        printf("tapez 9 pour quitter\n");
        printf("\n");
        int num;
        scanf("%d",&num);
        printf("\n");
        // Default settings
        int nb_operateur = 1;
        // ce lambda correspond en moyenne à un client toutes les 12 minutes
        double lambda = 1.0/(60.0);
        int heure_debut = 8;
        int heure_fin = 19 ;
        Temps temps_min = {.heure = 0, .minute = 3, .seconde = 0};
        Temps temps_max = {.heure = 0, .minute = 15, .seconde = 0};
        int nb_jours = 1;
        switch(num)
        {
            case 1:
                printf("Choisir le nombre d'operateur\n\n");
                scanf("%d",&nb_operateur);
                printf("\nLe nombre d'operateur a ete actualise a %d\n", nb_operateur);
                break;
            case 2:
                printf("Choisir la valeur de lamdba\n\n");
                scanf("%lf",&lambda);
                printf("\nLambda vaut maintenant %lf\n",lambda);
                break;
            case 3:
                printf("Choisir la nouvelle heure de debut de journee\n\n");
                scanf("%d",&heure_debut);
                init_clock_deb(heure_debut);
                printf("\nLa journee debute a %dh\n",heure_debut);
                break;
            case 4:
                printf("Choisir la nouvelle heure de fin de journee\n");
                printf("\n");
                scanf("%d",&heure_fin);
                printf("\n");
                init_clock_fin(heure_fin);
                printf("La journee se termine a %dh\n",heure_fin);
                break;
            case 5:
                printf("Choisir le nouveau temps minimal pour un appel (heure,minute,seconde)\n");
                printf("\n");
                scanf("%d",&temps_min.heure);
                printf("\n");
                scanf("%d",&temps_min.minute);
                printf("\n");
                scanf("%d",&temps_min.seconde);
                printf("\n");
                break;
            case 6:
                printf("Choisir le nouveau temps maximal pour un appel (heure,minute,seconde)\n");
                printf("\n");
                scanf("%d",&temps_max.heure);
                printf("\n");
                scanf("%d",&temps_max.minute);
                printf("\n");
                scanf("%d",&temps_max.seconde);
                printf("\n");
                break;
            case 7 :
                printf("Choisir le nombre de jours\n");
                printf("\n");
                scanf("%d",&nb_jours);
                printf("\n");
                break;
            case 8 :
                simulation(heure_debut, heure_fin, nb_jours, nb_operateur, lambda, temps_min, temps_max, "test.txt");
                quitter=false;
                break;
            case 9 :
                quitter=false;
                break;
            default :
                break;
        }
    }
}

int main(){
    srand(time(NULL));
    menu();

    // double lambda = 1.0/(12.0*60.0);
    // int cpt = 0;
    // attente a ;
    // queue q=createqueue();
    // Temps t = init_clock_deb(0);
    // for (int i =0; i<3600; i++){
    //     if(explaw(lambda)<1.0){
    //             char* nom = generenom();
    //             addtail(&q,nom,t);
    //             actualise_info_attente_cliententrant(&a, q);
    //             ajouteclient(*q.tail, i+1, "testajouteclient.txt");
    //             free(nom);
    //             cpt++;
    //     }
    //     add_sec(&t);
    // }
    // printf("%d\n", cpt);
    // printlist(q);

    // queue q=createqueue();
    // attente a ;
    // Temps t = init_clock_deb(0);
    // for (int i =0; i<30000; i++){
    //     add_sec(&t);
    // }
    // printf("%d %d %d", t.heure, t.minute, t.seconde);
    // char* nom = generenom();
    // addtail(&q,nom,t);
    // printf(" %s", q.tail->nom);
    // actualise_info_attente_cliententrant(&a, q);
    // ajouteclient(*q.tail, 1, "testajouteclient.txt");
    // free(nom);

    // addtail(&q, "JeanMichMich",t);
    // addtail(&q, "Jean Culetamaire",t);
    // addtail(&q, "Jean neymar",t);
    // printlist(q);
    // pophead(&q);
    // printlist(q);
    // pophead(&q);
    // printlist(q);
    // pophead(&q);
    // printlist(q);
    // pophead(&q);
    return 0 ;
}
