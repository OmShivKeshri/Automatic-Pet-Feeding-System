#include <ESP8266_Lib.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <Servo.h>
#include "ThingSpeak.h"
#include <SoftwareSerial.h>
#define ESP8266_BAUD 115200
Servo myservo;
int LED = 13;
int pir = 2;
int buzzer = 6;
int tempSensor = A0;
const int triPin = 11;
const int echoPin = 12;
//variables
int val = 0;
int duration;
double dis_cm = 0.0;
double dis_inches = 0.0;
double tempValue = 0.0;
double realTemp = 0.0;
//SoftwareSerial EspSerial(2, 3);
char ssid[] = "air12871"; // your network SSID (name)
char pass[] = "pranay0707"; // your network password
int keyIndex = 0; // your network key Index number (needed only for WEP)
WiFiClient client;
unsigned long myChannelNumber = 1578216;
const char * myWriteAPIKey = "M7WZ4KP0L6PT6FK5";
int number1 = 0;
int number2 = random(0,100);
int number3 = random(0,100);
int number4 = random(0,100);
String myStatus = "";

void setup()
{
pinMode(LED, OUTPUT);
pinMode(tempSensor,INPUT);
pinMode(buzzer, OUTPUT);
pinMode(pir,INPUT);
pinMode(triPin, OUTPUT);
pinMode(echoPin, INPUT);
myservo.attach(9);
Serial.begin(115200);
Wifi.mode(WIFI_STA);
ThingSpeak.begin(client);
}
void loop()
{
//detect motion
val = digitalRead(pir);
//measure and calibrate temperature value
tempValue = analogRead(tempSensor);
realTemp = (((tempValue/1024.0) * 5.0)-.5)*100;
// listen for user input and send it to the ESP8266
if(WiFi.status() != WL_CONNECTED){
Serial.print("Attempting to connect to SSID: ");
Serial.println(SECRET_SSID);
while(WiFi.status() != WL_CONNECTED){
WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
Serial.print(".");
delay(5000);
}
Serial.println("\nConnected.");
}
ThingSpeak.setField(1, number1);
ThingSpeak.setField(2, number2);
ThingSpeak.setField(3, number3);

//shoot the pin at the object
digitalWrite(triPin, LOW);
delayMicroseconds(5);
digitalWrite(triPin, HIGH);
delayMicroseconds(10);
digitalWrite(triPin, LOW);
delayMicroseconds(5);
//capture echo
duration = pulseIn(echoPin, HIGH);
//measure distance
dis_cm = (duration/2) / 29.1;
dis_inches = (duration/2) /74.0;
}
//main program
if(val == HIGH && dis_cm > 50 && realTemp >=38.3 && realTemp <= 40.2 )
{
digitalWrite(LED, HIGH);
tone(buzzer, 200, 250);
delay(300);
tone(buzzer, 300, 300);
delay(350);
tone(buzzer, 450, 500);
myservo.write(180);
delay(500);
myservo.write(0);
}
else
{
digitalWrite(LED, LOW);
digitalWrite(buzzer,LOW);
myservo.write(0);
}
ThingSpeak.setStatus(myStatus);
// write to the ThingSpeak channel

int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
if(x == 200){
Serial.println("Channel update successful.");
}
else{
Serial.println("Problem updating channel. HTTP error code " + String(x));
}
number1 = (int)realTemp;
number2 = (int)dis_cm;
number3++;
{
//Display reading in Serial Monitor
Serial.print("Distance reading: ");
Serial.print(dis_cm);
Serial.print(" cm, ");
Serial.print(dis_inches);
Serial.println(" inches ");
Serial.print("Cat Temperature: ");
Serial.print(realTemp);
Serial.println(" degree celcius ");
delay(1000);
}
}
}
