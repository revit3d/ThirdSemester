with open('4.bin', 'rb') as f:
    bb = ''.join([bin(int(i, base=16))[2:].rjust(4, '0') for i in bytes(f.read()).hex()])
    bb = [(bb[i:i + 8])[-1::-1] for i in range(0, len(bb), 8)]
    print(bb)