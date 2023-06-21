#include <Preferences.h>
#include <WiFi.h>
#include <WebSocketsClient.h>  // Include before MQTTPubSubClient.h
#include <MQTTPubSubClient.h>

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
