from os import listdir
from sys import argv

name = argv[1]
if not name.endswith('.s'):
    name += '.s'
existing = listdir()
i = 0
while f"{name[:-2]}.t{i}.s" in existing and i < len(existing):
    i += 1

f = open(name, 'r')
lines = f.readlines()
f.close()
o = open(f"{name[:-2]}.t{i}.s", 'w')

i = 0
while not lines[i] == "==> .text\n":
    i += 1
    assert(i < len(lines))
s = lines[i]
i += 1
while not lines[i].startswith("==>"):
    s += lines[i]
    i += 1
o.write(s)
o.close()