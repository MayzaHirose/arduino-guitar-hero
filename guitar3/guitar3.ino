#include <LedControl.h>
#include <LiquidCrystal.h>
#include "pin_config.h"
#include "mario_music.h"
#include "matriz_jogo1.h"

#define COL_BTN_BRANCO  1
#define COL_BTN_AZUL    3
#define COL_BTN_VERM    6
#define TEMPO_MAX_BOTAO 200
#define TEMPO_LED       100
  
LedControl MATRIZ(DIN_MATRIZ,CLK_MATRIZ,CS_MATRIZ, QTD_MODULOS);
LiquidCrystal LCD(RS, ENABLE, D4, D5, D6, D7);

//CONTAGEM REGRESSIVA
const uint64_t CONTAGEM[] = {
  0x3c66603860663c00,
  0x7e060c3060663c00,
  0x7e1818181c181800
};
const int CONTAGEM_LEN = sizeof(CONTAGEM)/8;

String frase = "5 JOGOS EM 1 - SELECIONE O JOGO - ";
int btn_branco, btn_azul, btn_verm; 
int acertos, erros, seqMax, seqAtual;
int jogoSelecionado = 1;

void setup(){
  //Serial.begin(9600);

  LCD.begin(16, 2);
  inicializaPins();
  inicializaMatriz();
  inicializaPlacarJogo();
}

void loop(){
  frase = frase.substring(1,frase.length()) + frase.substring(0,1);
  displayMenu(frase,jogoSelecionado);
  delay(300);
 
  if(!digitalRead(BTN_BRANCO)){
    if(jogoSelecionado > 1){
      jogoSelecionado--;
    }
  }
  else if(!digitalRead(BTN_VERM)){
    if(jogoSelecionado < 5){
      jogoSelecionado++;
    }
  }    
  else if(!digitalRead(BTN_AZUL)){
    displayContagemRegressiva();
    iniciaJogo(jogoSelecionado);
    jogoSelecionado = 1;
    inicializaMatriz();
    inicializaPlacarJogo();
  }
}

void inicializaPins(){
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERM, OUTPUT);
  pinMode(BZR_ERRO,OUTPUT); 
  pinMode(BZR_RESERVA,OUTPUT);  
  pinMode(SPEAKER,OUTPUT); 
  pinMode(BTN_BRANCO,INPUT);
  pinMode(BTN_ROSA,INPUT); 
  pinMode(BTN_AZUL,INPUT);
  pinMode(BTN_VERM,INPUT); 
}

void inicializaMatriz(){
  MATRIZ.clearDisplay(0);
  MATRIZ.shutdown(0, false);
  MATRIZ.setIntensity(0, 1);
}

void inicializaPlacarJogo(){
  acertos = 0;
  erros = 0;
  seqMax = 0;
  seqAtual = 0;
}

void displayMenu(String texto, int jogo){
  LCD.setCursor(0,0);
  LCD.print(texto);
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
  LCD.clear();
  LCD.setCursor(0,0);
  LCD.print("PREPARAR.");
  LCD.setCursor(0,1);
  for(int i=0;i<CONTAGEM_LEN;i++){
    LCD.setCursor(i+9,0);
    LCD.print(".");
    displayImagemMatriz(CONTAGEM[i]);
    delay(900);
  } 
  LCD.setCursor(0,1);
  LCD.print("GO!!!");
  delay(1000);
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
  boolean acertou = true;
  int nota = 0;
  int index = 0;
  int pause = 0;
  
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Placar:");
  LCD.setCursor(0, 1);
  LCD.print("Pts:");
  LCD.print(acertos);
  
  LCD.setCursor(8, 1);
  LCD.print("Seq:");
  LCD.print(seqAtual);

  displayImagemMatriz(JOGO_1[index]);
  delay(pausadepoisdasnotas[nota]);
  
  for(int i=0;i<156;i++){
    
    btn_branco = digitalRead(BTN_BRANCO); 
    btn_azul = digitalRead(BTN_AZUL); 
    btn_verm = digitalRead(BTN_VERM);
 
    if(btn_branco == LOW){ //ARRUMAR, O PUSHBUTTON TA LENDO SEMPRE INVERTIDO
      acertou = verificaAcertou(1, JOGO_1[index]);
      if(!acertou){
        tone(BZR_ERRO, 262, 200);
        pause = pausadepoisdasnotas[nota] - TEMPO_LED;
        if(pause < 0) {pause=0;}
        delay(pause);
      } 
      atualizaPlacar(acertou);
      acendeLedErroOuAcerto(acertou);
    }
    else if(btn_azul == LOW){ //ARRUMAR, O PUSHBUTTON TA LENDO SEMPRE INVERTIDO
      acertou = verificaAcertou(2, JOGO_1[index]);
      if(!acertou){
        tone(BZR_ERRO, 262, 200);
        pause = pausadepoisdasnotas[nota] - TEMPO_LED;
        if(pause < 0) {pause=0;}
        delay(pause);
      } 
      atualizaPlacar(acertou);
      acendeLedErroOuAcerto(acertou);
    }
    else if(btn_verm == LOW){ //ARRUMAR, O PUSHBUTTON TA LENDO SEMPRE INVERTIDO
      acertou = verificaAcertou(3, JOGO_1[index]);
      if(!acertou){
        tone(BZR_ERRO, 262, 200);
        pause = pausadepoisdasnotas[nota] - TEMPO_LED;
        if(pause < 0) {pause=0;}
        delay(pause);
      } 
      atualizaPlacar(acertou);
      acendeLedErroOuAcerto(acertou);
    }
    if(index == 17){index=8;} else {index++;}
    
    if(acertou) {
      tone(SPEAKER, melodia[nota],duracaodasnotas[nota]);
      displayImagemMatriz(JOGO_1[index]);
      delay(pausadepoisdasnotas[nota]);
    } else {displayImagemMatriz(JOGO_1[index]); acertou=true;}

    nota++;
  }
  noTone(SPEAKER);
  noTone(BZR_ERRO);
  while(index < JOGO_1_LEN){
    displayImagemMatriz(JOGO_1[index]);
    delay(300);
    index++;
  }
  displayResultado();
  delay(5000);  
}

