# Carina-Car-Assistant
Carina is a car assistant project to make driving simpler. 

Connect Carina to your car and talk to her! She turns headlights on, moves driving mirrors and so on obeying your voice!

It is born in the University of Naples Parthenope by three master students in Computer Science.

Actually it's a prototype which simulate the communication to the car using an Arduino.

The language supported is Italian because this is an italian project.

# Authors
Vincenzo De Angelis

e-mail: vincenzo.deangelis2@studenti.uniparthenope.it

Gennaro Farina

e-mail: gennaro.farina@studenti.uniparthenope.it

Mario Ruggieri

e-mail: mario.ruggieri@uniparthenope.it

# Installation

The embedded system is composed of two nodes:

* A single board computer responsible of hotwords detection and feedback sounds generation. The code is in the `python` folder. 

* An Arduino node connected to SBC with a CAN-bus shield which will communicate to the car. The code is in the `arduino` folder.

To simulate the car you can use another arduino node with a second CAN-bus shield.

To make it working:

* Launch `demoCarina.py` on the single board computer (e.g. Raspberry Pi) 
* Connect it to an Arduino UNO or MEGA with a CAN-bus shield.
* Connect a light sensor to the Arduino and compile* the `Carina_CANnode.ino` sketch on it taking care about sensor pin number
* Simulate the car using another Arduino connected to the first one which runs the .ino sketch
* Connect a led to the second Arduino and compile* the `Light_CANnode.ino` sketch on it taking care about led pin number

Two commands available: "accendi i fari" (turn headlights on) and "spegni i fari" (turn headlights off)

According to your commands and the light in your room Carina will turn the light on using the CAN-bus interface.

It has been used the SAE J1939 standard for communication.

(*)You need to copy ARD1939 directory into your Arduino/libraries directory

# External Libraries

For hotwords detection we used [Snowboy](https://github.com/Kitt-AI/snowboy) [1] which is <b>Apache Licensed</b>.

For SAE J1939 on Arduino we used [ARD1939](http://copperhilltech.com/sae-j1939-ecu-programming-vehicle-bus-simulation-with-arduino/) library [2] which is under <b>GNU v3.0 License</b>
	
# Licensing
Please read LICENSE file.

# References

[1] https://snowboy.kitt.ai/

[2] http://copperhilltech.com/ard1939-sae-j1939-protocol-stack-for-aduino/
