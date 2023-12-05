package main

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"strings"
	"strconv"
	"math"
	"sort"
)

type mapping struct {
	dest int64
	src int64
	length int64
}

func Less(a, b mapping) bool {
	return a.src < b.src
}

func initialize(filename string) ([]int64, [][]mapping, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, nil, err
	}
	defer file.Close()
	reader := bufio.NewReader(file)
	pattern := regexp.MustCompile("\\b(\\d+)\\s+(\\d+)\\s+(\\d+)\\b")
	var seeds []int64
	var maps [][]mapping
	for {
		line, err := reader.ReadString('\n')
		if err != nil {
			break
		}
		if strings.Contains(line, "seeds:") {
			re := regexp.MustCompile("\\d+")
			for _, match := range re.FindAllString(line, -1) {
				num, _ := strconv.ParseInt(match, 10, 64)
				seeds = append(seeds, num)
			}
		}
		if strings.Contains(line, "map:") {
			line, _ = reader.ReadString('\n')
			temp := []mapping{}
			for pattern.MatchString(line) {
				matches := pattern.FindStringSubmatch(line)
				dest, _ := strconv.ParseInt(matches[1], 10, 64)
				src, _ := strconv.ParseInt(matches[2], 10, 64)
				length, _ := strconv.ParseInt(matches[3], 10, 64)
				temp = append(temp, mapping{dest, src, length})
				line, _ = reader.ReadString('\n')
			}
			maps = append(maps, temp)
		}
	}
	return seeds, maps, nil
}

func part1(seeds []int64, maps [][]mapping) {
	var result int64 = math.MaxInt64
	for _, seed := range seeds {
		temp := seed
		for _, m := range maps {
			for _, mapping := range m {
				if mapping.src <= temp && temp < mapping.src + mapping.length {
					temp += mapping.dest - mapping.src
					break
				}
			}
		}
		result = min(result, temp)
	}
	fmt.Println("Part 1:", result)
}

func part2(seeds []int64, maps [][]mapping) {
	var result int64 = math.MaxInt64
	for _, m := range maps {
		sort.Slice(m, func(i, j int) bool {
			return Less(m[i], m[j])
		})
	}
	//fmt.Println(maps)
	for i := 0; i < len(seeds); i+=2 {
		mapRanges := map[int64]int64{ seeds[i]: seeds[i+1] }
		for _, m := range maps {
			newRanges := map[int64]int64{}
			for src, length := range mapRanges {
				temp := map[int64]int64{}
				for _, mapping := range m {
					offset := mapping.dest - mapping.src
					if mapping.src <= src && src < mapping.src + mapping.length {
						if mapping.src + mapping.length >= src + length {
							temp[src + offset] = length
						} else {
							length -= (mapping.src + mapping.length - src)
							temp[src + offset] = mapping.src + mapping.length - src
							src = mapping.src + mapping.length
						}
					}
				}
				if len(temp) == 0 {	
					newRanges[src] = length
				} else {
					for key, value := range temp {
						newRanges[key] = value
					}
				}
			}
			mapRanges = map[int64]int64{}
			for src, length := range newRanges {
				mapRanges[src] = length
			}
		}
		var minSource int64 = math.MaxInt64
		for src := range mapRanges {
			minSource = min(minSource, src)
		}
		result = min(result, minSource)
	}
	fmt.Println("Part 2:", result)
}

func main() {
	filename := "input.txt"
	seeds, maps, err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	part1(seeds, maps)
	part2(seeds, maps)
}