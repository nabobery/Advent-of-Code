package main

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"math"
)

const INT_MIN = math.MinInt32

type Solution struct {
	vertexToInt map[string]int
	graph       map[int]map[int]bool
}

func (s *Solution) init(input *os.File) {
	s.vertexToInt = make(map[string]int)
	s.graph = make(map[int]map[int]bool)
	scanner := bufio.NewScanner(input)
	regexPattern := regexp.MustCompile(`[a-z]{3}`)
	vertexIndex := 0
	for scanner.Scan() {
		line := scanner.Text()
		matches := regexPattern.FindAllString(line, -1)

		if len(matches) > 0 {
			vertexA := matches[0]
			if _, ok := s.vertexToInt[vertexA]; !ok {
				s.vertexToInt[vertexA] = vertexIndex
				vertexIndex++
			}
			for i := 1; i < len(matches); i++ {
				vertexB := matches[i]

				if _, ok := s.vertexToInt[vertexB]; !ok {
					s.vertexToInt[vertexB] = vertexIndex
					vertexIndex++
				}
				edgeA := s.vertexToInt[vertexA]
				edgeB := s.vertexToInt[vertexB]
				if _, ok := s.graph[edgeA]; !ok {
					s.graph[edgeA] = make(map[int]bool)
				}
				if _, ok := s.graph[edgeB]; !ok {
					s.graph[edgeB] = make(map[int]bool)
				}
				s.graph[edgeA][edgeB] = true
				s.graph[edgeB][edgeA] = true
			}
		}
	}
}

func (s *Solution) globalMinCut() (int, []int) {
	n := len(s.vertexToInt)
	best := []int{n, 0}
	mat := make([][]int, n)
	for i := range mat {
		mat[i] = make([]int, n)
	}

	for src, neighbors := range s.graph {
		for dest := range neighbors {
			mat[src][dest] = 1
		}
	}

	co := make([][]int, n)
	for i := 0; i < n; i++ {
		co[i] = []int{i}
	}

	for ph := 1; ph < n; ph++ {
		w := make([]int, n)
		for i := 0; i < n; i++ {w[i] = mat[0][i]}
		s, t := 0, 0
		for j := 0; j < n-ph; j++ {
			w[t] = INT_MIN
			s, t = t, getMaxConnectivity(w)
			for i := 0; i < n; i++ {
				w[i] += mat[t][i]
			}
		}
		currentMinCut := w[t] - mat[t][t]
		if currentMinCut < best[0] {
			best[0] = currentMinCut
			best[1] = t
		}

		co[s] = append(co[s], co[t]...)
		for i := 0; i < n; i++ {
			mat[s][i] += mat[t][i]
			mat[i][s] = mat[s][i]
		}

		mat[0][t] = INT_MIN
	}

	return best[0], co[best[1]]
}

func (s *Solution) part1() {
	_, nodes := s.globalMinCut()
	result := len(nodes) * (len(s.vertexToInt) - len(nodes))
	fmt.Printf("Part 1: %d\n", result)
}

func (s *Solution) part2() {
	fmt.Println("Happy Christmas!")
	fmt.Println("THE END")
}

func getMaxConnectivity(w []int) int {
	maxIndex := 0
	for i := range w {
		if w[i] > w[maxIndex] {
			maxIndex = i
		}
	}
	return maxIndex
}

func main() {
	file, err := os.Open("input.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		return
	}
	defer file.Close()

	var s Solution
	s.init(file)
	s.part1()
	s.part2()
}
