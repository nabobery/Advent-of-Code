package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
	"math"
)

func parseCard(str string) int {
	result := 0
	temp := strings.Split(str, " | ")
	win_nums := make([]bool, 100)
	nums := make([]bool, 100)
	for i := 0; i < len(temp[0]); i+= 3 {
		num, _ := strconv.Atoi(strings.TrimSpace(temp[0][i:min(i+3, len(temp[0]))]))
		win_nums[num] = true
	}
	for i := 0; i < len(temp[1]); i+= 3 {
		num, _ := strconv.Atoi(strings.TrimSpace(temp[1][i:min(i+3, len(temp[1]))]))
		nums[num] = true
	}
	for i := 0; i < 100; i++ {
		if win_nums[i] && nums[i] {
			result++
		}
	}
	return result
}

func initialize(filename string) ([]int, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	var scores []int
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		scores = append(scores, parseCard(scanner.Text()[strings.Index(scanner.Text(), ":")+1:]))
	}
	if err := scanner.Err(); err != nil {
		return nil, err
	}
	return scores, nil
}

func part1(input []int) {
	result := 0
	for _, score := range input {
		if score > 0 {	
			result += int(math.Pow(2, float64(score) - 1))
		}
	}
	fmt.Println("Part 1:", result)
}

func part2(input []int) {
	result := 0
	visited := make([]int, len(input))
	for i := 0; i < len(input); i++ {
		visited[i] = 1
	}
	for i := 0; i < len(input); i++ {
		for j := i+1; j <= i+input[i]; j++ {
			visited[j] += visited[i]
		}
	}
	for _, score := range visited {
		result += score
	}
	fmt.Println("Part 2:", result)
}

func main(){
	filename := "input.txt"
	input, err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	part1(input)
	part2(input)
}