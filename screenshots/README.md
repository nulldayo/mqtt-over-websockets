# Visualized Packets on Wireshark
![Wireshark UI showing capture of MQTT packets](../screenshots/3.png?raw=true "MQTT QoS-2")
... the above picture shows the behaviour of three published messages on MQTT from QoS-0 to QoS-2.

![MQTT Explorer configuration](../screenshots/4.png?raw=true "MQTT Explorer") 
shows the configuration settings on MQTT Explorer to connect to the local Raspberry Pi.

![Wireshark UI showing capture of WS packets from MQTT Explorer](../screenshots/5.png?raw=true "WebSocket connection from MQTT Explorer") 
shows the initial connection of MQTT Explorer to the Broker on the pi. Here the MQTT Explorer polls the Broker on information periodically.

![MQTT Explorer publish settings](../screenshots/6.png?raw=true "MQTT Explorer") 
shows the publish window of MQTT Explorer.

![MQTT Explorer showing topics](../screenshots/7.png?raw=true "MQTT Explorer") 
shows the various topics of the MQTT Broker (pi)

![Wireshark UI showing capture of WebSocket packets](../screenshots/8.png?raw=true "MQTT over WebSockets") 
here you can see how the packet arrived at the MQTT broker. packet no. 352 to 359 are showing the communication sequence between the VM and pi. Packet no. 356 is the subscription of mosquitto on the VM for the pi. The entire communication proceeds over TCP (1883), since WebSockets is a TCP-based protocol.

![Wireshark UI showing capure of WebSocket packets](../screenshots/9.png?raw=true "MQTT over WebSockets") 
the content of the mqtt message (over ws) can also be seen in packet no. 358.
