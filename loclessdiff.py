from colorama import init, Fore
init()
from sys import argv

def endWithS(strn):
    if not strn.endswith('.s'):
        strn += '.s'
    return strn

def noNewline(strn):
    if strn.endswith('\n'):
        strn = strn[:-1]
    return strn

def noAddress(strn):
    if len(strn) >= 8 and strn[8] == ':':
        strn = strn[10:]
    return strn

def noComment(strn):
    if ';' in strn:
        strn = strn[:strn.find(';')]
    return strn

f1 = open(endWithS(argv[1]), 'r')
f2 = open(endWithS(argv[2]), 'r')
n = 1
for line1 in f1.readlines():
    line1 = noComment(noAddress(noNewline(line1)))
    line2 = noComment(noAddress(noNewline(f2.readline())))
    if line1 != line2:
        print(Fore.CYAN + f"[{n:04}]: {line1}")
        print(Fore.YELLOW + f"{{{n:04}}}: {line2}")
    n += 1
f1.close()
f2.close()