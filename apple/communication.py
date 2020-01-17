import serial
import time
import os

ser = serial.Serial('/dev/tty.usbserial-695290E141', 115200)

path = './frames'
filenames = filter(lambda s: s.startswith('image'), list(os.listdir(path)))
filenames.sort()

# move files into separate directories...
# image2lcd may run too long if converting all images at one time ...
# path = './bmps'
# filenames = list(os.listdir(path))
# filenames.sort()

# maxCnt = 500
# cnt = 0
# dirIdx = 1
# mvDir = ''
# for filename in filenames:
#     if cnt == 0:
#         mvDir = path + '/' + str(dirIdx)
#         os.mkdir(mvDir)
#     os.rename(path + '/' + filename, mvDir + '/' + filename)
#     cnt += 1
#     if cnt >= maxCnt:
#         cnt = 0
#         dirIdx += 1
#
# # run shell: mv bmps/[1-6]/batch/image-*.bin frames

print('__START__')
for filename in filenames:
    file = open(path + '/' + filename)
    while True:
        line = file.read(1) #read 1 byte, readline() seems not to work fine
        if not line:
            break
        ser.write(line)
    print(filename)
    time.sleep(2)

print('__The End__')