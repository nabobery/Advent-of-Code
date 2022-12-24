import re

file = open("input.txt", "r")

lines = file.read().splitlines()

arr = []
total_size = 0
result = 0
i = 2

def helper():
    s = 0
    global i
    while i < len(lines):
        if "$ ls" == lines[i] or "dir" in lines[i]:
            i += 1
            continue
        else:
            if bool(re.search(r'\d', lines[i])):
                s += int(re.sub("\D", "", lines[i]))
            else:
                if "$ cd .." == lines[i]:
                    if s < 100000:
                        global result
                        result += s
                    arr.append(s)
                    return s
                else:
                    i += 1
                    s += helper()
        i += 1
    return s


while i < len(lines):
    if bool(re.search(r'\d', lines[i])):
        total_size += int(re.sub("\D", "", lines[i]))
    elif "$ cd " not in lines[i]:
        i += 1
        continue
    else:
        i += 1
        x = helper()
        total_size += x
        #arr.append(x)
    i += 1

print("The Result for the 1st star is: " + str(result))

#print(arr)

print("The Result for the 2nd star is: " +
      str(min(n for n in arr if n > (total_size - 40000000))))
