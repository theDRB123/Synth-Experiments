import numpy as np
import sounddevice as sd

class Signal:
    duration: float=1.0
    amplitude: float=0.5
    sample_rate: int=44100
    frequency: int=440
    data: np.array
    
def sine_tone(signal: Signal = Signal):
    n_samples = int(signal.sample_rate * signal.duration)
    time_point = np.linspace(0, signal.duration, n_samples, False)
    sine = np.sin(2 * np.pi * signal.frequency * time_point)
    sine *= signal.amplitude
    signal.data = sine
    return signal

def add_tone(signal1: Signal, signal2: Signal):
    signalSum = np.add(signal1.data, signal2.data)
    return signalSum


def main():
    sine1 = sine_tone()
    signal2 = Signal()
    signal2.frequency = 1440
    sine2 = sine_tone(signal2)
    # print(sine)
    signal3 = add_tone(sine1, sine2)
    sd.play(sine1.data)
    sd.wait()
    sd.play(sine2.data)
    sd.wait()
    sd.play(signal3)
    sd.wait()
    
if __name__ == "__main__":
    main() 
