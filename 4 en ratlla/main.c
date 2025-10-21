//
//  main.c
//  4 en ratlla
//
//  Created by Domènec Huerta Estradé on 30/9/25.
//

#include "main.h"
#define N 8
#define terminal 1
typedef struct node{
    struct node* *fills;
    int n_fills;
    int nivell;
    int last_move;
    char **tauler;
    float qualitat;
}Node;

int CalcNumDeFills(char **tauler){
    //donat un tauler retorna quates tirades possibles hi ha, de fet en el fons aquesta funció no serveix de res!
    int fills =0;
    for (int i=0; i<N; i++){
        if (tauler[0][i]== '0') fills++;
    }
    return fills;
}
void printNodeTree(Node *arrel){
    //Printeja l'arbre sencer sota d'un node donat recursivament
    for (int i=0; i < arrel->nivell ; i++){
        printf("   ");
    }
    printf("%d : %f\n",arrel->last_move, arrel->qualitat);
    for (int i=0;i<arrel->n_fills; i++) printNodeTree(arrel->fills[i]);
    return;
}

char **copyTauler(char ** original){
    //Retorna un tauler copia d'un ja existent!
    char **copia = malloc(N*sizeof(char*));
    for (int i=0; i<N;i++){
        copia[i] = malloc(sizeof(char)*N);
        for(int j=0; j<N; j++) copia[i][j]=original[i][j];
    }
    return copia;
}

void freeTauler(char **tauler){
    for(int i =0; i<N; i++){
        free(tauler[i]);
    }
    free(tauler);
}
void copyWord (char* string, const char* word,int start){
    //Copies a certain text in the given position of a string by its starting value (with const char)
    for (int i =0; i <strlen(word); i++){
        string[start+i] = word[i];
        
    }
    return;
}
const char* colorPalette(int num) {
    //for a player number it returns the text to print its
    if (num == '1') return "\x1b[31m■\x1b[37m "; // red █ però també mola amb ■
    if (num == '2') return "\x1b[34m■\x1b[37m "; // blue
    return "\x1b[37m■\x1b[37m ";               // default white
}

void deleteLines(int lineNum){
    //deletes lineNum lines from the terminal
    char *del ="\033[1A\033[2K";//terminal code to delete a line
    int delSize = sizeof(del);
    char*text = malloc(delSize*lineNum*sizeof(char));
    for(int i=0; i<lineNum; i++){
        copyWord(text, del, delSize*i);
    }
    printf("%s", text);
    free(text);
}
char** zerosArray(void){
    //retorna un array de caracters 0
    char **array = malloc(sizeof(char*)*N);
    for(int i=0;i<N; i++){
        array[i]= malloc(sizeof(char)*N);
    }
    
    for(int i = 0; i<N; i++){
        for(int j = 0; j<N; j++){
            array[i][j]='0';
        }
    }
    return array;
}

void printTauler(char **tauler){
    //Printeja per la terminal l'estat actual de la paartida esborrant el torn anterior
    int tokenSize = sizeof(colorPalette(0));
    int textSize =sizeof(char) *(4*N+N*(5*N+1)) + tokenSize*(N*N+1+N)*sizeof(char);
    int textCounter =0;
    int fila = N;
    char *text= malloc(textSize);
    while (1){
        copyWord(text, "\x1b[37m", textCounter);
        textCounter +=5;
        text[textCounter++]='0'+fila;
        text[textCounter++]= ' ';
        for (int i =0; i<N; i++){
            copyWord(text, colorPalette(tauler[N-fila][i]), textCounter);
            textCounter +=tokenSize;
            text[textCounter++]= ' ';
        }
        text[textCounter++]='\n';
        if (--fila ==0) break;
    }
    copyWord(text, "\x1b[37m", textCounter);
    textCounter +=5; //Aquest 5 m'ha portat mals de cap, el \x1b és interpretat per c com un sol caràcter que indica que això serà un codi ansi.
    text[textCounter++] = ' ';
    for (int i =0; i<N; i++){
        text[textCounter++] = ' ';
        text[textCounter++] = '1' + i;
    }
    printf("\n%s\n", text);
    free(text);
    return;
}

char **tirada(char jugador, char **tauler, int columna){
    //Donada la columna a la que decideix tirar un jugador, afegeix la seva fitxa
    char ** nouTauler = copyTauler(tauler);
    for (int i =N-1; i>=0; i--){
        if (nouTauler[i][columna] == '0'){
            nouTauler[i][columna] = jugador;
            return nouTauler;
        }
    }
    return nouTauler;
}

int direccioGuanyadora(char jugador, int x, int y, int dirx, int diry, char**tauler){
    //fixada una de les 4 possibles direccions, mira si hi ha 4 en ratlla en aquella direcció
    int llargada = 1;
    while(1){ //Troba la fitxa mes llunyana connexa en aquella direcció en el sentit donat
        x = x+dirx;
        y = y+diry;
        if (x <0 ||x>=N || y>=N||y<0||tauler[y][x]!=jugador) break;
        }
    while (llargada<5){ //Comprova en el sentit contrari si n'hi ha 4 seguides
        x = x-dirx;
        y = y-diry;
        if (x<0 ||x>=N ||y>=N||y<0){ //Important també comprovar que el segment no surti del tauler
            return 0;
        }
        if (tauler[y][x]!=jugador ){
            return 0;
        }
        llargada++;
    }
    return 1;
}

