/**
 * # Advent of Code 2025 - Day 12: Present Packing Problem
 *
 * This solution handles the problem of fitting oddly-shaped Christmas presents into regions
 * under trees at the North Pole.
 *
 * ## Problem Summary:
 * - **Part 1**: Determine how many regions can fit all their listed presents. This is fundamentally
 *   an **NP-hard bin packing problem** (specifically, a 2D irregular bin packing variant), where we
 *   need to pack polyomino-like shapes into rectangular regions without overlap.
 *
 * - **Part 2**: Not implemented (no puzzle description provided).
 *
 * ## NP-Hard Complexity Note:
 * The general 2D bin packing problem with irregular shapes is NP-hard and computationally
 * expensive. However, this solution uses a **trivial area-based heuristic** that simply checks:
 * ```
 * sum(shape_area * count) <= region_area
 * ```
 *
 * **This "dumb" solution only works because of the specific input characteristics in day12.txt**:
 * - The number of presents required is massive compared to the available area
 * - Most regions are clearly over-packed or clearly have sufficient space
 * - The puzzle likely has no tricky edge cases requiring actual spatial packing validation
 *
 * In the general case, this heuristic would produce false positives (regions that pass the area
 * test but where the shapes cannot actually be arranged to fit). A proper solution would require:
 * - Backtracking with spatial placement
 * - Constraint satisfaction programming (CSP)
 * - SAT solvers
 * - Or heuristic packing algorithms (guillotine cuts, maximal rectangles, etc.)
 *
 * ## Key Algorithms:
 * - **Part 1 - Area Validation**: O(R * S) where R = regions, S = shapes per region.
 *   Simply validates that total shape area doesn't exceed region area.
 *
 * ## Data Structure:
 * - Shapes are represented as 2D grids where `#` indicates filled cells
 * - Regions are defined by width × height and a list of shape counts
 * - Input parsing separates shape definitions from region requirements
 */

package org.nabobery

// ================================================================================================
// DATA MODELS
// ================================================================================================

/**
 * Represents a region under a Christmas tree where presents need to be placed.
 *
 * @property area Total available area (width * height) in the region
 * @property shapeCounts List of quantities for each shape type that must fit in this region.
 *                       Index i corresponds to shape i, value is the count of that shape needed.
 */
private data class Region(
    val area: Int,
    val shapeCounts: List<Int>,
)

/**
 * Represents the parsed input containing shape definitions and region requirements.
 *
 * @property shapeAreas List of areas (count of `#` cells) for each shape type
 * @property regions List of regions with their area and required shape counts
 */
private data class PresentPackingInput(
    val shapeAreas: List<Int>,
    val regions: List<Region>,
)

// ================================================================================================
// INPUT PARSING
// ================================================================================================

/**
 * Parses the puzzle input into shape areas and region requirements.
 *
 * ## Input Format:
 * The input consists of two sections separated by blank lines:
 *
 * 1. **Shape Definitions**: Each shape starts with an index and colon, followed by a visual
 *    representation where `#` is part of the shape and `.` is empty space.
 *    ```
 *    0:
 *    ###
 *    ##.
 *    ##.
 *    ```
 *
 * 2. **Region Requirements**: Each line specifies width×height followed by shape counts.
 *    ```
 *    12x5: 1 0 1 0 2 2
 *    ```
 *    This means: width=12, height=5, needs 1 of shape 0, 0 of shape 1, 1 of shape 2, etc.
 *
 * @param input Raw input lines from the puzzle
 * @return Parsed [PresentPackingInput] containing shape areas and region requirements
 */
private fun parseInput(input: List<String>): PresentPackingInput {
    val shapeAreas = mutableListOf<Int>()
    val regions = mutableListOf<Region>()

    // Split input into sections by blank lines
    val sections =
        input
            .joinToString("\n")
            .split("\n\n")
            .map { it.lines() }
            .filter { it.isNotEmpty() }

    for (section in sections) {
        val firstLine = section.first()

        when {
            // Region section: lines contain "x" (e.g., "12x5: 1 0 1 0 2 2")
            firstLine.contains("x") -> {
                for (regionLine in section) {
                    val (dimensions, countsStr) = regionLine.split(':', limit = 2)
                    val (width, height) = dimensions.split('x').map(String::toInt)

                    val shapeCounts =
                        countsStr
                            .trim()
                            .split("\\s+".toRegex())
                            .filter { it.isNotBlank() }
                            .map(String::toInt)

                    regions.add(
                        Region(
                            area = width * height,
                            shapeCounts = shapeCounts,
                        ),
                    )
                }
            }

            // Shape section: lines start with index and colon (e.g., "0:")
            firstLine.contains(":") -> {
                // Drop the index line and count '#' characters in the shape grid
                val gridLines = section.drop(1)
                val shapeArea = gridLines.sumOf { line -> line.count { it == '#' } }
                shapeAreas.add(shapeArea)
            }
        }
    }

    return PresentPackingInput(
        shapeAreas = shapeAreas,
        regions = regions,
    )
}

// ================================================================================================
// SOLVER CLASS
// ================================================================================================

/**
 * Encapsulates the Day 12 puzzle logic for present packing validation.
 *
 * **IMPORTANT**: This solver uses a trivial area-based heuristic that works only for
 * the specific characteristics of the puzzle input. See file-level KDoc for details on
 * why this is NP-hard in the general case.
 *
 * @param input Raw input lines from the puzzle
 */
private class Day12Solver(
    input: List<String>,
) {
    private val parsedInput: PresentPackingInput = parseInput(input)

    /**
     * **Part 1**: Count how many regions can fit all their required presents.
     *
     * ## Algorithm (Trivial Area Heuristic):
     * For each region:
     * 1. Calculate total required area: sum(shape_count[i] * shape_area[i])
     * 2. Compare with available region area
     * 3. Count region as valid if total_required_area <= region_area
     *
     * **NOTE**: This is NOT a true packing validation. It only checks if the total area
     * is sufficient, not whether the shapes can actually be arranged spatially. This works
     * for the given input but would fail for harder instances of the bin packing problem.
     *
     * Time Complexity: O(R * S) where R = number of regions, S = number of shape types
     * Space Complexity: O(1) auxiliary space
     *
     * @return Number of regions that have sufficient area for their required presents
     */
    fun solvePart1(): Int {
        val shapeAreas = parsedInput.shapeAreas

        return parsedInput.regions.count { region ->
            // Calculate total area needed for all required shapes
            val totalRequiredArea =
                region.shapeCounts
                    .zip(shapeAreas) { count, area -> count * area }
                    .sum()

            // Simple area validation: does the region have enough space?
            // This is a necessary but NOT sufficient condition for actual packing
            region.area >= totalRequiredArea
        }
    }

    /**
     * **Part 2**: Not implemented as the puzzle description was not provided.
     *
     * @return Always returns 0
     */
    fun solvePart2(): Int = 0
}

// ================================================================================================
// MAIN ENTRY POINT
// ================================================================================================

/**
 * Main entry point for Day 12 solution.
 */
fun main() {
    val input = readInput(12)
    val solver = Day12Solver(input)

    println("Part 1: ${solver.solvePart1()}")
    println("Part 2: ${solver.solvePart2()}")
}
