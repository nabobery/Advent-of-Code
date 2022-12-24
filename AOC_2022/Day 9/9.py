file = open("input.txt", "r")

lines = file.read().splitlines()

import numpy as np

head = (0, 0)
tail = (0, 0)
visited = set()
visited.add(tail)

for line in lines:
    direction, steps = line.split()
    for _ in range(int(steps)):
        if direction == 'U':
                head = (head[0], head[1] + 1)
        elif direction == 'D':
                head = (head[0], head[1] - 1)
        elif direction == 'R':
                head = (head[0] + 1, head[1])
        elif direction == 'L':
                head = (head[0] - 1, head[1])
        diff_x = head[0] - tail[0]
        diff_y = head[1] - tail[1]
        if abs(diff_x) > 1 or abs(diff_y) > 1:
            tail = (tail[0] + np.sign(diff_x), tail[1] + np.sign(diff_y))
            visited.add(tail)

print("The Result for the 1st star is: " + str(len(visited)))

rope = [(0, 0) for _ in range(10)]

visited = set()
visited.add((0, 0))

for line in lines:
    direction, steps = line.split()
    for _ in range(int(steps)):
        if direction == 'U':
            rope[0] = (rope[0][0], rope[0][1] + 1)
        elif direction == 'D':
            rope[0] = (rope[0][0], rope[0][1] - 1)
        elif direction == 'R':
            rope[0] = (rope[0][0] + 1, rope[0][1])
        elif direction == 'L':
            rope[0] = (rope[0][0] - 1, rope[0][1])
        for i in range(1,10):
            diff_x = rope[i][0] - rope[i-1][0]
            diff_y = rope[i][1] - rope[i-1][1]
            if abs(diff_x) > 1 or abs(diff_y) > 1:
                rope[i] = (rope[i][0] - np.sign(diff_x), rope[i][1] - np.sign(diff_y))
            
        visited.add(rope[9])

print("The Result for the 2nd star is: " + str(len(visited)))


