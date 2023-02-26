
#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"
#define LED1 13
#define LED2 12
#define LED3 32
char * endpoint = "https://ecse-three-led-api.onrender.com/api/state";   //api url


void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  Serial.begin(9600);
  WiFi.begin(WIFI_NAME, WIFI_PASS);
  Serial.println("Connecting");

  while(WiFi.status() != WL_CONNECTED)    
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  if(WiFi.status()== WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(endpoint);       //start connection to api url
    http.addHeader("X-API-KEY","Louis#1156");
    int httpResponseCode = http.GET();      //performs get request and receives status code response
    String http_response;

    if (httpResponseCode>0) 
    {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      Serial.print("Response from server: ");
      http_response = http.getString();       //gets worded/verbose response
      Serial.println(http_response);
    }
    else 
    {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
      
    http.end();
    StaticJsonDocument<1024> data;            //document to store deserialized json
    DeserializationError error = deserializeJson(data, http_response);

    if (error) 
    {
      Serial.print("Could not deserialize json");
      Serial.println(error.c_str());
      return;
    }

    const bool led1_state = data["light_switch_1"];
    const bool led2_state = data["light_switch_2"]; 
    const bool led3_state = data["light_switch_3"]; 

    Serial.println("data:");
    Serial.println(led1_state);
    Serial.println(led2_state);
    Serial.println(led3_state);

    if (led1_state == 1)
    {
      digitalWrite(LED1, HIGH);
    }
    else
    {
      digitalWrite(LED1, LOW);
    }

    if (led2_state == 1)
    {
      digitalWrite(LED2, HIGH);
    }
    else
    {
      digitalWrite(LED2, LOW);
    }

    if (led3_state == 1)
    {
      digitalWrite(LED3, HIGH);
    }
    else
    {
      digitalWrite(LED3, LOW);
    }
    
  }
  else
  {
    return;
  }
}