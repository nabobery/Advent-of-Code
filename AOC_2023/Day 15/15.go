package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type lensSlot struct {
	focalLength int
	slot int
}

func initialize(filename string) ([]string, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	var hashes []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		text := scanner.Text()
		for _, hash := range strings.Split(text, ",") {
			hashes = append(hashes, hash)
		}
	}
	if err := scanner.Err(); err != nil {
		return nil, err
	}
	return hashes, nil
}

func hashAlgo(input string) int {
	result := 0
	for _, char := range input {
		result += int(char)
		result *= 17
		result %= 256
	}
	return result
}

func part1(input []string) {
	result := 0
	for _, hash := range input {
		result += hashAlgo(hash)
	}
	fmt.Println("Part 1:", result)
}

func part2(input []string) {
	result := 0
	boxes := make([]map[string]*lensSlot, 256)
	for i := range boxes {
		boxes[i] = make(map[string]*lensSlot)
	}
	for _, hash := range input {
		if strings.Contains(hash, "=") {
			label := hash[:strings.Index(hash, "=")]
			boxNum := hashAlgo(label)
			focalLength := int(hash[len(hash)-1] - '0')
			if _, ok := boxes[boxNum][label]; ok {
				boxes[boxNum][label] = &lensSlot{focalLength, boxes[boxNum][label].slot}
			} else {
				boxes[boxNum][label] = &lensSlot{focalLength, len(boxes[boxNum])+1}
			}
		} else{
			label := hash[:strings.Index(hash, "-")]
			boxNum := hashAlgo(label)
			if _, ok := boxes[boxNum][label]; ok {
				pos := boxes[boxNum][label].slot
				delete(boxes[boxNum], label)
				for _, lens := range boxes[boxNum] {
					if lens.slot > pos {
						lens.slot -= 1
					}
				}
			}
		}
	}
	for i, box := range boxes {
		for _, lens := range box {
			result += (lens.focalLength * lens.slot*(i+1))
		}
	}
	fmt.Println("Part 2:", result)
}

func main(){
	filename := "input.txt"
	hashes, err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	part1(hashes)
	part2(hashes)
}