void displayImagemMatriz(uint64_t imagem) {
  for (int i = 0; i < 8; i++) {
    byte linha = (imagem >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      MATRIZ.setLed(0, i, j, bitRead(linha, j));
    }
  }
}

boolean verificaAcertou(int botao, uint64_t estado){
  byte linha = (estado >> 7 * 8) & 0xFF; //verifica coluna correspondente da ultima linha
  switch(botao){
    case 1: //botao branco, coluna 1
      return bitRead(linha, COL_BTN_BRANCO);
      break;
    case 2: //botao azul, coluna 3
      return bitRead(linha, COL_BTN_AZUL);
      break;
    case 3: //botao vermelho, coluna 6
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
  LCD.setCursor(4, 1);
  LCD.print(acertos);
  
  LCD.setCursor(12, 1);
  LCD.print(seqAtual);
}

void acendeLedErroOuAcerto(boolean acertou){
  if(acertou){
    digitalWrite(LED_VERDE, HIGH);
    delay(TEMPO_LED);
    digitalWrite(LED_VERDE, LOW); 
  } else {
    digitalWrite(LED_VERM, HIGH);
    delay(TEMPO_LED);
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

void jogo2(){
  int matriz[8][8];
  boolean bateu = false;
  int direcao = 3; // ESQ=1 CIMA=2 DIR=3 BAIXO=4
  int delei = 300;
  int row,col;
  
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Placar:");
  LCD.setCursor(0, 1);
  LCD.print("Pts:");
  LCD.print(acertos);
  
  LCD.setCursor(8, 1);
  LCD.print("Tam:");
  LCD.print(seqMax);

  //Ponto Inicial
  //matriz[rand()%8][rand()%8] = 1;
  matriz[1][0] = 1;
  row = 1;
  col = 0;
  //Poe comida
  //matriz[rand()%8][rand()%8] = 1;
  matriz[2][6] = 1;
  //bateu = andar(&matriz[8][8], direcao);
  
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      MATRIZ.setLed(0, i, j, matriz[i][j]);
    }
  }
  switch(direcao){
    case: 1 //esquerda
      if(col != 0){ //se nao estiver na parede esq ou coluna 0
        matriz[row][col] = 0;
        matriz[row][col-1] = 1;    
      }else {
        bateu = true;
      }
      break;
    case: 2 //cima
      if(row != 0){ //se nao estiver na parede esq ou coluna 0
        matriz[row][col] = 0;
        matriz[row-1][col] = 1;    
      }else {
        bateu = true;
      }
      break;
    case: 3 //direita
      if(col != 7){ //se nao estiver na parede esq ou coluna 0
        matriz[row][col] = 0;
        matriz[row][col+1] = 1;    
      }else {
        bateu = true;
      }
      break;
    case: 4 //baixo
      if(row != 7){ //se nao estiver na parede esq ou coluna 0
        matriz[row][col] = 0;
        matriz[row+1][col] = 1;    
      }else {
        bateu = true;
      }
      break;
  }
  matriz[
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      MATRIZ.setLed(0, i, j, matriz[i][j]);
    }
  }
  
  while(!bateu){
      
  }
}

boolean andar(int *matriz, int direcao){
  inicializaMatriz();
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      MATRIZ.setLed(0, i, j, *matriz[i][j]);
    }
  }
}

void comer(){
  
}

void jogo3(){
  
}

void jogo4(){
  
}

void jogo5(){
  
}


