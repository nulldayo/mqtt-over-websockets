#include <Preferences.h>
#include <WiFi.h>
#include <WebSocketsClient.h>  // Include before MQTTPubSubClient.h
#include <MQTTPubSubClient.h>
#include <BLEDevice.h> // Use ESP32_BLE_Arduino Library instead of ArduinoBLE

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

const int LED = 2;

static BLEUUID serviceUUID("181A");
static BLEUUID charUUID("2A1C");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
//    huge struggle here lol
//    int tmp = (int)strtol((char*)pData, NULL, 16);
//    float tmp = atof(strrev((char*)pData));
//    float tmp = atof((char*)pData);
    Serial.print("data: ");
//    Serial.println((char*)pData);
//    Serial.println(*pData);
    float value;
    memcpy(&value, (char*)pData,  sizeof((char*)pData));

    mqtt.update();
    digitalWrite(LED, HIGH);
    String topic;
    String payload;
    topic = "/des/mqtt-over-websockets/nicla/";
    payload = "Nicla Sense ME | Humidity: " + String(value) + "*C";
    mqtt.publish(topic, payload);
    Serial.println("MQTT message published: " + payload);
    digitalWrite(LED, LOW);
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback);

    connected = true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks


void setup() {
//  Serial.begin(115200);
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
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

  // Connect to BLE peripheral  
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
}

void loop() {

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }
  delay(1000);
}