int winCondition(char jugador, char **tauler, char columna){
    //Comprova si amb la tirada que acaba de fer el jugador ha guanyat la partida, cas el qual retorna 1
    //L'algorisme consisteix en donar la volta per les 8 posiscions adjacents a la fitxa colocada i mirar si hi ha una seqüència de 4 del matexi jugador
    int fila;
    for (fila =0; fila<N;fila++){
        if (tauler[fila][columna]==jugador){
            break;
        }
    }
    for(int i=-1; i<=1; i++){
        for(int j =0; j<=1; j++){
            if((j!=0 ||i!=0 )&!(i==1&j==0)){
                if (direccioGuanyadora(jugador, columna, fila, i, j, tauler)==1){
                    return 1;
                }
            }
        }
    }
    return 0;
}
int tornJugador(char jugador, char ***tauler, int *torn, char mode){
    //Incorpora tots els pasos del torn d'un jugador
    //comença esborrant la jugada anterior si es pertinent
    int i =0;
    char c;
    int columna_triada;
    printf("Jugador %c,  a quina columna vols col·locar la teva propera fitxa?", jugador);
    while(1){
        columna_triada = 0;
        c = 'a';
        scanf("%d", &columna_triada);
        while (c != '\n')c = getchar();
        if(columna_triada>0&columna_triada<=N&(*tauler)[0][columna_triada-1] =='0'){ //Comprova que la tirada escollida pel jugador sigui vàlida
            break;
        }
        else{
            i++;
            printf("Aquesta jugada no és vàlida, quina vols fer?");
        }
    }
    if (mode ==1) deleteLines(N+3+i); //Elimina el text del torn anterior
    fflush(stdout);
    char **nouTauler = tirada(jugador, *tauler, columna_triada-1); // A la columna triada li restem 1 per tenir bé la numeració
    freeTauler(*tauler);
    *tauler = nouTauler;
    printTauler(*tauler);
    *torn = *torn+1;
    return winCondition(jugador, *tauler, columna_triada-1); //Comprova si el jugador ha guanyat amb aquesta ronda
}


Node* CreateNode(char** tauler, int nivell, int columna){
    Node* newNode = malloc(sizeof(Node));
    newNode->tauler=tauler;
    newNode->nivell=nivell;
    newNode->qualitat=0;
    newNode->last_move=columna;
    return newNode;
}


void buildTree (Node* arrel, int profunditat, char jugador, char enemic){
    //Construeix un arbre amb totes les tirades portant a partides possibles dels dos jugadors. L'arrel ha de tenir definida la profunditat, que indica quants passos més es vol baixar i el tauler de la partida
    if (profunditat == 0)
        return;
    int numDeFills =CalcNumDeFills(arrel->tauler);
    arrel->n_fills= numDeFills;
    arrel->fills = malloc(sizeof(Node*)*numDeFills);
    int aux = 0; //Variable auxiliar per anar contant quants fills s'han afegit
    for (int columna =0 ; columna<N ; columna++) {
        if ((arrel->tauler)[0][columna]=='0'){
            (arrel->fills)[aux]= CreateNode(tirada(jugador, arrel->tauler, columna), arrel->nivell+1, columna); //Afegeix el node com a fill
            //Comprova si la partida s'acaba directament en aquest torn i en tal cas assigna la qualitat o amplia l'arbre.
            if (winCondition(jugador, (arrel->fills)[aux]->tauler, (arrel->fills)[aux]->last_move)== 1 ){
                (arrel->fills)[aux]->qualitat = (float) ('1'-jugador+0.5)*2;
                (arrel->fills)[aux++]->n_fills =0; //La arrel aquesta ja no té fills pq la partida no segueix x aquí!!!!!
            }
            else buildTree((arrel->fills)[aux++], profunditat-1, enemic, jugador); //Fixar-se que en un dels dos casos ha d'acabar sumant 1 a aux, pel que ens va explicar el Vicens l'altre dia això es pot posar als arguments del for.
        }
    }
    return;
}

void destroyTree(Node* arrel){
    freeTauler(arrel->tauler);
    for (int i=0; i< arrel->n_fills; i++){
        destroyTree((arrel->fills)[i]);
    }
    if (arrel->n_fills!=0) free(arrel->fills); //Important alliberar aquest array d'apuntadors cap a els fills a part
    free(arrel);
}

