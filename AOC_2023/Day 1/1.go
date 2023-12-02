package main

import (
	"bufio"
	"fmt"
	"os"
	"unicode"
)

func initialize(filename string) ([]string, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	var lines []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}
	if err := scanner.Err(); err != nil {
		return nil, err
	}
	return lines, nil
}

func part1(input []string) {
	result := 0
	for _, str := range input {
		for i := 0; i < len(str); i++ {
			if unicode.IsDigit(rune(str[i])) {
				result += 10*int(str[i] - '0')
				break
			}
		}
		for i := len(str) - 1; i >= 0; i-- {
			if unicode.IsDigit(rune(str[i])) {
				result += int(str[i] - '0')
				break
			}
		}
	}
	fmt.Println("Part 1:", result)
}

func part2(input []string) {
	result := 0
	mp := map[string]int{"one": 1,"two": 2,"three": 3,"four": 4,"five": 5,"six": 6,"seven": 7,"eight": 8,"nine": 9,}
	start := map[rune][]string{'o' : []string{"one"},'t' : []string{"two", "three"},'f' : []string{"four", "five"},'s' : []string{"six", "seven"}, 'e' : []string{"eight"},'n' : []string{"nine"},}
	end := map[rune][]string{'e' : []string{"one", "three", "five", "nine"},'o' : []string{"two"},'r' : []string{"four"},'x' : []string{"six"},'n' : []string{"seven"}, 't' : []string{"eight"},}
	for _, str := range input {
		for i := 0; i < len(str); i++ {
			if val, ok := start[rune(str[i])]; ok {
				b := -1
				for _, s := range val {
					if (i+len(s) <= len(str)) && str[i:i+len(s)] == s {
						b = 10*mp[s]
						break
					}
				}
				if b != -1 {
					result += b
					break
				}
			} 
			if unicode.IsDigit(rune(str[i])) {
				result += 10*int(str[i] - '0')
				break
			}
		}
		for i := len(str) - 1; i >= 0; i-- {
			if val, ok := end[rune(str[i])]; ok {
				b := -1
				for _, s := range val {
					if (i - len(s) >= 0) && str[i-len(s)+1:i+1] == s {
						b = mp[s]
						break
					}
				}
				if b != -1 {
					result += b
					break
				}
			} 
			if unicode.IsDigit(rune(str[i])) {
				result += int(str[i] - '0')
				break
			}
		}
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