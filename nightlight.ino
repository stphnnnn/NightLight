#include <ESP8266WiFi.h>
#include "config.h"
 
int ledPin = BUILTIN_LED;
int value = 0;
WiFiServer server(80);

void setup() {
 initHardware();
 connectWiFi();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("New client");
  while(!client.available()){
    delay(1);
  }
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if (request.indexOf("/off") != -1) {
    value = 1;
  }
  else if (request.indexOf("/on") != -1){
    value = 0;
  }
  else {
    value = digitalRead(ledPin);
  }

  digitalWrite(ledPin, value);
  client.flush();

  String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  if (value) {
    response += "<a href=\"on\"><button>ON</button></a>";
  }
  else {
    response += "<a href=\"off\"><button>OFF</button></a>";
  }
  response += "</html>\n";

  client.print(response);
  delay(1);
  Serial.println("Client disonnected");
}

void connectWiFi() {
  Serial.printf("Connecting to %s\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  server.begin();
  Serial.println("");
  Serial.print("Server started at: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void initHardware() {
  Serial.begin(115200);
  delay(10);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
}
