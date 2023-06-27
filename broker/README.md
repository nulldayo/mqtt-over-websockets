# Setting up the broker for MQTT over Websockets
First, install mosquitto (and optionally mosquitto-clients)

```
sudo apt install mosquitto
```

then, change the configuration of mosquitto with nano
```
sudo nano /etc/mosquitto/mosquitto.conf
```

setup the listeners settings: 
```
listener 1883
listener 8080
protocol websockets
allow_anonymous true
```

in the logging section, add the following:
```
connection_messages true
log_timestamp true
```

Now you can run Mosquitto with these settings using the -c flag
```
mosquitto -c /etc/mosquitto/mosquitto.conf
```

If Mosquitto is already running, stop the service
```
sudo systemctl stop mosquitto
````