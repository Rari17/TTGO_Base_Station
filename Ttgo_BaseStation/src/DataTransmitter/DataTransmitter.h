#ifndef DATA_TRANSMITTER_H
#define DATA_TRANSMITTER_H

#include <WiFi.h>
#include <WiFiClientSecure.h>  // Für sichere Verbindung mit MQTT
#include <PubSubClient.h>
#include "./ReceiveLoRa/ReceiveLoRa.h"  // Für Zugriff auf PositionData-Struktur
/*
class DataTransmitter {
public:
    DataTransmitter();

    // Initialisiert das WLAN und stellt die Verbindung zum MQTT-Broker her
    void init(const char* ssid, const char* password);

    // Verarbeitet Nachrichten aus der Queue und sendet sie weiter
    void processAndSend(QueueHandle_t messageQueue);

private:
    WiFiClientSecure espClient; // WiFiClientSecure für eine sichere Verbindung
    PubSubClient mqttClient;    // PubSubClient für MQTT

    const char* mqttServer = "8819b63f076046d2b857dd11d9a6007b.s1.eu.hivemq.cloud";
    const int mqttPort = 8883;
    const char* mqttUser = "RaresTest";
    const char* mqttPassword = "HTLPlusCoo2025";

    // Methoden für die Brokerverbindung und das Senden von Nachrichten
    void connectToBroker();
    void sendToMQTTBroker(const PositionData &data);
    void sendToLocalServer(const PositionData &data);
};

#endif // DATA_TRANSMITTER_H
*/
/*
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/queue.h"
#include "./ReceiveLoRa/ReceiveLoRa.h"  // Für Zugriff auf PositionData-Struktur

class DataTransmitter {

public:
    // Konstruktor
    DataTransmitter(const char* mqttServer, int mqttPort, const char* mqttUser, const char* mqttPassword);

    // Initialisiert WLAN und MQTT-Verbindung
    void init(const char* ssid, const char* password);

    // Startet die Queue-Verarbeitung
    void startQueueProcessing();

    // Stellt die Verbindung zum MQTT-Broker her
    void connectToBroker();

    // Stellt diese Instanz als `static`-Referenz bereit
    static DataTransmitter* instance;
    void sendToMQTTBroker(const PositionData& data);

private:
    // Timer-Callback zum Lesen aus der Queue
    static void queueCallback(TimerHandle_t xTimer);

    // Sendet Daten an den MQTT-Broker
    

    WiFiClientSecure espClient;
    PubSubClient mqttClient;
    const char* mqttServer;
    int mqttPort;
    const char* mqttUser;
    const char* mqttPassword;

    // FreeRTOS Queue und Timer
    static QueueHandle_t messageQueue;
    static TimerHandle_t dataTimer;
};

#endif*/


#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

class DataTransmitter {
public:
    DataTransmitter(const char* ssid, const char* password, const char* mqttServer, int mqttPort, const char* mqttUser, const char* mqttPassword);
    void begin();
    void sendData(float latitude, float longitude);

private:
    void connectToWiFi();
    void connectToBroker();

    const char* _ssid;
    const char* _password;
    const char* _mqttServer;
    int _mqttPort;
    const char* _mqttUser;
    const char* _mqttPassword;

    WiFiClientSecure _espClient;
    PubSubClient _client;
};

#endif
