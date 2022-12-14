import heapq

file = open("input.txt","r")

lines = file.read().splitlines()

#print(lines)

result,curr = 0,0

for line in lines:
    if len(line) == 0:
        if curr > result:
            result = curr
        curr = 0
    else:
        curr += int(line)

if curr > result:   
    result = curr

print("Result for 1st star is : " + str(result))

heap = []

heapq.heapify(heap)

result,curr = 0,0

for line in lines:
    if len(line) == 0:
        if len(heap) >= 3:
            if curr > heap[0]:
                heapq.heappop(heap)
                heapq.heappush(heap,curr)
        else:
            heapq.heappush(heap,curr)
        curr = 0
    else:
        curr += int(line)

if curr > heap[0]:
    heapq.heappop(heap)
    heapq.heappush(heap,curr)

while len(heap) > 0:
    result += heapq.heappop(heap)

print("Result for 2nd star is : " + str(result))


