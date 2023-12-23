package main

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"strconv"
	"sort"
)

type coord [3]int

type brick struct {
	start coord
	end   coord
}

type Solution struct {
	bricks       []brick
	support      map[int]map[int]bool
	supported    map[int]map[int]bool
	unsafebricks map[int]bool
}

func compare(b1, b2 brick) bool {
	return b1.start[2] < b2.start[2]
}

func (s *Solution) init(input *os.File) error {
	scanner := bufio.NewScanner(input)
	s.bricks = make([]brick, 0)
	re := regexp.MustCompile(`\d+`)
	for scanner.Scan() {
		line := scanner.Text()
		matches := re.FindAllString(line, -1)
		coords := make([]int, len(matches))
		for i, match := range matches {
			coords[i], _ = strconv.Atoi(match)
		}
		s.bricks = append(s.bricks, brick{coord{coords[0], coords[1], coords[2]}, coord{coords[3], coords[4], coords[5]}})
	}
	if err := scanner.Err(); err != nil {
		return err
	}
	s.support = make(map[int]map[int]bool)
	s.supported = make(map[int]map[int]bool)
	s.unsafebricks = make(map[int]bool)
	return nil
}

func (s *Solution) part1() {
	sort.Slice(s.bricks, func(i, j int) bool {
		return compare(s.bricks[i], s.bricks[j])
	})
	highest := make(map[[2]int][2]int)
	for i := 0; i < len(s.bricks); i++ {
		maxHeight := 0
		for x := s.bricks[i].start[0]; x <= s.bricks[i].end[0]; x++ {
			for y := s.bricks[i].start[1]; y <= s.bricks[i].end[1]; y++ {
				if val, ok := highest[[2]int{x, y}]; ok {
					maxHeight = max(maxHeight, val[0])
				}
			}
		}
		height := s.bricks[i].end[2] - s.bricks[i].start[2] + 1
		for x := s.bricks[i].start[0]; x <= s.bricks[i].end[0]; x++ {
			for y := s.bricks[i].start[1]; y <= s.bricks[i].end[1]; y++ {
				temp, ok := highest[[2]int{x, y}]
				if !ok { temp = [2]int{0, -1} }
				if ok && temp[0] == maxHeight {
					if _, ok := s.supported[temp[1]]; !ok { s.supported[temp[1]] = make(map[int]bool) }
					if _, ok := s.support[i]; !ok { s.support[i] = make(map[int]bool) }
					s.supported[temp[1]][i] = true
					s.support[i][temp[1]] = true
				}
				highest[[2]int{x, y}] = [2]int{maxHeight + height, i}
			}
		}
	}
	for i := range s.support[-1] {
		delete(s.support, i)
	}
	delete(s.supported, -1)
	for i := range s.support {
		if len(s.support[i]) == 1 {
			for j := range s.support[i] {
				s.unsafebricks[j] = true
			}
		}
	}
	fmt.Printf("Part 1: %d\n", len(s.bricks)-len(s.unsafebricks))
}

func (s *Solution) part2() {
	result := 0
	for brick := range s.unsafebricks {
		indegree := make(map[int]int)
		for k, v := range s.support {
			indegree[k] = len(v)
		}
		q := []int{brick}
		for len(q) > 0 {
			curr := q[0]
			q = q[1:]
			result++

			for it := range s.supported[curr] {
				indegree[it]--
				if indegree[it] == 0 {
					q = append(q, it)
				}
			}
		}
		result--
	}
	fmt.Printf("Part 2: %d\n", result)
}

func main() {
	file, err := os.Open("input.txt")
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	defer file.Close()
	var s Solution
	err = s.init(file)
	if err != nil {
		fmt.Println("Error initializing solution:", err)
		return
	}
	s.part1()
	s.part2()
}