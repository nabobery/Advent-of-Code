package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

var directions map[rune][2]int =  map[rune][2]int{'N' : {-1, 0},'E' : {0, 1},'S' : {1, 0},'W' : {0, -1},}

type point struct {
	x int
	y int
}

func initialize(filename string) ([]string, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	var grid []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		grid = append(grid, scanner.Text())
	}
	if err := scanner.Err(); err != nil {
		return nil, err
	}
	return grid, nil
}

func energise(grid []string, start point, dir rune) int {
	visited := make(map[point]map[rune]bool)
	var dfs func(point, rune)
	dfs = func(p point, d rune) {
		if p.x < 0 || p.x >= len(grid) || p.y < 0 || p.y >= len(grid[0]){
			return
		}
		if _, ok := visited[p]; !ok {
			visited[p] = make(map[rune]bool)
		} else if _, ok := visited[p][d]; ok {
			return
		}
		visited[p][d] = true
		if grid[p.x][p.y] == '|' &&  strings.Contains("EW", string(d)) {
			dfs(point{p.x + directions['N'][0], p.y + directions['N'][1]}, 'N')
			dfs(point{p.x + directions['S'][0], p.y + directions['S'][1]}, 'S')
		} else if grid[p.x][p.y] == '-' && strings.Contains("NS", string(d)) {
			dfs(point{p.x + directions['E'][0], p.y + directions['E'][1]}, 'E')
			dfs(point{p.x + directions['W'][0], p.y + directions['W'][1]}, 'W')
		} else if grid[p.x][p.y] == '/' {
			if d == 'N' {
				dfs(point{p.x + directions['E'][0], p.y + directions['E'][1]}, 'E')
			} else if d == 'E' {
				dfs(point{p.x + directions['N'][0], p.y + directions['N'][1]}, 'N')
			} else if d == 'S' {
				dfs(point{p.x + directions['W'][0], p.y + directions['W'][1]}, 'W')
			} else if d == 'W' {
				dfs(point{p.x + directions['S'][0], p.y + directions['S'][1]}, 'S')
			}
		} else if grid[p.x][p.y] == '\\' {
			if d == 'N' {
				dfs(point{p.x + directions['W'][0], p.y + directions['W'][1]}, 'W')
			} else if d == 'E' {
				dfs(point{p.x + directions['S'][0], p.y + directions['S'][1]}, 'S')
			} else if d == 'S' {
				dfs(point{p.x + directions['E'][0], p.y + directions['E'][1]}, 'E')
			} else if d == 'W' {
				dfs(point{p.x + directions['N'][0], p.y + directions['N'][1]}, 'N')
			}
		} else{
			dfs(point{p.x + directions[d][0], p.y + directions[d][1]}, d)
		}
	}	
	dfs(start, dir)
	return len(visited)
}

func part1(grid []string) {
	fmt.Println("Part 1:", energise(grid, point{0, 0}, 'E'))
}

func part2(grid []string) {
	result := 0
	for i := 0; i < len(grid); i++ {
		result = max(result, energise(grid, point{i, 0}, 'E'))
		result = max(result, energise(grid, point{i, len(grid[0]) - 1}, 'W'))
	}
	for i := 0; i < len(grid[0]); i++ {
		result = max(result, energise(grid, point{0, i}, 'S'))
		result = max(result, energise(grid, point{len(grid) - 1, i}, 'N'))
	}
	fmt.Println("Part 2:", result)
}

func main(){
	filename := "input.txt"
	grid, err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	part1(grid)
	part2(grid)
}