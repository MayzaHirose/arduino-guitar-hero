#include <LedControl.h>
#include <LiquidCrystal.h>
#include "pin_config.h"
#include "guitar_vars.h"
#include "snake_vars.h"
#include "global_vars.h"

LedControl MATRIZ(DIN_MATRIZ,CLK_MATRIZ,CS_MATRIZ, QTD_MODULOS);
LiquidCrystal LCD(RS, ENABLE, D4, D5, D6, D7);

void setup() {
  Serial.begin(9600);

  inicializaModulos();
  zeraMatriz();
  zeraLCD();
  zeraPlacar();
}

void loop() {
  millisExecucao = 300;
  millisCorrente = millis();    //Tempo atual em ms
  if(millisCorrente - millisAnterior >= millisExecucao){
    millisAnterior = millisCorrente;
    menuFrase = menuFrase.substring(1,menuFrase.length()) + menuFrase.substring(0,1);
    displayFrase();
  }
  displayMenu();
  
  if(!digitalRead(BTN_BRANCO)){
    if(jogoSelecionado == 2){
      --jogoSelecionado;
    }
  }
  else if(!digitalRead(BTN_VERM)){
    if(jogoSelecionado == 1){
      ++jogoSelecionado;
    }
  }    
  else if(!digitalRead(BTN_ROSA) || !digitalRead(BTN_AZUL)){
    iniciaJogo();
    jogoSelecionado = 1;
    zeraMatriz();
    zeraPlacar();
  }
}