void HumaVsHuma(char**partida, char mode){
    int torn = 0;
    while(1){
        if (tornJugador('1', &partida, &torn, mode)==1){
            printf("Felicitats jugador 1, has guanyat!\n");
            break;
        }
        if (tornJugador('2', &partida, &torn, mode)==1){
            printf("Felicitats jugador 2, has guanyat!\n");
            break;
        }
        if (torn >= N*N-1){
            printf("Ben jugat tots dos, heu empatat!");
            break;
        }
    }
    return;
}

float MaxQuali(Node arrel){
    float quali =-1.;
    for(int i=0; i< arrel.n_fills; i++){
        if (arrel.fills[i]->qualitat >quali) quali =arrel.fills[i]->qualitat;
    }
    return quali;
}
float MinQuali(Node arrel){
    float quali =1;
    for(int i=0; i< arrel.n_fills; i++){
        if (arrel.fills[i]->qualitat <quali) quali =arrel.fills[i]->qualitat;
    }
    return quali;
}
int nephewsNum(Node pare){
    int nephews =0;
    for (int i=0;i<pare.n_fills; i++ ){
        nephews += pare.fills[i]->n_fills;
    }
    return nephews;
}
void minMax(Node *pare,char maquina, int profunditat){
    //Comprova si el pare té nets per si està ben completat el minMax, si té nets fa el minMax dels fills i així iterativament
    if (nephewsNum(*pare)!=0){
        for (int i=0; i<pare->n_fills; i++){
            minMax(pare->fills[i], maquina, profunditat-1);
        }
    }
    if (profunditat%2==0) pare->qualitat =MaxQuali(*pare);
    if (profunditat%2==1) pare->qualitat =MinQuali(*pare);
    return;
}
void evaluateTree (Node* arrel, int profunditat, char jugador, char enemic){
//Evalua la qualitat de tots els nodes de l'arbre assignant 1 si es guanya la partida i -1 si es perd la partida. I altres mètodes heurístics que he de decidir de moment no fa res.
    for(int i=0; i<arrel->n_fills; i++){
        evaluateTree((arrel->fills)[i], profunditat-1, jugador, enemic);
    }
    return;
}
int triaJugada(Node arrel){
    //Amb la quali del pare, tria quina és la jugada que ha de fer la màquina, si n'hi ha d'empatades en tria una d'aleatoria
    int repeticions=0;
    for(int i=0; i<arrel.n_fills; i++){
        if(arrel.fills[i]->qualitat==arrel.qualitat) repeticions++;
    }
    repeticions += -rand()%repeticions;
    for(int i=0; i<arrel.n_fills; i++){
        if(arrel.fills[i]->qualitat==arrel.qualitat) repeticions--;
        if (repeticions==0) return arrel.fills[i]->last_move;
    }
    return -1;
}
int tornMaquina(char maquina, char enemic, char ***tauler, int *torn, int profunditat, char mode){
    //Decideix quina jugada fa la màquina i la fa, comença construint l'arbre de jugades. Aquí el jugador és l'enemic!
    Node *pare = CreateNode(*tauler, 0, -1);
    buildTree(pare, profunditat, maquina, enemic);
    //printNodeTree(pare);
    //Evaluate l'Arbre donant-li els valors que sigui als nodes de qualitat
    //Evaluationniidnvvkrjn fer per millorar el programa ja que només està posat el hard-choice!!
    //Utilitza l'algorisme del MinMax per veure quin punt és el millor
    //printNodeTree(pare);
    minMax(pare, maquina, profunditat);
    //printNodeTree(pare);
    //Si hi ha jugades empatades en puntuació en tria una d'aleatoria
    int jugadaOptima = triaJugada(*pare);
    //Actualitza la partida amb la jugada de la màquina
    char ** nouTauler = tirada(maquina, *tauler, jugadaOptima);
    destroyTree(pare);
    *tauler = nouTauler;
    printf("Calculant la jugada de la màquina...");
    fflush(stdout);
    sleep(1);
    if(mode ==1) deleteLines(N+2); //Elimina el text del torn anterior, espera una estona i printeja la jugada de la màquina
    printTauler(*tauler);
    fflush(stdout);   //Per asegurar que es printeja bé la cosa.
    return winCondition(maquina, *tauler,jugadaOptima); //Comprova si la maquina ha guanyat amb aquesta ronda
}

void HumaVsMaquina(char**partida, int profunditat, char mode){
    int torn = 0;
    while(1){
        if (tornJugador('1', &partida, &torn, mode)==1){
            printf("Feliciats, has guanyat!\n");
            break;
        }
        if (tornMaquina('2','1',&partida, &torn, profunditat, mode)==1){
            printf("Brrrr Brrr Brrrr, torna-ho a intentar!\n");
            break;
        }
        if (torn >= N*N-1){
            printf("Ets un digne adversari, estem igualats");
            break;
        }
    }
    return;
}
int main(void) {
    //inicialització de la partida
    char modeSilencions = 1;
    srand( (int) time(NULL));
    char **partida = zerosArray();
    int profunditat = 6;
    printTauler(partida);
    //HumaVsHuma(partida, modeSilencios);
    HumaVsMaquina(partida, profunditat, modeSilencions);
    return 0;
}
