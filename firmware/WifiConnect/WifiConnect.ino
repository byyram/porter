#include <WiFi.h>
#include <WebServer.h>
#include "secrets.h"

const int lightPin = 2;

WebServer server(80);

void handleLight() {
  if (server.hasArg("on")){
    String call = server.arg("on");
    int call_ = call.toInt();

    if (call_ == 1){
      digitalWrite(lightPin, HIGH);
      server.send(200, "text/plain", "Light is on.");

    } else{
      digitalWrite(lightPin, LOW);
      server.send(200, "text/plain", "Light is off.");
    } 
    
  } else {
      server.send(400, "text/plain", "Wrong parameter, nothing worked.");
  }
}

void setup() {

  /*Initialize*/
  Serial.begin(115200);
  delay(250);

  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, LOW);

  /*Wifi commands*/
  Serial.println("Connecting to the WiFi...");
  WiFi.begin(ssid, password);

/*Waiting for connection*/
  while(WiFi.status() != WL_CONNECTED){
    delay(3000);
    Serial.println("Still trying to connect... be patient");
  }

  Serial.print("Connected to");
  Serial.println(ssid);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  

  server.on("/light", handleLight);

  server.begin();
  Serial.println("HTTP server began");

}

void loop() {
  server.handleClient();

}
