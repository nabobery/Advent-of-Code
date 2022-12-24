file = open("input.txt","r")

lines = file.read().splitlines()

for line in lines:
    result,l = 0,0
    s = set()
    s.add(line[0])
    for i in range(1,len(line)):
        if line[i] in s:
            while l < i:
                if line[l] != line[i]:
                    s.remove(line[l])
                    l += 1
                else:
                    l += 1
                    break            
        else:
            s.add(line[i])
            if len(s) == 4:
                result = i+1
                break
    print("The Result for the 1st star is: " + str(result))


for line in lines:
    result,l = 0,0
    s = set()
    s.add(line[0])
    for i in range(1,len(line)):
        if line[i] in s:
            while l < i:
                if line[l] != line[i]:
                    s.remove(line[l])
                    l += 1
                else:
                    l += 1
                    break            
        else:
            s.add(line[i])
            if len(s) == 14:
                result = i+1
                break
    print("The Result for the 2nd star is: " + str(result))
