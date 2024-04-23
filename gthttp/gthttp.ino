#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <DHT.h>

// Configuration parameters for WiFi connection
const char *ssid = "ALHN-E689";
const char *password = "077813nasro";

// Set up the server object
ESP8266WebServer server;

// Initialize DHT parameters
#define DHTPIN D1       // Pin where the DHT11 is connected
#define DHTTYPE DHT11   // Type of the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// GPIO pin for the force sensor
const int forceSensorPin = A0; // Assuming analog input

// Keep track of the sensor data that's going to be sent by the client
float temperature = 0.0;
float humidity = 0.0;
int forceValue = 0; // Variable to hold force sensor data

// Configuration parameters for the Node.js server
const char *server_ip = "192.168.1.72"; // IP address of your Node.js server
const int server_port = 3000;         // Port on which your Node.js server is listening

// Set up WiFi client to connect to the server
WiFiClient client;

void setup() {
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");

    // Print IP Address as a sanity check
    Serial.begin(115200);
    Serial.println();
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Configure the server's routes
    server.on("/", handleIndex);       // use the top root path to report the last sensor value
    server.on("/update", handleUpdate); // use this route to update the sensor value
    server.begin();

    // Initialize DHT sensor
    dht.begin();
}

void loop() {
    
    server.handleClient();

    // Read sensor data
    //temperature = dht.readTemperature();
    //humidity = dht.readHumidity();

    // Read force sensor data
    //forceValue = analogRead(forceSensorPin);

    // Check if client is connected to the server
    if (!client.connected()) {
        // Attempt to connect to the server
        if (client.connect(server_ip, server_port)) {
            Serial.println("Connected to server");
        } else {
            Serial.println("Connection to server failed");
        }
    }

    // If client is connected to the server, send sensor data
    if (client.connected()) {
        sendSensorData();
    }

    delay(2000); // wait for 2 seconds before the next reading
}

void sendSensorData() {
    // Construct the URL with the sensor values
    String url = "/update?temp=" + String(temperature) + "&humidity=" + String(humidity) + "&force=" + String(forceValue);

    // Send HTTP request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + server_ip + "\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.println("Sent data to server");
}

void handleIndex() {
    // Respond with the latest temperature, humidity, and force sensor values
    String response = "Temperature: " + String(temperature) + " °C\n";
    response += "Humidity: " + String(humidity) + " %\n";
    response += "Force: " + String(forceValue) + "\n";
    server.send(200, "text/plain", response);
}

void handleUpdate() {
    // The value will be passed as URL parameters
    temperature = server.arg("temp").toFloat();
    humidity = server.arg("humidity").toFloat();
    forceValue = server.arg("force").toInt();
    Serial.println("Temperature: " + String(temperature) + " °C");
    Serial.println("Humidity: " + String(humidity) + " %");
    Serial.println("Force: " + String(forceValue));
    server.send(200, "text/plain", "Updated");
}

// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>
// #include <WiFiClient.h>
// #include <DHT.h>

// // Configuration parameters for WiFi connection
// const char *ssid = "ALHN-E689";
// const char *password = "077813nasro";

// // Set up the server object
// ESP8266WebServer server;

// // Initialize DHT parameters
// #define DHTPIN D1       // Pin where the DHT11 is connected
// #define DHTTYPE DHT11   // Type of the DHT sensor
// DHT dht(DHTPIN, DHTTYPE);

// // GPIO pin for the force sensor
// const int forceSensorPin = A0; // Assuming analog input

// // Keep track of the sensor data that's going to be sent by the client
// float temperature = 0.0;
// float humidity = 0.0;
// int forceValue = 0; // Variable to hold force sensor data

// // Configuration parameters for the Node.js server
// const char *server_ip = "192.168.1.72"; // IP address of your Node.js server
// const int server_port = 3000;         // Port on which your Node.js server is listening

// // Set up WiFi client to connect to the server
// WiFiClient client;

// void setup() {
//     // Connect to WiFi
//     WiFi.begin(ssid, password);
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(1000);
//         Serial.println("Connecting to WiFi...");
//     }

//     Serial.println("Connected to WiFi");

//     // Print IP Address as a sanity check
//     Serial.begin(115200);
//     Serial.println();
//     Serial.print("IP Address: ");
//     Serial.println(WiFi.localIP());

//     // Configure the server's routes
//     server.on("/", handleIndex);       // use the top root path to report the last sensor value
//     server.on("/update", handleUpdate); // use this route to update the sensor value
//     server.begin();

//     // Initialize DHT sensor
//     dht.begin();
// }

// void loop() {
    
//     server.handleClient();

//     // Read sensor data
//     temperature = dht.readTemperature();
//     humidity = dht.readHumidity();

//     // Read force sensor data
//     forceValue = analogRead(forceSensorPin);

//     // Check if client is connected to the server
//     if (!client.connected()) {
//         // Attempt to connect to the server
//         if (client.connect(server_ip, server_port)) {
//             Serial.println("Connected to server");
//         } else {
//             Serial.println("Connection to server failed");
//         }
//     }

//     // If client is connected to the server, send sensor data
//     if (client.connected()) {
//         sendSensorData();
//     }

