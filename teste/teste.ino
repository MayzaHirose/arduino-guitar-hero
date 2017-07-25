#include <LedControl.h>
#include <LiquidCrystal.h>

//PINS MATRIZ
#define QTD_MODULOS 1
#define DIN_MATRIZ  2
#define CS_MATRIZ   3
#define CLK_MATRIZ  4

//PINS LCD
#define RS      13
#define ENABLE  12
#define D4      11
#define D5      10
#define D6      9
#define D7      8

//PINS LED, BUZZER E BUTTON
#define BTN_BRANCO  5 //coluna 1
#define BTN_AZUL    6 //coluna 3
#define BTN_VERM    7 //coluna 6
#define LED_VERDE   14 //ANALOGICO
#define LED_VERM    15 //ANALOGICO
#define BZR_ACERTO  16 //ANALOGICO
#define BZR_ERRO    17 //ANALOGICO
#define SPEAKER     19 //ANALOGICO

#define COL_BTN_BRANCO 1
#define COL_BTN_AZUL   3
#define COL_BTN_VERM   6
  
LedControl MATRIZ(DIN_MATRIZ,CLK_MATRIZ,CS_MATRIZ, QTD_MODULOS);
LiquidCrystal LCD(RS, ENABLE, D4, D5, D6, D7);

//melodia do MARIO THEME
int melodia[] = {660,660,660,510,660,770,380,510,380,320,440,480,
                 450,430,380,660,760,860,700,760,660,520,580,480,
                 510,380,320,440,480,450,430,380,660,760,860,700,
                 760,660,520,580,480,500,760,720,680,620,650,380,
                 430,500,430,500,570,500,760,720,680,620,650,1020,
                 1020,1020,380,500,760,720,680,620,650,380,430,500,
                 430,500,570,585,550,500,380,500,500,500,500,760,720,
                 680,620,650,380,430,500,430,500,570,500,760,720,680,
                 620,650,1020,1020,1020,380,500,760,720,680,620,650,380,
                 430,500,430,500,570,585,550,500,380,500,500,500,500,500,
                 500,500,580,660,500,430,380,500,500,500,500,580,660,870,
                 760,500,500,500,500,580,660,500,430,380,660,660,660,510,660,770,380};

//duraçao de cada nota
int duracaodasnotas[] = {100,100,100,100,100,100,100,100,100,100,100,80,
                         100,100,100,80,50,100,80,50,80,80,80,80,100,100,
                         100,100,80,100,100,100,80,50,100,80,50,80,80,80,
                         80,100,100,100,100,150,150,100,100,100,100,100,
                         100,100,100,100,100,150,200,80,80,80,100,100,100,
                         100,100,150,150,100,100,100,100,100,100,100,100,100,
                         100,100,100,100,100,100,100,100,150,150,100,100,100,
                         100,100,100,100,100,100,100,150,200,80,80,80,100,100,
                         100,100,100,150,150,100,100,100,100,100,100,100,100,100,
                         100,100,100,100,60,80,60,80,80,80,80,80,80,60,80,60,80,80,
                         80,80,80,60,80,60,80,80,80,80,80,80,100,100,100,100,100,100,100};

int pausadepoisdasnotas[] ={150,300,300,100,300,550,575,450,400,500,300,330,150,300,
                            200,200,150,300,150,350,300,150,150,500,450,400,500,300,
                            330,150,300,200,200,150,300,150,350,300,150,150,500,300,
                            100,150,150,300,300,150,150,300,150,100,220,300,100,150,
                            150,300,300,300,150,300,300,300,100,150,150,300,300,150,
                            150,300,150,100,420,450,420,360,300,300,150,300,300,100,
                            150,150,300,300,150,150,300,150,100,220,300,100,150,150,
                            300,300,300,150,300,300,300,100,150,150,300,300,150,150,
                            300,150,100,420,450,420,360,300,300,150,300,150,300,350,
                            150,350,150,300,150,600,150,300,350,150,150,550,325,600,
                            150,300,350,150,350,150,300,150,600,150,300,300,100,300,550,575};

//CONTAGEM REGRESSIVA
const uint64_t CONTAGEM[] = {
  0x3c66603860663c00,
  0x7e060c3060663c00,
  0x7e1818181c181800
};
const int CONTAGEM_LEN = sizeof(CONTAGEM)/8;

