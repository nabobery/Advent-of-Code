package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"

)

type ffModule struct {
	rhs   []string
	state bool
}

type conModule struct {
	rhs   []string
	state map[string]bool
}

type state struct {
	parent, curr string
	state bool
}

type Solution struct {
	broadcasters       []string
	flipflopmodules    map[string]ffModule
	conjunctionmodules map[string]conModule
	inputs             map[string][]string
	part1result        [2]int
	part2result        []int
	visited            map[string]bool
}

func isAlpha(c byte) bool {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
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

func (s *Solution) initialize(filename string) () {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Println("Error reading file", err)
	}
	defer file.Close()
	s.flipflopmodules = make(map[string]ffModule)
	s.conjunctionmodules = make(map[string]conModule)
	s.inputs = make(map[string][]string)
	s.visited = make(map[string]bool)
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		if !isAlpha(line[0]) {
			mod := line[1:strings.Index(line, " ")]
			rhs := line[strings.Index(line, ">")+1:]
			dest := make([]string, 0)
			for _, token := range strings.Split(rhs, ",") {
				input := token[1:]
				s.inputs[input] = append(s.inputs[input], mod)
				dest = append(dest, input)
			}
			if line[0] == '%' {
				s.flipflopmodules[mod] = ffModule{rhs: dest, state: false}
			} else {
				s.conjunctionmodules[mod] = conModule{rhs: dest, state: make(map[string]bool)}
			}
		} else {
			rhs := line[strings.Index(line, ">")+1:]
			for _, token := range strings.Split(rhs, ",") {
				s.broadcasters = append(s.broadcasters, token[1:])
			}
		}
	}
	for name, con := range s.conjunctionmodules {
		for _, input := range s.inputs[name] {
			con.state[input] = false
		}
	}
	for _, it := range s.inputs["zh"] {
		s.visited[it] = true
	}
	s.simulate()
}

func (s *Solution) simulate() {
	push := 0
	for len(s.visited) > 0 || push < 1000 {
		q := make([]state, 0)
		for _, b := range s.broadcasters {
			q = append(q, state{"button", b, false})
		}
		for len(q) > 0 {
			curr := q[0]
			q = q[1:]
			if push < 1000 {
				if curr.state {
					s.part1result[1]++
				} else {
					s.part1result[0]++
				}
			}
			if !curr.state && s.visited[curr.curr] {
				delete(s.visited, curr.curr)
				s.part2result = append(s.part2result, push+1)
				if len(s.visited) == 0 {
					return
				}
			}
			if val, ok := s.flipflopmodules[curr.curr]; ok {
				if curr.state {
					continue
				} else {
					if val.state {
						s.flipflopmodules[curr.curr] = ffModule{rhs: val.rhs, state: false}
						for _, dest := range val.rhs {
							q = append(q, state{curr.curr, dest, false})
						}
					} else {
						s.flipflopmodules[curr.curr] = ffModule{rhs: val.rhs, state: true}
						for _, dest := range val.rhs {
							q = append(q, state{curr.curr, dest, true})
						}
					}
				}
			} else if val, ok := s.conjunctionmodules[curr.curr]; ok {
				val.state[curr.parent] = curr.state
				b := true
				for _, st := range val.state {
					b = b && st
				}
				if b {
					for _, dest := range val.rhs {
						q = append(q, state{curr.curr, dest, false})
					}
				} else {
					for _, dest := range val.rhs {
						q = append(q, state{curr.curr, dest, true})
					}
				}
			}
		}
		push++
	}
}

func (s *Solution) part1() {
	fmt.Printf("Part 1: %d\n", s.part1result[0]*s.part1result[1])
}

func (s *Solution) part2() {
	result := int64(1)
	for _, cycle := range s.part2result {
		result = lcm(result, int64(cycle))
	}
	fmt.Printf("Part 2: %d\n", result)
}

func main(){
	filename := "input.txt"
	s := Solution{}
	s.initialize(filename)
	s.part1()
	s.part2()
}