void inicializaModulos(){
  LCD.begin(16,2);
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

void zeraMatriz(){
  MATRIZ.clearDisplay(0);
  MATRIZ.shutdown(0, false);
  MATRIZ.setIntensity(0, 1);
}

void zeraLCD(){
  LCD.clear();
  LCD.setCursor(0,0);
}

void zeraPlacar(){
  acertos = 0;
  erros = 0;
  seqMax = 0;
  seqAtual = 0;
}

void displayFrase(){
  LCD.setCursor(0,0);
  LCD.print(menuFrase);
}

void displayMenu(){
  LCD.setCursor(0,1);
  switch(jogoSelecionado){
    case 1:
      LCD.print(">Guitar   Snake");
      break;
    case 2:
      LCD.print(" Guitar  >Snake");
      break;
  }
}

void displayLevel(int dificuldade){
  LCD.setCursor(0,0);
  LCD.print("Nivel:          ");
  switch(dificuldade){
    case 1:
      nivel = EASY;
      LCD.setCursor(6,0);
      LCD.print("EASY            ");
      LCD.setCursor(0,1);
      LCD.print(">E  M  H  EXP   ");
      break;
    case 2:
      nivel = MEDIUM;
      LCD.setCursor(6,0);
      LCD.print("MEDIUM          ");
      LCD.setCursor(0,1);
      LCD.print(" E >M  H  EXP   ");
      break;
    case 3:
      nivel = HARD;
      LCD.setCursor(6,0);
      LCD.print("HARD            ");
      LCD.setCursor(0,1);
      LCD.print(" E  M >H  EXP   ");
      break;
    case 4:
      nivel = EXPERT;
      LCD.setCursor(6,0);
      LCD.print("EXPERT          ");
      LCD.setCursor(0,1);
      LCD.print(" E  M  H >EXP   ");
      break;
  }
}

void displayContagemRegressiva(){
  uint64_t estado;
  zeraLCD();
  LCD.print("PREPARAR");
  LCD.setCursor(0,1);
  for(int i=0;i<CONTAGEM_LEN;i++){
    LCD.setCursor(i+8,0);
    LCD.print(".");
    memcpy_P(&estado,&CONTAGEM[i],sizeof estado);
    displayImagemMatriz(estado);
    delay(900);
  } 
  LCD.setCursor(0,1);
  LCD.print("GO!!!");
  delay(1000);
}

void displayImagemMatriz(uint64_t imagem) {
  for (int i = 0; i < 8; i++) {
    byte linha = (imagem >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      MATRIZ.setLed(0, i, j, bitRead(linha, j));
    }
  }
}

void iniciaJogo(){
  int dificuldade = 1;
  boolean selecionado = false;
  
  switch(jogoSelecionado){
    case 1:
      displayContagemRegressiva();
      jogo1();
      break;
    case 2:
      while(!selecionado){
        delay(200); //Para nao pega atraso de soltura do botao anterior
        displayLevel(dificuldade);
        if(!digitalRead(BTN_BRANCO)){
            if(dificuldade > 1){
              --dificuldade;
            }
        }else if(!digitalRead(BTN_VERM)){
            if(dificuldade < 4){
              ++dificuldade;
            }
        }else if(!digitalRead(BTN_ROSA) || !digitalRead(BTN_AZUL)){
          dificuldade = 1;
          selecionado = true;
        }    
      }
      displayContagemRegressiva();
      jogo2();
      break;
  }
}

void jogo1(){
  uint64_t estado; //PROGMEM
  int melodiaMus;  //PROGMEM
  int duracaoMus;  //PROGMEM
  int pausaNota;   //PROGMEM
  
  boolean acertou = true;
  int nota = 0;
  int index = 0;
  int pause = 0;
  
  zeraLCD();
  LCD.print("Placar:");
  LCD.setCursor(0, 1);
  LCD.print("Pts:");
  LCD.print(acertos);
  
  LCD.setCursor(8, 1);
  LCD.print("Seq:");
  LCD.print(seqAtual);

  memcpy_P(&estado,&JOGO_1[index],sizeof estado);
  memcpy_P(&melodiaMus,&melodia[nota],sizeof melodiaMus);
  memcpy_P(&duracaoMus,&duracaodasnotas[nota],sizeof duracaoMus);
  memcpy_P(&pausaNota,&pausadepoisdasnotas[nota],sizeof pausaNota);
  
  displayImagemMatriz(estado);
  delay(pausaNota);
  
  for(int i=0;i<156;i++){
    
    btn_branco = digitalRead(BTN_BRANCO); 
    btn_azul = digitalRead(BTN_AZUL); 
    btn_verm = digitalRead(BTN_VERM);
 
    if(btn_branco == LOW){ //ARRUMAR, O PUSHBUTTON TA LENDO SEMPRE INVERTIDO
      acertou = verificaAcertou(1, estado);
      if(!acertou){
        tone(BZR_ERRO, 262, 200);
        pause = pausaNota - TEMPO_LED;
        if(pause < 0) {pause=0;}
        delay(pause);
      } 
      atualizaPlacar(acertou);
      acendeLedErroOuAcerto(acertou);
    }
    else if(btn_azul == LOW){ //ARRUMAR, O PUSHBUTTON TA LENDO SEMPRE INVERTIDO
      acertou = verificaAcertou(2, estado);
      if(!acertou){
        tone(BZR_ERRO, 262, 200);
        pause = pausaNota - TEMPO_LED;
        if(pause < 0) {pause=0;}
        delay(pause);
      } 
      atualizaPlacar(acertou);
      acendeLedErroOuAcerto(acertou);
    }
    else if(btn_verm == LOW){ //ARRUMAR, O PUSHBUTTON TA LENDO SEMPRE INVERTIDO
      acertou = verificaAcertou(3, estado);
      if(!acertou){
        tone(BZR_ERRO, 262, 200);
        pause = pausaNota - TEMPO_LED;
        if(pause < 0) {pause=0;}
        delay(pause);
      } 
      atualizaPlacar(acertou);
      acendeLedErroOuAcerto(acertou);
    }
    if(index == 17){index=8;} else {index++;}
    memcpy_P(&estado,&JOGO_1[index],sizeof estado);
    if(acertou) {
      tone(SPEAKER, melodiaMus,duracaoMus);
      displayImagemMatriz(estado);
      delay(pausaNota);
    } else {displayImagemMatriz(estado); acertou=true;}
    nota++;
    memcpy_P(&melodiaMus,&melodia[nota],sizeof melodiaMus);
    memcpy_P(&duracaoMus,&duracaodasnotas[nota],sizeof duracaoMus);
    memcpy_P(&pausaNota,&pausadepoisdasnotas[nota],sizeof pausaNota);
  }
  noTone(SPEAKER);
  noTone(BZR_ERRO);
  while(index < JOGO_1_LEN){
    displayImagemMatriz(estado);
    delay(300);
    index++;
    memcpy_P(&estado,&JOGO_1[index],sizeof estado);
  }
  displayResultado();
  delay(5000);  
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
  LCD.print("  ");
  
  LCD.setCursor(12, 1);
  LCD.print(seqAtual);
  LCD.print("   ");
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
  int isGo = false;
  
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Placar:");
  LCD.setCursor(0, 1);
  LCD.print("Pts:");
  LCD.print(acertos);
  
  LCD.setCursor(8, 1);
  LCD.print("Tam:");
  LCD.print(tamSnake);

  zerarSnake();
  pontoInicial();
  poeComida();
  exibeSnake();

  while(!isGo){
    if(!digitalRead(BTN_BRANCO)){
      isGo = true;
      proximaDirecao = ESQ;
    } else if(!digitalRead(BTN_ROSA)){
      isGo = true;
      proximaDirecao = CIMA;
    } else if(!digitalRead(BTN_AZUL)){
      isGo = true;
      proximaDirecao = BAIXO;
    } else if(!digitalRead(BTN_VERM)){
      isGo = true;
      proximaDirecao = DIR;
    }
  }
  
  while(!bateu){
    andar();  
    direcaoAtual = proximaDirecao; 
    delay(100);
    
    btn_branco = digitalRead(BTN_BRANCO); 
    btn_rosa = digitalRead(BTN_ROSA); 
    btn_azul = digitalRead(BTN_AZUL); 
    btn_verm = digitalRead(BTN_VERM);
  
    if(btn_branco == LOW){proximaDirecao = ESQ;}
    else if(btn_rosa == LOW){proximaDirecao = CIMA;}
    else if(btn_azul == LOW){proximaDirecao = BAIXO;}
    else if(btn_verm == LOW){proximaDirecao = DIR;}
    
    if(comeu){
      tone(SPEAKER,392,200);
      comeu = false;
      acertos++;
      tamSnake++; 
      poeComida();
      LCD.setCursor(4, 1);
      LCD.print(acertos);
      LCD.setCursor(12, 1);
      LCD.print(tamSnake);
    }
    exibeSnake();
    delay(nivel);
  }
  LCD.clear();
  LCD.setCursor(5, 0);
  LCD.print("BATEU!");
  LCD.setCursor(0, 1);
  LCD.print("Pts:");
  LCD.print(acertos);
  
  LCD.setCursor(8, 1);
  LCD.print("Tam:");
  LCD.print(tamSnake);
  delay(5000);
}

void zerarSnake(){
  tamSnake = 2;
  direcaoAtual = ESQ;
  proximaDirecao = ESQ;
  cabecaX = 0;
  cabecaY = 0;
  proximoX = 0;
  proximoY = 0;
  bateu = false;
  
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      matriz[i][j] = 0;
    }
  }
}