const uint64_t JOGO_1[] = {
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
  0x00000000000000ff
};
const int JOGO_1_LEN = sizeof(JOGO_1)/8;

const int TEMPO_MAX_BOTAO = 200;

String frase = "5 JOGOS EM 1 - SELECIONE O JOGO - ";
int btn_branco, btn_azul, btn_verm; 
int acertos, erros, seqMax, seqAtual;
int jogoSelecionado = 1;
int opcaoEsq = 0;
int opcaoDir = 0;
int comecar = 0;

void setup(){
  //Serial.begin(9600);

  LCD.begin(16, 2);
  inicializaPins();
  inicializaMatriz();
  inicializaLCD(frase);
  inicializaPlacarJogo();
}

void loop(){
  frase = frase.substring(1,frase.length()) + frase.substring(0,1);
  inicializaLCD(frase);
  displayMenu(jogoSelecionado);
  delay(200);
  opcaoEsq = digitalRead(BTN_BRANCO);
  opcaoDir = digitalRead(BTN_VERM);
  comecar = digitalRead(BTN_AZUL);
  
  if(!opcaoEsq){
    if(jogoSelecionado > 1){
      jogoSelecionado--;
    }
  }
  if(!opcaoDir){
    if(jogoSelecionado < 5){
      jogoSelecionado++;
    }
  }
    
  if(!comecar){
    displayContagemRegressiva();
    iniciaJogo(jogoSelecionado);
    jogoSelecionado = 1;
    displayResultado();
    delay(5000);
    inicializaMatriz();
    inicializaLCD(frase);
    inicializaPlacarJogo();
  }
}

void inicializaPins(){
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERM, OUTPUT);
  pinMode(BZR_ERRO,OUTPUT); 
  pinMode(BZR_ACERTO,OUTPUT); 
  pinMode(BTN_VERM,INPUT); 
  pinMode(BTN_AZUL,INPUT); 
  pinMode(BTN_BRANCO,INPUT);
}

void inicializaMatriz(){
  MATRIZ.clearDisplay(0);
  MATRIZ.shutdown(0, false);
  MATRIZ.setIntensity(0, 1);
}

void inicializaLCD(String texto){ 
  LCD.setCursor(0,0);
  LCD.print(texto);
}

void inicializaPlacarJogo(){
  acertos = 0;
  erros = 0;
  seqMax = 0;
  seqAtual = 0;
}

void displayMenu(int jogo){
  LCD.setCursor(0,1);
  switch(jogo){
    case 1:
      LCD.print(">1  2  3  4  5");
      break;
    case 2:
      LCD.print(" 1 >2  3  4  5");
      break;
    case 3:
      LCD.print(" 1  2 >3  4  5");
      break;
    case 4:
      LCD.print(" 1  2  3 >4  5");
      break;
    case 5:
      LCD.print(" 1  2  3  4 >5");
      break;
  }
}

void displayContagemRegressiva(){
  for(int i=0;i<CONTAGEM_LEN;i++){
    displayImagemMatriz(CONTAGEM[i]);
    delay(900);
  } 
}

void iniciaJogo(int jogo){
  switch(jogo){
    case 1:
      jogo1();
      break;
    case 2:
      jogo2();
      break;
    case 3:
      jogo3();
      break;
    case 4:
      jogo4();
      break;
    case 5:
      jogo5();
      break;
    default:
      jogo1();
      break;
  }
}

