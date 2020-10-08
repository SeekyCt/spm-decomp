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
fun = argv[3]
lines1 = f1.readlines()
lines2 = f2.readlines()
f1.close()
f2.close()
n1 = 0
n2 = 0
f = ""
while n1 < len(lines1):
    line1 = noComment(noAddress(noNewline(lines1[n1])))
    line2 = noComment(noAddress(noNewline(lines2[n2])))
    
    if line1.startswith('Hunk:'):
        f = line1.split('"')[1]
    if line1 != line2:
        if line1 == '':
            n2 += 1
        elif line2 == '':
            n1 += 1
        else:
            if f == fun:
                print(Fore.CYAN + f"[{n1 + 1:04}]: {line1}")
                print(Fore.YELLOW + f"{{{n2 + 1:04}}}: {line2}")
            n1 += 1
            n2 += 1
    else :
        n1 += 1
        n2 += 1
f1.close()
f2.close()