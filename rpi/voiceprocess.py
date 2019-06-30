import matplotlib.pyplot as plt
import numpy as np
import wave
import sys
import math
import contextlib
import time
import serial

ser = serial.Serial('/dev/serial0',115200,timeout=None,parity=serial.PARITY_NONE)

fname = 'voice3.wav'
outname = 'voice2.wav'

cutOffFrequency = 400.0

def running_mean(x, windowSize):
  cumsum = np.cumsum(np.insert(x, 0, 0))
  return (cumsum[windowSize:] - cumsum[:-windowSize]) / windowSize

def interpret_wav(raw_bytes, n_frames, n_channels, sample_width, interleaved = True):

    if sample_width == 1:
        dtype = np.uint8 # unsigned char
    elif sample_width == 2:
        dtype = np.int16 # signed 2-byte short
    else:
        raise ValueError("Only supports 8 and 16 bit audio formats.")

    channels = np.fromstring(raw_bytes, dtype=dtype)

    if interleaved:
        channels.shape = (n_frames, n_channels)
        channels = channels.T
    else:
        channels.shape = (n_channels, n_frames)

    return channels

with contextlib.closing(wave.open(fname,'rb')) as spf:
    print 'wav file has sucessfully changed into digital signal'
    sampleRate = spf.getframerate()
    ampWidth = spf.getsampwidth()
    nChannels = spf.getnchannels()
    nFrames = spf.getnframes()

    signal = spf.readframes(nFrames*nChannels)
    spf.close()

    ser.write(signal)
    print 'digital signal has transmitted'
    filtered = ser.read()
    print 'signal recived'
    ser.close()

    filtered = running_mean(channels[0], N).astype(channels.dtype)

    wav_file = wave.open(outname, "w")
    wav_file.setparams((1, ampWidth, sampleRate, nFrames, spf.getcomptype(), spf.getcompname()))
    wav_file.writeframes(filtered.tostring('C'))
    wav_file.close()
