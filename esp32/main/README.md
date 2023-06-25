# Edit Wi-Fi credentials
The Wi-Fi credentials are hardcoded in /credentials/credentials.ino. Run [Ruis Santos' code](https://github.com/RuiSantosdotme/Random-Nerd-Tutorials/blob/master/Projects/ESP32/Preferences/ESP32_Save_Credentials.ino "Random Nerd Tutorials") to save your WiFi credentials on the SPIFFS of the ESP32.

# Edit MQTT credentials
The MQTT credentials in main.ino are set to public.cloud.shiftr.io with port number 443. The MQTT user and password are both "public". For the project task this has to be changed to a private MQTT broker.

## Some public MQTT brokers
[A List of public MQTT brokers](https://github.com/tigoe/mqtt-examples/blob/main/readme.md "MQTT examples")