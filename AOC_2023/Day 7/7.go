package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"strconv"
	"sort"
)

func initialize(filename string) (map[string]int, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	scanner := bufio.NewScanner(file)
	hands := make(map[string]int)
	for scanner.Scan() {
		camelCard := scanner.Text()
		hand := camelCard[:strings.Index(camelCard, " ")]
		card, _ := strconv.Atoi(camelCard[strings.Index(camelCard, " ")+1:])
		hands[hand] = card
	}
	return hands,nil
}

func order(hand string, newOrder bool) int {
	count := map[string]int{}
	for i := 0; i < len(hand); i++ {
		count[string(hand[i])]++
	}
	if !newOrder || count["J"] == 0 {
		if len(count) == 1 {
			return 7
		} else if len(count) == 2 {
			for _, v := range count {
				if v == 4 {
					return 6
				}
			}
			return 5
		} else if len(count) == 3 {
			for _, v := range count {
				if v == 3 {
					return 4
				}
			}
			return 3
		} else if len(count) == 4 {
			return 2
		}
		return 1
	}
	if len(count) == 1 || len(count) == 2 {
		return 7
	}
	if len(count) == 3 {
		for _, v := range count {
			if v == 3 {
				return 6
			}
		}
		if count["J"] == 2 {
			return 6
		}
		return 5
	}
	if len(count) == 4 {
		return 4
	}
	return 2
}

func part1(hands map[string]int) {
	result := 0
	keys := make([]string, 0, len(hands))
	for k := range hands {
		keys = append(keys, k)
	}
	ordering := map[rune]int{'A':14, 'K':13, 'Q':12, 'J':11, 'T':10, '9':9, '8':8, '7':7, '6':6, '5':5, '4':4, '3':3, '2':2}
	sort.Slice(keys, func(i, j int) bool {
		order1 := order(keys[i], false)
		order2 := order(keys[j], false)
		if order1 != order2 {
			return order1 < order2
		}
		for k := 0; k < len(keys[i]); k++ {
			if ordering[rune(keys[i][k])] != ordering[rune(keys[j][k])] {
				return ordering[rune(keys[i][k])] < ordering[rune(keys[j][k])]
			}
		}
		return false
	})
	for i := 0; i < len(keys); i++ {
		result += hands[keys[i]] * (i+1)
	}
	fmt.Println("Part 1:", result)
}

func part2(hands map[string]int) {
	result := 0
	keys := make([]string, 0, len(hands))
	for k := range hands {
		keys = append(keys, k)
	}
	ordering := map[rune]int{'A':14, 'K':13, 'Q':12, 'T': 10, '9':9, '8':8, '7':7, '6':6, '5':5, '4':4, '3':3, '2':2, 'J':1}
	sort.Slice(keys, func(i, j int) bool {
		order1 := order(keys[i], true)
		order2 := order(keys[j], true)
		if order1 != order2 {
			return order1 < order2
		}
		for k := 0; k < len(keys[i]); k++ {
			if ordering[rune(keys[i][k])] != ordering[rune(keys[j][k])] {
				return ordering[rune(keys[i][k])] < ordering[rune(keys[j][k])]
			}
		}
		return false
	})
	for i := 0; i < len(keys); i++ {
		result += hands[keys[i]] * (i+1)
	}
	fmt.Println("Part 2:", result)
}

func main() {
	filename := "input.txt"
	hands, err := initialize(filename)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	part1(hands)
	part2(hands)
}