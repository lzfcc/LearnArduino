# https://www.meccanismocomplesso.org/en/tutorial-sending-values-from-pc-to-arduino-by-serial-communication/

import serial
import time

ser = serial.Serial('/dev/tty.usbserial-695290E141', 115200)
file = open('./communication.py')
while True:
    line = file.readline()
    if not line:
        break
    ser.write(line)
    time.sleep(1)