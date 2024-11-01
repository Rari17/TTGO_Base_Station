#include "DataTransmitter.h"
#include <ArduinoJson.h>

/*
// Static Initialisierungen der Queue und des Timers
QueueHandle_t DataTransmitter::messageQueue;
TimerHandle_t DataTransmitter::dataTimer;

DataTransmitter* DataTransmitter::instance = nullptr;

DataTransmitter::DataTransmitter(const char* mqttServer, int mqttPort, const char* mqttUser, const char* mqttPassword)
    : mqttServer(mqttServer), mqttPort(mqttPort), mqttUser(mqttUser), mqttPassword(mqttPassword), mqttClient(espClient) {
    messageQueue = xQueueCreate(10, sizeof(PositionData));
    dataTimer = xTimerCreate("DataTimer", pdMS_TO_TICKS(5000), pdTRUE, 0, queueCallback);

    // Setze die statische Instanz-Variable auf diese Instanz
    instance = this;
}
void DataTransmitter::init(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi...");
    }
    Serial.print("Connected! IP Address: ");
    Serial.println(WiFi.localIP());

    espClient.setInsecure();
    Serial.println("Gehhtt vor MQTT")
    mqttClient.setServer(mqttServer, mqttPort);

    Serial.println("Gehhtt vor MQTT")
    connectToBroker();
}

void DataTransmitter::connectToBroker() {
    if (!mqttClient.connected()) {
        if (mqttClient.connect("ESP32Client", mqttUser, mqttPassword)) {
            mqttClient.subscribe("Basisstation/TTGO");
        }
    }
}

void DataTransmitter::startQueueProcessing() {
    xTimerStart(dataTimer, 0);  // Timer starten
}

// Timer-Callback: Liest Daten aus der Queue und sendet sie an MQTT
void DataTransmitter::queueCallback(TimerHandle_t xTimer) {
    PositionData data;
    if (xQueueReceive(DataTransmitter::messageQueue, &data, 0) == pdPASS) {
        Serial.printf("<%s> Data retrieved from queue. Sending to MQTT...\n", __FUNCTION__);
        
        // Verwende instance, um auf die sendToMQTTBroker-Methode zuzugreifen
        if (instance != nullptr) {
            instance->sendToMQTTBroker(data);
        }
    } else {
        Serial.printf("<%s> No data in queue to send.\n", __FUNCTION__);
    }
}


void DataTransmitter::sendToMQTTBroker(const PositionData& data) {
    StaticJsonDocument<300> JSONencoder;
    JSONencoder["latitude"] = data.lat;
    JSONencoder["longitude"] = data.lng;
    JSONencoder["id"] = data.id;

    char JSONmessageBuffer[300];
    serializeJson(JSONencoder, JSONmessageBuffer);

    if (mqttClient.publish("Basisstation/TTGO", JSONmessageBuffer)) {
        Serial.println("Data sent to MQTT");
    } else {
        Serial.println("Failed to send data to MQTT");
    }
}
  // Subnetzmaske deines Netzwerks
/*

// Konstruktor
 DataTransmitter::DataTransmitter() : mqttClient(espClient) {
    // Mögliche Initialisierung
    }

void DataTransmitter::init(const char* ssid, const char* password) {
    Serial.print("VOR FUNKTION  ");
    WiFi.disconnect(true);  // Setzt das WiFi-Modul zurück
delay(1000);            // Kurze Pause einlegen
//WiFi.begin(ssid, password);

   Serial.print("NACH FUNKTION  ");
    // Warte, bis die WLAN-Verbindung hergestellt ist
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi...");
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Connected to the WiFi network");

    // TLS ohne Zertifikatsüberprüfung
    espClient.setInsecure();
    
    // MQTT-Broker konfigurieren
    mqttClient.setServer(mqttServer, mqttPort);
    connectToBroker();
Serial.print("Free heap before WiFi: ");
Serial.println(ESP.getFreeHeap());
       WiFi.disconnect(true);
    delay(1000);

    // Setze die feste IP-Adresse
    if (!WiFi.config(local_IP, gateway, subnet)) {
        Serial.println("Failed to configure static IP");
    }

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi...");
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Connected to the WiFi network");

    espClient.setInsecure();
    mqttClient.setServer(mqttServer, mqttPort);
    connectToBroker();
}



// Methode zur Herstellung der Brokerverbindung
void DataTransmitter::connectToBroker() {
    while (!mqttClient.connected()) {
        Serial.println("Connecting to MQTT...");
        if (mqttClient.connect("ESP32Client", mqttUser, mqttPassword)) {
            Serial.println("Connected to MQTT Broker");
            mqttClient.subscribe("Basisstation/TTGO");
        } else {
            Serial.print("Failed to connect, rc=");
            Serial.println(mqttClient.state());
            delay(2000);
        }
    }
}

// Verarbeitet Nachrichten aus der Queue und sendet sie weiter
void DataTransmitter::processAndSend(QueueHandle_t messageQueue) {
   if (messageQueue == NULL) {
        Serial.println("Error: Queue handle is NULL");
        return; // Abbruch, wenn die Queue ungültig ist
    }

    PositionData data;  // Lokale Variable vom Typ PositionData
    while (xQueueReceive(messageQueue, &data, portMAX_DELAY)) {  // Empfang aus der Queue
        Serial.println("Data received from queue:");
        Serial.print("ID: "); Serial.println(data.id);
        Serial.print("Latitude: "); Serial.println(data.lat);
        Serial.print("Longitude: "); Serial.println(data.lng);
        // Senden der Daten an den MQTT-Broker und ggf. lokalen Server
        sendToMQTTBroker(data);
        sendToLocalServer(data);
    }
}

// Sendet die Positionsdaten als JSON an den MQTT-Broker
void DataTransmitter::sendToMQTTBroker(const PositionData &data) {
    StaticJsonDocument<300> JSONencoder;
    JSONencoder["device"] = "ESP32";
    JSONencoder["sensorType"] = "GPS_Coordinates";

    // ID hinzufügen
    JSONencoder["id"] = data.id;

    // GPS- und Zeitdaten aus `data` hinzufügen
    JsonObject coordinates = JSONencoder.createNestedObject("Coordinates");
    coordinates["latitude"] = data.lat;
    coordinates["longitude"] = data.lng;

    // Datum und Uhrzeit hinzufügen
    coordinates["date"] = String(data.year) + "-" + String(data.month) + "-" + String(data.day);
    coordinates["time"] = String(data.hour) + ":" + String(data.minutes) + ":" + String(data.seconds);

    // JSON-Nachricht serialisieren
    char JSONmessageBuffer[300];
    serializeJson(JSONencoder, JSONmessageBuffer, sizeof(JSONmessageBuffer));

    // Nachricht veröffentlichen
    Serial.println("Sending message to MQTT topic...");
    Serial.println(JSONmessageBuffer);

    if (mqttClient.publish("Basisstation/TTGO", JSONmessageBuffer)) {
        Serial.println("Success sending message");
    } else {
        Serial.println("Error sending message");
    }

    mqttClient.loop(); // MQTT-Client aufrechterhalten
}


void DataTransmitter::sendToLocalServer(const PositionData &data) {
    // Implementiere Logik zum Senden an lokalen Server
}
*/
#include "DataTransmitter.h"

