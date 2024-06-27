#include <DHTesp.h>
#include <ESP8266WiFi.h>

#define DHT11_PIN D1
DHTesp dht;
const char* ssid = "gatekeeper";
const char* password = "gatekeeper11";
const char* host = "www.dweet.io";
const char* thing  = "CactusServer";
const char* thing_content = "Humidity";

void setup() {
  Serial.begin(9600);
  dht.setup(DHT11_PIN, DHTesp::DHT11);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
    Serial.println(WiFi.localIP());
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void loop() {
  float myHumid = dht.getHumidity();
  delay(2000);
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  while(!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    delay(5000);
  }
  
  // We now create a URI for the request
  String url = "/dweet/for/";
  url += thing;
  url += "?";
  url += thing_content;
  url += "=";
  url += myHumid;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  int timeout = millis() + 5000;
  while (client.available() == 0) {
    if (timeout - millis() < 0) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println(myHumid);
  delay(1000000);
}