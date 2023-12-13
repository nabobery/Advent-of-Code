package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"strconv"
)

func initialize(filename string) ([]string, [][]int, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, nil, err
	}
	defer file.Close()
	var rows []string
	var info [][]int
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		text := scanner.Text()
		rows = append(rows, text[0: strings.Index(text, " ")])
		var temp []int
		for _, v := range strings.Split(text[strings.Index(text, " ")+1:], ",") {
			val, _ := strconv.Atoi(v)
			temp = append(temp, val)
		}
		info = append(info, temp)
	}
	if err := scanner.Err(); err != nil {
		return nil, nil, err
	}
	return rows,info,err
}

func helper(row string, record []int) int64{
	dp := make(map[[3]int]int64)
	var numberOfArrangements func(int, int, int) int64
	numberOfArrangements = func(i int, ci int, groupSize int) int64{
		key := [3]int{i, ci, groupSize}
		if val, ok := dp[key]; ok {
			return val
		}
		if i == len(row) {
			if (ci == len(record) && groupSize == 0) || (ci == len(record) - 1 && record[ci] == groupSize) {
				return int64(1)
			}
			return int64(0)
		}
		result := int64(0)
		if row[i] == '.' || row[i] == '?' {
			if groupSize == 0 {
				result += numberOfArrangements(i+1, ci, 0)
			} else if ci < len(record) && record[ci] == groupSize {
				result += numberOfArrangements(i+1, ci+1, 0)
			}
		}
		if row[i] == '#' || row[i] == '?' {
			result += numberOfArrangements(i+1, ci, groupSize+1)
		}
		dp[key] = result
		return result
	}
	return numberOfArrangements(0, 0, 0)
}

func part1(rows []string, info [][]int) {
	result := int64(0)
	for i := 0; i < len(rows); i++ {
		result += helper(rows[i], info[i])
	}
	fmt.Println("Part 1:", result)
}

func part2(rows []string, info [][]int){
	result := int64(0)
	for i := 0; i < len(rows); i++ {
		row := rows[i]
		record := info[i]
		for j := 1; j < 5;j++ {
			row += "?"
			row += rows[i]
			record = append(record, info[i]...)
		}
		result += helper(row, record)
	}
	fmt.Println("Part 2:", result)
}

func main(){
	filename := "input.txt"
	rows,info, err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	part1(rows, info)
	part2(rows, info)
}