void pontoInicial(){
  cabecaX = rand()%6+1;
  cabecaY = rand()%6+1;
  matriz[cabecaX][cabecaY] = 1;
  matriz[cabecaX][cabecaY+1] = 1;
}

void poeComida(){
  boolean disponivel = false;
  int row, col;
  do{
    row = rand()%8;
    col = rand()%8;
    if(matriz[row][col] != 1){
      disponivel = true;
      matriz[row][col] = COMIDA;
    }
  }while(!disponivel);
}

void exibeSnake(){
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      MATRIZ.setLed(0, i, j, matriz[i][j]);
    }
  }
}

void andar(){
  boolean cima = false;
  boolean baixo = false;
  boolean esq = false;
  boolean dir = false;
  
  if(tamSnake != 1 && direcaoAtual == ((-1)*proximaDirecao)){proximaDirecao = direcaoAtual; }// Se tentar andar para a diracao contrário ele nao muda nada. Apenas no comeco
  switch(proximaDirecao){
    case ESQ: //esquerda
      if(cabecaY != 0){ //se nao estiver na parede esquerda ou coluna 0
        proximoX = cabecaX;
        proximoY = cabecaY - 1;
        cabecaY = proximoY;
        if(matriz[proximoX][proximoY] == COMIDA){
          comeu = true;
        } else if(matriz[proximoX][proximoY] == 1){ //Caso bata no corpo
          bateu = true;
          return;
        }
        /*if(proximoX == 0){cima=true;}
        else if(proximoX == 7){baixo=true;}
        if(proximoY == 0){esq=true;}
        else if(proximoY == 7){dir=true;}*/
         
        
        //Move a cobrinha
        for(int i=0;i<tamSnake;i++){
          cima = false;
          baixo = false;
          esq = false;
          dir = false;
          if(proximoX == 0){cima=true;}
          else if(proximoX == 7){baixo=true;}
          if(proximoY == 0){esq=true;}
          else if(proximoY == 7){dir=true;}
          // Procura onde ta o corpinho p passar p frente        
          if(!cima /*&& (deOndeVeio != BAIXO)*/){if(matriz[proximoX-1][proximoY] == 1){
                       /*matriz[proximoX][proximoY] = matriz[proximoX-1][proximoY];*/ //ou 1
                       matriz[proximoX][proximoY] = 4;
                       if(!comeu)
                         matriz[proximoX-1][proximoY] = 0;
                       else if(i+1 == tamSnake){
                         matriz[proximoX-1][proximoY] = 1;
                       } 
                       proximoX = proximoX-1; 
                       //deOndeVeio = CIMA;
                       continue;}}
          if(!baixo /*&& (deOndeVeio != CIMA)*/){if(matriz[proximoX+1][proximoY] == 1){
                        /*matriz[proximoX][proximoY] = matriz[proximoX+1][proximoY];*/ //ou 1
                        matriz[proximoX][proximoY] = 4;
                        if(!comeu)
                          matriz[proximoX+1][proximoY] = 0;
                        else if(i+1 == tamSnake){
                          matriz[proximoX+1][proximoY] = 1;
                        }
                        proximoX = proximoX+1;
                        //deOndeVeio = BAIXO;
                        continue;}}
         if(!esq /*&& (deOndeVeio != DIR)*/){if(matriz[proximoX][proximoY-1] == 1){
                      /*matriz[proximoX][proximoY] = matriz[proximoX][proximoY-1];*/ //ou 1
                      matriz[proximoX][proximoY] = 4;
                      if(!comeu)
                        matriz[proximoX][proximoY-1] = 0;
                      else if(i+1 == tamSnake){
                        matriz[proximoX][proximoY-1] = 1;
                      }
                      proximoY = proximoY-1;
                      //deOndeVeio = ESQ;
                      continue;}}
          if(!dir /*&& (deOndeVeio != ESQ)*/){if(matriz[proximoX][proximoY+1] == 1){
                      /*matriz[proximoX][proximoY] = matriz[proximoX][proximoY+1];*/ //ou 1
                      matriz[proximoX][proximoY] = 4;
                      if(!comeu)
                        matriz[proximoX][proximoY+1] = 0;
                      else if(i+1 == tamSnake){
                        matriz[proximoX][proximoY+1] = 1;
                      }
                      proximoY = proximoY+1;
                      //deOndeVeio = DIR;
                      continue;}}   
        }
      }else {
        bateu = true;
      }
      arrumaMatriz();
      break;
      
    case CIMA: //cima
      if(cabecaX != 0){ //se nao estiver na parede cima ou linha 0
        proximoX = cabecaX - 1;
        proximoY = cabecaY;
        cabecaX = proximoX;
        if(matriz[proximoX][proximoY] == COMIDA){
          comeu = true;
        } else if(matriz[proximoX][proximoY] == 1){ //Caso bata no corpo
          bateu = true;
          return;
        }
        /*if(proximoX == 0){cima=true;}
        else if(proximoX == 7){baixo=true;}
        if(proximoY == 0){esq=true;}
        else if(proximoY == 7){dir=true;}*/
               
        //Move a cobrinha
        for(int i=0;i<tamSnake;i++){
          cima = false;
          baixo = false;
          esq = false;
          dir = false;
          if(proximoX == 0){cima=true;}
          else if(proximoX == 7){baixo=true;}
          if(proximoY == 0){esq=true;}
          else if(proximoY == 7){dir=true;}
          // Procura onde ta o corpinho p passar p frente        
          if(!cima /*&& (deOndeVeio != BAIXO)*/){if(matriz[proximoX-1][proximoY] == 1){
                       /*matriz[proximoX][proximoY] = matriz[proximoX-1][proximoY];*/ //ou 1
                       matriz[proximoX][proximoY] = 4;
                       if(!comeu)
                         matriz[proximoX-1][proximoY] = 0;
                       else if(i+1 == tamSnake){
                         matriz[proximoX-1][proximoY] = 1;
                       } 
                       proximoX = proximoX-1; 
                       //deOndeVeio = CIMA;
                       continue;}}
          if(!baixo /*&& (deOndeVeio != CIMA)*/){if(matriz[proximoX+1][proximoY] == 1){
                        /*matriz[proximoX][proximoY] = matriz[proximoX+1][proximoY];*/ //ou 1
                        matriz[proximoX][proximoY] = 4;
                        if(!comeu)
                          matriz[proximoX+1][proximoY] = 0;
                        else if(i+1 == tamSnake){
                          matriz[proximoX+1][proximoY] = 1;
                        }
                        proximoX = proximoX+1;
                        //deOndeVeio = BAIXO;
                        continue;}}
          if(!esq /*&& (deOndeVeio != DIR)*/){if(matriz[proximoX][proximoY-1] == 1){
                      /*matriz[proximoX][proximoY] = matriz[proximoX][proximoY-1]; *///ou 1
                      matriz[proximoX][proximoY] = 4;
                      if(!comeu)
                        matriz[proximoX][proximoY-1] = 0;
                      else if(i+1 == tamSnake){
                        matriz[proximoX][proximoY-1] = 1;
                      }
                      proximoY = proximoY-1;
                      //deOndeVeio = ESQ;
                      continue;}}
          if(!dir /*&& (deOndeVeio != ESQ)*/){if(matriz[proximoX][proximoY+1] == 1){
                      /*matriz[proximoX][proximoY] = matriz[proximoX][proximoY+1];*/ //ou 1
                      matriz[proximoX][proximoY] = 4;
                      if(!comeu)
                        matriz[proximoX][proximoY+1] = 0;
                      else if(i+1 == tamSnake){
                        matriz[proximoX][proximoY+1] = 1;
                      }
                      proximoY = proximoY+1;
                      //deOndeVeio = DIR;
                      continue;}}   
        }
      }else {
        bateu = true;
      }
      arrumaMatriz();
      break;
      
    case DIR: //direita
      if(cabecaY != 7){ //se nao estiver na parede direita ou coluna 7
        proximoX = cabecaX;
        proximoY = cabecaY + 1;
        cabecaY = proximoY;
        if(matriz[proximoX][proximoY] == COMIDA){
          comeu = true;
        } else if(matriz[proximoX][proximoY] == 1){ //Caso bata no corpo
          bateu = true;
          return;
        }
        /*if(proximoX == 0){cima=true;}
        else if(proximoX == 7){baixo=true;}
        if(proximoY == 0){esq=true;}
        else if(proximoY == 7){dir=true;}*/
         
        
        //Move a cobrinha
        for(int i=0;i<tamSnake;i++){
          cima = false;
          baixo = false;
          esq = false;
          dir = false;
          if(proximoX == 0){cima=true;}
          else if(proximoX == 7){baixo=true;}
          if(proximoY == 0){esq=true;}
          else if(proximoY == 7){dir=true;}
          // Procura onde ta o corpinho p passar p frente        
          if(!cima /*&& (deOndeVeio != BAIXO)*/){if(matriz[proximoX-1][proximoY] == 1){
                       /*matriz[proximoX][proximoY] = matriz[proximoX-1][proximoY];*/ //ou 1
                       matriz[proximoX][proximoY] = 4;
                       if(!comeu)
                        matriz[proximoX-1][proximoY] = 0;
                       else if(i+1 == tamSnake){
                        matriz[proximoX-1][proximoY] = 1;
                       }
                       proximoX = proximoX-1; 
                       //deOndeVeio = CIMA;
                       continue;}}
          if(!baixo /*&& (deOndeVeio != CIMA)*/){if(matriz[proximoX+1][proximoY] == 1){
                        /*matriz[proximoX][proximoY] = matriz[proximoX+1][proximoY];*/ //ou 1
                        matriz[proximoX][proximoY] = 4;
                        if(!comeu)
                          matriz[proximoX+1][proximoY] = 0;
                        else if(i+1 == tamSnake){
                          matriz[proximoX+1][proximoY] = 1;
                        }
                        proximoX = proximoX+1;
                        //deOndeVeio = BAIXO;
                        continue;}}
          if(!esq /*&& (deOndeVeio != DIR)*/){if(matriz[proximoX][proximoY-1] == 1){
                      /*matriz[proximoX][proximoY] = matriz[proximoX][proximoY-1];*/ //ou 1
                      matriz[proximoX][proximoY] = 4;
                      if(!comeu)
                        matriz[proximoX][proximoY-1] = 0;
                      else if(i+1 == tamSnake){
                        matriz[proximoX][proximoY-1] = 1;
                      }
                      proximoY = proximoY-1;
                      //deOndeVeio = ESQ;
                      continue;}}
          if(!dir /*&& (deOndeVeio != ESQ)*/){if(matriz[proximoX][proximoY+1] == 1){
                      /*matriz[proximoX][proximoY] = matriz[proximoX][proximoY+1];*/ //ou 1
                      matriz[proximoX][proximoY] = 4;
                      if(!comeu)
                        matriz[proximoX][proximoY+1] = 0;
                      else if(i+1 == tamSnake){
                        matriz[proximoX][proximoY+1] = 1;
                      }
                      proximoY = proximoY+1;
                      //deOndeVeio = DIR;
                      continue;}}   
         
        }
         
      }else {
        bateu = true;
      }
      arrumaMatriz();
      break;
      
    case BAIXO: //baixo
      if(cabecaX != 7){ //se nao estiver na parede baixo ou linha 7
        proximoX = cabecaX + 1;
        proximoY = cabecaY;
        cabecaX = proximoX;
        if(matriz[proximoX][proximoY] == COMIDA){
          comeu = true;
        } else if(matriz[proximoX][proximoY] == 1){ //Caso bata no corpo
          bateu = true;
          return;
        }
        /*if(proximoX == 0){cima=true;}
        else if(proximoX == 7){baixo=true;}
        if(proximoY == 0){esq=true;}
        else if(proximoY == 7){dir=true;}*/
         
        
        
        //Move a cobrinha
        for(int i=0;i<tamSnake;i++){
          cima = false;
          baixo = false;
          esq = false;
          dir = false;
          if(proximoX == 0){cima=true;}
          else if(proximoX == 7){baixo=true;}
          if(proximoY == 0){esq=true;}
          else if(proximoY == 7){dir=true;}
          // Procura onde esta o corpinho para avancar       
          if(!cima /*&& (deOndeVeio != BAIXO)*/){if(matriz[proximoX-1][proximoY] == 1){
                       /*matriz[proximoX][proximoY] = matriz[proximoX-1][proximoY];*/ //ou 1
                       matriz[proximoX][proximoY] = 4;
                       if(!comeu)
                        matriz[proximoX-1][proximoY] = 0;
                       else if(i+1 == tamSnake){
                        matriz[proximoX-1][proximoY] = 1;
                       }
                       proximoX = proximoX-1; 
                       //deOndeVeio = CIMA;
                       continue;}}
          if(!baixo /*&& (deOndeVeio != CIMA)*/){if(matriz[proximoX+1][proximoY] == 1){
                        /*matriz[proximoX][proximoY] = matriz[proximoX+1][proximoY];*/ //ou 1
                        matriz[proximoX][proximoY] = 4;
                        if(!comeu)
                          matriz[proximoX+1][proximoY] = 0;
                        else if(i+1 == tamSnake){
                          matriz[proximoX+1][proximoY] = 1;
                        }
                        proximoX = proximoX+1;
                        //deOndeVeio = BAIXO;
                        continue;}}
          if(!esq /*&& (deOndeVeio != DIR)*/){if(matriz[proximoX][proximoY-1] == 1){
                      /*matriz[proximoX][proximoY] = matriz[proximoX][proximoY-1];*/ //ou 1
                      matriz[proximoX][proximoY] = 4;
                      if(!comeu)
                        matriz[proximoX][proximoY-1] = 0;
                      else if(i+1 == tamSnake){
                        matriz[proximoX][proximoY-1] = 1;
                      }
                      proximoY = proximoY-1;
                      //deOndeVeio = ESQ;
                      continue;}}
          if(!dir /*&& (deOndeVeio != ESQ)*/){if(matriz[proximoX][proximoY+1] == 1){
                      /*matriz[proximoX][proximoY] = matriz[proximoX][proximoY+1];*/ //ou 1
                      matriz[proximoX][proximoY] = 4;
                      if(!comeu)
                        matriz[proximoX][proximoY+1] = 0;
                      else if(i+1 == tamSnake){
                        matriz[proximoX][proximoY+1] = 1;
                      }
                      proximoY = proximoY+1;
                      //deOndeVeio = DIR;
                      continue;}}   
        }          
      }else {
        bateu = true;
      }
      arrumaMatriz();
      break;
  }
}

void arrumaMatriz(){
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if(matriz[i][j] == 4){
        matriz[i][j] = 1;
      }
    }
  }
}

