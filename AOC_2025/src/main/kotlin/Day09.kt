/**
 * # Advent of Code 2025 - Day 9: Maximum Rectangle in Orthogonal Polygon
 *
 * This solution finds the largest rectangle using red tiles (polygon vertices)
 * as opposite corners.
 *
 * ## Problem Summary:
 * - **Part 1**: Find the largest rectangle using any two vertices as opposite corners
 *   (ignoring polygon boundary constraints).
 * - **Part 2**: Find the largest rectangle where all interior points are within
 *   the polygon (on or inside the boundary).
 *
 * ## Key Algorithms:
 * - **Coordinate Compression**: Reduces the coordinate space from O(max_coord²) to O(n²)
 * - **Ray-Casting**: Determines if a point is inside the orthogonal polygon
 * - **2D Prefix Sum**: Enables O(1) rectangle interior validation queries
 *
 */

package org.nabobery

/**
 * Holds pre-computed data structures for O(1) interior rectangle validation.
 *
 * The coordinate compression maps the sparse original coordinates to a dense grid,
 * allowing efficient point-in-polygon queries via the prefix sum array.
 *
 * @property grid Binary grid where 1 = cell is inside polygon, 0 = outside
 * @property prefix 2D prefix sum array for O(1) range sum queries
 * @property xMap Maps original X coordinates to compressed grid indices
 * @property yMap Maps original Y coordinates to compressed grid indices
 * @property xCoords Sorted list of unique X coordinates
 * @property yCoords Sorted list of unique Y coordinates
 */
private data class InteriorGridData(
    val grid: Array<IntArray>,
    val prefix: Array<LongArray>,
    val xMap: Map<Int, Int>,
    val yMap: Map<Int, Int>,
    val xCoords: List<Int>,
    val yCoords: List<Int>,
) {
    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is InteriorGridData) return false
        return xMap == other.xMap && yMap == other.yMap
    }

    override fun hashCode(): Int = 31 * xMap.hashCode() + yMap.hashCode()
}

/**
 * Encapsulates Day 9 puzzle logic with coordinate compression and prefix sum optimization.
 *
 * The solver preprocesses the polygon vertices to build a compressed interior grid,
 * enabling O(1) validation of whether a candidate rectangle lies entirely within
 * the polygon boundary.
 *
 * @param input Raw input lines containing vertex coordinates in "X,Y" format
 */
