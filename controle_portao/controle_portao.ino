/*
  TÍTULO: Receptor RF multifuncional com arduíno
  ADAPTAÇÃO: Danilo Melotti Nascimento
  E-MAIL: danilo.me.nascimento@gmail.com
  DATA: 14.09.2016
  VERSÃO: 1

  VERSÃO ORIGINAL:
  TITLE:       DECODER FOR HT6P20B ENCODER
  CREATED BY:  AFONSO CELSO TURCATO
  DATE:        18/JUN/2014
  REV.:        04
  DESCRIPTION:
  http://acturcato.wordpress.com/2014/01/04/decoder-for-ht6p20b-encoder-on-arduino-board-english/
  DESCRIÇÃO:
  http://acturcato.wordpress.com/2013/12/20/decodificador-para-o-encoder-ht6p20b-em-arduino/

*/

#define rl_1 3 //define a saída do rele
#define rl_2 4

boolean estado_rl_1 = 1; //define o estado do rele
boolean bot2, bot3; //auxiliares que armazena os respectivos botões acionados
unsigned long t_1, t_2, t0, ta, tb; //auxiliares que armazenam o tempo (serão usadas em temporizadores)

const byte pinRF = 2;  // Pin where RF Module is connected. If necessary, change this for your project

int lambda;      // on pulse clock width (if fosc = 2KHz than lambda = 500 us)

struct rfControl        //Struct for RF Remote Controls
{
  unsigned long addr;  //ADDRESS CODE
  boolean btn1;        //BUTTON 1
  boolean btn2;        //BUTTON 2
};

boolean ACT_HT6P20B_RX(struct rfControl &_rfControl) {

  static boolean startbit;      //checks if start bit was identified
  static int counter;           //received bits counter: 22 of Address + 2 of Data + 4 of EndCode (Anti-Code)
  static unsigned long buffer;  //buffer for received data storage

  int dur0, dur1;  // pulses durations (auxiliary)

  if (!startbit)
  { // Check the PILOT CODE until START BIT;
    dur0 = pulseIn(pinRF, LOW);  //Check how long DOUT was "0" (ZERO) (refers to PILOT CODE)

    Serial.print("dur0: ");
    Serial.println(dur0);

    //If time at "0" is between 9200 us (23 cycles of 400us) and 13800 us (23 cycles of 600 us).
    if ((dur0 > 9200) && (dur0 < 13800) && !startbit)
    {
      //calculate wave length - lambda
      lambda = dur0 / 23;

      //Reset variables
      dur0 = 0;
      buffer = 0;
      counter = 0;

      startbit = true;
    }
  }

  //If Start Bit is OK, then starts measure os how long the signal is level "1" and check is value is into acceptable range.
  if (startbit && counter < 28)
  {
    ++counter;

    dur1 = pulseIn(pinRF, HIGH);

    if ((dur1 > 0.5 * lambda) && (dur1 < (1.5 * lambda))) //If pulse width at "1" is between "0.5 and 1.5 lambda", means that pulse is only one lambda, so the data é "1".
    {
      buffer = (buffer << 1) + 1;   // add "1" on data buffer
    }
    else if ((dur1 > 1.5 * lambda) && (dur1 < (2.5 * lambda))) //If pulse width at "1" is between "1.5 and 2.5 lambda", means that pulse is two lambdas, so the data é "0".
    {
      buffer = (buffer << 1);       // add "0" on data buffer
    }
    else
    {
      //Reset the loop
      startbit = false;
    }
  }

  //Check if all 28 bits were received (22 of Address + 2 of Data + 4 of Anti-Code)
  if (counter == 28)
  {
    // Check if Anti-Code is OK (last 4 bits of buffer equal "0101")
    if ((bitRead(buffer, 0) == 1) && (bitRead(buffer, 1) == 0) && (bitRead(buffer, 2) == 1) && (bitRead(buffer, 3) == 0))
    {
      counter = 0;
      startbit = false;

      //Get ADDRESS CODE from Buffer
      _rfControl.addr = buffer >> 6;

      //Get Buttons from Buffer
      _rfControl.btn1 = bitRead(buffer, 4);
      _rfControl.btn2 = bitRead(buffer, 5);

      //If a valid data is received, return OK
      return true;
    }
    else
    {
      //Reset the loop
      startbit = false;
    }
  }

  //If none valid data is received, return NULL and FALSE values
  _rfControl.addr = NULL;
  _rfControl.btn1 = NULL;
  _rfControl.btn2 = NULL;

  return false;
}

