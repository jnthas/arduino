//Programa: Teste de Display LCD 16 x 2
//Autor: FILIPEFLOP
 
//Carrega a biblioteca LiquidCrystal
#include <LiquidCrystal.h>

byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};
 
//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int contador = 0; 
void setup()
{
  lcd.createChar(0, smiley);
  lcd.begin(16, 2);  
  
}
 
void loop()
{
  lcd.clear(); // limpa tela do LCD
  lcd.setCursor(0, 0); // selecionando coluna 0 e linha 0
  lcd.print("Hello World! "); // mostra no LCD
  lcd.write(byte(0));
  lcd.setCursor(1, 1); // selecionando coluna 1 e linha 1
  lcd.print(contador); // mostra no LCD a contagem
  contador++; // incrementa contador
  delay(1000); 
}
