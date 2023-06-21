void loop() {
  
    // Should be called to trigger callbacks
    mqtt.update();
    
    // Publish message
    static uint32_t prev_ms = millis();
    if (millis() > prev_ms + 1000) {
        prev_ms = millis();
        mqtt.publish("/des_topic", "Hello World!");
    }
}
