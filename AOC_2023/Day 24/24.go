package main

import (
	"bufio"
	"fmt"
	"math"
	"os"
	"strconv"
	"regexp"
)

type hailstone struct {
	positions, velocities []float64
}

type solution struct {
	hailstones []hailstone
}

func parseFloat(s string) float64 {
	f, err := strconv.ParseFloat(s, 64)
	if err != nil { panic(err) }
	return f
}

func (s *solution) init(scanner *bufio.Scanner) {
	for scanner.Scan() {
		line := scanner.Text()
		var matches []string
		pattern := regexp.MustCompile(`-?\d+`)
		for _, match := range pattern.FindAllString(line, -1) {
			matches = append(matches, match)
		}
		h := hailstone{
			positions:  []float64{parseFloat(matches[0]), parseFloat(matches[1]), parseFloat(matches[2])},
			velocities: []float64{parseFloat(matches[3]), parseFloat(matches[4]), parseFloat(matches[5])},
		}
		s.hailstones = append(s.hailstones, h)
	}
}

func (s *solution) helper(pair [2]int, mn, mx int64) bool {
	i, j := pair[0], pair[1]
	x1, y1 := s.hailstones[i].positions[0], s.hailstones[i].positions[1]
	x2, y2 := s.hailstones[j].positions[0], s.hailstones[j].positions[1]
	vx1, vy1 := s.hailstones[i].velocities[0], s.hailstones[i].velocities[1]
	vx2, vy2 := s.hailstones[j].velocities[0], s.hailstones[j].velocities[1]
	t2 := (vy1*(x1-x2) + vx1*(y2-y1)) / (vy1*vx2 - vy2*vx1)
	t1 := (x2 + vx2*t2 - x1) / vx1
	if t1 < 0 || t2 < 0 {
		return false
	}
	posx := x1 + vx1*t1
	posy := y1 + vy1*t1
	if posx < float64(mn) || posx > float64(mx) || posy < float64(mn) || posy > float64(mx) {
		return false
	}
	return true
}

func (s *solution) part1() {
	result := 0
	mn, mx := int64(200000000000000), int64(400000000000000)
	for i := 0; i < len(s.hailstones); i++ {
		for j := i + 1; j < len(s.hailstones); j++ {
			if s.helper([2]int{i, j}, mn, mx) {
				result++
			}
		}
	}
	fmt.Println("Part 1:", result)
}

func determinant(mat [][]float64) float64 {
	size := len(mat)
	det := 0.0
	if size == 1 {
		return mat[0][0]
	} else if size == 2 {
		return mat[0][0]*mat[1][1] - mat[0][1]*mat[1][0]
	} else {
		for i := 0; i < size; i++ {
			temp := make([][]float64, size-1)
			for j := 1; j < size; j++ {
				temp[j-1] = make([]float64, size-1)
				for k := 0; k < size; k++ {
					if k < i {
						temp[j-1][k] = mat[j][k]
					} else if k > i {
						temp[j-1][k-1] = mat[j][k]
					}
				}
			}
			det += mat[0][i] * math.Pow(-1, float64(i)) * determinant(temp)
		}
	}
	return det
}

func cofactorMatrix(matrix [][]float64) [][]float64 {
	n := len(matrix)
	cofactors := make([][]float64, n)
	for i := 0; i < n; i++ {
		cofactors[i] = make([]float64, n)
		for j := 0; j < n; j++ {
			submatrix := make([][]float64, n-1)
			for k := 0; k < n-1; k++ { submatrix[k] = make([]float64, n-1) }
			for k := 0; k < n; k++ {
				for l := 0; l < n; l++ {
					if k != i && l != j {
						k_temp, l_temp := k, l
						if k >= i {k_temp--}
						if l >= j {l_temp--}
						submatrix[k_temp][l_temp] = matrix[k][l]
					}
				}
			}
			cofactors[i][j] = math.Pow(-1, float64(i+j)) * determinant(submatrix)
		}
	}
	return cofactors
}

func inverse(mat [][]float64) [][]float64 {
	size := len(mat)
	inv := make([][]float64, size)
	for i := 0; i < size; i++ {
		inv[i] = make([]float64, size)
	}
	det := determinant(mat)
	if det == 0 {
		fmt.Println("Inverse of the matrix doesn't exist")
		return inv
	}
	cofactors := cofactorMatrix(mat)
	for i := 0; i < size; i++ {
		for j := 0; j < size; j++ {
			inv[i][j] = cofactors[j][i] / det
		}
	}
	return inv
}

func multiply(mat1 [][]float64, mat2 []float64) []float64 {
	res := make([]float64, len(mat1))
	for i := 0; i < len(mat1); i++ {
		for j := 0; j < len(mat2); j++ {
			res[i] += mat1[i][j] * mat2[j]
		}
	}
	return res
}

func (s *solution) part2() {
	p1, p2, p3, v1, v2, v3 := s.hailstones[0].positions, s.hailstones[1].positions, s.hailstones[2].positions, s.hailstones[0].velocities, s.hailstones[1].velocities, s.hailstones[2].velocities
	A := [][]float64{
		{v1[1] - v2[1], v2[0] - v1[0], 0, p2[1] - p1[1], p1[0] - p2[0], 0},
		{v1[1] - v3[1], v3[0] - v1[0], 0, p3[1] - p1[1], p1[0] - p3[0], 0},
		{0, v1[2] - v2[2], v2[1] - v1[1], 0, p2[2] - p1[2], p1[1] - p2[1]},
		{0, v1[2] - v3[2], v3[1] - v1[1], 0, p3[2] - p1[2], p1[1] - p3[1]},
		{v2[2] - v1[2], 0, v1[0] - v2[0], p1[2] - p2[2], 0, p2[0] - p1[0]},
		{v3[2] - v1[2], 0, v1[0] - v3[0], p1[2] - p3[2], 0, p3[0] - p1[0]},
	}
	B := []float64{
		v2[0]*p2[1] + v1[1]*p1[0] - v1[0]*p1[1] - v2[1]*p2[0],
		v3[0]*p3[1] + v1[1]*p1[0] - v1[0]*p1[1] - v3[1]*p3[0],
		v2[1]*p2[2] + v1[2]*p1[1] - v1[1]*p1[2] - v2[2]*p2[1],
		v3[1]*p3[2] + v1[2]*p1[1] - v1[1]*p1[2] - v3[2]*p3[1],
		v2[2]*p2[0] + v1[0]*p1[2] - v1[2]*p1[0] - v2[0]*p2[2],
		v3[2]*p3[0] + v1[0]*p1[2] - v1[2]*p1[0] - v3[0]*p3[2],
	}
	res := multiply(inverse(A), B)
	//fmt.Println("Part 2:", res)
	fmt.Println("Part 2:", int64(res[0])+int64(res[1])+int64(res[2]))
}

func main() {
	file, err := os.Open("input.txt")
	if err != nil {
		fmt.Println("Error opening file:", err)
		return
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	s := solution{}
	s.init(scanner)
	s.part1()
	s.part2()
}
