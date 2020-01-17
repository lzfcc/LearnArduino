import serial
import time
import os

ser = serial.Serial('/dev/tty.usbserial-695290E141', 115200)

path = 'compress'
filenames = filter(lambda s: s.startswith('image'), list(os.listdir(path)))
filenames.sort()

print('__START__')
for filename in filenames:
    file = open(path + '/' + filename)
    while True:
        line = file.read(1) #read 1 byte, readline() seems not to work fine
        if not line:
            break
        ser.write(line)
    print(filename)
    time.sleep(0.2)

print('__The End__')