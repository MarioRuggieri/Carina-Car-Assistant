# callbacks.py file
# definition of all callbacks for snowboy detector
from carina import *

dir = os.path.dirname(os.path.abspath(__file__))
res = os.path.join(dir, '../resources/')

interrupted = False
carina = False

def interrupt_callback():
    global interrupted
    return interrupted
       
def lightOnCallback():
    global carina
    if carina:
        #light = HeadLight.getIstance()
        light = HeadLight()
        if not(light.isON()):
            light.turnON()
            print("light on!")
        else:
            print('the light is already on!')
            Speaker.play(res + "fariGiaAccesi.mpeg")
        carina = False
        
def lightOffCallback():
    global carina
    if carina:
        #light = HeadLight.getIstance()
        light = HeadLight()
        if light.isON():
            light.turnOFF()
            print("light off!")
        else:
            print('the light is already off!')
            Speaker.play(res + "fariGiaSpenti.mpeg")
        carina = False

def carinaCallback():
    Speaker.play(res + "dimmi.wav")
    print("Tell me!")
    global carina
    carina = True
