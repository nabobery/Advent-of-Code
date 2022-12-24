file = open("input.txt", "r")

lines = file.read().splitlines()

cycle,x = 1,1
result = 0

cycles = [20,60,100,140,180,220]

for line in lines:
    if "addx" in line:
        cycle += 1
        if cycle in cycles:
            result += (cycle*x)
        x += int(line[5:])
        cycle += 1
        if cycle in cycles:
            result += (cycle*x)
    else:
        cycle += 1
        if cycle in cycles:
            result += (cycle*x)

print("The Result for the 1st star is: " + str(result))

cycle,x = 0,1

result = [['.' for _ in range(40)] for _ in range(6)]

for line in lines:
    i,j = int(cycle/40), cycle%40
    if "addx" in line:
        if j >= x-1 and j <= x+1:
            result[i][j] = '#'
        j += 1
        if j >= x-1 and j <= x+1:
            result[i][j] = '#'
        cycle += 2
        x += int(line[5:])
    else:
        if j >= x-1 and j <= x+1:
            result[i][j] = '#'
        cycle += 1

print("The Result for the 2nd star is: ")
for i in range(6):
    print("".join(result[i]))
        