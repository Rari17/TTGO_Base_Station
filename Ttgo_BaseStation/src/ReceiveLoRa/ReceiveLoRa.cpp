#include "ReceiveLoRa.h"
#include <Arduino.h>
/*
// Konstruktor: Initialisiert die Queue auf nullptr
ReceiveLoRa::ReceiveLoRa() : messageQueue(nullptr) {}

// Initialisiert das LoRa-Modul und erstellt die Queue
void ReceiveLoRa::init() {
    // Initialisiere das LoRa-Modul
    if (!LoRa.begin(868E6)) {
        Serial.println("LoRa init failed. Check your connections.");
        while (true);
    }
    Serial.println("LoRa initialized");

    // Erstelle die Queue für empfangene Positionsdaten
    messageQueue = xQueueCreate(10, sizeof(PositionData));
    if (messageQueue != nullptr) {
        Serial.println("Queue created successfully");
    } else {
        Serial.println("Failed to create queue");
    }
}

// Empfängt Nachrichten über LoRa und fügt sie der Queue hinzu
bool ReceiveLoRa::receiveAndQueue() {
    if (LoRa.parsePacket()) {
        char receivedMessage[256];
        int index = 0;
        
        while (LoRa.available() && index < sizeof(receivedMessage) - 1) {
            receivedMessage[index++] = LoRa.read();
        }
        receivedMessage[index] = '\0';  // Zeichenkette terminieren

        PositionData position;
        if (parseMessage(receivedMessage, position)) {
            // Daten in die Queue einfügen
            if (xQueueSendToBack(messageQueue, &position, 0) != pdPASS) {
                Serial.println("Failed to enqueue position data");
                return false;
            }
            Serial.println("Position data enqueued");
            return true;
        }
    }
    return false;
}

// Gibt das Queue-Handle zurück, um in anderen Klassen darauf zugreifen zu können
QueueHandle_t ReceiveLoRa::getQueueHandle() const {
    return messageQueue;
}

// Parse-Funktion zur Verarbeitung des empfangenen Nachrichtenformats
bool ReceiveLoRa::parseMessage(const char *str, PositionData &position) {
    char *token;
    token = strtok((char *)str, " ");
    if (token == NULL) return false;
    position.id = atoi(token);

    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position.hour = atoi(token);

    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position.minutes = atoi(token);

    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position.seconds = atoi(token);

    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position.day = atoi(token);

    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position.month = atoi(token);

    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position.year = atoi(token);

    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position.lat = atof(token) / 100000.0;
    
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position.lng = atof(token) / 100000.0;

    return true;
}
*/


#include "ReceiveLoRa.h"

ReceiveLoRa::ReceiveLoRa(int sckPin, int misoPin, int mosiPin, int ssPin, int rstPin, int dio0Pin, long frequency)
    : _sckPin(sckPin), _misoPin(misoPin), _mosiPin(mosiPin), _ssPin(ssPin), _rstPin(rstPin), _dio0Pin(dio0Pin), _frequency(frequency) {}

void ReceiveLoRa::begin() {
    SPI.begin(_sckPin, _misoPin, _mosiPin, _ssPin);
    LoRa.setPins(_ssPin, _rstPin, _dio0Pin);
    if (!LoRa.begin(_frequency)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
    Serial.println("LoRa Initialized");
}

bool ReceiveLoRa::receiveData(Point &position) {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        String rec = "";
        while (LoRa.available()) {
            rec += (char)LoRa.read();
        }
        Serial.print("Received LoRa packet: ");
        Serial.println(rec);

        return parseString(rec.c_str(), position);
    }
    return false;
}

bool ReceiveLoRa::parseString(const char *str, Point &position) {
    char *token;
    token = strtok((char *)str, " ");
    if (token == NULL) return false;
    position.id = atoi(token);
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position.hour = atoi(token);
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position.minutes = atoi(token);
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position.seconds = atoi(token);
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position.day = atoi(token);
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position.month = atoi(token);
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position.year = atoi(token);
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position.lat = atof(token) / 100000.0;
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position.lng = atof(token) / 100000.0;
    return true;
}
