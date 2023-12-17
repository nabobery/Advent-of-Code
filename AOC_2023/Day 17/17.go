package main

import (
	"bufio"
	"fmt"
	"os"
	"container/heap"
)

type state struct {
	coord     [2]int
	dir       int
	stepCount int
}

type comparator []*pair

type pair struct {
	val  int
	item state
}

func (c comparator) Len() int           { return len(c) }
func (c comparator) Less(i, j int) bool { return c[i].val < c[j].val }
func (c comparator) Swap(i, j int)      { c[i], c[j] = c[j], c[i] }

func (c *comparator) Push(x interface{}) {
	*c = append(*c, x.(*pair))
}

func (c *comparator) Pop() interface{} {
	old := *c
	n := len(old)
	item := old[n-1]
	*c = old[0 : n-1]
	return item
}

const INF int = 1e7
var dirs = [][2]int{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}
var n,m int

func initialize(filename string) ([]string, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	var grid []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		grid = append(grid, scanner.Text())
	}
	if err := scanner.Err(); err != nil {
		return nil, err
	}
	return grid, nil
}

func isValid(curr [2]int) bool {
	return curr[0] >= 0 && curr[0] < n && curr[1] >= 0 && curr[1] < m
}

func getNeighbours(curr state, minStepCount, maxStepCount int) []state {
	var neighbours []state
	if curr.stepCount >= minStepCount {
		d1 := (curr.dir + 1) % 4
		d2 := (curr.dir + 3) % 4
		next := [2]int{curr.coord[0] + dirs[d1][0], curr.coord[1] + dirs[d1][1]}
		if isValid(next) {
			neighbours = append(neighbours, state{next, d1, 1})
		}
		next = [2]int{curr.coord[0] + dirs[d2][0], curr.coord[1] + dirs[d2][1]}
		if isValid(next) {
			neighbours = append(neighbours, state{next, d2, 1})
		}
	}
	if curr.stepCount < maxStepCount {
		next := [2]int{curr.coord[0] + dirs[curr.dir][0], curr.coord[1] + dirs[curr.dir][1]}
		if isValid(next) {
			neighbours = append(neighbours, state{next, curr.dir, curr.stepCount + 1})
		}
	}
	return neighbours
}

func djikstra(grid []string, minStepCount int, maxStepCount int) int {
	pq := make(comparator, 0)
	heap.Init(&pq)
	visited := make([][][][]int, n)
	for i := range visited {
		visited[i] = make([][][]int, m)
		for j := range visited[i] {
			visited[i][j] = make([][]int, 4)
			for k := range visited[i][j] {
				visited[i][j][k] = make([]int, maxStepCount+1)
				for l := range visited[i][j][k] {
					visited[i][j][k][l] = INF
				}
			}
		}
	}

	for i := 0; i < len(dirs); i++ {
		pq.Push(&pair{0, state{[2]int{0, 0}, i, 0}})
		visited[0][0][i][0] = 0
	}

	for pq.Len() > 0 {
		item := heap.Pop(&pq).(*pair)
		heatloss, curr := item.val, item.item
		if heatloss > visited[curr.coord[0]][curr.coord[1]][curr.dir][curr.stepCount] {
			continue
		}
		for _, next := range getNeighbours(curr, minStepCount, maxStepCount) {
			newHeatLoss := heatloss + int(grid[next.coord[0]][next.coord[1]] - '0')
			if newHeatLoss < visited[next.coord[0]][next.coord[1]][next.dir][next.stepCount] {
				visited[next.coord[0]][next.coord[1]][next.dir][next.stepCount] = newHeatLoss
				heap.Push(&pq, &pair{newHeatLoss, next})
			}
		}
	}
	ans := INF
	for i := 0; i < 4; i++ {
		for j := 0; j <= maxStepCount; j++ {
			ans = min(ans, visited[n-1][m-1][i][j])
		}
	}
	return ans
}

func part1(grid []string) {
	fmt.Println("Part 1:", djikstra(grid, 0, 3))
}

func part2(grid []string) {
	fmt.Println("Part 2:", djikstra(grid, 4, 10))
}

func main(){
	filename := "input.txt"
	grid, err := initialize(filename)
	n = len(grid)
	m = len(grid[0])
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	part1(grid)
	part2(grid)
}