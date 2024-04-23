// const int pinCapteurForce = A0; // Pin analogique où est connecté le capteur de force

// void setup() {
//   Serial.begin(115200);
//   delay(1000); // Attendez une seconde avant de commencer
// }

// void loop() {
//   int valeurCapteurForce = analogRead(pinCapteurForce);
//   Serial.println(valeurCapteurForce);
//   delay(1000); // Attendez une seconde avant de lire à nouveau
// }


// #include <ESP8266WiFi.h>

// // Initialize force sensor parameters
// const int forceSensorPin = A0; // Pin analogique où est connecté le capteur de force

// // Initialize network parameters
// const char* ssid = "ALHN-E689";
// const char* password = "077813nasro";
// const char* host = "192.168.1.162"; // Replace with the IP address of your ESP gateway

// // Set up the client object
// WiFiClient client;

// // Configure deep sleep in between measurements
// const int sleepTimeSeconds = 2;

// void setup() {
//   // Connect to Wi-Fi
//   WiFi.begin(ssid, password);
//   Serial.begin(115200);

//   // Wait for connection
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println();
//   Serial.print("IP Address: ");
//   Serial.println(WiFi.localIP());

//   // Read force sensor value
//   int forceValue = analogRead(forceSensorPin);

//   // Connect to the server and send the data as URL parameter
//   if (client.connect(host, 80)) {
//     String url = "/update?force=";
//     url += String(forceValue);
//     client.print(String("GET ") + url + " HTTP/1.1\r\n" +
//                  "Host: " + host + "\r\n" +
//                  "Connection: keep-alive\r\n\r\n");
//     delay(10);

//     // Read all the lines of the response and print them to Serial
//     Serial.println("Response:200ok");
//     while (client.available()) {
//       String line = client.readStringUntil('\r');
//       Serial.print(line);
//     }
//   }

//   Serial.println("ESP8266 in sleep mode");
//   ESP.deepSleep(sleepTimeSeconds * 1e6);
// }

// void loop() {
//   // Empty loop as ESP8266 will be in deep sleep
// }

//sans sleeptime
// #include <ESP8266WiFi.h>

// // Initialize force sensor parameters
// const int forceSensorPin = A0; // Pin analogique où est connecté le capteur de force

// // Initialize network parameters
// const char* ssid = "ALHN-E689";
// const char* password = "077813nasro";
// const char* host = "192.168.1.162"; // Replace with the IP address of your ESP gateway

// // Set up the client object
// WiFiClient client;

// void setup() {
//   // Connect to Wi-Fi
//   WiFi.begin(ssid, password);
//   Serial.begin(115200);

//   // Wait for connection
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println();
//   Serial.print("IP Address: ");
//   Serial.println(WiFi.localIP());

//   // Read force sensor value
//   int forceValue = analogRead(forceSensorPin);
//   Serial.print("Force value: ");
//   Serial.println(forceValue);

//   // Connect to the server and send the data as URL parameter
//   if (client.connect(host, 80)) {
//     String url = "/update?force=";
//     url += String(forceValue);
//     client.print(String("GET ") + url + " HTTP/1.1\r\n" +
//                  "Host: " + host + "\r\n" +
//                  "Connection: keep-alive\r\n\r\n");
//     delay(10);

//     // Read all the lines of the response and print them to Serial
//     Serial.println("Response:200ok");
//     while (client.available()) {
//       String line = client.readStringUntil('\r');
//       Serial.print(line);
//     }
//   }
// }

// void loop() {
//   // Your code continues here
// }

#include <ESP8266WiFi.h>

// Initialize force sensor parameters
const int forceSensorPin = A0; // Pin analogique où est connecté le capteur de force

// Initialize network parameters
const char* ssid = "ALHN-E689";
const char* password = "077813nasro";
const char* host = "192.168.1.162"; // Replace with the IP address of your ESP gateway

// Set up the client object
WiFiClient client;

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
}

void loop() {
  // Read force sensor value
  int forceValue = analogRead(forceSensorPin);
  Serial.print("Force value: ");
  Serial.println(forceValue);

  // Connect to the server and send the data as URL parameter
  if (client.connect(host, 80)) {
    String url = "/update?force=";
    url += String(forceValue);
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
    client.stop(); // Fermer la connexion après l'envoi des données
  }

  delay(1000); // Attendre 1 seconde avant la prochaine lecture
}




 