//     delay(2000); // wait for 2 seconds before the next reading
// }

// void sendSensorData() {
//     // Construct the JSON object with the sensor values
//     String jsonData = "{\"temperature\": " + String(temperature) +
//                       ", \"humidity\": " + String(humidity) +
//                       ", \"force\": " + String(forceValue) + "}";

//     // Send HTTP request to the server
//     client.print(String("POST /data HTTP/1.1\r\n") +
//                  "Host: " + String(server_ip) + "\r\n" +
//                  "Content-Type: application/json\r\n" +
//                  "Content-Length: " + String(jsonData.length()) + "\r\n" +
//                  "Connection: close\r\n\r\n" +
//                  jsonData);

//     Serial.println("Sent data to server");
// }

// void handleIndex() {
//     // Respond with the latest temperature, humidity, and force sensor values
//     String response = "Temperature: " + String(temperature) + " °C\n";
//     response += "Humidity: " + String(humidity) + " %\n";
//     response += "Force: " + String(forceValue) + "\n";
//     server.send(200, "text/plain", response);
// }

// void handleUpdate() {
//     // The values will be passed as JSON
//     String jsonPayload = server.arg("plain");
//     Serial.println("Received JSON: " + jsonPayload);
//     // Here you can parse the JSON payload and update sensor values if needed
//     server.send(200, "text/plain", "Updated");
// }




// 




// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>
// #include <WiFiClient.h>
// #include <DHT.h>
// #include <ArduinoJson.h> // Bibliothèque pour manipuler JSON

// // Configuration parameters for WiFi connection
// const char *ssid = "ALHN-E689";
// const char *password = "077813nasro";

// // Set up the server object
// ESP8266WebServer server;

// // Initialize DHT parameters
// #define DHTPIN D1       // Pin where the DHT11 is connected
// #define DHTTYPE DHT11   // Type of the DHT sensor
// DHT dht(DHTPIN, DHTTYPE);

// // GPIO pin for the force sensor
// const int forceSensorPin = A0; // Assuming analog input

// // Keep track of the sensor data that's going to be sent by the client
// float temperature = 0.0;
// float humidity = 0.0;
// int forceValue = 0; // Variable to hold force sensor data

// // Configuration parameters for the Node.js server
// const char *server_ip = "192.168.1.72"; // IP address of your Node.js server
// const int server_port = 3000;            // Port on which your Node.js server is listening

// // Set up WiFi client to connect to the server
// WiFiClient client;

// void setup() {
//     // Connect to WiFi
//     WiFi.begin(ssid, password);
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(1000);
//         Serial.println("Connecting to WiFi...");
//     }

//     Serial.println("Connected to WiFi");

//     // Print IP Address as a sanity check
//     Serial.begin(115200);
//     Serial.println();
//     Serial.print("IP Address: ");
//     Serial.println(WiFi.localIP());

//     // Configure the server's routes
//     server.on("/", handleIndex);       // use the top root path to report the last sensor value
//     server.begin();

//     // Initialize DHT sensor
//     dht.begin();
// }

// void loop() {
//     server.handleClient();

//     // Read sensor data
//     //temperature = dht.readTemperature();
//     // = dht.readHumidity();

//     // Read force sensor data
//     //forceValue = analogRead(forceSensorPin);
//         // Check if client is connected to the server
//     if (!client.connected()) {
//         // Attempt to connect to the server
//         if (client.connect(server_ip, server_port)) {
//             Serial.println("Connected to server");
//         } else {
//             Serial.println("Connection to server failed");
//         }
//     }

//     // If client is connected to the server, send sensor data
//     if (client.connected()) {
//         sendSensorData();
//     }

//     delay(2000); // wait for 2 seconds before the next reading
// }

//     // Send sensor data to the server
//     //sendSensorData();

//     //delay(2000); // wait for 2 seconds before the next reading
// //}

// void sendSensorData() {
//     // Create a JSON object with sensor data
//     StaticJsonDocument<200> jsonDocument;
//     jsonDocument["temperature"] = temperature;
//     jsonDocument["humidity"] = humidity;
//     jsonDocument["force"] = forceValue;

//     // Serialize JSON to string
//     String jsonString;
//     serializeJson(jsonDocument, jsonString);

//     // Initialize a new HTTP client
//     if (!client.connect(server_ip, server_port)) {
//         Serial.println("Connection to server failed");
//         return;
//     }

//     // Send HTTP POST request to the server's /data endpoint with sensor data as JSON payload
//     client.println("POST /data HTTP/1.1");
//     client.println("Host: " + String(server_ip) + ":" + String(server_port));
//     client.println("Content-Type: application/json");
//     client.println("Content-Length: " + String(jsonString.length()));
//     client.println();
//     client.println(jsonString);

//     Serial.println("Sent data to server");
// }

// void handleIndex() {
//     // Respond with the latest temperature, humidity, and force sensor values
//     String response = "Temperature: " + String(temperature) + " °C\n";
//     response += "Humidity: " + String(humidity) + " %\n";
//     response += "Force: " + String(forceValue) + "\n";
//     server.send(200, "text/plain", response);
// }
