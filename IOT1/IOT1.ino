// CONSTANTS
const int pinLed1 = 11;
const int pinLed2 = 12;
const int pinLed3 = 13;
const int pinSensor1 = 5;
const int pinSensor2 = 6;
const int pinSolenoide = 7;
byte sensorInterrupt = 0;  
byte sensorPin       = 8;
float calibrationFactor = 4.5;
volatile byte pulseCount;  
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;

void setup() {

  Serial.begin(9600);

// SENSORS
  pinMode(pinSensor1, INPUT);  
  pinMode(pinSensor2, INPUT);  
  pinMode(pinSolenoide, INPUT);  
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

}

void loop() {

   if((millis() - oldTime) > 1000){ 
 
    detachInterrupt(sensorInterrupt);        
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;  
    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    unsigned int frac;
    
    Serial.println("");    
    Serial.print("Fluxo Total: ");
    Serial.print(int(flowRate)); 
    Serial.print(" L/min | "  );
    Serial.print("Quantidade de Vazão: ");        
    Serial.print(totalMilliLitres);
    Serial.print(" mL | "  ); 
    Serial.print(totalMilliLitres/1000);
    Serial.print(" L");
    Serial.println("");
        
    pulseCount = 0;    
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);    
  }

   delay(2000);
   
// SENSOR SOLENOIDE SIMPLES
  if(digitalRead(pinSensor2) == LOW) {
    digitalWrite(pinSolenoide, HIGH);
    Serial.println("");
    Serial.println("SOLENOIDE LIGADA");
  } else {
    digitalWrite(pinSolenoide, LOW);
    Serial.println("");    
    Serial.println("SOLENOIDE DESLIGADA.");    
  }

// SENSOR DE NIVEL MINIMO
  if(digitalRead(pinSensor1) == HIGH) {
    Serial.println("");    
    Serial.println("1 - NIVEL MINIMO.");
  } else {
    Serial.println("");    
    Serial.println("1 - CAIXA VAZIA");
  }
  
// SENSOR DE NIVEL MAXIMO
  if(digitalRead(pinSensor2) == HIGH){
    Serial.println("");    
    Serial.println("2 - NIVEL MAXIMO NA CAIXA DE AGUA.") ;
  } else {
    Serial.println("");    
    Serial.println("2 - ABASTECENDO CAIXA DE AGUA.");
  }
    
}

void pulseCounter()
{
  pulseCount++;
}
