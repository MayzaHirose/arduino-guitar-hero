#include <LedControl.h>
#include <LiquidCrystal.h>

#define din  2
#define cs   3
#define clk  4

const int ledAcerto = 14;
const int ledErro = 15;
const int pinBuzAcerto = 16;
const int pinBuzErro = 17; //Buzzer Pin
const int pinVerm = 7; // Push-button vermelho coluna 6
const int pinAzul = 6; // Push-button azul coluna 3
const int pinBranco = 5; // Push-button branco columa 1
int botaoVerm, botaoAzul, botaoBranco; 
int contadorJogo = 0;
  
LedControl ledCtrl = LedControl(din,clk,cs, 1);
//LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal LCD(13, 12, 11, 10, 9, 8);

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

const byte FIM_MUSICA_1[][8] = {
{
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
const int FIM_LEN = sizeof(FIM_MUSICA_1)/8;
/*const byte MUSICA_1[][8] = {
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
}};*/
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
  B00010000,
  B01000000,
  B00000010,
  B00010000,
  B00010000,
  B01000000,
  B00010000
},{
  B11111111,
  B00000010,
  B00010000,
  B01000000,
  B00000010,
  B00010000,
  B00010000,
  B01000000
},{
  B11111111,
  B01000000,
  B00000010,
  B00010000,
  B01000000,
  B00000010,
  B00010000,
  B00010000
},{
  B11111111,
  B00000010,
  B01000000,
  B00000010,
  B00010000,
  B01000000,
  B00000010,
  B00010000
},{
  B11111111,
  B00010000,
  B00000010,
  B01000000,
  B00000010,
  B00010000,
  B01000000,
  B00000010
},{
  B11111111,
  B01000000,
  B00010000,
  B00000010,
  B01000000,
  B00000010,
  B00010000,
  B01000000
}};
const int MUSICA_1_LEN = sizeof(MUSICA_1)/8;

//melodia do MARIO THEME
int melodia[] = {660,660,660,510,660,770,380,510,380,320,440,480,450,430,380,660,760,860,700,760,660,520,580,480,510,380,320,440,480,450,430,380,660,760,860,700,760,660,520,580,480,500,760,720,680,620,650,380,430,500,430,500,570,500,760,720,680,620,650,1020,1020,1020,380,500,760,720,680,620,650,380,430,500,430,500,570,585,550,500,380,500,500,500,500,760,720,680,620,650,380,430,500,430,500,570,500,760,720,680,620,650,1020,1020,1020,380,500,760,720,680,620,650,380,430,500,430,500,570,585,550,500,380,500,500,500,500,500,500,500,580,660,500,430,380,500,500,500,500,580,660,870,760,500,500,500,500,580,660,500,430,380,660,660,660,510,660,770,380};
int duracaodasnotas[] = {100,100,100,100,100,100,100,100,100,100,100,80,100,100,100,80,50,100,80,50,80,80,80,80,100,100,100,100,80,100,100,100,80,50,100,80,50,80,80,80,80,100,100,100,100,150,150,100,100,100,100,100,100,100,100,100,100,150,200,80,80,80,100,100,100,100,100,150,150,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,150,150,100,100,100,100,100,100,100,100,100,100,150,200,80,80,80,100,100,100,100,100,150,150,100,100,100,100,100,100,100,100,100,100,100,100,100,60,80,60,80,80,80,80,80,80,60,80,60,80,80,80,80,80,60,80,60,80,80,80,80,80,80,100,100,100,100,100,100,100};
int pausadepoisdasnotas[] ={150,300,300,100,300,550,575,450,400,500,300,330,150,300,200,200,150,300,150,350,300,150,150,500,450,400,500,300,330,150,300,200,200,150,300,150,350,300,150,150,500,300,100,150,150,300,300,150,150,300,150,100,220,300,100,150,150,300,300,300,150,300,300,300,100,150,150,300,300,150,150,300,150,100,420,450,420,360,300,300,150,300,300,100,150,150,300,300,150,150,300,150,100,220,300,100,150,150,300,300,300,150,300,300,300,100,150,150,300,300,150,150,300,150,100,420,450,420,360,300,300,150,300,150,300,350,150,350,150,300,150,600,150,300,350,150,150,550,325,600,150,300,350,150,350,150,300,150,600,150,300,300,100,300,550,575};             
int nota = 0;

