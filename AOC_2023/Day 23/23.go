package main

import (
	"bufio"
	"fmt"
	"os"
)

type edge struct {
	vertex pair
	weight int
}

type pair struct {
	first, second int
}

type Solution struct {
	grid        []string
	start, goal pair
	n, m        int
	dirs        [4]pair
	slopetoDir  map[byte]int
}

func (s *Solution) init(input *os.File) error {
	s.grid = make([]string, 0)
	s.dirs = [4]pair{{0, 1}, {0, -1}, {1, 0}, {-1, 0}}
	s.slopetoDir = map[byte]int{'>' : 0, '<' : 1, 'v' : 2, '^' : 3}
	scanner := bufio.NewScanner(input)
	for scanner.Scan() {
		s.grid = append(s.grid, scanner.Text())
	}
	if err := scanner.Err(); err != nil {
		return err
	}
	s.n = len(s.grid)
	s.m = len(s.grid[0])
	for j := 0; j < s.m; j++ {
		if s.grid[0][j] == '.' {
			s.start = pair{0, j}
			break
		}
	}
	for j := 0; j < s.m; j++ {
		if s.grid[s.n-1][j] == '.' {
			s.goal = pair{s.n - 1, j}
			break
		}
	}
	return nil
}

func (s *Solution) inBounds(p pair) bool {
	return p.first >= 0 && p.first < s.n && p.second >= 0 && p.second < s.m
}

func (s *Solution) helper(part2 bool) int {
	vertices := make(map[pair]bool)
	vertices[s.start] = true
	vertices[s.goal] = true
	for i := 0; i < s.n; i++ {
		for j := 0; j < s.m; j++ {
			neighbors := 0
			for _, dir := range s.dirs {
				if s.inBounds(pair{i + dir.first, j + dir.second}) && s.grid[i+dir.first][j+dir.second] != '#' {
					neighbors++
				}
			}
			if neighbors > 2 && s.grid[i][j] != '#' {
				vertices[pair{i, j}] = true
			}
		}
	}
	graph := make(map[pair][]edge)
	for v := range vertices {
		q := []edge{{v, 0}}
		visited := make(map[pair]bool)
		for len(q) > 0 {
			curr := q[0]
			q = q[1:]
			if visited[curr.vertex] {
				continue
			}
			visited[curr.vertex] = true
			if vertices[curr.vertex] && curr.vertex != v {
				graph[v] = append(graph[v], curr)
				continue
			}
			for _, dir := range s.dirs {
				next := pair{curr.vertex.first + dir.first, curr.vertex.second + dir.second}
				if s.inBounds(next) && s.grid[next.first][next.second] != '#' {
					val, ok := s.slopetoDir[s.grid[next.first][next.second]]
					if !part2 && ok && s.dirs[val] != dir {
						continue
					}
					q = append(q, edge{next, curr.weight + 1})
				}
			}
		}
	}
	result := 0
	visited := make(map[pair]bool)
	var dfs func(pair, int, map[pair]bool)
	dfs = func(curr pair, weight int, visited map[pair]bool) {
		if visited[curr] {
			return
		}
		visited[curr] = true
		if curr == s.goal {
			result = max(result, weight)
		}
		for _, e := range graph[curr] {
			dfs(e.vertex, weight+e.weight, visited)
		}
		delete(visited, curr)
	}
	dfs(s.start, 0, visited)
	return result
}

func (s *Solution) part1() {
	fmt.Println("Part 1:",s.helper(false))
}

func (s *Solution) part2() {
	fmt.Println("Part 2:",s.helper(true))
}

func main() {
	file, err := os.Open("input.txt")
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	defer file.Close()
	var s Solution
	err = s.init(file)
	if err != nil {
		fmt.Println("Error initializing solution:", err)
		return
	}
	s.part1()
	s.part2()
}