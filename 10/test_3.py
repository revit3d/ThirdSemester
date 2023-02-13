import struct

rows, cols = map(int, input().split())
with open('3.bin', 'rb') as f:
    for i in range(rows):
        for j in range(cols):
            print(struct.unpack('i', f.read(4))[0], end='\t')
        print()