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

#define COL_BTN_BRANCO 1
#define COL_BTN_AZUL   3
#define COL_BTN_VERM   6
  
LedControl MATRIZ(DIN_MATRIZ,CLK_MATRIZ,CS_MATRIZ, QTD_MODULOS);
LiquidCrystal LCD(RS, ENABLE, D4, D5, D6, D7);

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
  0x08020808400200ff,
  0x02080840020000ff,
  0x08084002000000ff,
  0x08400200000000ff,
  0x40020000000000ff,
  0x02000000000000ff,
  0x00000000000000ff
};
const int JOGO_1_LEN = sizeof(JOGO_1)/8;

const int TEMPO_MAX_BOTAO = 200;

int btn_branco, btn_azul, btn_verm; 
int acertos, erros, sequencia;
int jogoSelecionado = 1;
int contadorJogo;

void setup(){
  //Serial.begin(9600);
  inicializaPins();
  inicializaMatriz();
  inicializaLCD();
  inicializaPlacarJogo();
}

void loop(){
  if(jogoSelecionado != 0){
    displayContagemRegressiva();
    iniciaJogo(jogoSelecionado);
    jogoSelecionado = 0;
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

void inicializaLCD(){
  LCD.begin(16, 2);
  LCD.print("> 3 JOGOS EM 1 <");
}

void inicializaPlacarJogo(){
  acertos = 0;
  erros = 0;
  sequencia = 0;
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
    default:
      jogo1();
      break;
  }
}

void jogo1(){
  boolean acertou = false;
  int seqMaxima = 0;
  
  for(int i=0;i<JOGO_1_LEN;i++){
    atualizaLCD();
    displayImagemMatriz(JOGO_1[i]);
    delay(TEMPO_MAX_BOTAO); 
    btn_branco = digitalRead(BTN_BRANCO); 
    btn_azul = digitalRead(BTN_AZUL); 
    btn_verm = digitalRead(BTN_VERM);
 
    if(btn_branco == LOW){ //ARRUMAR, O PUSHBUTTON TA LENDO SEMPRE INVERTIDO
      acertou = verificaAcerto(1, JOGO_1[i]);
      seqMaxima = atualizaPlacar(acertou, seqMaxima);
      acendeLedErroOuAcerto(acertou);
    }
    if(btn_azul == LOW){ //ARRUMAR, O PUSHBUTTON TA LENDO SEMPRE INVERTIDO
      acertou = verificaAcerto(2, JOGO_1[i]);
      seqMaxima = atualizaPlacar(acertou, seqMaxima);
      acendeLedErroOuAcerto(acertou);
    }
    if(btn_verm == LOW){ //ARRUMAR, O PUSHBUTTON TA LENDO SEMPRE INVERTIDO
      acertou = verificaAcerto(3, JOGO_1[i]);
      seqMaxima = atualizaPlacar(acertou, seqMaxima);
      acendeLedErroOuAcerto(acertou);
    }
  }
}

void jogo2(){
  
}

void jogo3(){
  
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

int atualizaPlacar(boolean acertou, int seqMax){
  if(acertou){
    acertos++;
    if(sequencia < seqMax){
      sequencia = seqMax + 1;
    }
    return seqMax++;
  } else {
    erros++;
    if(sequencia < seqMax){
      sequencia = seqMax;
    }
    return seqMax = 0;
  }
}

void atualizaLCD(){
  LCD.setCursor(0, 1);
  LCD.print("Pt:");
  LCD.print(acertos);
  LCD.setCursor(5, 1);
  LCD.print("E:");
  LCD.print(erros);
  LCD.setCursor(10, 1);
  LCD.print("S:");
  LCD.print(sequencia);
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

