import os
if os.path.exists('pose.txt'):
    os.remove('pose.txt')
with open('laserlogs.log', 'r') as f:
    txt = f.read()
log = txt.split(';')
log = log[:-1]
for i in range(len(log)):
    filename = 'log'+str(i+1)+'.txt'
    txt = log[i]
    txt = txt.split('\n')
    pose = txt[0] if txt[0] else txt[1]
    laser = txt[1:] if txt[0] else txt[2:]
    laser = '\n'.join(laser)
    with open(filename, 'w+') as f:
        f.write(laser)
    with open('pose.txt', 'a+') as f:
        f.write(pose+'\n')