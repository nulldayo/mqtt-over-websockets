#include <Preferences.h>
#include <WiFi.h>
#include <WebSocketsClient.h>  // Include before MQTTPubSubClient.h
#include <MQTTPubSubClient.h>
#include <DHT.h>

Preferences preferences;
WebSocketsClient client;
MQTTPubSubClient mqtt;

// For reading preferences
String device;
String ssid;
String password;

// MQTT credentials
const char* mqtt_host = "public.cloud.shiftr.io";
const unsigned int mqtt_port = 443;
const char* mqtt_user = "public";
const char* mqtt_pass = "public";

// Assign Pins
const int LED = 2;
const int PIN_DHT = 4;
// const int PWM_CH = 0;
DHT dht(PIN_DHT,DHT11);

void setup() {
    Serial.begin(115200);
    
    pinMode(PIN_DHT, INPUT);
    pinMode(LED, OUTPUT);
//    ledcSetup(PWM_CH, 1000, 12);
//    ledcAttachPin(LED, 0);
    dht.begin();

    // Read credentials from flash memory
    Serial.print("Getting credentials for WLAN ..");
    preferences.begin("credentials", false);
    device = preferences.getString("device", "");
    ssid = preferences.getString("ssid", ""); 
    password = preferences.getString("password", "");

    if (ssid == "" || password == ""){
      Serial.println(" no values were saved for ssid or password!");
    }
    else {
    Serial.println(" complete!");
    // Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(1000);
    }
    Serial.println(" connected as " + WiFi.localIP().toString());  
  }
    // Connect to Host with MQTT over WebSocket
    Serial.print("Connecting to MQTT broker ..");
    client.beginSSL(mqtt_host, mqtt_port);
    client.setReconnectInterval(2000);

    // Initialize the MQTT client
    mqtt.begin(client);

    while (!mqtt.connect(device, mqtt_user, mqtt_pass)) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println(" connected!");

    // Subscribe callback which is called when every packet has come
    mqtt.subscribe([](const String& topic, const String& payload, const size_t size) {
        Serial.println("MQTT message received: " + topic + " - " + payload);
    });

    // Subscribe topic and callback which is called when /des/mqtt-over-websockets has come
    mqtt.subscribe("/des/mqtt-over-websockets/#", [](const String& payload, const size_t size) {
        Serial.print("/des/mqtt-over-websockets/#");
        Serial.println(payload);
    });
}

void loop() {
//    unsigned int ledPow = 0;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    String topic;
    String payload;
    
    // Should be called to trigger callbacks
    mqtt.update();
    
    // Publish message
    static uint32_t prev_ms = millis();
    if (millis() > prev_ms + 1000) {
        prev_ms = millis();
        digitalWrite(LED, HIGH);
//        ledPow = 0x0FFF;
        if(isnan(t) || isnan(h)){
          Serial.println("Failed to read from DHT.");
        return;
        }else{
          topic = "/des/mqtt-over-websockets/esp32/";
          payload = device + " | Humidity: " + String(h) + " Temperature: " + String(t) + "*C";
          mqtt.publish(topic, payload);
          Serial.println("MQTT message published: " + payload);
        }
    }
    digitalWrite(LED, LOW);
//    ledcWrite(PWM_CH, ledPow);
}
