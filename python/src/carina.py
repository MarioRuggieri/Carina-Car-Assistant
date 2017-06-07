# carina.py file
# classes composing Carina
import pygame
import serial
import threading
from subprocess import call

import warnings
import serial.tools.list_ports
import os


port = ""
ports = list(serial.tools.list_ports.comports())
for p in ports:
    port = p[0]
    print("Using port: " + port)
    
ser = serial.Serial(port, 9600)

class Microphone:
    @staticmethod
    def start():
        call(['pulseaudio',  '--start'])
        call(['jack_control', 'start'])

    @staticmethod
    def stop():
        call(['jack_control',  'stop'])
        call(['pulseaudio',  '--kill'])

class Speaker:
    @staticmethod
    def activate():
        pygame.mixer.init()
        
    @staticmethod
    def play(soundFile):
        pygame.mixer.music.load(soundFile)
        pygame.mixer.music.play()

#defining metaclass Singleton overriding __call__ method to
#change the behaviour when calling a class of type Singleton
class Singleton(type):
    __istances = {}

    def __call__(cls, *args, **kwargs):
        if cls not in cls.__istances:
            cls.__istances[cls] = super(Singleton,cls).__call__(*args, **kwargs)
        return cls.__istances[cls]

class HeadLight():
    __metaclass__ = Singleton
    __on = False
    
    def turnON(self):
        ser.write('turnOn')
        
    def turnOFF(self):
        ser.write('turnOff')
        
    def isON(self):
        return self.__on

    def setON(self,on):
        self.__on = on


class IntelligentSystem:
    @staticmethod
    def start():
        threading.Thread(target=IntelligentSystem.eventListener).start()
        
    @staticmethod
    def eventListener():
        dir = os.path.dirname(os.path.abspath(__file__))
        res = os.path.join(dir, '../resources/')
        while(True):
            command = ser.readline()
            if 'lightOn' in command or 'on_success' in command:
                light = HeadLight()
                light.setON(True)
                Speaker.play(res + "fariAccesi.mpg")
            elif 'lightOff' in command or 'off_success' in command:
                light = HeadLight()
                light.setON(False)
                Speaker.play(res + "fariSpenti.mpg")

 


