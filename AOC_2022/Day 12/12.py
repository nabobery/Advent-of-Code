from typing import List, Tuple

class Solution:
    def __init__(self):
        self.grid = []
        self.start = (0, 0)
        self.finish = (0, 0)
    
    def init(self, input: List[str]):
        self.grid = [list(row) for row in input]
        for i in range(len(self.grid)):
            for j in range(len(self.grid[i])):
                if self.grid[i][j] == 'S':
                    self.start = (i, j)
        #print(len(self.grid), len(self.grid[0]))
        # for row in self.grid:
        #     print(row)

    def within_boundary(self, n: int, m: int, p: Tuple[int, int]) -> bool:
        if (0 <= p[0] and p[0] < n) and (0 <= p[1] and p[1] < m):
            return True
        #print(p)
        return False
    
    def part1(self):
        q = [(self.start, 0)]
        self.grid[self.start[0]][self.start[1]] = 'a'
        dp = [[-1 for _ in range(len(self.grid[0]))] for _ in range(len(self.grid))]
        dp[self.start[0]][self.start[1]] = 0
        while q:
            curr, curr_dist = q.pop(0)
            if self.grid[curr[0]][curr[1]] == 'E':
                self.finish = curr
                print(f"Part1: {dp[curr[0]][curr[1]]}")
                return
            ch = chr(ord(self.grid[curr[0]][curr[1]]) + 1)
            if self.within_boundary(len(self.grid), len(self.grid[0]), (curr[0]+1, curr[1])):
                if self.grid[curr[0]+1][curr[1]] == 'E':
                    if self.grid[curr[0]][curr[1]] >= 'y' and dp[curr[0]+1][curr[1]] == -1:
                        q.append(((curr[0]+1, curr[1]), curr_dist+1))
                        dp[curr[0]+1][curr[1]] = curr_dist+1
                else:
                    if self.grid[curr[0]+1][curr[1]] <= ch and dp[curr[0]+1][curr[1]] == -1:
                        q.append(((curr[0]+1, curr[1]), curr_dist+1))
                        dp[curr[0]+1][curr[1]] = curr_dist+1
            if self.within_boundary(len(self.grid), len(self.grid[0]), (curr[0]-1, curr[1])):
                if self.grid[curr[0]-1][curr[1]] == 'E':
                    if self.grid[curr[0]][curr[1]] >= 'y' and dp[curr[0]-1][curr[1]] == -1:
                        q.append(((curr[0]-1, curr[1]), curr_dist+1))
                        dp[curr[0]-1][curr[1]] = curr_dist+1
                else:
                    if self.grid[curr[0]-1][curr[1]] <= ch and dp[curr[0]-1][curr[1]] == -1:
                        q.append(((curr[0]-1, curr[1]), curr_dist+1))
                        dp[curr[0]-1][curr[1]] = curr_dist+1
            if self.within_boundary(len(self.grid), len(self.grid[0]), (curr[0], curr[1]+1)):
                if self.grid[curr[0]][curr[1]+1] == 'E':
                    if self.grid[curr[0]][curr[1]] >= 'y' and dp[curr[0]][curr[1]+1] == -1:
                        q.append(((curr[0], curr[1]+1), curr_dist+1))
                        dp[curr[0]][curr[1]+1] = curr_dist+1
                else:
                    if self.grid[curr[0]][curr[1]+1] <= ch and dp[curr[0]][curr[1]+1] == -1:
                        q.append(((curr[0], curr[1]+1), curr_dist+1))
                        dp[curr[0]][curr[1]+1] = curr_dist+1
            if self.within_boundary(len(self.grid), len(self.grid[0]), (curr[0], curr[1]-1)):
                if self.grid[curr[0]][curr[1]-1] == 'E':
                    if self.grid[curr[0]][curr[1]] >= 'y' and dp[curr[0]][curr[1]-1] == -1:
                        q.append(((curr[0], curr[1]-1), curr_dist+1))
                        dp[curr[0]][curr[1]-1] = curr_dist+1
                else:
                    if self.grid[curr[0]][curr[1]-1] <= ch and dp[curr[0]][curr[1]-1] == -1:
                        q.append(((curr[0], curr[1]-1), curr_dist+1))
                        dp[curr[0]][curr[1]-1] = curr_dist+1
    
    def part2(self):
        q = [(self.finish, 0)]
        self.grid[self.finish[0]][self.finish[1]] = 'z'
        dp = [[-1 for _ in range(len(self.grid[0]))] for _ in range(len(self.grid))]
        dp[self.finish[0]][self.finish[1]] = 0
        while q:
            curr, curr_dist = q.pop(0)
            if self.grid[curr[0]][curr[1]] == 'a':
                print(f"Part2: {dp[curr[0]][curr[1]]}")
                return
            ch = chr(ord(self.grid[curr[0]][curr[1]]) - 1)
            if self.within_boundary(len(self.grid), len(self.grid[0]), (curr[0]+1, curr[1])):
                if self.grid[curr[0]+1][curr[1]] == 'a':
                    if self.grid[curr[0]][curr[1]] <= 'b' and dp[curr[0]+1][curr[1]] == -1:
                        q.append(((curr[0]+1, curr[1]), curr_dist+1))
                        dp[curr[0]+1][curr[1]] = curr_dist+1
                else:
                    if self.grid[curr[0]+1][curr[1]] >= ch and dp[curr[0]+1][curr[1]] == -1:
                        q.append(((curr[0]+1, curr[1]), curr_dist+1))
                        dp[curr[0]+1][curr[1]] = curr_dist+1
            if self.within_boundary(len(self.grid), len(self.grid[0]), (curr[0]-1, curr[1])):
                if self.grid[curr[0]-1][curr[1]] == 'a':
                    if self.grid[curr[0]][curr[1]] <= 'b' and dp[curr[0]-1][curr[1]] == -1:
                        q.append(((curr[0]-1, curr[1]), curr_dist+1))
                        dp[curr[0]-1][curr[1]] = curr_dist+1
                else:
                    if self.grid[curr[0]-1][curr[1]] >= ch and dp[curr[0]-1][curr[1]] == -1:
                        q.append(((curr[0]-1, curr[1]), curr_dist+1))
                        dp[curr[0]-1][curr[1]] = curr_dist+1
            if self.within_boundary(len(self.grid), len(self.grid[0]), (curr[0], curr[1]+1)):
                if self.grid[curr[0]][curr[1]+1] == 'a':
                    if self.grid[curr[0]][curr[1]] <= 'b' and dp[curr[0]][curr[1]+1] == -1:
                        q.append(((curr[0], curr[1]+1), curr_dist+1))
                        dp[curr[0]][curr[1]+1] = curr_dist+1
                else:
                    if self.grid[curr[0]][curr[1]+1] >= ch and dp[curr[0]][curr[1]+1] == -1:
                        q.append(((curr[0], curr[1]+1), curr_dist+1))
                        dp[curr[0]][curr[1]+1] = curr_dist+1        
            if self.within_boundary(len(self.grid), len(self.grid[0]), (curr[0], curr[1]-1)):
                if self.grid[curr[0]][curr[1]-1] == 'a':
                    if self.grid[curr[0]][curr[1]] <= 'b' and dp[curr[0]][curr[1]-1] == -1:
                        q.append(((curr[0], curr[1]-1), curr_dist+1))
                        dp[curr[0]][curr[1]-1] = curr_dist+1
                else:
                    if self.grid[curr[0]][curr[1]-1] >= ch and dp[curr[0]][curr[1]-1] == -1:
                        q.append(((curr[0], curr[1]-1), curr_dist+1))
                        dp[curr[0]][curr[1]-1] = curr_dist+1

def main():
    # with open("input.txt") as f:
    #     lines = f.readlines()
    file = open("input.txt", "r")
    lines = file.read().splitlines()
    s = Solution()
    s.init(lines)
    s.part1()
    s.part2()

if __name__ == "__main__":
    main()



