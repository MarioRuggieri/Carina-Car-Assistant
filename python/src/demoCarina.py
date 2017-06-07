import sys
import signal
from callbacks import *
import snowboydecoder


def signal_handler(signal, frame):
    global interrupted
    interrupted = True

if __name__ == '__main__':
    dir = os.path.dirname(os.path.abspath(__file__))
    models = os.path.join(dir, '../models/')
    
    Microphone.start()
    Speaker.activate()

    IntelligentSystem.start()
    
    models = [models + 'carina.pmdl', models + 'accendiFari.pmdl', models + 'spegniFari.pmdl']
    callbacks = [carinaCallback, lightOnCallback, lightOffCallback]

    # capture SIGINT signal, e.g., Ctrl+C
    signal.signal(signal.SIGINT, signal_handler)

    sensitivity = [0.48]*len(models)
    detector = snowboydecoder.HotwordDetector(models, sensitivity=sensitivity)
    print('Listening... Press Ctrl+C to exit')

    # main loop
    detector.start(detected_callback=callbacks, #snowboydecoder.play_audio_file, 
                                                    sleep_time=0.03)
    detector.terminate()

    Microphone.stop()
