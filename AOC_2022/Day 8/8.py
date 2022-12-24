file = open("input.txt", "r")

lines = file.read().splitlines()

grid = [[False for _ in range(len(lines[0]))] for _ in range(len(lines))]

for i in range(len(lines)):
    grid[i][0] = True
    grid[i][len(lines) - 1] = True

for i in range(len(lines[0])):
    grid[0][i] = True
    grid[len(lines[0]) - 1][i] = True

for i in range(len(lines)):
    maximum = 0
    for j in range(len(lines[i])):
        if int(lines[i][j]) > maximum :
            grid[i][j] = True
            maximum = int(lines[i][j])
    maximum = 0
    for j in range(len(lines[0])-1,0,-1):
        if int(lines[i][j]) > maximum :
            grid[i][j] = True
            maximum = int(lines[i][j])

for i in range(len(lines[0])):
    maximum = 0
    for j in range(len(lines)):
        if int(lines[j][i]) > maximum :
            grid[j][i] = True
            maximum = int(lines[j][i])
    maximum = 0
    for j in range(len(lines)-1,0,-1):
        if int(lines[j][i]) > maximum :
            grid[j][i] = True
            maximum = int(lines[j][i])

result = sum([1 for row in grid for value in row if value])

print("The Result for the 1st star is: " + str(result))

result = 0

for i in range(1,len(lines)-1):
    for j in range(1,len(lines[i])-1):
        curr = 1
        k = j+1
        while(k < len(lines[i])-1 and lines[i][k] < lines[i][j]):
            k += 1
        curr *= k-j
        k = j-1
        while(k > 0 and lines[i][k] < lines[i][j]):
            k -= 1
        curr *= j-k
        k = i+1
        while(k < len(lines)-1 and lines[k][j] < lines[i][j]):
            k += 1
        curr *= k-i
        k = i-1
        while(k > 0 and lines[k][j] < lines[i][j]):
            k -= 1
        curr *= i-k
        result = max(result, curr)

print("The Result for the 2nd star is: " + str(result))