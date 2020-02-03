#define pino_sinal_analogico A0
int valor_analogico;
int output_value;
 
void setup()
{
  Serial.begin(9600);
  pinMode(pino_sinal_analogico, INPUT);
  Serial.println("Inicializando sensor...");
  delay(2000);
}
 
void loop()
{
  //Le o valor do pino A0 do sensor
  valor_analogico = analogRead(pino_sinal_analogico);
  output_value = map(valor_analogico,450,380,0,100);


  //384 (wet) - 442 (dry)
  //391 (wet) - 443 (dry) - output led
  //Mostra o valor da porta analogica no serial monitor
  Serial.print("Porta analogica: ");
  Serial.println(valor_analogico);

  Serial.print("Umidade : ");
  Serial.println(output_value);
  
  delay(1000);
}
