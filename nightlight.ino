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
  while(!client.available()){
    delay(1);
  }
  String request = client.readStringUntil('\r');
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
  response += "<style>button {background: #fff; font-size: 18px; width: 100%; height: 200px; border: 2px solid rgb(30,205,151); color: rgb(30, 205, 151); padding: 20px; border-radius: 30px;} button:active { background: rgb(30,205,151); color: #fff; } input:*{ outline: none; }</style>\r\n";

  if (value) {
    response += "<a href=\"on\"><button>On</button></a>";
  }
  else {
    response += "<a href=\"off\"><button>Off</button></a>";
  }
  response += "</html>\n";

  client.print(response);
  delay(1);
}

void initHardware() {
  Serial.begin(115200);
  delay(10);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
}

void connectWiFi() {
  Serial.printf("Connecting to %s\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  server.begin();
  Serial.println("");
  Serial.print("Server started at: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