DataTransmitter::DataTransmitter(const char* ssid, const char* password, const char* mqttServer, int mqttPort, const char* mqttUser, const char* mqttPassword)
    : _ssid(ssid), _password(password), _mqttServer(mqttServer), _mqttPort(mqttPort), _mqttUser(mqttUser), _mqttPassword(mqttPassword), _client(_espClient) {}

void DataTransmitter::begin() {
    connectToWiFi();
    _client.setServer(_mqttServer, _mqttPort);
}

void DataTransmitter::connectToWiFi() {
    WiFi.begin(_ssid, _password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Set insecure mode to allow TLS connection without certificate verification
    _espClient.setInsecure();
}

void DataTransmitter::connectToBroker() {
    if (!_client.connected()) {
        Serial.print("Connecting to MQTT...");
        if (_client.connect("ESP32Client", _mqttUser, _mqttPassword)) {
            Serial.println("connected");
            _client.subscribe("Basisstation/TTGO");
        } else {
            Serial.print("failed, rc=");
            Serial.println(_client.state());
            delay(2000);
        }
    }
}

void DataTransmitter::sendData(float latitude, float longitude) {
    connectToBroker();
    if (_client.connected()) {
        StaticJsonDocument<300> JSONencoder;
        JSONencoder["device"] = "ESP32";
        JSONencoder["sensorType"] = "GPS_Coordinates";

        JsonObject coordinates = JSONencoder.createNestedObject("Coordinates");
        coordinates["latitude"] = latitude;
        coordinates["longitude"] = longitude;
        coordinates["altitude"] = 450.5;  // Beispielwert für Höhe

        char JSONmessageBuffer[300];
        serializeJsonPretty(JSONencoder, JSONmessageBuffer, sizeof(JSONmessageBuffer));

        Serial.println("Sending message to MQTT topic...");
        Serial.println(JSONmessageBuffer);

        if (_client.publish("Basisstation/TTGO", JSONmessageBuffer)) {
            Serial.println("Message sent successfully");
        } else {
            Serial.println("Failed to send message");
        }
    }
    _client.loop();
}
