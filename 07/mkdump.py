import subprocess

with open('dump.txt', 'w') as f:
    for i in range(1902, 2038):
        s = subprocess.check_output(['./5', str(i)])
        f.write(s.decode())