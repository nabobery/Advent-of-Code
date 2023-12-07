package main

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"strconv"
)

func initialize(filename string) (map[string]string, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	reader := bufio.NewReader(file)
	re := regexp.MustCompile("\\d+")
	line1, _ := reader.ReadString('\n')
	line2, _ := reader.ReadString('\n')
	matches1 := re.FindAllString(line1, -1)
	matches2 := re.FindAllString(line2, -1)
	var result = map[string]string{}
	for i := 0; i < len(matches1); i++ {
		result[matches1[i]] = matches2[i]
	}
	return result, nil	
}

func part1(races map[string]string) {
	var result int64 = 1
	for time,dist := range races{
		time, _ := strconv.ParseInt(time, 10, 64)
		dist, _ := strconv.ParseInt(dist, 10, 64)
		for i := int64(1); i <= time; i++ {
			if (time-i)*i > dist {
				result *= (time - 2*i + 1)
				break
			}
		}
	}
	fmt.Println("Part 1:", result)
}

func part2(races map[string]string) {
	result := 0
	// concatenate all key and value strings into one string and convert to int64
	var t, d string
	for time, dist := range races{
		t += time
		d += dist
	}
	var time, dist int64
	time, _ = strconv.ParseInt(t, 10, 64)
	dist, _ = strconv.ParseInt(d, 10, 64)
	for i := int64(1); i <= time; i++ {
		if (time-i)*i > dist {
			result = int(time - 2*i + 1)
			break
		}
	}
	fmt.Println("Part 2:", result)
}

func main() {
	filename := "input.txt"
	races, err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	part1(races)
	part2(races)
}