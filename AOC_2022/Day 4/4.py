file = open("input.txt","r")

lines = file.read().splitlines()

result = 0

for line in lines:
    pairs = line.split(",")
    pair1 = pairs[0].split("-")
    pair2 = pairs[1].split("-")
    if (int(pair1[0]) <= int(pair2[0]) and int(pair1[1]) >= int(pair2[1]) or int(pair1[0]) >= int(pair2[0]) and int(pair1[1]) <= int(pair2[1])):
        result += 1

print("The Result for the 1st star is: " + str(result))

result = 0

for line in lines:
    pairs = line.split(",")
    pair1 = pairs[0].split("-")
    pair2 = pairs[1].split("-")
    if (int(pair1[0]) <= int(pair2[0]) and int(pair1[1]) >= int(pair2[0]) or int(pair1[0]) >= int(pair2[0]) and int(pair1[0]) <= int(pair2[1])):
        result += 1

print("The Result for the 2nd star is: " + str(result))