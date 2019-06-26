import serial
from time import sleep


ser = serial.Serial('/dev/serial0', 16000, timeout=None,parity=serial.PARITY_NONE)
i=0
data = [2,5,123,24]
while True:
    try:
		while data[i]!=NULL:
			ser.write(data[i])
			i++;
    except KeyboardInterrupt:
        break

print 'Done!'
