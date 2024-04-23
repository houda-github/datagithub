#include <ESP8266WiFi.h>
#include <DHT.h>

// Initialize sensor parameters
#define DHTPIN D1       // Pin where the DHT11 is connected
#define DHTTYPE DHT11   // Type of the DHT sensor
DHT dht(DHTPIN, DHTTYPE);
float temperatureC = 0.0;
float humidity = 0.0;

// Initialize network parameters
const char* ssid = "ALHN-E689";
const char* password = "077813nasro";
const char* host = "192.168.1.162"; // Replace with the IP address of your ESP gateway

// Set up the client object
WiFiClient client;

// Configure deep sleep in between measurements
const int sleepTimeSeconds = 2;

void setup() {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.begin(115200);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize DHT sensor
  dht.begin();

  // Read temperature and humidity from the sensor
  temperatureC = dht.readTemperature();
  humidity = dht.readHumidity();

  // Connect to the server and send the data as URL parameters
  if (client.connect(host, 80)) {
    String url = "/update?temp=";
    url += String(temperatureC);
    url += "&humidity=";
    url += String(humidity);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: keep-alive\r\n\r\n");
    delay(10);

    // Read all the lines of the response and print them to Serial
    Serial.println("Response:200ok");
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  }

  Serial.println("ESP8266 in sleep mode");
  ESP.deepSleep(sleepTimeSeconds * 1e6);
}

void loop() {
  // Empty loop as ESP8266 will be in deep sleep
}

