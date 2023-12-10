package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type Point struct {
	x int
	y int
}

func initialize(filename string) ([]string, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	var lines []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}
	if err := scanner.Err(); err != nil {
		return nil, err
	}
	return lines, nil
}

func part1(input []string) [] Point{
	pipes := map[rune][]rune{
		'|' : {'N', 'S'},
		'-' : {'E', 'W'},
		'L' : {'N', 'E'},
		'J' : {'N', 'W'},
		'7' : {'S', 'W'},
		'F' : {'S', 'E'},
		'S' : {'N','S', 'E', 'W'},
	}
	directions := map[rune]Point{
		'N' : {-1, 0},
		'S' : {1, 0},
		'E' : {0, 1},
		'W' : {0, -1},
	}
	directions_to_pipes := map[rune]string{
		'N' : "|7FS",
		'S' : "|JLS",
		'E' : "-7JS",
		'W' : "-LFS",
	}
	parent := map[Point]Point{}
	var path []Point
	var start Point
	for i, line := range input {
		for j, char := range line {
			if char == 'S' {
				start = Point{i, j}
				break
			}
		}
	}
	queue := make([]Point, 0)
	queue = append(queue, start)
	non := Point{-1, -1}
	parent[start] = non
	for len(queue) > 0 {
		curr := queue[0]
		queue = queue[1:]
		if curr == start && parent[start] != non {break}
		if input[curr.x][curr.y] == '.' {continue}
		for _, dir := range pipes[rune(input[curr.x][curr.y])] {
			x := curr.x + directions[dir].x
			y := curr.y + directions[dir].y
			if x >= 0 && x < len(input) && y >= 0 && y < len(input[0])  && strings.Contains(directions_to_pipes[dir], string(input[x][y])) {
				neighbour := Point{x, y}
				if parent[curr] == neighbour {continue}
				parent[neighbour] = curr
				queue = append(queue, neighbour)
			}
		}
	}
	path = append(path, start)
	for true{
		curr := parent[path[len(path)-1]]
		if curr == start {break}
		path = append(path, curr)
	}
	fmt.Println("Part 1:", len(path)/2)
	return path
}

func part2(path []Point) {
	path = append(path, path[0])
	area := 0
	for i := 0; i < len(path)-1; i++ {
		area += path[i].x * path[i+1].y - path[i].y * path[i+1].x
	}
	result := area/2
	if result < 0 {result *= -1}
	result -= len(path)/2
	result += 1
	fmt.Println("Part 2:", result)
}

func main(){
	filename := "input.txt"
	input, err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	path := part1(input)
	part2(path)
}