private class Day09Solver(
    input: List<String>,
) {
    /** Parsed polygon vertices as Points */
    private val vertices: List<Point> = input.map(Point::parse)

    /** Lazily computed interior grid data (only needed for Part 2) */
    private val interiorData: InteriorGridData by lazy { buildInteriorGrid() }

    /**
     * **Part 1**: Find the maximum rectangle area using any two vertices as opposite corners.
     *
     * This ignores the polygon boundary constraint - we simply find the pair of vertices
     * that produces the largest rectangle.
     *
     * Time Complexity: O(n²) where n = number of vertices
     *
     * @return Maximum rectangle area
     */
    fun solvePart1(): Long =
        vertices.indices
            .asSequence()
            .flatMap { i ->
                (i + 1 until vertices.size).asSequence().map { j -> i to j }
            }.maxOfOrNull { (i, j) -> vertices[i].rectangleAreaWith(vertices[j]) }
            ?: 0L

    /**
     * **Part 2**: Find the maximum rectangle area where all interior points are within the polygon.
     *
     * Uses coordinate compression + prefix sum to validate each candidate rectangle in O(1).
     * A rectangle is valid if every compressed cell between the two corner vertices
     * is marked as "inside" the polygon.
     *
     * Time Complexity: O(n² log n) preprocessing + O(n²) validation = O(n²)
     *
     * @return Maximum valid rectangle area
     */
    fun solvePart2(): Long {
        val (_, prefix, xMap, yMap, _, _) = interiorData

        return vertices.indices
            .asSequence()
            .flatMap { i ->
                (i + 1 until vertices.size).asSequence().map { j -> i to j }
            }.filter { (i, j) ->
                val p1 = vertices[i]
                val p2 = vertices[j]

                // Map vertices to compressed grid coordinates
                val cx1 = minOf(xMap[p1.x]!!, xMap[p2.x]!!)
                val cx2 = maxOf(xMap[p1.x]!!, xMap[p2.x]!!)
                val cy1 = minOf(yMap[p1.y]!!, yMap[p2.y]!!)
                val cy2 = maxOf(yMap[p1.y]!!, yMap[p2.y]!!)

                isValidInteriorRectangle(cx1, cy1, cx2, cy2, prefix)
            }.maxOfOrNull { (i, j) -> vertices[i].rectangleAreaWith(vertices[j]) }
            ?: 0L
    }

    /**
     * Builds the compressed interior grid using ray-casting for point-in-polygon tests.
     *
     * Algorithm:
     * 1. Extract unique X and Y coordinates, creating coordinate-to-index mappings
     * 2. For each compressed cell, test if its center is inside the polygon
     * 3. Build 2D prefix sum for O(1) range queries
     */
    private fun buildInteriorGrid(): InteriorGridData {
        // Phase 1: Coordinate compression
        val xCoords = vertices.map { it.x }.distinct().sorted()
        val yCoords = vertices.map { it.y }.distinct().sorted()
        val xMap = xCoords.withIndex().associate { it.value to it.index }
        val yMap = yCoords.withIndex().associate { it.value to it.index }

        val gridWidth = xCoords.size
        val gridHeight = yCoords.size

        // Phase 2: Build interior grid using ray-casting
        val grid =
            Array(gridHeight) { y ->
                IntArray(gridWidth) { x ->
                    // Last row/column are boundary cells, always outside
                    if (x == gridWidth - 1 || y == gridHeight - 1) {
                        0
                    } else {
                        // Test center of the compressed cell region
                        val centerX = (xCoords[x] + xCoords[x + 1]) / 2.0
                        val centerY = (yCoords[y] + yCoords[y + 1]) / 2.0
                        if (isPointInPolygon(centerX, centerY)) 1 else 0
                    }
                }
            }

        // Phase 3: Build 2D prefix sum
        val prefix = Array(gridHeight) { LongArray(gridWidth) }
        for (y in 0 until gridHeight) {
            for (x in 0 until gridWidth) {
                prefix[y][x] = grid[y][x].toLong()
                if (y > 0) prefix[y][x] += prefix[y - 1][x]
                if (x > 0) prefix[y][x] += prefix[y][x - 1]
                if (y > 0 && x > 0) prefix[y][x] -= prefix[y - 1][x - 1]
            }
        }

        return InteriorGridData(grid, prefix, xMap, yMap, xCoords, yCoords)
    }

    /**
     * Tests if a point is inside the polygon using the ray-casting algorithm.
     *
     * Casts a horizontal ray to the right from the test point and counts
     * how many polygon edges it crosses. An odd count means inside.
     *
     * @param px X coordinate of test point
     * @param py Y coordinate of test point
     * @return true if point is inside the polygon
     */
    private fun isPointInPolygon(
        px: Double,
        py: Double,
    ): Boolean {
        var crossings = 0

        for (i in vertices.indices) {
            val p1 = vertices[i]
            val p2 = vertices[(i + 1) % vertices.size]

            val y1 = p1.y.toDouble()
            val y2 = p2.y.toDouble()

            // Check if edge straddles the horizontal line y = py
            if ((py in y1..<y2) || (py in y2..<y1)) {
                val x1 = p1.x.toDouble()
                val x2 = p2.x.toDouble()

                // Calculate x-coordinate where edge crosses y = py
                val intersectionX = x1 + (py - y1) * (x2 - x1) / (y2 - y1)

                // Count if intersection is to the right of test point
                if (intersectionX > px) crossings++
            }
        }

        return crossings % 2 == 1
    }

    /**
     * Validates if a compressed rectangle contains only interior cells.
     *
     * Uses prefix sum for O(1) range sum query. A rectangle is valid if
     * the sum of all cells equals the expected count (all 1s).
     *
     * @param cx1 Left compressed X coordinate
     * @param cy1 Top compressed Y coordinate
     * @param cx2 Right compressed X coordinate
     * @param cy2 Bottom compressed Y coordinate
     * @param prefix 2D prefix sum array
     * @return true if all cells in the rectangle are inside the polygon
     */
    private fun isValidInteriorRectangle(
        cx1: Int,
        cy1: Int,
        cx2: Int,
        cy2: Int,
        prefix: Array<LongArray>,
    ): Boolean {
        if (cx2 <= cx1 || cy2 <= cy1) return true

        val expectedSum = (cx2 - cx1).toLong() * (cy2 - cy1)
        val actualSum = rectangleSum(cx1, cy1, cx2 - 1, cy2 - 1, prefix)
        return actualSum == expectedSum
    }

    /**
     * Computes the sum of a rectangular region using prefix sum in O(1).
     *
     * Uses inclusion-exclusion principle:
     * sum(x1,y1 to x2,y2) = prefix[y2][x2] - prefix[y2][x1-1] - prefix[y1-1][x2] + prefix[y1-1][x1-1]
     */
    private fun rectangleSum(
        x1: Int,
        y1: Int,
        x2: Int,
        y2: Int,
        prefix: Array<LongArray>,
    ): Long {
        var sum = prefix[y2][x2]
        if (x1 > 0) sum -= prefix[y2][x1 - 1]
        if (y1 > 0) sum -= prefix[y1 - 1][x2]
        if (x1 > 0 && y1 > 0) sum += prefix[y1 - 1][x1 - 1]
        return sum
    }
}

/**
 * Main entry point for Day 9 solution.
 */
fun main() {
    val input = readInput(9)
    val solver = Day09Solver(input)

    println("Part 1: ${solver.solvePart1()}")
    println("Part 2: ${solver.solvePart2()}")
}
