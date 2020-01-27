/*
 * using  NodeMCU-12E v3 ESP8266 WiFi board and 2 x Waterproof HC-SR04 type ultrasonic sounders to measure the volume of water in 2 water tanks.
 * The sonar pings for the top of the water, gets the distance in cm, 
 * calculates depth of water in cm and then calculates volume of water in liters.
 * 
 * All the info is passed by WiFi on the WLAN to the Blynk server
 * Info is available on a phone using the Blynk App logged into the Blynk server.
 * 
*  pin mapping is different on these boards - CAUTION. Pin numbers are generally GPIO numbers
*  
 */

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NewPing.h>
//#include <ArduinoOTA.h>                                                             //added for OTA
 
//    #define SONAR_NUM 2                                         // Number of sensors. Change to suit your requirements.
    #define SONAR_NUM 1                                         // Number of sensors. Change to suit your requirements.
    #define PI 3.1415926535897932384626433832795
    
    //** TANK DIMENSIONS
    const int MAX_DISTANCE = 300;                       //max distance to measure
    const int Diameter = 276;		                            //internal Diameter of tank 1 in cm		    
    const int Depth = 265;			                            //total depth of tank 1 in cm	, from sensor to base	inside 

    const unsigned int Period = 2000;    //period between pings, in milliseconds. i.e  1 munute = 60,000. max 65535. Want longer? use unsigned long
    
    //** SENSOR PINS
  
    const int PingPin1 = 5;                          //     GPIO5,  D1 
    const int EchoPin1 = 4;                        //     GPIO4,  D2
//    const int PingPin2 = 14;                         //     GPIO14,  D5
//    const int EchoPin2 = 12;                         //     GPIO12,  D6
    
    const int Area = PI * ((Diameter / 2) * (Diameter / 2));	        //area of base of tank 1
    
    // Global variables
    int Litres, Distance, WaterDepth;
    
    // Set password to "" for open networks.
    char ssid[] = "Lanbastos";		                                            //local wifi network SSID
    char pass[] = "@ek)Z5w*1984#";			                                //local network password
   
   char auth[] = "Project Authority Token";                                     // You should get Authority Token in your email.  

    BlynkTimer timer;                                                                   //config timer

    NewPing sonar[SONAR_NUM] = {		                                // Sensor object array.
	    NewPing(PingPin1, EchoPin1, MAX_DISTANCE) //,	        // Each sensor's trigger pin, echo pin, and max distance to ping.
//	    NewPing(PingPin2, EchoPin2, MAX_DISTANCE)
    };

void sendSensorReadings()
{
// Readings Tank 1
    Distance1 = sonar[0].ping_cm();                                       //get distance to the top of the water tank 1
    if (Distance >= Depth || Distance == 0 )  Distance = Depth;                   //check it does not go negative
	WaterDepth = Depth - Distance;							        //calculate the depth of the water
	Litres = (Area * WaterDepth) / 1000;	                         //calculate the volume of the water in litres

// SEND INFO TO BLYNK
    Blynk.virtualWrite(V1, WaterDepth);                                //send depth to Blynk server
    Blynk.virtualWrite(V2, Litres);                                            //send litres to Blynk server
    Blynk.virtualWrite(V3, Litres / 10);                                   //send litres to Blynk server. for vertical level widget & chart, 
                                                                                                //scaled to 1/10 as Blynk only goes up to 9999 and we need up to 16000
    digitalWrite(13, HIGH);                                                //flash the LED on D7, just to let us know it's running
    delay(2);
    digitalWrite(13, LOW);

//************************* can be commented out, test use only
    Serial.println();
    Serial.println();
    Serial.println("Tank water distance: " + String(Distance1));   //print depth
    Serial.println("Tank water depth: " + String(WaterDepth1));  //print depth
    Serial.println("Tank Litres: " + String(Litres1));                //print litres
    
//***********************************************   
 
}

void setup() {
    //ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });       //added for OTA
    //ArduinoOTA.begin();                                                                     //added for OTA
    
    pinMode(13, OUTPUT);                                                //LED D7
    
	timer.setInterval(Period, sendSensorReadings);		// Setup a function to be called every n seconds
	delay(10);
  
//**    can be commented out, test only
    Serial.begin(9600);										        // Open serial console. 
    Serial.println();
    Serial.println("Connecting to " + String(ssid));      // Connected to WiFi network
//**	
    Blynk.begin(auth, ssid, pass);
	delay(20);
    
//**    can be commented out, test only
	Serial.println(WiFi.localIP());								     //this is local IP for this board
	Serial.println("WiFi connected");

}

void loop() {
   
	Blynk.run();
	timer.run();
     //ArduinoOTA.handle();                                            //added for OTA
}
