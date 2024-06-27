#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid = "gatekeeper";
const char* password = "gatekeeper11";
const char *serverIP = "192.168.50.183";
const int serverPort = 80;
#define relayPin D0
int tracker = 0;
void sendHumidityToServer(float humidity) {
  // Create an HTTP client
  WiFiClient client;
  HTTPClient http;

  // Construct the URL for the PHP script on the server
  String url = "http://" + String(serverIP) + ":" + String(serverPort) + "/humidity_monitor/sendHumToServer.php?humidity=" + String(humidity);

  // Send HTTP GET request
  http.begin(client,url.c_str());
  int httpCode = http.GET();

  // Check the response
  if (httpCode == HTTP_CODE_OK) {
    Serial.println("Humidity data sent to server successfully");
  } else {
    Serial.println("Failed to send humidity data to server");
  }

  // Close the connection
  http.end();
}

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  WiFiClient client;
  HTTPClient clientHTTP;
  if(WiFi.status() == WL_CONNECTED)
  {
    String host = "http://dweet.io";
    String request = "/get/latest/dweet/for/CactusServer";
    String url = host + request;
    clientHTTP.begin(client,url.c_str());
    if(clientHTTP.GET()>0)
    {
      String payload = clientHTTP.getString();
      JSONVar jsonVersion = JSON.parse(payload);
      String value = jsonVersion["With"][0]["content"]["Humidity"];
      //sendHumidityToServer(value.toFloat());
      if(value == "ON")
      {
        digitalWrite(relayPin, 1);
        tracker = 1;
      }
      else if(value.toFloat() >= 60.0)
      {
        digitalWrite(relayPin, 1);
        tracker = 1;
      }
      else
      {
        digitalWrite(relayPin,0);
        tracker = 0;
      }
      delay(60000);
    }
    else
    {
      Serial.println("get Failed");
      delay(5000);
    }
    clientHTTP.end();
  }
  else
    Serial.println("connection failed");
}