#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LoRa.h>
#include <SPI.h>

const char *ssid = "ZTE_C8AFC2";
const char *password = "5C243EC32H";

AsyncWebServer server(80);
StaticJsonDocument<1024> doc;  // JSON-Dokument für das gesamte JSON-Objekt

JsonArray positionsArray;  // Array für die LoRa-Daten

#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

typedef struct Point {
    byte id;
    double lat, lng;
    byte day, month, year;
    byte hour, minutes, seconds;
} Point;

Point* position;

bool parseString(const char *str, struct Point *position) {
    char *token;

    token = strtok((char *)str, " ");
    if (token == NULL) return false;
    position->id = atoi(token);
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position->hour = atoi(token);
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position->minutes = atoi(token);
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position->seconds = atoi(token);
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position->day = atoi(token);
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position->month = atoi(token);
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position->year = atoi(token);
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position->lat = atof(token) / 100000.0;
    token = strtok(NULL, " ");
    if (token == NULL) return false;
    position->lng = atof(token) / 100000.0;

    return true;
}

void setup() {
    Serial.begin(115200);

    position = new Point;
    WiFi.mode(WIFI_STA);  // WiFi im stationären Modus
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());

    // LoRa initialisieren
    SPI.begin(SCK, MISO, MOSI, SS);
    LoRa.setPins(SS, RST, DIO0);

    if (!LoRa.begin(868.3E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
    Serial.println("LoRa Initialized");

    // JSON-Array initialisieren
    positionsArray = doc.createNestedArray("locations");  // "locations" als Array-Schlüssel hinzufügen

    // POST-Route zum Abrufen des Arrays
    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
        String jsonOutput;
        serializeJson(doc, jsonOutput);
        request->send(200, "application/json", jsonOutput);  // Array der empfangenen Daten senden
    });

    // GET-Route zum Anzeigen der aktuellen Daten
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String jsonOutput;
        serializeJson(doc, jsonOutput);
        request->send(200, "application/json", jsonOutput);  // Array der empfangenen Daten anzeigen
    });

    // 404 Route
    server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "NOT FOUND");
    });

    server.begin();
}

void loop() {
    int packetSize = LoRa.parsePacket();

    if (packetSize) {
        String rec = "";
        while (LoRa.available()) {
            rec += (char)LoRa.read();
        }

        // Empfangenes LoRa-Paket verarbeiten
        Serial.print(rec);
        Serial.print(" with RSSI ");
        Serial.println(LoRa.packetRssi());

        if (!parseString(rec.c_str(), position)) return;

        // Neues JSON-Objekt für die empfangenen Daten erstellen
        JsonObject newPosition = positionsArray.createNestedObject();
        newPosition["lat"] = position->lat;
        newPosition["lng"] = position->lng;
        newPosition["id"] = position->id;
        newPosition["timezone"] = position->hour, position->minutes;
        // JSON-Dokument in der Konsole anzeigen
        serializeJsonPretty(doc, Serial);
        Serial.println("\n\n");
    }
}


