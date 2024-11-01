#include <Arduino.h>
#include "./ReceiveLoRa/ReceiveLoRa.h"
//#include "./DataTransmitter/DataTransmitter.h"

//Erstellen von Instanzen für LoRaReceiver und DataTransmitter
//ReceiveLoRa loRaReceiver;
//DataTransmitter dataTransmitter;

/*void setup() {
    Serial.begin(115200);

    // LoRa- und WLAN/MQTT-Initialisierung
    loRaReceiver.init();                         // LoRa-Modul initialisieren
   dataTransmitter.init("iPhone_von_Rares", "Jooojo187Johid");    // WLAN und MQTT-Broker initialisieren

    // Task zur Ausführung der LoRa-Empfangsfunktion
   xTaskCreatePinnedToCore(
        [](void* param) {
            while (true) {
                loRaReceiver.receiveAndQueue();
                delay(10); // Kurze Verzögerung zur Stabilisierung
            }
        },
        "LoRaReceiveTask",  // Task-Name
        15000,              // Stack-Größe
        NULL,               // Parameter (keiner benötigt)
        1,                  // Priorität
        NULL,               // Task-Handle (nicht erforderlich)
        1                   // Kern-ID (1 oder 0)
    );

    // Task zur Verarbeitung und Übertragung der Daten
    xTaskCreatePinnedToCore(
        [](void* param) {
            dataTransmitter.processAndSend(loRaReceiver.getQueueHandle());
        },
        "DataTransmitTask", // Task-Name
        15000,              // Stack-Größe
        NULL,               // Parameter (keiner benötigt)
        1,                  // Priorität
        NULL,               // Task-Handle (nicht erforderlich)
        0                   // Kern-ID (1 oder 0)
    );
    
}

void loop() {
    // Hauptloop bleibt leer, da alle Tasks parallel laufen
}
*/
/*
void setup() {
    Serial.begin(115200);

    loRaReceiver.init();
    dataTransmitter.init("iPhone_von_Rares", "Jooojo187Johid");
}

void loop() {
    //loRaReceiver.receiveAndQueue();
    //dataTransmitter.processAndSend(loRaReceiver.getQueueHandle());
    delay(1000);  // Simulierter Verzögerung zur Stabilisierung
}
*/

/*
#include <Arduino.h>


// Netzwerk- und MQTT-Zugangsdaten
#include <Arduino.h>
#include "./ReceiveLoRa/ReceiveLoRa.h"
#include "./DataTransmitter/DataTransmitter.h"
const char* ssid = "ZTE_C8AFC2";
const char* password = "5C243EC32H";
const char* mqttServer = "8819b63f076046d2b857dd11d9a6007b.s1.eu.hivemq.cloud";
const int mqttPort = 8883;
const char* mqttUser = "RaresTest";
const char* mqttPassword = "HTLPlusCoo2025";
// Netzwerk- und MQTT-Zugangsdaten
// Instanzen von ReceiveLoRa und DataTransmitter erstellen
  // Stelle sicher, dass dies nur einmal existiert

 ReceiveLoRa loRaReceiver;
DataTransmitter dataTransmitter(mqttServer, mqttPort, mqttUser, mqttPassword);
void setup() {
    Serial.begin(115200);
   
    // Initialisiere WLAN und MQTT
    dataTransmitter.init(ssid, password);       // Stellt WLAN-Verbindung her und verbindet zum MQTT-Broker

    // Initialisiere das LoRa-Modul
    loRaReceiver.init();                        // LoRa-Modul und Queue initialisieren
}

void loop() {
    // Empfange Daten über LoRa und speichere sie in der Queue
    if (loRaReceiver.receiveAndQueue()) {
        Serial.println("Data received and enqueued.");

        // Daten aus der Queue holen und an den MQTT-Broker senden
        PositionData data;
        if (xQueueReceive(loRaReceiver.getQueueHandle(), &data, 0) == pdPASS) {
            dataTransmitter.sendToMQTTBroker(data);  // Sende Daten an MQTT-Broker
        }
    }
    delay(1000);  // Testweise Verzögerung zur Stabilisierung
}*/

#include <Arduino.h>
#include "./DataTransmitter/DataTransmitter.h"
#include "./ReceiveLoRa/ReceiveLoRa.h"

// Definiere LoRa-Pins und -Frequenz
#include <Arduino.h>


#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26
#define FREQUENCY 868.3E6  // LoRa-Frequenz

const char *ssid = "ZTE_C8AFC2";
const char *password = "5C243EC32H";
const char* mqttServer = "8819b63f076046d2b857dd11d9a6007b.s1.eu.hivemq.cloud";
const int mqttPort = 8883;
const char* mqttUser = "RaresTest";
const char* mqttPassword = "HTLPlusCoo2025";

ReceiveLoRa loraReceiver(SCK, MISO, MOSI, SS, RST, DIO0, FREQUENCY);
DataTransmitter dataTransmitter(ssid, password, mqttServer, mqttPort, mqttUser, mqttPassword);
Point position;

void setup() {
    Serial.begin(115200);
    loraReceiver.begin();
    dataTransmitter.begin();
}

void loop() {
    if (loraReceiver.receiveData(position)) {
        Serial.print("Received Position - ID: ");
        Serial.print(position.id);
        Serial.print(", Latitude: ");
        Serial.print(position.lat, 6);
        Serial.print(", Longitude: ");
        Serial.print(position.lng, 6);
        Serial.print(", Date: ");
        Serial.print(position.day); Serial.print("/");
        Serial.print(position.month); Serial.print("/");
        Serial.print(position.year);
        Serial.print(", Time: ");
        Serial.print(position.hour); Serial.print(":");
        Serial.print(position.minutes); Serial.print(":");
        Serial.println(position.seconds);
        Serial.println("----");

        dataTransmitter.sendData(position.lat, position.lng);
    }
    //delay(1000);
}
