#define ESQ    1 //Direcao esquerda
#define CIMA   2 //Direcao cima
#define DIR   -1 //Direcao direita
#define BAIXO -2 //Direcao Baixo
#define COMIDA 3 //Flag Comida
#define EASY   200
#define MEDIUM 150
#define HARD   125
#define EXPERT 100

//Variáveis Snake
int matriz[8][8] = {0};
int tamSnake = 2;
int direcaoAtual = ESQ;
int proximaDirecao = ESQ;
int proximoX = 0;
int proximoY = 0;
int nivel;
boolean bateu = false;
boolean comeu = false;

typedef struct{
  int x;
  int y;
} Corpo;

Corpo corpo[64];

