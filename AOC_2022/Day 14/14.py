import re

class Solution:
    def __init__(self):
        self.grid = [['.' for _ in range(1000)] for _ in range(200)]
        self.copy_grid = []
        self.wall_index = 0

    def init(self, lines):
        pattern = re.compile(r"(\d+),(\d+)")
        for line in lines:
            matches = re.findall(pattern, line)
            prev = (int(matches[0][0]), int(matches[0][1]))
            self.wall_index = max(self.wall_index, prev[1])

            for match in matches[1:]:
                curr = (int(match[0]), int(match[1]))
                if prev[0] == curr[0]:
                    for i in range(min(prev[1], curr[1]), max(prev[1], curr[1]) + 1):
                        self.grid[i][prev[0]] = '#'
                else:
                    for i in range(min(prev[0], curr[0]), max(prev[0], curr[0]) + 1):
                        self.grid[prev[1]][i] = '#'
                prev = curr
                self.wall_index = max(self.wall_index, prev[1])

        self.wall_index += 2
        self.copy_grid = [row[:] for row in self.grid]

    def dfs(self, pos):
        if pos[0] == len(self.grid) - 1 or self.grid[0][500] == 'o':
            return False

        if self.grid[pos[0] + 1][pos[1]] == '.':
            return self.dfs((pos[0] + 1, pos[1]))

        if self.grid[pos[0] + 1][pos[1]] == '#' or self.grid[pos[0] + 1][pos[1]] == 'o':
            if self.grid[pos[0] + 1][pos[1] - 1] == '.':
                return self.dfs((pos[0], pos[1] - 1))
            elif self.grid[pos[0] + 1][pos[1] + 1] == '.':
                return self.dfs((pos[0], pos[1] + 1))
            else:
                self.grid[pos[0]][pos[1]] = 'o'
                return True

        return False

    def part1(self):
        result = 0
        self.grid[0][500] = '+'
        while self.dfs((0, 500)):
            result += 1
        print(f"Part 1: {result}")

    def part2(self):
        result = 0
        self.grid = [row[:] for row in self.copy_grid]
        self.grid[0][500] = '+'
        self.grid[self.wall_index] = ['#' for _ in range(len(self.grid[0]))]
        while self.dfs((0, 500)):
            result += 1
        print(f"Part 2: {result}")


def main():
    file = open("input.txt", "r")
    lines = file.read().splitlines()
    s = Solution()
    s.init(lines)
    s.part1()
    s.part2()


if __name__ == "__main__":
    main()
