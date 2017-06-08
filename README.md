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
* Connect a light sensor to the Arduino and compile the `Carina_CANnode.ino` sketch on it taking care about sensor pin number
* Simulate the car using another Arduino connected to the first one which runs the .ino sketch
* Connect a led to the second Arduino and compile the `Light_CANnode.ino` sketch on it taking care about led pin number

Two commands available: "accendi i fari" (turn headlights on) and "spegni i fari" (turn headlights off)

According to your commands and the light in your room Carina will turn the light on using the CAN-bus interface.

It has been used the SAE J1939 standard for communication.

# External Libraries

For hotwords detection we used [Snowboy](https://github.com/Kitt-AI/snowboy) [1]

For SAE J1939 on Arduino we used [ARD1939](http://copperhilltech.com/sae-j1939-ecu-programming-vehicle-bus-simulation-with-arduino/) library [2]
	
# Licensing
Please read LICENSE file.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

# References

[1] https://snowboy.kitt.ai/

[2] http://copperhilltech.com/ard1939-sae-j1939-protocol-stack-for-aduino/
