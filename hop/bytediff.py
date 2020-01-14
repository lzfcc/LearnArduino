import os

framesPath = 'frames'
filenames = list(os.listdir(framesPath))
filenames.sort()

rfile = open('{}/image-0001.bin'.format(framesPath), 'rb')
byteArr1 = bytearray(rfile.read())

diffsPath = 'diffs'
os.mkdir(diffsPath)

for k, filename in enumerate(filenames):
    if k == 0:
        continue
    diffs = bytearray()
    rfile = open('{}/{}'.format(framesPath, filename), 'rb')
    byteArr2 = bytearray(rfile.read())
    assert len(byteArr1) == len(byteArr2)
    for i, bt in enumerate(byteArr2):
        diffs.append(bt ^ byteArr1[i])
    wfile = open('{}/diff-{:0>4d}.bin'.format(diffsPath, k), 'wb')
    wfile.write(diffs)
    byteArr1 = byteArr2
    rfile.close()
    wfile.close()