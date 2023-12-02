package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"strconv"
	"unicode"
)

func initialize(filename string) ([]Game, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	var games []Game
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		itrs := strings.Split(scanner.Text()[strings.Index(scanner.Text(), ":")+2:], ";")
		var itrsList []Iterator
		for _, itr := range itrs {
			cubes := strings.Split(itr, ",")
			nums := make([]int, 3)
			for _, cube := range cubes {
				num,err := strconv.Atoi(strings.TrimSpace(strings.TrimRightFunc(cube, unicode.IsLetter)))
				if err != nil {
					return nil, err
				}
				str := strings.TrimLeftFunc(cube, func(r rune) bool {
					return unicode.IsDigit(r) || unicode.IsSpace(r)
				})
				if str == "red" {
					nums[0] = num
				} else if str == "green" {
					nums[1] = num
				}
				if str == "blue" {
					nums[2] = num
				}
			}
			itrsList = append(itrsList, Iterator{Red: nums[0], Green: nums[1], Blue: nums[2]})
		}
		games = append(games, Game{Iterations: itrsList})
	}
	if err := scanner.Err(); err != nil {
		return nil, err
	}
	return games, nil
}

type Iterator struct {
	Red int
	Green int
	Blue int
}

type Game struct {
	Iterations []Iterator
}

func part1(input []Game) {
	res := 0
	for i, game := range input {
		b := true
		for _, itr := range game.Iterations {
			if itr.Red > 12 || itr.Green > 13 || itr.Blue > 14 {
				b = false
				break
			}
		}
		if b {
			res += i+1
		}
	}
	fmt.Println("Part 1:", res)
}

func part2(input []Game) {
	res := 0
	for _, game := range input {
		req := make([]int, 3)
		for _, itr := range game.Iterations {
			req[0] = max(req[0], itr.Red)
			req[1] = max(req[1], itr.Green)
			req[2] = max(req[2], itr.Blue)
		}
		res += req[0] * req[1] * req[2]
	}
	fmt.Println("Part 2:", res)
}


func main() {
	filename := "input.txt"
	games, err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	part1(games)
	part2(games)
}