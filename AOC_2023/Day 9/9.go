package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"strconv"
)

func parseHistory(history string) []int {
	var result []int
	nums := strings.Split(history, " ")
	for _, num := range nums {
		n, _ := strconv.Atoi(num)
		result = append(result, n)
	}
	return result
}

func initialize(filename string) ([][]int, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	var histories [][]int
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		history := parseHistory(scanner.Text())
		histories = append(histories, history)
	}
	if err := scanner.Err(); err != nil {
		return nil, err
	}
	return histories, nil
}

func notZeroes(nums []int) bool {
	for _, num := range nums {
		if num != 0 {
			return true
		}
	}
	return false
}

func extrapolateHistory1(history []int) int {
	result := 0
	for notZeroes(history) {
		var temp []int
		for i := 1; i < len(history); i++ {
			temp = append(temp, history[i] - history[i-1])
		}
		result += history[len(history)-1]
		history = temp
	}
	return result
}

func extrapolateHistory2(history []int) int {
	result := history[0]
	cnt := 1
	for notZeroes(history) {
		var temp []int
		for i := 1; i < len(history); i++ {
			temp = append(temp, history[i] - history[i-1])
		}
		history = temp
		if cnt % 2 == 0{
			result += history[0]
		} else {
			result -= history[0]
		}
		cnt++
	}
	return result
}

func part1(histories [][]int) {
	result := 0
	for _, history := range histories {
		result += extrapolateHistory1(history)
	}
	fmt.Println("Part 1:", result)
}

func part2(histories [][]int) {
	result := 0
	for _, history := range histories {
		result += extrapolateHistory2(history)
	}
	fmt.Println("Part 2:", result)
}

func main(){
	filename := "input.txt"
	histories, err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	part1(histories)
	part2(histories)
}