#define COL_BTN_BRANCO  1   //Coluna do botao branco
#define COL_BTN_AZUL    3   //Coluna do botao azul
#define COL_BTN_VERM    6   //Coluna do botao vermelho
#define TEMPO_MAX_BOTAO 200 //Tempo de espera antes de ler o botao
#define TEMPO_LED       100 //Tempo de led acesa

const uint64_t JOGO_1[] PROGMEM = {
  0x00000000000000ff,
  0x00000000000008ff,
  0x00000000000840ff,
  0x00000000084002ff,
  0x00000008400240ff,
  0x00000840024008ff,
  0x00084002400802ff,
  0x08400240080208ff,
  0x40024008020808ff,
  0x02400802080840ff,
  0x40080208084002ff,
  0x08020808400208ff,
  0x02080840020840ff,
  0x08084002084002ff,
  0x08400208400240ff,
  0x40020840024008ff,
  0x02084002400802ff,
  0x08400240080208ff,
  0x40024008020800ff,
  0x02400802080000ff,
  0x40080208000000ff,
  0x08020800000000ff,
  0x08000000000000ff,
  0x00000000000000ff
};
const int JOGO_1_LEN = sizeof(JOGO_1)/8;

//Melodia do Tema do Mario por http://www.nubiasouza.com.br/musica-tema-mario-arduino-speaker/

const int melodia[] PROGMEM = {660,660,660,510,660,770,380,510,380,320,440,480,450,430,380,660,760,860,700,760,660,520,580,480,510,380,320,440,480,450,430,380,660,760,860,700,
                       760,660,520,580,480,500,760,720,680,620,650,380,430,500,430,500,570,500,760,720,680,620,650,1020,1020,1020,380,500,760,720,680,620,650,380,430,500,
                       430,500,570,585,550,500,380,500,500,500,500,760,720,680,620,650,380,430,500,430,500,570,500,760,720,680,620,650,1020,1020,1020,380,500,760,720,680,
                       620,650,380,430,500,430,500,570,585,550,500,380,500,500,500,500,500,500,500,580,660,500,430,380,500,500,500,500,580,660,870,760,500,500,500,500,580,
                       660,500,430,380,660,660,660,510,660,770,380};

//Duracao de cada nota
const int duracaodasnotas[] PROGMEM = {100,100,100,100,100,100,100,100,100,100,100,80,100,100,100,80,50,100,80,50,80,80,80,80,100,100,100,100,80,100,100,100,80,50,100,80,50,80,80,80,
                               80,100,100,100,100,150,150,100,100,100,100,100,100,100,100,100,100,150,200,80,80,80,100,100,100,100,100,150,150,100,100,100,100,100,100,100,100,
                               100,100,100,100,100,100,100,100,100,150,150,100,100,100,100,100,100,100,100,100,100,150,200,80,80,80,100,100,100,100,100,150,150,100,100,100,100,
                               100,100,100,100,100,100,100,100,100,60,80,60,80,80,80,80,80,80,60,80,60,80,80,80,80,80,60,80,60,80,80,80,80,80,80,100,100,100,100,100,100,100};

//Pausa depois das notas
const int pausadepoisdasnotas[] PROGMEM ={150,300,300,100,300,550,575,450,400,500,300,330,150,300,200,200,150,300,150,350,300,150,150,500,450,400,500,300,330,150,300,200,200,150,300,
                                  150,350,300,150,150,500,300,100,150,150,300,300,150,150,300,150,100,220,300,100,150,150,300,300,300,150,300,300,300,100,150,150,300,300,150,
                                  150,300,150,100,420,450,420,360,300,300,150,300,300,100,150,150,300,300,150,150,300,150,100,220,300,100,150,150,300,300,300,150,300,300,300,
                                  100,150,150,300,300,150,150,300,150,100,420,450,420,360,300,300,150,300,150,300,350,150,350,150,300,150,600,150,300,350,150,150,550,325,600,
                                  150,300,350,150,350,150,300,150,600,150,300,300,100,300,550,575};
