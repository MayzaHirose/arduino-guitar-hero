int ANIMDELAY = 0;  // animation delay, deafault value is 100
int INTENSITYMIN = 0; // minimum brightness, valid range [0,15]
int INTENSITYMAX = 8; // maximum brightness, valid range [0,15]

int DIN_PIN = 2;      // data in pin
int CS_PIN = 3;       // load (CS) pin
int CLK_PIN = 4;      // clock pin

int lin1 = 1;
int lin2 = 1;
int lin3 = 1;
//int led = 6;
int led = 15;
//const int pinBuz = 5; //Buzzer Pin
const int pinBuz = 14; //Buzzer Pin
const int pinVerm = 5; // Push-button vermelhor
const int pinAzul = 6; // Push-button azul
const int pinBranco = 7; // Push-button branco

// MAX7219 registers
byte MAXREG_DECODEMODE = 0x09;
byte MAXREG_INTENSITY  = 0x0a;
byte MAXREG_SCANLIMIT  = 0x0b;
byte MAXREG_SHUTDOWN   = 0x0c;
byte MAXREG_DISPTEST   = 0x0f;

const unsigned char guitar[] =
{
  B00000000,
  B10000000,
  B11000000,
  B10100000,
  B10010000,
  B10001000,
  B10000100,
  B10000010,
  B10000001  
};

//B01000010, B01111111, B01000000, B00000000, B00000000, // 1
//B01100010, B01010001, B01001001, B01000110, B00000000, // 2
//B00100010, B01000001, B01001001, B00110110, B00000000, // 3

void setup() {
  pinMode(DIN_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  pinMode(led, OUTPUT);

  Serial.begin(9600); // Opens Serial communication
  pinMode(pinBuz,OUTPUT); //Defines pinBuz as an Output
  pinMode(pinVerm,INPUT); // Defines pinSwi as an input
  pinMode(pinAzul,INPUT); // Defines pinSwi as an input
  pinMode(pinBranco,INPUT); // Defines pinSwi as an input

  // initialization of the MAX7219
  setRegistry(MAXREG_SCANLIMIT, 0x07);
  setRegistry(MAXREG_DECODEMODE, 0x00);  // using an led matrix (not digits)
  setRegistry(MAXREG_SHUTDOWN, 0x01);    // not in shutdown mode
  setRegistry(MAXREG_DISPTEST, 0x00);    // no display test
  setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMIN);

    // draw hearth
  setRegistry(1, guitar[1]);
  setRegistry(2, guitar[1]);
  setRegistry(3, guitar[1]);
  setRegistry(4, guitar[1]);
  setRegistry(5, guitar[1]);
  setRegistry(6, guitar[1]);
  setRegistry(7, guitar[1]);
  setRegistry(8, guitar[1]);
}

void loop() {
  int botaoVerm, botaoAzul, botaoBranco; // To save the last logic state of the button
  botaoVerm = digitalRead(pinVerm); //Put the reading value of the switch on botao
  botaoAzul = digitalRead(pinAzul); //Put the reading value of the switch on botao
  botaoBranco = digitalRead(pinBranco); //Put the reading value of the switch on botao
  
  setRegistry(1, guitar[lin1]);
  delay(100);
  if(botaoVerm == 1){
    if(lin1 == 8){
    digitalWrite(pinBuz,1);
    digitalWrite(led, HIGH); 
    delay(150);
    digitalWrite(pinBuz, 0);
    digitalWrite(led, LOW); 
    }
  }
  delay(ANIMDELAY);
  setRegistry(4, guitar[lin2]);
  delay(100);
  if((botaoAzul == 1) && (lin2 == 8)){
    digitalWrite(pinBuz,1);
    digitalWrite(led, HIGH); 
    delay(150);
    digitalWrite(pinBuz, 0);
    digitalWrite(led, LOW); 
  }
  delay(ANIMDELAY);
  setRegistry(8, guitar[lin3]);
  delay(100);
  if((botaoBranco == 1) && (lin3 == 8)){
    digitalWrite(pinBuz,1);
    digitalWrite(led, HIGH); 
    delay(150);
    digitalWrite(pinBuz, 0);
    digitalWrite(led, LOW); 
  }
  delay(ANIMDELAY);
  
  //Serial.println(botaoVerm); //Shows the logic state of the input on Serial Monitor
  /*if((botaoVerm == 1) && (lin1 == 8)){
    digitalWrite(pinBuz,1);
    digitalWrite(led, HIGH); 
    delay(150);
    digitalWrite(pinBuz, 0);
    digitalWrite(led, LOW); 
  }else{
    digitalWrite(pinBuz,0);
  }*/

  /*if((botaoAzul == 1) && (lin2 == 8)){
    digitalWrite(pinBuz,1);
    digitalWrite(led, HIGH); 
    delay(150);
    digitalWrite(pinBuz, 0);
    digitalWrite(led, LOW); 
  }else{
    digitalWrite(pinBuz,0);
  }*/

  /*if((botaoBranco == 1) && (lin3 == 8)){
    digitalWrite(pinBuz,1);
    digitalWrite(led, HIGH); 
    delay(150);
    digitalWrite(pinBuz, 0);
    digitalWrite(led, LOW); 
  }//else{
    //digitalWrite(pinBuz,0);
  //}*/
  if(lin1==8) lin1 = 0;
  if(lin2==8) lin2 = 0;
  if(lin3==8) lin3 = 0;
  lin1++;
  lin2++;
  lin3++;

  delay(10);
  
  /*if (botaoVerm == 1){ // Pressed button, logic State HIGH (5V)
     if(lin1==8) {
        digitalWrite(pinBuz,1);
        digitalWrite(led, HIGH); 
        delay(150);
     } //Switch pressed, buzzer on
     digitalWrite(led, LOW); 
  }else{
    digitalWrite(pinBuz,0); //If the switch isn’t pressed, buzzer off.
  }
  delay(10); //reading delay
  if(lin1==8)
      lin1 = 0;
  lin1++;*/
}

void setRegistry(byte reg, byte value)
{
  digitalWrite(CS_PIN, LOW);

  putByte(reg);   // specify register
  putByte(value); // send data

  digitalWrite(CS_PIN, LOW);
  digitalWrite(CS_PIN, HIGH);
}

void putByte(byte data)
{
  byte i = 8;
  byte mask;
  while (i > 0)
  {
    mask = 0x01 << (i - 1);        // get bitmask
    digitalWrite( CLK_PIN, LOW);   // tick
    if (data & mask)               // choose bit
      digitalWrite(DIN_PIN, HIGH); // send 1
    else
      digitalWrite(DIN_PIN, LOW);  // send 0
    digitalWrite(CLK_PIN, HIGH);   // tock
    --i;                           // move to lesser bit
  }
}