void setup() {
  int i = 0;
  //Serial.begin(9600);
  pinMode(ledAcerto, OUTPUT);
  pinMode(ledErro, OUTPUT);
  pinMode(pinBuzErro,OUTPUT); 
  pinMode(pinBuzAcerto,OUTPUT); 
  pinMode(pinVerm,INPUT); 
  pinMode(pinAzul,INPUT); 
  pinMode(pinBranco,INPUT);
  
  ledCtrl.shutdown(0, false);
  ledCtrl.setIntensity(0, 1);
  ledCtrl.clearDisplay(0);
  
  LCD.begin(16, 2);
  LCD.print("hello, world!");
  LCD.begin(16, 2);
  LCD.print("hello, world!");
  
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
  LCD.setCursor(0, 1);
  LCD.print(millis() / 1000);
  //for(int i=0;i<MUSICA_1_LEN;i++){
  if(contadorJogo < MUSICA_1_LEN){
    displayJogo(MUSICA_1[contadorJogo]); 
    if(contadorJogo > 6){
      if(nota<156){
        tone(pinBuzAcerto, melodia[nota],duracaodasnotas[nota]);
        delay(pausadepoisdasnotas[nota]);
        nota++;
      } else {
        noTone(pinBuzAcerto);
      }
    } else {delay(300);} 
    botaoVerm = digitalRead(pinVerm); //Put the reading value of the switch on botao
    botaoAzul = digitalRead(pinAzul); //Put the reading value of the switch on botao
    botaoBranco = digitalRead(pinBranco); //Put the reading value of the switch on botao
    //if(contadorJogo > 6){
      //if(nota<156){
       // delay(pausadepoisdasnotas[nota]);
        if(botaoVerm == 1){
          if(bitRead(MUSICA_1[contadorJogo][7], 1)){
            digitalWrite(ledAcerto, HIGH); 
            //delay(150);
            tone(pinBuzAcerto, melodia[nota],duracaodasnotas[nota]);
            digitalWrite(ledAcerto, LOW); 
          } else{
            //digitalWrite(pinBuzErro,1);
            digitalWrite(ledErro, HIGH); 
            delay(100);
            //digitalWrite(pinBuzErro,0);
            digitalWrite(ledErro, LOW); 
          }
        }
        if(botaoAzul == 1){
          if(bitRead(MUSICA_1[contadorJogo][7], 4)){
            digitalWrite(ledAcerto, HIGH); 
            //delay(150);
            tone(pinBuzAcerto, melodia[nota],duracaodasnotas[nota]);
            digitalWrite(ledAcerto, LOW); 
          } else{
            //digitalWrite(pinBuzErro,1);
            digitalWrite(ledErro, HIGH); 
            delay(100);
            //digitalWrite(pinBuzErro,0);
            digitalWrite(ledErro, LOW);
          }
        } 
        if(botaoBranco == 1){
          if(bitRead(MUSICA_1[contadorJogo][7], 6)){
            digitalWrite(ledAcerto, HIGH); 
            //delay(150);
            tone(pinBuzAcerto, melodia[nota],duracaodasnotas[nota]);
            digitalWrite(ledAcerto, LOW); 
          }else {
            //digitalWrite(pinBuzErro,1);
            digitalWrite(ledErro, HIGH); 
            delay(100);
            //digitalWrite(pinBuzErro,0);
            digitalWrite(ledErro, LOW);
          }
        }
        //nota++;
        //contadorJogo++; 
      //}else {
       // noTone(pinBuzAcerto);
      //}
    //}else {delay(300);}
    //nota++;
    contadorJogo++; 
  } else{
    if(nota <156)
      contadorJogo = 7; //para ficar o loop
    else {
      //if(!(contadorJogo < MUSICA_1_LEN)){ //para a matriz terminar de descer antes de começar a musica novamente
        for(int i=0;i<FIM_LEN;i++){
          displayJogo(FIM_MUSICA_1[i]); 
        }
        contadorJogo = 0;
        nota = 0;
      //} else {contadorJogo++;}
    }
  }
}
