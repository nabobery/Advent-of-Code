/**
 * # Advent of Code 2025 - Day 4: Paper Roll Cascade
 *
 * The Elves in the printing department have large rolls of paper arranged on a grid.
 * They need to figure out which rolls their forklifts can access so they can free
 * up the forklifts to break through a wall to the cafeteria.
 *
 * ## Problem Summary
 *
 * - **Part 1**: Count how many paper rolls (`@`) are accessible by a forklift.
 *   A roll is accessible if it has fewer than 4 adjacent paper rolls (8-directional neighbors).
 *
 * - **Part 2**: Simulate the cascade removal process. Once a roll is removed, its neighbors
 *   may become accessible. Continue removing accessible rolls until no more can be removed.
 *   Count the total number of rolls removed.
 *
 * ## Key Algorithms
 *
 * - **Part 1 - Neighbor Counting**: For each paper roll, count adjacent rolls using
 *   8-directional neighbors. If count < 4, the roll is accessible.
 *   Time Complexity: O(N) where N = number of paper rolls.
 *
 * - **Part 2 - BFS-Based Cascade Simulation**: Uses a queue-based approach:
 *   1. Initialize queue with all currently accessible rolls
 *   2. Remove each queued roll and check if its neighbors become accessible
 *   3. Newly accessible neighbors are added to the queue
 *   4. Continue until queue is empty
 *
 *   Time Complexity: O(N) amortized - each roll is processed at most once.
 *   Space Complexity: O(N) for the active set and pending removal tracking.
 *
 * ## Example Visualization
 *
 * Initial state (accessible rolls marked):
 * ```
 * ..@@.@@@@.    → ..xx.xx@x.
 * @@@.@.@.@@    → x@@.@.@.@@
 * @@@@@.@.@@    → @@@@@.x.@@
 * ...
 * ```
 *
 * The cascade continues until a stable state where all remaining rolls
 * have 4 or more adjacent neighbors.
 */

package org.nabobery

// ================================================================================================
// DATA MODELS
// ================================================================================================

/**
 * Represents the grid of paper rolls with operations for accessibility checks and cascade simulation.
 *
 * Paper rolls are represented by `@` characters in the input. The grid uses the shared [Point]
 * class from Utils with x=column, y=row mapping.
 *
 * @property rows Number of rows in the grid
 * @property cols Number of columns in the grid
 * @property paperRolls Set of positions containing paper rolls
 */
