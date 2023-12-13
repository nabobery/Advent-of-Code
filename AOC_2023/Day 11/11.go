package main

import (
	"bufio"
	"fmt"
	"os"
	"math"
)

type Point struct {
	x int
	y int
}

func initialize(filename string) ([]Point, []Point, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, nil, err
	}
	defer file.Close()
	var grid []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		grid = append(grid, scanner.Text())
	}
	if err := scanner.Err(); err != nil {
		return nil, nil, err
	}
	var coords, increments []Point
	var rows, cols []int
	for i, row := range grid {
		b := true
		for _, col := range row {
			if col == '#' {
				b = false
				break
			}
		}
		if b {
			rows = append(rows, i)
		}
	}
	for j := 0; j < len(grid[0]); j++ {
		b := true
		for i := 0; i < len(grid); i++ {
			if grid[i][j] == '#' {
				b = false
				break
			}
		}
		if b {
			cols = append(cols, j)
		}
	}
	for i := 0; i < len(grid); i++ {
		for j := 0; j < len(grid[0]); j++ {
			if grid[i][j] == '#' {
				inc_i, inc_j := 0, 0
				for _, row := range rows {
					if row < i {
						inc_i++
					}
				}
				for _, col := range cols {
					if col < j {
						inc_j++
					}
				}
				increments = append(increments, Point{inc_i, inc_j})
				coords = append(coords, Point{i, j})
			}
		}
	}			
	return coords, increments, nil
}

func subtract(p1, p2 Point) int {
	return int(math.Abs(float64(p1.x-p2.x)) + math.Abs(float64(p1.y-p2.y)))
}

func part1(coords []Point, increments []Point) {
	result := 0
	for i := 0; i < len(coords)-1; i++ {
		for j := i+1; j < len(coords); j++ {
			result += (subtract(coords[i], coords[j]) + subtract(increments[i], increments[j]))
		}
	}
	fmt.Println("Part 1:", result)
}

func part2(coords []Point, increments []Point) {
	result := int64(0)
	for i := 0; i < len(coords)-1; i++ {
		for j := i+1; j < len(coords); j++ {
			result += int64(subtract(coords[i], coords[j]) + 999999*subtract(increments[i], increments[j]))
		}
	}
	fmt.Println("Part 2:", result)
}

func main(){
	filename := "input.txt"
	coords, increments, err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	part1(coords, increments)
	part2(coords, increments)
}