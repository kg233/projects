import re,random,goody

    
def repl(m):
    inner = list(m.group(2)) # separate characters in (\w+) group as list
    random.shuffle(inner)    # shuffle the list
    return m.group(1) + "".join(inner) + m.group(3) # list->string

for l in goody.safe_open('File to translate','r','Reenter file name', default='sub.txt'):
    print(re.sub(r'(\w)(\w+)(\w)', repl, l.strip()))
