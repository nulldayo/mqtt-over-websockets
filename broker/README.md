# Setting up the broker for MQTT over Websockets
First, install mosquitto (and optionally mosquitto-clients)

```
sudo apt install mosquitto
```
then, change the configuration of mosquitto in /etc/mosquitto/mosquitto.conf or create a new configuration file for mosquitto with nano
```
sudo nano /etc/mosquitto/conf.d/mosquitto.conf
```

add the following lines: 
```
listener 1883
listener 8080
protocol websockets
allow_anonymous true

connection_messages true
log_timestamp true
```

Now Websocket packets will be received on port 8080. Run Mosquitto with these settings using the -c flag
```
mosquitto -c /etc/mosquitto/conf.d/mosquitto.conf
```

Or if Mosquitto is already running, restart the service with
```
sudo systemctl restart mosquitto
````
