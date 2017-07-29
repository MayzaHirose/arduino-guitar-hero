//Contagem Regressiva
const uint64_t CONTAGEM[] PROGMEM = {
  0x3c66603860663c00,
  0x7e060c3060663c00,
  0x7e1818181c181800
};
const int CONTAGEM_LEN = sizeof(CONTAGEM)/8;

//Variaveis Globais
String menuFrase = "2 JOGOS EM 1    ";
int btn_branco, btn_rosa, btn_azul, btn_verm; 
int acertos, erros, seqMax, seqAtual;
int jogoSelecionado = 1;

long millisAnterior = 0;        // Variável de controle do tempo
long millisExecucao = 300;     // Tempo em ms do intervalo a ser executado
unsigned long millisCorrente = 0;

