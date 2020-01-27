// CONSTANTS
const int pinLed1 = 11;
const int pinLed2 = 12;
const int pinLed3 = 13;

const int pinSensor1 = 5;
const int pinSensor2 = 6;
const int pinSolenoide = 7;
const int pinWater = 8;

int pulse; // variavel para a quantidade de pulsos
int j=0; // variavel para contagem
float waterFlux; // variavel para armazenar o valor em L/min
float waterMedia=0; // variavel para tirar a media a cada 1 minuto

void setup() {

  Serial.begin(9600);

// SENSORS
  pinMode(pinSensor1, INPUT);
  
  pinMode(pinSensor2, INPUT);
  
  pinMode(pinSolenoide, INPUT);
  
  pinMode(pinWater, INPUT);
  attachInterrupt(0, incrpulso, RISING); 
  
// LEDS
  pinMode(pinLed1, OUTPUT);
  digitalWrite(pinLed1, LOW);
  
  pinMode(pinLed2,OUTPUT);
  digitalWrite(pinLed, LOW);
  
  pinMode(pinLed3, OUTPUT);
  digitalWrite(pinLed2, LOW);

}

void loop() {

// Water Flux Sensor
  pulse = 0; // comeca do 0 variavel para contar os giros das pas internas em segundos
  startWaterFlux(); //liga interrupcao
  delay(2000); // espera 2 segundos
  stopWater(); // desliga a interrupcao
  
  waterFlux = pulse / 5.5; // converter para Litros/minuto
  waterMedia = waterMedia + waterFlux;// soma a vazao para o calculo da waterMedia
  j++;

  serial.print(waterFlux); // imprime na serial o valor da vazao
  serial.print("  L/minuto - "); // imprime L/min
  if(j == 60) {
    waterMedia = waterMedia / 60 ; // Tira a media dividindo por 60
    serial.print("\n Media por Minuto = ");// imprime frase valor de waterMedia
    serial.print(waterMedia); // mostra watermedia
    serial.println(" Litros/minutos - "); // Mostra L/min
    waterMedia = 0 ; // Torna variavel waterMedia = 0,  para uma nova amostragem
    j=0; // zera a variavel para uma nova contagem
  }
   
// SENSOR SOLENOIDE SIMPLES
  if(digitalRead(pinSensor1) == LOW {
    digitalWrite(pinSolenoide, HIGH);
    digitalWrite(pinLed3,HIGH);
    serail.print("SOLENOIDE LIGADA");
  } else {
    digitalWrite(pinSolenoide, LOW);
    digitalWrite(pinLed3, LOW);
    serial.print("SOLENOIDE DESLIGADA.")
    
  }

// SENSOR DE NIVEL MINIMO
  if(digitalRead(pinSensor1) == HIGH {
    digitalWrite(pinLed1, HIGH);
    serial.print("NIVEL MINIMO.")
  } else {
    digitalWrite(pinLed1,LOW;
    serial.print("CAIXA VAZIA")
  }
  
// SENSOR DE NIVEL MAXIMO
  if(digitalRead(pinSensor2) == HIGH){
    digitalWrite(pinLed2, HIGH);  
    serial.print("NIVEL MAXIMO NA CAIXA DE AGUA.") 
  } else {
    digitalWrite(pinLed2, LOW;
    serial.print("ABASTECENDO CAIXA DE AGUA.")
  }
    
}

void incrpulso(){
  pulse++;
}

}