void jogo1(){
  boolean acertou = false;
  int nota = 0;
  int index = -1;
  
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Placar:");
  
  for(int i=0;i<156;i++){
    index++;
    if(index == 18) index=8;
    atualizaLCD();
    displayImagemMatriz(JOGO_1[index]);
    
    if(i<8){
      tone(BZR_ACERTO, melodia[nota],duracaodasnotas[nota]);
      //delay(pausadepoisdasnotas[nota]-TEMPO_MAX_BOTAO);
    }
    delay(pausadepoisdasnotas[nota]);
    //delay(TEMPO_MAX_BOTAO); 
    btn_branco = digitalRead(BTN_BRANCO); 
    btn_azul = digitalRead(BTN_AZUL); 
    btn_verm = digitalRead(BTN_VERM);
 
    if(btn_branco == LOW){ //ARRUMAR, O PUSHBUTTON TA LENDO SEMPRE INVERTIDO
      acertou = verificaAcerto(1, JOGO_1[index]);
      if(acertou){
        tone(BZR_ACERTO, melodia[nota],duracaodasnotas[nota]);
        //delay(pausadepoisdasnotas[nota]);
      }
      atualizaPlacar(acertou);
      acendeLedErroOuAcerto(acertou);
    }
    if(btn_azul == LOW){ //ARRUMAR, O PUSHBUTTON TA LENDO SEMPRE INVERTIDO
      acertou = verificaAcerto(2, JOGO_1[index]);
      if(acertou){
        tone(BZR_ACERTO, melodia[nota],duracaodasnotas[nota]);
        //delay(pausadepoisdasnotas[nota]);
      }
      atualizaPlacar(acertou);
      acendeLedErroOuAcerto(acertou);
    }
    if(btn_verm == LOW){ //ARRUMAR, O PUSHBUTTON TA LENDO SEMPRE INVERTIDO
      acertou = verificaAcerto(3, JOGO_1[index]);
      if(acertou){
        tone(BZR_ACERTO, melodia[nota],duracaodasnotas[nota]);
        //delay(pausadepoisdasnotas[nota]);
      }
      atualizaPlacar(acertou);
      acendeLedErroOuAcerto(acertou);
    }
    nota++;
  }
  noTone(BZR_ACERTO);
}

void jogo2(){
  
}

void jogo3(){
  
}

void jogo4(){
  
}

void jogo5(){
  
}

void displayImagemMatriz(uint64_t imagem) {
  for (int i = 0; i < 8; i++) {
    byte linha = (imagem >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      MATRIZ.setLed(0, i, j, bitRead(linha, j));
    }
  }
}

boolean verificaAcerto(int botao, uint64_t estado){
  byte linha;
  switch(botao){
    case 1: //botao branco, coluna 1
      linha = (estado >> 7 * 8) & 0xFF; //verifica coluna correspondente da ultima linha
      return bitRead(linha, COL_BTN_BRANCO);
      break;
    case 2: //botao azul, coluna 3
      linha = (estado >> 7 * 8) & 0xFF; //verifica coluna correspondente da ultima linha
      return bitRead(linha, COL_BTN_AZUL);
      break;
    case 3: //botao vermelho, coluna 6
      linha = (estado >> 7 * 8) & 0xFF; //verifica coluna correspondente da ultima linha
      return bitRead(linha, COL_BTN_VERM);
      break;
  }
}

void atualizaPlacar(boolean acertou){
  if(acertou){
    acertos++;
    seqAtual++;
    if(seqMax < seqAtual){
      seqMax = seqAtual;
    }
  } else {
    erros++;
    if(seqMax < seqAtual){
      seqMax = seqAtual;
    }
    seqAtual = 0;
  }
}

void atualizaLCD(){
  LCD.setCursor(0, 1);
  LCD.print("Pts:");
  LCD.print(acertos);
  
  LCD.setCursor(8, 1);
  LCD.print("Seq:");
  LCD.print(seqAtual);
}

void acendeLedErroOuAcerto(boolean isAcerto){
  if(isAcerto){
    digitalWrite(LED_VERDE, HIGH);
    delay(200);
    digitalWrite(LED_VERDE, LOW); 
  } else {
    digitalWrite(LED_VERM, HIGH);
    delay(200);
    digitalWrite(LED_VERM, LOW); 
  }
}

void displayResultado(){
  LCD.clear();
  LCD.setCursor(0,0); 
  LCD.print("Seu placar: ");
  delay(300);
 
  for(int i=15;i>=0;i--){
    LCD.setCursor(i,1); 
    LCD.print("P: ");
    delay(50);
  }

  for(int i=15;i>=2;i--){
    LCD.setCursor(i,1); 
    LCD.print(acertos);
    LCD.print(" ");
    delay(50);
  }

  for(int i=15;i>=5;i--){
    LCD.setCursor(i,1); 
    LCD.print("E: ");
    delay(50);
  }

  for(int i=15;i>=7;i--){
    LCD.setCursor(i,1); 
    LCD.print(erros);
    LCD.print(" ");
    delay(50);
  }

  for(int i=15;i>=10;i--){
    LCD.setCursor(i,1); 
    LCD.print("S: ");
    delay(50);
  }

  for(int i=15;i>=12;i--){
    LCD.setCursor(i,1); 
    LCD.print(seqMax);
    LCD.print(" ");
    delay(50);
  }
}

