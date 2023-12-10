package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func initialize(filename string) (string, map[string][]string, error) {
	file, err := os.Open(filename)
	if err != nil {
		return "", nil, err
	}
	defer file.Close()
	var directions string
	nodes := make(map[string][]string)
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		text := scanner.Text()
		if strings.Contains(text, "=") {
			var temp []string
			node := text[0: strings.Index(text, " =")]
			temp = append(temp, text[strings.Index(text, "(") + 1: strings.Index(text, ",")] )
			temp = append(temp, text[strings.Index(text, ",") + 2: strings.Index(text, ")")] )
			nodes[node] = temp
			
		} else {
			if directions == "" {
				directions = scanner.Text()
			}
		}	
	}
	if err := scanner.Err(); err != nil {
		return "", nil, err
	}
	return directions, nodes, nil
}

func gcd(a, b int64) int64 {
	for b != 0 {
		a, b = b, a%b
	}
	return a
}

func lcm(a, b int64) int64 {
	return a * b / gcd(a, b)
}

func part1(directions string, nodes map[string][]string) {
	result := 0
	n := len(directions)
	start := "AAA"
	end := "ZZZ"
	for start != end {
		if directions[result%n] == 'L' {
			start = nodes[start][0]
		} else{
			start = nodes[start][1]
		}
		result++
	}
	fmt.Println("Part 1:", result)
}

func part2(directions string, nodes map[string][]string) {
	result := int64(1)
	n := len(directions)
	var startpos []string
	for key := range nodes {
		if key[2] == 'A'{
			startpos = append(startpos, key)
		}
	}
	for _, start := range startpos {
		cnt := int64(0)
		for start[2] != 'Z' {
			if directions[int(cnt)%n] == 'L' {
				start = nodes[start][0]
			} else{
				start = nodes[start][1]
			}
			cnt++
		}
		result = lcm(result, cnt)
	}
	fmt.Println("Part 2:", result)
}

func main(){
	filename := "input.txt"
	directions, nodes, err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	part1(directions, nodes)
	part2(directions, nodes)
}