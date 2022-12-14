import re
file = open("input.txt","r")

lines = file.read().splitlines()

result = ""

flag = False

stks = [[] for x in range(9)]

#print(lines)

for line in lines:
    if not flag:
        if(len(line) == 0):
            for i in range(9):
                stks[i].reverse()
            flag = True
        else:
            i = 0
            while(i < len(line)):
                if line[i] ==  ' ':
                    i += 1
                    continue
                elif line[i] == '[':
                    stks[int(i/4)].append(line[i+1])
                    i += 2
                else: 
                    break
                i += 1
    else:
        matchregex = re.compile(r'move ((\d)|(1\d)|(2\d)|(3\d)) from (\d) to (\d)')
        mo = matchregex.search(line)
        size, fromStk, toStk = int(mo.group(1)), int(mo.group(6)), int(mo.group(7))
        #print(size, fromStk, toStk)
        while size:
            stks[toStk-1].append(stks[fromStk-1].pop())
            size -= 1

for i in range(9):
    if len(stks[i]): 
        result += stks[i][-1]

print("The Result for the 1st star is: ",result)

result = ""

stks = [[] for x in range(9)]

flag = False

for line in lines:
    if not flag:
        if(len(line) == 0):
            for i in range(9):
                stks[i].reverse()
            flag = True
        else:
            i = 0
            while(i < len(line)):
                if line[i] ==  ' ':
                    i += 1
                    continue
                elif line[i] == '[':
                    stks[int(i/4)].append(line[i+1])
                    i += 2
                else: 
                    break
                i += 1
    else:
        matchregex = re.compile(r'move ((\d)|(1\d)|(2\d)|(3\d)) from (\d) to (\d)')
        mo = matchregex.search(line)
        size, fromStk, toStk = int(mo.group(1)), int(mo.group(6)), int(mo.group(7))
        #print(size, fromStk, toStk)
        stks[toStk-1].extend(stks[fromStk-1][len(stks[fromStk-1])-size:])
        del stks[fromStk-1][len(stks[fromStk-1])-size:]

for i in range(9):
    if len(stks[i]): 
        result += stks[i][-1]
    

print("The Result for the 2nd star is: ",result)