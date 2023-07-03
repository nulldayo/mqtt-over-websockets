# Visualized Packets on Wireshark
The ![Alt text](../screenshots/1.png?raw=true "Optional Title") shows the behaviour of a normal QoS-0 published message on MQTT over TCP (1883).
![Alt text](../screenshots/2.png?raw=true "Optional Title") shows the behaviour of a normal QoS-1 sequence.
![Alt text](../screenshots/3.png?raw=true "Optional Title") shows the behaviour of a normal QoS-2 sequence.
![Alt text](../screenshots/4.png?raw=true "Optional Title") shows the configuration settings on MQTT Explorer.
![Alt text](../screenshots/5.png?raw=true "Optional Title") shows the initial connection of MQTT Explorer to the Broker on the pi.
![Alt text](../screenshots/6.png?raw=true "Optional Title") shows the publish window of MQTT Explorer.
![Alt text](../screenshots/7.png?raw=true "Optional Title") shows the various topics of the MQTT Broker (pi)
![Alt text](../screenshots/8.png?raw=true "Optional Title") here you can see how the packet arrived at the MQTT broker. packet no. 352 to 359 are showing the communication sequence between the VM and pi. Packet no. 356 is the subscription of mosquitto on the VM for the pi.
![Alt text](../screenshots/9.png?raw=true "Optional Title") the content of the mqtt message (over ws) can also be seen in packet no. 358.