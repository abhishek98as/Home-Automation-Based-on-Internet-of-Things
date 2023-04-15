#define BLYNK_TEMPLATE_ID           "TMPLkJGD_r-w"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "l9DxpxLSgiAjGJe_pbLU8xkAwHGV3oFA"
#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Abhi";
char pass[] = "12345678";
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX
#define ESP8266_BAUD 9600
ESP8266 wifi(&EspSerial);
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

const int load1=6;
const int load2=7;
//const int Buzzer=2;
#define cs_total A2  // current sensor
#define cs_theft A3  // current sensor

int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module
double Voltage = 0;
double VRMS = 0;
double AmpsRMSTotal = 0;
double AmpsRMSTheft= 0;

void setup() 
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(cs_total, INPUT);
  pinMode(cs_theft, INPUT);
  pinMode(load1, OUTPUT);
  pinMode(load2, OUTPUT);
  digitalWrite(load1, LOW);
  digitalWrite(load2, LOW);
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass, "blynk.cloud", 80);
}
  
  void loop()
{
 Blynk.run();


    
  Voltage = getVPP(cs_total);
   VRMS = (Voltage/2.0) *0.707;
   AmpsRMSTotal = (VRMS * 1000)/mVperAmp;
   float AmpsRMSTotal = getVPP(cs_total);

   
   Voltage = getVPP(cs_theft);
   VRMS = (Voltage/2.0) *0.707;
   AmpsRMSTheft = (VRMS * 1000)/mVperAmp;
  float AmpsRMSTheft = getVPP(cs_theft);
  
   Serial.print("load1 Load: ");
  Serial.println(AmpsRMSTotal);
  Serial.print("load2 Load: ");
  Serial.println(AmpsRMSTheft);
  
   if (AmpsRMSTotal < 0.18)  //load1
{
    AmpsRMSTotal = 0;
  }

   
  if (AmpsRMSTheft < 0.30)  //load2
  {
    AmpsRMSTheft = 0;
  }



  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("load1: ");
  lcd.print(AmpsRMSTotal);
  lcd.print(" Amp ");
  
  lcd.setCursor(0, 1);
  lcd.print("load2: ");
  lcd.print(AmpsRMSTheft);
  lcd.print(" Amp ");
  Blynk.virtualWrite(V0,AmpsRMSTotal);
  Blynk.virtualWrite(V1,AmpsRMSTheft);
  delay(1000);
  

 
  }

float getVPP(int cs)
{
  float result;
 
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 1024;          // store min value here
 
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(cs);
       // see if you have a new maxValue
       if (readValue > maxValue)
       {
           /record the maximum sensor value/
           maxValue = readValue;
       }
       if (readValue < minValue)
       {
           /record the maximum sensor value/
           minValue = readValue;
       }
   }
  
   // Subtract min from max
   result = ((maxValue - minValue) * 5.0)/1024.0;
     
   return result;
 }