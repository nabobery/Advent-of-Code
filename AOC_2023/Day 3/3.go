package main

import (
	"fmt"
	"bufio"
	"os"
	"regexp"
	"strconv"
	"unicode"
)

type Point struct {
	x int
	y int
}

var directions = []Point{
	{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1},
}

func (p Point) add(q Point) Point {
	return Point{p.x + q.x, p.y + q.y}
}

func initialize(filename string) (map[Point]string ,map[Point][]int, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, nil, err
	}
	defer file.Close()
	scanner := bufio.NewScanner(file)
	symbols := map[Point]string{}
	partnumbers := map[Point][]int{}
	y := 0
	for scanner.Scan() {
		for x, r := range scanner.Text() {
			if r != '.' && !unicode.IsDigit(r) {
				symbols[Point{x, y}] = string(r)
			}
		}
		y++
	}
	_, _ = file.Seek(0, 0)
	scanner = bufio.NewScanner(file)
	re := regexp.MustCompile(`\d+`)
	y = 0
	for scanner.Scan() {
		for _, match := range re.FindAllStringIndex(scanner.Text(), -1) {
			keys := map[Point]bool{}
			// add all points of digit in the number matched by the regex
			for x := match[0]; x < match[1]; x++ {
				for _, d := range directions {
					keys[Point{x, y}.add(d)] = true
				}
			}
			// convert the number matched by the regex to an int
			n, _ := strconv.Atoi(scanner.Text()[match[0]:match[1]])
			// check if the number is a part number i.e. it has a symbol in its neighborhood
			for p := range keys {
				if _, exists := symbols[p]; exists {
					partnumbers[p] = append(partnumbers[p], n)
				}
			}
		}
		y++
	}
	if err := scanner.Err(); err != nil {
		return nil, nil, err
	}
	return symbols, partnumbers, nil
}

func part1(partnumbers map[Point][]int) {
	result := 0
	for _, values := range partnumbers {
		for _, value := range values {
			result += value
		}
	}
	fmt.Println("Part 1:", result)
}

func part2(symbols map[Point]string, partnumbers map[Point][]int) {
	result := 0
	for coord, neighbors := range partnumbers {
		if symbols[coord] == "*" && len(neighbors) == 2 {
			result += neighbors[0] * neighbors[1]
		}
	}
	fmt.Println("Part 2:", result)
}

func main() {
	filename := "input.txt"
	symbols,partnumbers, err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	part1(partnumbers)
	part2(symbols,partnumbers)
}