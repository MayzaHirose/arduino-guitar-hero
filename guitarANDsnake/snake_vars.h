#define ESQ    1 //Direcao esquerda
#define CIMA   2 //Direcao cima
#define DIR   -1 //Direcao direita
#define BAIXO -2 //Direcao Baixo
#define COMIDA 3 //Flag Comida
#define EASY   200
#define MEDIUM 100
#define HARD   25
#define EXPERT 10

//Variáveis Snake
int matriz[8][8] = {0};
int tamSnake = 2;
int direcaoAtual = ESQ;
int proximaDirecao = ESQ;
int cabecaX = 0;
int cabecaY = 0;
int proximoX = 0;
int proximoY = 0;
int nivel = EXPERT;
boolean bateu = false;
boolean comeu = false;