private class PaperRollGrid(
    private val rows: Int,
    private val cols: Int,
    private val paperRolls: Set<Point>,
) {
    /**
     * Checks if a point is within the grid boundaries.
     *
     * @return true if point coordinates are valid grid positions
     */
    private fun Point.isValid(): Boolean = y in 0 until rows && x in 0 until cols

    /**
     * Counts how many adjacent positions contain paper rolls.
     *
     * Uses 8-directional neighbors (cardinal + diagonal directions).
     *
     * @param position The position to check neighbors for
     * @param activeRolls Current set of active paper roll positions
     * @return Number of adjacent paper rolls (0-8)
     */
    private fun countAdjacentRolls(
        position: Point,
        activeRolls: Set<Point>,
    ): Int = position.neighbors().count { it in activeRolls }

    /**
     * Finds all paper rolls that are currently accessible by forklifts.
     *
     * A paper roll is accessible if it has fewer than [ACCESSIBILITY_THRESHOLD] adjacent rolls.
     *
     * @param activeRolls Current set of active paper roll positions
     * @return Set of positions that are accessible
     */
    private fun findAccessibleRolls(activeRolls: Set<Point>): Set<Point> =
        activeRolls.filterTo(mutableSetOf()) { position ->
            countAdjacentRolls(position, activeRolls) < ACCESSIBILITY_THRESHOLD
        }

    /**
     * Counts paper rolls accessible in the initial configuration.
     *
     * This is the solution for Part 1.
     *
     * @return Number of initially accessible paper rolls
     */
    fun countInitialAccessible(): Int = findAccessibleRolls(paperRolls).size

    /**
     * Simulates the cascade removal process and counts total removed rolls.
     *
     * ## Algorithm
     *
     * The cascade works as follows:
     * 1. Find all initially accessible rolls and add them to a removal queue
     * 2. For each roll in the queue:
     *    - Remove it from the active set
     *    - Check each neighbor: if it's still active and now has < 4 adjacent rolls,
     *      add it to the queue
     * 3. Continue until the queue is empty
     *
     * The `pendingRemoval` set prevents re-adding rolls that are already queued,
     * ensuring O(N) amortized complexity.
     *
     * This is the solution for Part 2.
     *
     * @return Total number of paper rolls removed during the cascade
     */
    fun simulateCascadeRemoval(): Int {
        val activeRolls = paperRolls.toMutableSet()
        val removalQueue = ArrayDeque<Point>()

        // Initialize queue with all currently accessible rolls
        activeRolls.forEach { position ->
            if (countAdjacentRolls(position, activeRolls) < ACCESSIBILITY_THRESHOLD) {
                removalQueue.add(position)
            }
        }

        var totalRemoved = 0
        val pendingRemoval = removalQueue.toMutableSet()

        while (removalQueue.isNotEmpty()) {
            val currentPosition = removalQueue.removeFirst()

            // Skip if already removed (could happen due to queue initialization)
            if (currentPosition !in activeRolls) continue

            // Remove the roll
            activeRolls.remove(currentPosition)
            totalRemoved++

            // Check neighbors: their support count dropped, they might now be accessible
            currentPosition
                .neighbors()
                .filter { it.isValid() && it in activeRolls && it !in pendingRemoval }
                .forEach { neighbor ->
                    if (countAdjacentRolls(neighbor, activeRolls) < ACCESSIBILITY_THRESHOLD) {
                        removalQueue.add(neighbor)
                        pendingRemoval.add(neighbor)
                    }
                }
        }

        return totalRemoved
    }

    companion object {
        /**
         * Minimum number of adjacent rolls required for a roll to be inaccessible.
         * Rolls with fewer than this many neighbors can be accessed by forklifts.
         */
        private const val ACCESSIBILITY_THRESHOLD = 4

        /** Character representing a paper roll in the input grid */
        private const val PAPER_ROLL_CHAR = '@'

        /**
         * Parses input lines into a [PaperRollGrid].
         *
         * @param lines Raw input lines where '@' marks paper roll positions
         * @return Parsed grid instance
         */
        fun parse(lines: List<String>): PaperRollGrid {
            val rows = lines.size
            val cols = lines.firstOrNull()?.length ?: 0

            val paperRolls =
                lines
                    .flatMapIndexed { row, line ->
                        line.mapIndexedNotNull { col, char ->
                            if (char == PAPER_ROLL_CHAR) Point(x = col, y = row) else null
                        }
                    }.toSet()

            return PaperRollGrid(rows, cols, paperRolls)
        }
    }
}

// ================================================================================================
// SOLVER CLASS
// ================================================================================================

/**
 * Encapsulates the Day 4 puzzle logic for paper roll accessibility and cascade simulation.
 *
 * Handles parsing of the grid input and provides solutions for both parts:
 * - Part 1: Count initially accessible paper rolls
 * - Part 2: Simulate cascade removal and count total removed rolls
 *
 * @param input Raw input lines representing the paper roll grid
 */
private class Day04Solver(
    input: List<String>,
) {
    /** The parsed paper roll grid */
    private val grid: PaperRollGrid = PaperRollGrid.parse(input)

    /**
     * **Part 1**: Count how many paper rolls are accessible by forklifts.
     *
     * A roll is accessible if fewer than 4 of its 8-directional neighbors contain rolls.
     * This identifies rolls that forklifts can reach without being blocked.
     *
     * @return Number of accessible paper rolls in the initial configuration
     */
    fun solvePart1(): Int = grid.countInitialAccessible()

    /**
     * **Part 2**: Simulate complete cascade removal and count total removed rolls.
     *
     * When an accessible roll is removed, its neighbors may become accessible.
     * This continues until no more rolls can be removed (all remaining rolls
     * have 4+ adjacent neighbors).
     *
     * @return Total number of paper rolls removed during the entire cascade
     */
    fun solvePart2(): Int = grid.simulateCascadeRemoval()
}

// ================================================================================================
// MAIN ENTRY POINT
// ================================================================================================

/**
 * Main entry point for Day 4 solution.
 */
fun main() {
    val input = readInput(4)
    val solver = Day04Solver(input)

    println("Part 1: ${solver.solvePart1()}")
    println("Part 2: ${solver.solvePart2()}")
}
