file = open("input.txt","r")

lines = file.read().splitlines()

result = 0

for line in lines :
    set1 = set(line[0:int(len(line)/2)])
    set2 = set(line[int(len(line)/2):])
    if list(set1 & set2)[0].islower():
        result += ord(list(set1 & set2)[0]) - 96
    else:
        result += (ord(list(set1 & set2)[0]) - 64 + 26)


print("The Result for the 1st star is: " + str(result))

result = 0

for i in range(0,len(lines),3):
    set1 = set(lines[i])
    set2 = set(lines[i+1])
    set3 = set(lines[i+2])
    if list(set1 & set2 & set3)[0].islower():
        result += ord(list(set1 & set2 & set3)[0]) - 96
    else:
        result += (ord(list(set1 & set2 & set3)[0]) - 64 + 26)

print("The Result for the 2nd star is: " + str(result))