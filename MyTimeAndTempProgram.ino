#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "SSD1306Wire.h"

const char* Ssid = "Shortening";
const char* Password = "ThiaIsAwesome";

ESP8266WiFiMulti WiFiMulti;
SSD1306Wire  display(0x3c, D3, D5);

const char * headerKeys[] = {"date"};
const size_t numberOfHeaders = 1;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(Ssid, Password);

  Serial.print("Connecting to Wifi");
  while ((WiFiMulti.run() != WL_CONNECTED)) {
    Serial.print(".");
    delay(500);
  }
  Serial.print(" Connected!\n");
}

void drawTime(String time)
{
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawString(64, 12, time);
}

void loop() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    http.begin("http://andrewcombs13.com/clock/");
    http.collectHeaders(headerKeys, numberOfHeaders);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String headerDate = http.header("date");
      String headerTime = headerDate.substring(17, 25);
      Serial.printf("Time: ");
      Serial.println(headerTime);

      display.clear();
      drawTime(headerTime);
      display.display();
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(1000);
}
