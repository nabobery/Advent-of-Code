package main

import (
	"bufio"
	"fmt"
	"os"
)

func initialize(filename string) ([][] rune, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	var grid [][] rune
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		grid = append(grid, []rune(scanner.Text()))
	}
	if err := scanner.Err(); err != nil {
		return nil, err
	}
	return grid, nil
}

func simulate(direction rune, grid [][]rune) {
    n, m, k := len(grid), len(grid[0]), 0
	switch direction {
	case 'N':
		for i := 1; i < n; i++ {
			for j := 0; j < m; j++ {
				if grid[i][j] == 'O'{
					for k = i; k > 0 && grid[k-1][j] == '.'; k-- {}
					grid[i][j], grid[k][j] = '.', 'O'
				}
			}
		}
	case 'W':
		for i := 0; i < n; i++ {
			for j := 1; j < m; j++ {
				if grid[i][j] == 'O'{
					for k = j; k > 0 && grid[i][k-1] == '.'; k-- {}
					grid[i][j], grid[i][k] = '.', 'O'
				}
			}
		}
	case 'S':
		for i := n-2; i >= 0; i-- {
			for j := 0; j < m; j++ {
				if grid[i][j] == 'O'{
					for k = i; k < n-1 && grid[k+1][j] == '.'; k++ {}
					grid[i][j], grid[k][j] = '.', 'O'
				}
			}
		}
	case 'E':
		for i := 0; i < n; i++ {
			for j := m-2; j >= 0; j-- {
				if grid[i][j] == 'O'{
					for k = j; k < m-1 && grid[i][k+1] == '.'; k++ {}
					grid[i][j], grid[i][k] = '.', 'O'
				}
			}
		}

	}

}

func getLoad(grid [][]rune) int {
	result := 0
	n, m := len(grid), len(grid[0])
	for i := 0; i < n; i++ {
		for j := 0; j < m; j++ {
			if grid[i][j] == 'O' {
				result += n-i
			}
		}
	}
	return result
}

func gridToString(grid [][]rune) string {
	result := ""
	for _, row := range grid {
		result += string(row)
	}
	return result
}

func part1(grid [][] rune) {
	simulate('N', grid)
	fmt.Println("Part 1:", getLoad(grid))
}

func part2(grid [][] rune) {
	cycles,sim := 1000000000, 0
	directions := []rune{'N', 'W', 'S', 'E'}
	seen := make(map[string]int)
	for i := 1; i <= cycles; i++ {
		for _, direction := range directions {
			simulate(direction, grid)
		}
		gridString := gridToString(grid)
		if _, ok := seen[gridString]; ok {
			sim = i + (((cycles - i)/(i - seen[gridString])) * (i - seen[gridString]))
			break
		}
		seen[gridString] = i
	}
	for i := sim+1; i <= cycles; i++ {
		for _, direction := range directions {
			simulate(direction, grid)
		}
	}
	fmt.Println("Part 2:", getLoad(grid))
}

func main(){
	filename := "input.txt"
	grid, err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	copyGrid := make([][]rune, len(grid))
	for i := range grid {
		copyGrid[i] = make([]rune, len(grid[i]))
		copy(copyGrid[i], grid[i])
	}
	part1(grid)
	grid = copyGrid
	part2(grid)
}