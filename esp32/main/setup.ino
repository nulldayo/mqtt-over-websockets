void setup() {
    Serial.begin(115200);

    // Read credentials from flash memory
    Serial.println("Getting credentials for WLAN ..");
    preferences.begin("credentials", false);
    device = preferences.getString("device", "");
    ssid = preferences.getString("ssid", ""); 
    password = preferences.getString("password", "");

    if (ssid == "" || password == ""){
      Serial.println(" no values were saved for ssid or password!");
    }
    else {
    // Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(1000);
    }
    Serial.println(" connected as " + WiFi.localIP());  
  }
    // Connect to Host with MQTT over WebSocket
    Serial.println("Connecting to MQTT broker ..");
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

    // Subscribe topic and callback which is called when /des_topic has come
    mqtt.subscribe("/des_topic", [](const String& payload, const size_t size) {
        Serial.print("/des_topic ");
        Serial.println(payload);
    });
}
