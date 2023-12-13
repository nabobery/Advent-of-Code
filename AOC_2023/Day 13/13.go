package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func initialize(filename string) ([][]string , [][]string, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, nil, err
	}
	defer file.Close()
	var rows, columns [][]string
	scanner := bufio.NewScanner(file)
	var temp []string
	for scanner.Scan() {
		text := scanner.Text()
		if strings.TrimSpace(text) == "" {
			rows = append(rows, temp)
			temp = []string{}
			continue
		} 
		temp = append(temp, text)	
	}
	rows = append(rows, temp)
	if err := scanner.Err(); err != nil {
		return nil, nil, err
	}
	for i := 0; i < len(rows); i++ {
		temp = []string{}
		for j := 0; j < len(rows[i][0]); j++ {
			column := ""
			for k := 0; k < len(rows[i]); k++ {
				column += string(rows[i][k][j])
			}
			temp = append(temp, column)
		}
		columns = append(columns, temp)
	}
	return rows, columns, nil
}

func reflection(inp []string, part2 bool) int {
	n := len(inp)
	for i := 0; i < n-1; i++ {
		l,r := i, i+1
		smudges := 0
		for l >= 0 && r < n {
			for k := 0; k < len(inp[l]); k++ {
				if inp[l][k] != inp[r][k] {
					smudges++
				}
				if smudges > 1 {break}
			}
			if smudges > 1 {break}
			l--
			r++
		}
		if (smudges == 1 && part2) || (smudges == 0 && !part2) {
			return i+1
		}
	}
	return -1
}

func part1(rows [][]string, columns [][]string) {
	result := 0
	for i := 0; i < len(rows); i++ {
		if reflection(rows[i], false) != -1 {
			result += 100*reflection(rows[i], false)
			continue
		}
		if reflection(columns[i], false) != -1 {
			result += reflection(columns[i], false)
		}
	}
	fmt.Println("Part 1:", result)
}

func part2(rows [][]string, columns [][]string) {
	result := 0
	for i := 0; i < len(rows); i++ {
		if reflection(rows[i], true) != -1 {
			result += 100*reflection(rows[i], true)
			continue
		}
		if reflection(columns[i], true) != -1 {
			result += reflection(columns[i], true)
		}
	}
	fmt.Println("Part 2:", result)
}

func main(){
	filename := "input.txt"
	rows,cols, err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	part1(rows,cols)
	part2(rows,cols)
}