void setup() {

  pinMode(rl_1, OUTPUT);  //seta o pino como saída
  digitalWrite (rl_1, estado_rl_1); //define o relé inicialmente como desligado (estado_rl_1 = 1)
  pinMode(rl_2, OUTPUT); //seta o pino como saída
  digitalWrite (rl_2, HIGH); //seta o rele inicialmente como desligado

  pinMode(pinRF, INPUT);

  pinMode(13, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  digitalWrite(13, digitalRead(pinRF));  //blink de onboard LED when receive something


  struct rfControl rfControl_1;    //Set variable rfControl_1 as rfControl type

  if (ACT_HT6P20B_RX(rfControl_1))
  {
    //If a valid data is received, print ADDRESS CODE and Buttons values  - Exibe o código do controle e o botão pressionado
     Serial.print("Address: "); Serial.println(rfControl_1.addr, HEX);
     Serial.print("Button1: "); Serial.println(rfControl_1.btn1, BIN);
     Serial.print("Button2: "); Serial.println(rfControl_1.btn2, BIN);
     Serial.println();
  }



  if ((rfControl_1.addr == 0x24B2ED) && ((ta - tb) > 500))  {  /*verifica se o controle pressionado é o controle programado ("24B2ED" é o código do controle programado) 
                                                               e define um delay para evitar captaçoes indesejadas do botão que está sendo pressionado.
                                                               Para saber qual é o código do seu controle, basta cerregar esse sketch no arduino, pressionar o botão do controle,
                                                               copiar o código informado e colar no lugar de "24B2ED" obs: o "0x" deve ser mantido. */
                                                               
    tb = millis(); // um dos contadores de tempo do sistema que evita captaçoes indesejadas do botão pressionado

    Serial.print("Controle reconhecido, "); //debug

    if ((rfControl_1.btn1 == 1) && (rfControl_1.btn2 == 0)) { //se botão 1 for pressionado
      Serial.print("botao 1 pressionado, "); //debug
      estado_rl_1 = ! estado_rl_1; //inverte o estado atual do relé (1 ou 0)
      digitalWrite (rl_1, estado_rl_1); //liga ou desliga o relé
      if (digitalRead (rl_1) == 0) Serial.println("rele 1 acionado"); //debug
      if (digitalRead (rl_1) == 1 ) Serial.println("rele 1 desacionado"); //debug
      Serial.println();
    }

    if ((rfControl_1.btn1 == 0) && (rfControl_1.btn2 == 1)) { //se botão 2 for pressionado
      Serial.print("botao 2 pressionado, "); //debug
      Serial.println("rele 2 acionado"); //debug
      digitalWrite (rl_2, LOW); //liga o rele 2
      t_1 = millis(); //contador inicial do tempo de pulso programado para o botão 2
      bot2 = 1; //variável que armazena que o botão 2 foi pressionado
    }

    if ((rfControl_1.btn1 == 1) && (rfControl_1.btn2 == 1)) { //se botão 3 for pressionado
      Serial.print("botao 3 pressionado, "); //debug
      Serial.println("rele 2 acionado"); //debug
      digitalWrite (rl_2, LOW); //liga o rele 2
      t_2 = millis(); //contador inicial do tempo de pulso programado para o botão 3
      bot3 = 1; //variável que armazena que o botão 3 foi pressionado
    }

  }

  ta = millis();  //outro contador de tempo do sistema que evita captaçoes indesejadas do botão pressionado
  t0 = millis(); // contador final do tempo de pulso do relé 2

  if (((t0 - t_1) > 500) && (bot2 == 1)) { //desliga o relé 2 de acordo com a configuração de tempo do botão 2
    digitalWrite (rl_2, HIGH); //desliga o rele 2
    bot2 = 0; //zera a variável que armazena o estado do botão 2
    {
      Serial.println("rele 2 desacionado");  //debug
      Serial.println();
    }
  }

  if (((t0 - t_2) > 3000) && (bot3 == 1)) { //desliga o relé 2 de acordo com a configuração de tempo do botão 3
    digitalWrite (rl_2, HIGH); //desliga o rele 2
    bot3 = 0; //zera a variável que armazena o estado do botão 3
    {
      Serial.println("rele 2 desacionado");  //debug
      Serial.println();
    }
  }

}
