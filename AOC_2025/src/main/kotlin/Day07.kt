package org.nabobery

/**
 * Encapsulates the Day 7 puzzle logic for tachyon manifold beam simulation.
 * Processes a grid where 'S' represents the tachyon beam source and '^' represents splitters that split beams.
 */
private class Day07Solver(
    input: List<String>,
) {
    private val grid: List<String> = input
    private val rows: Int = grid.size
    private val cols: Int = grid.firstOrNull()?.length ?: 0

    /**
     * Finds the initial tachyon beam source position in the first row.
     * @return Column index where 'S' is found, or null if not found
     */
    private fun findInitialBeamSource(): Int? = grid.firstOrNull()?.indexOf('S')?.takeIf { it >= 0 }

    /**
     * Checks if a column index is within valid bounds.
     */
    private fun Int.isValidColumn(): Boolean = this in 0 until cols

    /**
     * Part 1: Counts how many times the beam is split by splitter ('^') characters.
     * Tachyon beams start at 'S' and move downward, splitting when hitting '^' splitters.
     */
    fun solvePart1(): Int {
        val initialSource = findInitialBeamSource() ?: return 0
        var activeBeams = setOf(initialSource)
        var splittersHit = 0

        for (row in 1 until rows) {
            val nextBeams = mutableSetOf<Int>()
            val mirrorsInRow = mutableSetOf<Int>()

            // Identify splitters in current row
            grid[row].forEachIndexed { col, char ->
                if (char == '^') {
                    mirrorsInRow.add(col)
                }
            }

            // Process beams: if they hit a splitter, split; otherwise continue
            activeBeams.forEach { beamCol ->
                if (beamCol in mirrorsInRow) {
                    splittersHit++
                    // Split beam: add left and right neighbors if valid
                    (beamCol - 1).takeIf { it.isValidColumn() }?.let { nextBeams.add(it) }
                    (beamCol + 1).takeIf { it.isValidColumn() }?.let { nextBeams.add(it) }
                } else {
                    // Beam continues straight down (same column)
                    nextBeams.add(beamCol)
                }
            }

            activeBeams = nextBeams
        }

        return splittersHit
    }

    /**
     * Part 2: Counts the total number of timelines using quantum tachyon interpretation.
     * In the quantum manifold, a single particle takes both left and right paths at each splitter,
     * creating multiple timelines. Counts the final number of active timelines.
     */
    fun solvePart2(): Long {
        val initialSource = findInitialBeamSource() ?: return 0L
        var timelines = LongArray(cols) { if (it == initialSource) 1L else 0L }

        for (row in 1 until rows) {
            val nextTimelines = LongArray(cols) { 0L }

            for (col in 0 until cols) {
                when (grid[row][col]) {
                    '^' -> {
                        // Splitter creates two timelines: left and right paths
                        if (col > 0) {
                            nextTimelines[col - 1] += timelines[col]
                        }
                        if (col < cols - 1) {
                            nextTimelines[col + 1] += timelines[col]
                        }
                        // Current column gets no timelines (particle splits into two timelines)
                    }
                    else -> {
                        // Beam continues straight down
                        nextTimelines[col] += timelines[col]
                    }
                }
            }

            timelines = nextTimelines
        }

        return timelines.sum()
    }
}

fun main() {
    val input = readInput(7)
    val solver = Day07Solver(input)

    println("part 1: ${solver.solvePart1()}")
    println("part 2: ${solver.solvePart2()}")
}
