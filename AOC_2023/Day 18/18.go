package main

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"strconv"
	"math"
)

type plan struct {
	dir rune
	dist int
	color string
}

func initialize(filename string) ([]plan, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	var plans []plan
	scanner := bufio.NewScanner(file)
	pattern := regexp.MustCompile(`([A-Z])\s+(\d+)\s+\((#[0-9a-fA-F]+)\)`)
	for scanner.Scan() {
		text := scanner.Text()
		matches := pattern.FindStringSubmatch(text)
		if len(matches) == 4{
			dir := rune(matches[1][0])
			dist, _ := strconv.Atoi(matches[2])
			color := matches[3]
			plans = append(plans, plan{dir, dist, color})
		}
	}
	if err := scanner.Err(); err != nil {
		return nil, err
	}
	return plans, nil
}

func gethexa(s string) int64{
	hexa, _ := strconv.ParseInt(s[1:6], 16, 64)
	return hexa
}

func helper(plans []plan, part2 bool) int64{
	area, b := int64(0), int64(0)
	dirs := [4][2]int64{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}
	chartodirs := map[rune]int64{'R': 0, 'D': 1, 'L': 2, 'U': 3}
	prev, curr := [2]int64{0, 0}, [2]int64{0, 0}
	for i := 0; i < len(plans); i++ {
		var dir,dist int64
		if part2 {
			dir, dist = int64(plans[i].color[len(plans[i].color) - 1] - '0'), gethexa(plans[i].color)
		} else {
			dir, dist = chartodirs[plans[i].dir], int64(plans[i].dist)
		}
		b += dist
		curr = [2]int64{prev[0] + dirs[dir][0]*dist, prev[1] + dirs[dir][1]*dist}
		area += prev[0] * curr[1] - prev[1] * curr[0]
		prev = curr
	}
	area = int64(math.Abs(float64(area)))/2
	return (area + b/2 + 1)
}

func part1(plans []plan) {
	fmt.Println("Part 1:", helper(plans, false))
}

func part2(plans []plan) {
	fmt.Println("Part 2:", helper(plans, true))
}

func main(){
	filename := "input.txt"
	plans, err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	part1(plans)
	part2(plans)
}