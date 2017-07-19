#include <LedControl.h>
 
#define din  2
#define cs   3
#define clk  4

const int led = 15;
const int pinBuz = 14; //Buzzer Pin
const int pinVerm = 5; // Push-button vermelho coluna 6
const int pinAzul = 6; // Push-button azul coluna 3
const int pinBranco = 7; // Push-button branco columa 1
int botaoVerm, botaoAzul, botaoBranco; 
  
LedControl ledCtrl = LedControl(din,clk,cs, 1);

const byte CONTAGEM[][8] = {
{
  B00000000,
  B00111100,
  B00000100,
  B00011100,
  B00011100,
  B00000100,
  B00111100,
  B00000000
},{
  B00000000,
  B00111100,
  B00100100,
  B00001100,
  B00011000,
  B00110000,
  B00111100,
  B00000000
},{
  B00000000,
  B00011000,
  B00101000,
  B00001000,
  B00001000,
  B00001000,
  B00111100,
  B00000000
}};
const int CONTAGEM_LEN = sizeof(CONTAGEM)/8;

const byte MUSICA_1[][8] = {
{
  B11111111,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
},{
  B11111111,
  B00010000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
},{
  B11111111,
  B00000010,
  B00010000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
},{
  B11111111,
  B01000000,
  B00000010,
  B00010000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
},{
  B11111111,
  B00000010,
  B01000000,
  B00000010,
  B00010000,
  B00000000,
  B00000000,
  B00000000
},{
  B11111111,
  B00010000,
  B00000010,
  B01000000,
  B00000010,
  B00010000,
  B00000000,
  B00000000
},{
  B11111111,
  B01000000,
  B00010000,
  B00000010,
  B01000000,
  B00000010,
  B00010000,
  B00000000
},{
  B11111111,
  B00010000,
  B01000000,
  B00010000,
  B00000010,
  B01000000,
  B00000010,
  B00010000
},{
  B11111111,
  B00010000,
  B00010000,
  B01000000,
  B00010000,
  B00000010,
  B01000000,
  B00000010
},{
  B11111111,
  B00000010,
  B00010000,
  B00010000,
  B01000000,
  B00010000,
  B00000010,
  B01000000
},{
  B11111111,
  B01000000,
  B00000010,
  B00010000,
  B00010000,
  B01000000,
  B00010000,
  B00000010
},{
  B11111111,
  B00000000,
  B01000000,
  B00000010,
  B00010000,
  B00010000,
  B01000000,
  B00010000
},{
  B11111111,
  B00000000,
  B00000000,
  B01000000,
  B00000010,
  B00010000,
  B00010000,
  B01000000
},{
  B11111111,
  B00000000,
  B00000000,
  B00000000,
  B01000000,
  B00000010,
  B00010000,
  B00010000
},{
  B11111111,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B01000000,
  B00000010,
  B00010000
},{
  B11111111,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B01000000,
  B00000010
},{
  B11111111,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B01000000
},{
  B11111111,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
}};
const int MUSICA_1_LEN = sizeof(MUSICA_1)/8;


void setup() {
  int i = 0;
  //Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(pinBuz,OUTPUT); 
  pinMode(pinVerm,INPUT); 
  pinMode(pinAzul,INPUT); 
  pinMode(pinBranco,INPUT);
  
  ledCtrl.shutdown(0, false);
  ledCtrl.setIntensity(0, 10);
  ledCtrl.clearDisplay(0);
  while(i<3){
    displayContador(CONTAGEM[i]);
    i++;
    delay(900);
  }  
}

void displayContador(const byte* contador) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      ledCtrl.setLed(0, i, j, bitRead(contador[i], 7 - j));
    }
  }
}

void displayJogo(const byte* jogo) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      ledCtrl.setLed(0, i, j, bitRead(jogo[i], 7 - j));
    }
  }
}

void loop() {
  for(int i=0;i<MUSICA_1_LEN;i++){
    displayJogo(MUSICA_1[i]);  
    delay(300);
    botaoVerm = digitalRead(pinVerm); //Put the reading value of the switch on botao
    botaoAzul = digitalRead(pinAzul); //Put the reading value of the switch on botao
    botaoBranco = digitalRead(pinBranco); //Put the reading value of the switch on botao
    if(botaoVerm == 1){
      if(bitRead(MUSICA_1[i][7], 1)){
      digitalWrite(pinBuz,1);
      digitalWrite(led, HIGH); 
      delay(150);
      digitalWrite(pinBuz, 0);
      digitalWrite(led, LOW); 
      }
    }
    if((botaoAzul == 1) && (bitRead(MUSICA_1[i][7], 4))){
      digitalWrite(pinBuz,1);
      digitalWrite(led, HIGH); 
      delay(150);
      digitalWrite(pinBuz, 0);
      digitalWrite(led, LOW); 
    }
    if((botaoBranco == 1) && (bitRead(MUSICA_1[i][7], 6))){
      digitalWrite(pinBuz,1);
      digitalWrite(led, HIGH); 
      delay(150);
      digitalWrite(pinBuz, 0);
      digitalWrite(led, LOW); 
    }
  }
}
