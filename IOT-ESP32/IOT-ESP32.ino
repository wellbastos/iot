
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define PI 3.1415926535897932384626433832795

    // CONSTANTS
    const int pinSensor1 = 12;
    const int pinSensor2 = 14;
    const int pinSolenoide = 32;
    
    byte sensorInterrupt = 0;  
    byte sensorPin       = 26;  
    float calibrationFactor = 4.5;
    volatile byte pulseCount;  
    float flowRate;
    unsigned int flowMilliLitres;
    unsigned long totalMilliLitres;
    unsigned long oldTime;

    int Litres, Distance, WaterDepth;

    const unsigned int Period = 2000; 

    //** TANK DIMENSIONS
    const int MAX_DISTANCE = 300;
    const int Diameter = 276;    
    const int Depth = 265;  
    
    const int Area = PI * ((Diameter / 2) * (Diameter / 2));   

    char ssid[] = "Lanbastos";                                             
    char pass[] = "@ek)Z5w*1984#";
    char auth[] = "r42tpBLlfqvhIGT6G5jG1DTR5XBHZ29H";         
    
    BlynkTimer timer;

void sendSensorReadings(){

// Readings Tank 1
    if (Distance >= Depth || Distance == 0 )  Distance = Depth;                   //check it does not go negative
    WaterDepth = Depth - Distance;                      //calculate the depth of the water
    Litres = (Area * WaterDepth) / 1000;                           //calculate the volume of the water in litres

// SEND INFO TO BLYNK
    Blynk.virtualWrite(V1, WaterDepth);                                //send depth to Blynk server
    Blynk.virtualWrite(V2, Litres);                                            //send litres to Blynk server
    Blynk.virtualWrite(V3, Litres / 10);                                   //send litres to Blynk server. for vertical level widget & chart, 
    Blynk.virtualWrite(V4, Distance);                                                                                            //scaled to 1/10 as Blynk only goes up to 9999 and we need up to 16000
    
    digitalWrite(13, HIGH);                                                //flash the LED on D7, just to let us know it's running
    delay(2);
    digitalWrite(13, LOW);

//************************* can be commented out, test use only
    Serial.println();
    Serial.println();
    Serial.println("Tank water distance: " + String(Distance));
    Serial.println("Tank water depth: " + String(WaterDepth)); 
    Serial.println("Tank Litres: " + String(Litres));         
    
//*********************************************** 

   if((millis() - oldTime) > 1000){ 
    pulseCount = 0;

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

    pulseCount++;    
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);         
  }   

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

void pulseCounter(){
  pulseCount++;
}

void setup() {
    //ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });       //added for OTA
    //ArduinoOTA.begin();                                                                     //added for OTA
    
    pinMode(13, OUTPUT);                                                //LED D7
    
    timer.setInterval(Period, sendSensorReadings);   // Setup a function to be called every n seconds
//    timer.setInterval( ,waterFluxSensor, pulseCounter); 
    delay(10);
  
//**    can be commented out, test only
    Serial.begin(9600);                           // Open serial console. 
    Serial.println();
    Serial.println("Connecting to " + String(ssid));      // Connected to WiFi network
//**  
    Blynk.begin(auth, ssid, pass);
  
    delay(20);
    
//**    can be commented out, test only
    Serial.println(WiFi.localIP());                    //this is local IP for this board
    Serial.println("WiFi connected");

}

void loop() {
    Blynk.run();
    timer.run();
    //ArduinoOTA.handle();                                            //added for OTA

}
