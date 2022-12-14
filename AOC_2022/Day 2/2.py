file = open("input.txt","r")

lines = file.read().splitlines()

score = 0

rules = {"A" : "X", "B" : "Y", "C" : "Z"}

scores = {"X" : 1, "Y" : 2, "Z" : 3}

for line in lines:
    opponent = line[0]
    player = line[2]
    if rules[opponent] != player:
        if ((player == "X" and opponent == "C") or (player == "Y" and opponent == "A") or (player == "Z" and opponent == "B")):
            score += 6
    else :
        score += 3
    score += scores[player]

print("The Result for the 1st star is: " + str(score))

score = 0

for line in lines:
    opponent = line[0]
    player = line[2]
    if player == 'X':
        if opponent == 'A':
            score += 3
        elif opponent == 'B':
            score += 1
        else:
            score += 2
    elif player == 'Y':
        score += 3
        score += scores[rules[opponent]]
    else:
        score += 6
        if opponent == 'A':
            score += 2
        elif opponent == 'B':
            score += 3
        else:
            score += 1


print("The Result for the 2nd star is: " + str(score))