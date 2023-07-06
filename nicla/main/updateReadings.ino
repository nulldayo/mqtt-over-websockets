void updateReadings() {
    float measured_temp = temperature.value();
    float measured_pres = pressure.value();

    if (measured_temp != previousTemperature) { // If reading has changed
        Serial.print("Temperature: ");
        Serial.println(measured_temp);
        tempCharacteristic.writeValue(measured_temp); // Update characteristic
        previousTemperature = measured_temp;          // Save value
    }
    if (measured_pres != previousPressure) { // If reading has changed
        Serial.print("Pressure: ");
        Serial.println(measured_pres);
        pressureCharacteristic.writeValue(measured_pres);
        previousPressure = measured_pres;
    }
}
