package main

import (
	"bufio"
	"fmt"
	"os"
)

type pair struct {
	first, second int
}

type Solution struct {
	n, m   int
	grid   []string
	start  pair
	dirs   []pair
	visited map[pair]bool
}

func (s *Solution) init(input *os.File) {
	scanner := bufio.NewScanner(input)
	for scanner.Scan() {
		s.grid = append(s.grid, scanner.Text())
	}
	s.n = len(s.grid)
	s.m = len(s.grid[0])
	s.dirs = []pair{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}
	s.visited = make(map[pair]bool)
	for i := 0; i < s.n; i++ {
		for j := 0; j < s.m; j++ {
			if s.grid[i][j] == 'S' {
				s.start = pair{i, j}
				break
			}
		}
	}
}

func (s *Solution) isValid(pos pair) bool {
	for pos.first < 0 {
		pos.first += s.n
	}
	for pos.second < 0 {
		pos.second += s.m
	}
	pos.first %= s.n
	pos.second %= s.m
	return s.grid[pos.first][pos.second] == '.'
}

func (s *Solution) part1() {
	q := []pair{s.start}
	steps := 0

	for steps < 64 && len(q) > 0 {
		size := len(q)
		for i := 0; i < size; i++ {
			curr := q[0]
			q = q[1:]
			s.grid[curr.first] = s.grid[curr.first][:curr.second] + "." + s.grid[curr.first][curr.second+1:]

			for _, dir := range s.dirs {
				next := pair{curr.first + dir.first, curr.second + dir.second}
				if s.isValid(next) && s.grid[next.first][next.second] == '.' {
					s.grid[next.first] = s.grid[next.first][:next.second] + "O" + s.grid[next.first][next.second+1:]
					q = append(q, next)
				}
			}
		}
		steps++
	}
	fmt.Println("Part 1:", len(q))
}

func function(n int64, temp []int) int64 {
	y0, y1, y2 := temp[0], temp[1], temp[2]
	a := (int64(y2) - 2*int64(y1) + int64(y0)) / 2
	b := int64(y2) - int64(y1) - a
	c := int64(y1)
	result := a*n*n + b*n + c
	return result
}

func (s *Solution) part2() {
	for i := 0; i < s.n; i++ {
		for j := 0; j < s.m; j++ {
			if s.grid[i][j] == 'O' {
				s.grid[i] = s.grid[i][:j] + "." + s.grid[i][j+1:]
			}
		}
	}
	s.grid[s.start.first] = s.grid[s.start.first][:s.start.second] + "." + s.grid[s.start.first][s.start.second+1:]
	temp := make([]int, 0)
	steps := 0
	prevVisited, currVisited := make(map[pair]bool), make(map[pair]bool)
	currVisited[s.start] = true

	for {
		size := len(currVisited)
		if steps%s.n == s.n/2 {
			temp = append(temp, size)
		}
		if len(temp) == 3 {
			break
		}
		prevVisited = make(map[pair]bool)
		for it := range currVisited {
			for _, dir := range s.dirs {
				next := pair{it.first + dir.first, it.second + dir.second}
				if s.isValid(next) {
					prevVisited[next] = true
				}
			}
		}
		currVisited = prevVisited
		steps++
	}
	fmt.Println("Part 2:", function(int64(26501365/s.n), temp))
}

func main() {
	file, err := os.Open("input.txt")
	if err != nil {
		fmt.Println("Error reading input file:", err)
		return
	}
	defer file.Close()
	s := Solution{}
	s.init(file)
	s.part1()
	s.part2()
}
