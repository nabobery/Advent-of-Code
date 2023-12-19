package main

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"strconv"
	"strings"
)

type part struct {
	xmas [4]int
}

var chartoIndex = map[rune]int{'x': 0, 'm': 1, 'a': 2, 's': 3}
var parts []part
var rules map[string][]string

func initialize(filename string) (error) {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()
	partRatingsPattern := regexp.MustCompile(`\d+`)
	workflowPattern := regexp.MustCompile(`(\w+)\{(.*)\}`)
	scanner := bufio.NewScanner(file)
	rules = make(map[string][]string)
	for scanner.Scan() {
		line := scanner.Text()
		if strings.Contains(line, ":") {
			match := workflowPattern.FindStringSubmatch(line)
			if len(match) == 3 {
				key := match[1]
				flows := strings.Split(match[2], ",")
				for i := range flows {
					flows[i] = strings.TrimSpace(flows[i])
				}
				rules[key] = flows
			}
		} else {
			var pr part
			matches := partRatingsPattern.FindAllString(line, -1)
			for i, match := range matches {
				pr.xmas[i], _ = strconv.Atoi(match)
			}
			parts = append(parts, pr)
		}
	}
	if err := scanner.Err(); err != nil {
		return err
	}
	return nil
}

func isPartAccepted(pr part) bool {
	curr := "in"
	for curr != "R" && curr != "A" {
		for _, flow := range rules[curr] {
			if strings.Contains(flow, ":") {
				b := false
				next := flow[strings.Index(flow, ":")+1:]
				expr := flow[:strings.Index(flow, ":")]
				if strings.Contains(expr, "<") {
					for char, index := range chartoIndex {
						if strings.Contains(expr, string(char)) {
							val, _ := strconv.Atoi(expr[strings.Index(expr, "<")+1:])
							if pr.xmas[index] < val {
								curr = next
								b = true
								break
							}
						}
					}
				} else if strings.Contains(expr, ">") {
					for char, index := range chartoIndex {
						if strings.Contains(expr, string(char)) {
							val, _ := strconv.Atoi(expr[strings.Index(expr, ">")+1:])
							if pr.xmas[index] > val {
								curr = next
								b = true
								break
							}
						}
					}
				}
				if b {break}
			} else {
				curr = flow
				break
			}
		}
	}
	return curr == "A"
}

func part1() {
	result := 0
	for _, p := range parts {
		if isPartAccepted(p) {
			for _, val := range p.xmas {
				result += val
			}
		}
	}
	fmt.Println("Part 1:", result)
}

func getCombinations(xmasIntervals [][]int) int64 {
	result := int64(1)
	for _, it := range xmasIntervals {
		result *= int64(it[1] - it[0] + 1)
	}
	return result
}

func helper(xmasIntervals [][]int, curr string) int64 {
	result := int64(0)
	for _, flow := range rules[curr] {
		if strings.Contains(flow, ":") {
			next := flow[strings.Index(flow, ":")+1:]
			expr := flow[:strings.Index(flow, ":")]
			xmasIntervalsNext := make([][]int, len(xmasIntervals))
			for i, it := range xmasIntervals {
				xmasIntervalsNext[i] = make([]int, 2)
				xmasIntervalsNext[i][0] = it[0]
				xmasIntervalsNext[i][1] = it[1]
			}
			if strings.Contains(expr, "<") {
				for char, index := range chartoIndex {
					if strings.Contains(expr, string(char)) {
						val, _ := strconv.Atoi(expr[strings.Index(expr, "<")+1:])
						if val > xmasIntervalsNext[index][0] {
							xmasIntervalsNext[index][1] = min(xmasIntervalsNext[index][1], val-1)
							if next == "A" {
								result += getCombinations(xmasIntervalsNext)
							} else if next != "R" {
								result += helper(xmasIntervalsNext, next)
							}
							xmasIntervals[index][0] = max(xmasIntervals[index][0], val)
						}
					}
				}
			} else if strings.Contains(expr, ">") {
				for char, index := range chartoIndex {
					if strings.Contains(expr, string(char)) {
						val, _ := strconv.Atoi(expr[strings.Index(expr, ">")+1:])
						if val < xmasIntervalsNext[index][1] {
							xmasIntervalsNext[index][0] = max(xmasIntervalsNext[index][0], val+1)
							if next == "A" {
								result += getCombinations(xmasIntervalsNext)
							} else if next != "R" {
								result += helper(xmasIntervalsNext, next)
							}
							xmasIntervals[index][1] = min(xmasIntervals[index][1], val)
						}
					}
				}
			}
		} else{
			if flow == "A" {
				result += getCombinations(xmasIntervals)
			} else if flow != "R" {
				result += helper(xmasIntervals, flow)
			}
		}
	}
	return result
}

func part2() {
	fmt.Println("Part 2:", helper([][]int{{1, 4000}, {1, 4000}, {1, 4000}, {1, 4000}}, "in"))
}

func main(){
	filename := "input.txt"
	err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	part1()
	part2()
}