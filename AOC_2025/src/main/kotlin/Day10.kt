/**
 * # Advent of Code 2025 - Day 10: Machine Initialization Procedure
 *
 * This solution handles the problem of configuring machines with indicator lights and joltage counters.
 *
 * ## Problem Summary:
 * - **Part 1**: Find the minimum button presses to configure indicator lights (binary toggle problem).
 *   Uses BFS over light states with XOR-based bitmask transitions.
 *
 * - **Part 2**: Find the minimum button presses to reach exact joltage counter values (additive problem).
 *   Formulated as a system of **Linear Diophantine Equations** and solved using **Gaussian Elimination**
 *   with **back-substitution** and **free variable search**.
 *
 * ## Key Algorithms:
 * - **Part 1 - BFS with Bitmasks**: Light toggles are XOR operations; BFS guarantees minimum presses.
 * - **Part 2 - Gaussian Elimination + CSP**: Converts exponential state-space search into polynomial
 *   matrix operations plus a bounded search over free variables.
 *
 */

package org.nabobery

import kotlin.math.abs

// ================================================================================================
// DATA MODELS
// ================================================================================================

/**
 * Represents a machine with indicator lights, button wiring schemas, and joltage requirements.
 *
 * @property indicatorLightDiagram The target light states: '.' = off, '#' = on
 * @property buttonWiringSchemas Each button's list of counter/light indices it affects
 * @property joltageRequirements Target values for each joltage counter
 */
private data class Machine(
    val indicatorLightDiagram: List<Char>,
    val buttonWiringSchemas: List<List<Int>>,
    val joltageRequirements: List<Int>,
) {
    companion object {
        /**
         * Parses a machine specification string.
         *
         * Input format: `[.##.] (3) (1,3) (2) {3,5,4,7}`
         * - `[.##.]` - Indicator light diagram
         * - `(3) (1,3) (2)` - Button wiring schemas
         * - `{3,5,4,7}` - Joltage requirements
         *
         * @param input The raw machine specification string
         * @return Parsed [Machine] instance
         * @throws IllegalArgumentException if the format is invalid
         */
        fun parse(input: String): Machine {
            require(input.startsWith("[") && input.endsWith("}")) {
                "Invalid machine format: must start with '[' and end with '}'"
            }

            val indicatorLightDiagram =
                input
                    .substringBefore(']')
                    .removePrefix("[")
                    .toList()

            val buttonWiringSchemas =
                input
                    .substringAfter("] (")
                    .substringBefore(") {")
                    .split(") (")
                    .map { schema -> schema.split(",").map(String::toInt) }

            val joltageRequirements =
                input
                    .substringAfter('{')
                    .substringBefore('}')
                    .split(",")
                    .map(String::toInt)

            return Machine(
                indicatorLightDiagram = indicatorLightDiagram,
                buttonWiringSchemas = buttonWiringSchemas,
                joltageRequirements = joltageRequirements,
            )
        }
    }
}

// ================================================================================================
// GAUSSIAN ELIMINATION SOLVER FOR PART 2
// ================================================================================================

/**
 * Solves the joltage counter configuration problem using Gaussian Elimination.
 *
 * ## Problem Formulation
 *
 * The button-counter relationship forms a system of Linear Diophantine Equations:
 * ```
 * A · x = T
 * ```
 * Where:
 * - `A` is an `n × m` coefficient matrix (counters × buttons)
 * - `A[j][i] = 1` if button `i` affects counter `j`, else `0`
 * - `x` is the `m × 1` solution vector (button press counts, all ≥ 0)
 * - `T` is the `n × 1` target vector (joltage requirements)
 * - Objective: minimize `Σxᵢ`
 *
 * ## Algorithm Overview
 *
 * 1. **Matrix Construction**: Build augmented matrix `[A | T]`
 * 2. **Gaussian Elimination**: Reduce to row echelon form, identify pivot/free variables
 * 3. **Consistency Check**: Detect infeasible systems (0 = non-zero)
 * 4. **Free Variable Search**: Enumerate assignments for free variables with pruning
 * 5. **Back-Substitution**: Compute pivot variables, validate non-negativity
 *
 * @property buttonSchemas Button wiring schemas (which counters each button affects)
 * @property targets Target joltage values for each counter
 */
private class GaussianEliminationSolver(
    private val buttonSchemas: List<List<Int>>,
    private val targets: List<Int>,
) {
    private val numCounters = targets.size
    private val numButtons = buttonSchemas.size

    /** Target values as Long to avoid overflow during elimination */
    private val targetArray = targets.map(Int::toLong).toLongArray()

    /**
     * Solves for the minimum total button presses.
     *
     * @return Minimum button presses, or -1 if no valid solution exists
     */
    fun solve(): Int {
        // Phase 1: Build augmented matrix [A | T]
        val augmented = buildAugmentedMatrix()

        // Phase 2: Gaussian elimination with partial pivoting
        val pivotColumns = performGaussianElimination(augmented)

        // Phase 3: Check for inconsistency
        if (isInconsistent(augmented, pivotColumns.size)) {
            return NO_SOLUTION
        }

        // Phase 4: Identify free variables and search for minimum solution
        val freeVariables = (0 until numButtons).filter { it !in pivotColumns }

        return searchForMinimumSolution(augmented, pivotColumns, freeVariables)
    }

    /**
     * Builds the augmented matrix `[A | T]`.
     *
     * Structure:
     * - Rows: One per counter (constraint)
     * - Columns 0..m-1: Button coefficients (1 if button affects counter, else 0)
     * - Column m: Target value for that counter
     *
     * @return The constructed augmented matrix
     */
    private fun buildAugmentedMatrix(): Array<LongArray> =
        Array(numCounters) { counterIdx ->
            LongArray(numButtons + 1) { colIdx ->
                when {
                    colIdx < numButtons -> if (counterIdx in buttonSchemas[colIdx]) 1L else 0L
                    else -> targetArray[counterIdx]
                }
            }
        }

    /**
     * Performs Gaussian elimination with partial pivoting.
     *
     * Transforms the augmented matrix to row echelon form using:
     * - **Partial Pivoting**: Select the row with the largest absolute pivot value
     *   for numerical stability
     * - **Row Operations**: Eliminate entries below each pivot
     *
     * Note: Uses integer arithmetic to maintain exact solutions for Diophantine equations.
     *
     * @param matrix The augmented matrix to transform (modified in-place)
     * @return List of pivot column indices in the order they were found
     */
    private fun performGaussianElimination(matrix: Array<LongArray>): List<Int> {
        var currentPivotRow = 0
        val pivotColumns = mutableListOf<Int>()

        for (col in 0 until numButtons) {
            if (currentPivotRow >= numCounters) break

            // Find the row with maximum absolute value in this column (partial pivoting)
            val maxRow =
                (currentPivotRow until numCounters)
                    .maxByOrNull { abs(matrix[it][col]) }
                    ?: continue

            // Skip if no pivot available in this column
            if (matrix[maxRow][col] == 0L) continue

            // Swap rows to bring pivot to current position
            matrix.swapRows(currentPivotRow, maxRow)
            pivotColumns.add(col)

            // Eliminate entries below the pivot
            val pivotValue = matrix[currentPivotRow][col]
            for (row in (currentPivotRow + 1) until numCounters) {
                if (matrix[row][col] != 0L) {
                    val factor = matrix[row][col]
                    // Row operation: R[row] = R[row] * pivotValue - R[pivotRow] * factor
                    // This avoids division and maintains integer arithmetic
                    for (c in col..numButtons) {
                        matrix[row][c] = matrix[row][c] * pivotValue - matrix[currentPivotRow][c] * factor
                    }
                }
            }

            currentPivotRow++
        }

        return pivotColumns
    }

    /**
     * Checks if the system is inconsistent (no solution exists).
     *
     * After Gaussian elimination, inconsistency is detected when a row has:
     * - All zero coefficients in button columns (left side = 0)
     * - Non-zero value in the target column (right side ≠ 0)
     * This represents the equation `0 = k` where `k ≠ 0`, which is impossible.
     *
     * @param matrix The eliminated augmented matrix
     * @param numPivots Number of pivot rows found
     * @return true if the system is inconsistent
     */
    private fun isInconsistent(
        matrix: Array<LongArray>,
        numPivots: Int,
    ): Boolean =
        (numPivots until numCounters).any { row ->
            val allCoefficientsZero = (0 until numButtons).all { col -> matrix[row][col] == 0L }
            allCoefficientsZero && matrix[row][numButtons] != 0L
        }

    /**
     * Searches over free variable assignments to find the minimum total button presses.
     *
     * Free variables arise when the system has more buttons than independent constraints.
     * They can take any non-negative integer value. We enumerate these values with:
     * - **Upper bound**: No free variable exceeds the maximum target value (or 500 for safety)
     * - **Pruning**: Skip branches where current sum already exceeds best solution
     *
     * For each assignment, we back-substitute to compute pivot variables and validate
     * the solution (all values must be non-negative integers).
     *
     * @param matrix The eliminated augmented matrix
     * @param pivotColumns Indices of pivot (dependent) variables
     * @param freeVariables Indices of free (independent) variables
     * @return Minimum total button presses, or -1 if no valid solution exists
     */
    private fun searchForMinimumSolution(
        matrix: Array<LongArray>,
        pivotColumns: List<Int>,
        freeVariables: List<Int>,
    ): Int {
        val numFreeVars = freeVariables.size
        val maxFreeValue = (targetArray.maxOrNull() ?: 0L).coerceAtMost(MAX_FREE_VARIABLE_VALUE)

        var bestSum = Long.MAX_VALUE

        /**
         * Recursive DFS over free variable assignments with pruning.
         *
         * @param freeIdx Current free variable index being assigned
         * @param assignments Current assignments for free variables
         * @param currentSum Running sum of assigned free variable values
         */
        fun searchRecursive(
            freeIdx: Int,
            assignments: LongArray,
            currentSum: Long,
        ) {
            // Prune: cannot improve on current best
            if (currentSum >= bestSum) return

            // Base case: all free variables assigned, compute and validate full solution
            if (freeIdx == numFreeVars) {
                val solution =
                    backSubstitute(matrix, pivotColumns, freeVariables, assignments)
                        ?: return // Invalid solution (negative or non-integer value)

                val totalSum = solution.sum()
                if (totalSum < bestSum) {
                    bestSum = totalSum
                }
                return
            }

            // Recursive case: enumerate values for current free variable
            val upperBound = minOf(maxFreeValue, bestSum - currentSum - 1)
            for (value in 0..upperBound) {
                assignments[freeIdx] = value
                searchRecursive(freeIdx + 1, assignments, currentSum + value)
            }
        }

        searchRecursive(0, LongArray(numFreeVars), 0L)

        return if (bestSum == Long.MAX_VALUE) NO_SOLUTION else bestSum.toInt()
    }

    /**
     * Back-substitutes to compute pivot variable values from free variable assignments.
     *
     * Starting from the last pivot row and working upward:
     * 1. Compute the right-hand side (RHS) by subtracting contributions from variables
     *    already solved (those to the right of the current pivot column)
     * 2. Check divisibility by the pivot value (required for integer solution)
     * 3. Check non-negativity (required constraint)
     *
     * @param matrix The eliminated augmented matrix
     * @param pivotColumns Pivot column indices
     * @param freeVariables Free variable indices
     * @param freeAssignments Values assigned to free variables
     * @return Complete solution array, or null if invalid (negative or non-integer values)
     */
    private fun backSubstitute(
        matrix: Array<LongArray>,
        pivotColumns: List<Int>,
        freeVariables: List<Int>,
        freeAssignments: LongArray,
    ): LongArray? {
        val solution = LongArray(numButtons)

        // Copy free variable assignments to solution
        freeVariables.forEachIndexed { idx, col ->
            solution[col] = freeAssignments[idx]
        }

        // Back-substitute for pivot variables (from last row to first)
        for (pivotIdx in pivotColumns.indices.reversed()) {
            val col = pivotColumns[pivotIdx]
            val row = pivotIdx

            // RHS = target - sum of (coefficient * already-solved variable)
            var rhs = matrix[row][numButtons]
            for (c in (col + 1) until numButtons) {
                rhs -= matrix[row][c] * solution[c]
            }

            val pivotValue = matrix[row][col]

            // Check for integer divisibility
            if (rhs % pivotValue != 0L) return null

            val value = rhs / pivotValue

            // Check for non-negativity
            if (value < 0) return null

            solution[col] = value
        }

        return solution
    }

    companion object {
        private const val NO_SOLUTION = -1
        private const val MAX_FREE_VARIABLE_VALUE = 500L
    }
}

// ================================================================================================
// EXTENSION FUNCTIONS
// ================================================================================================

/**
 * Swaps two rows in a 2D array in-place.
 *
 * @param row1 Index of the first row
 * @param row2 Index of the second row
 */
private fun Array<LongArray>.swapRows(
    row1: Int,
    row2: Int,
) {
    if (row1 != row2) {
        val temp = this[row1]
        this[row1] = this[row2]
        this[row2] = temp
    }
}

// ================================================================================================
// PART 1: INDICATOR LIGHT CONFIGURATION
// ================================================================================================

/**
 * Finds the minimum button presses to configure indicator lights.
 *
 * ## Algorithm: BFS over Light States
 *
 * - **State**: Binary representation of all light states (on/off)
 * - **Initial State**: All lights off (0)
 * - **Goal State**: Lights matching the diagram (bitmask from diagram)
 * - **Transitions**: XOR with each button's effect mask
 *
 * BFS guarantees the shortest path (minimum presses) since each button press
 * constitutes a single step.
 *
 * Time Complexity: O(2^L × B) where L = lights, B = buttons
 * Space Complexity: O(2^L) for the distance array
 *
 * Note: This is efficient when the number of lights is small (< 20).
 *
 * @param diagram The target light configuration ('.' = off, '#' = on)
 * @param buttonSchemas Each button's list of light indices it toggles
 * @return Minimum button presses to reach the goal, or -1 if unreachable
 */
private fun findMinPressesForLights(
    diagram: List<Char>,
    buttonSchemas: List<List<Int>>,
): Int {
    val numLights = diagram.size

    // Convert diagram to target bitmask (position 0 = bit 0)
    val targetState =
        diagram
            .reversed() // Reverse so position 0 maps to bit 0
            .joinToString("") { if (it == '#') "1" else "0" }
            .toInt(2)

    // Precompute XOR effect of each button as a bitmask
    val buttonEffects =
        buttonSchemas.map { positions ->
            positions.fold(0) { acc, pos -> acc xor (1 shl pos) }
        }

    // BFS setup
    val totalStates = 1 shl numLights
    val distance = IntArray(totalStates) { UNVISITED }
    distance[0] = 0 // Initial state: all lights off

    val queue = ArrayDeque<Int>().apply { add(0) }

    // BFS traversal
    while (queue.isNotEmpty()) {
        val currentState = queue.removeFirst()

        if (currentState == targetState) {
            return distance[currentState]
        }

        // Try pressing each button
        for (effect in buttonEffects) {
            val nextState = currentState xor effect
            if (distance[nextState] == UNVISITED) {
                distance[nextState] = distance[currentState] + 1
                queue.add(nextState)
            }
        }
    }

    return NO_SOLUTION // Goal unreachable
}

private const val UNVISITED = -1
private const val NO_SOLUTION = -1

// ================================================================================================
// PART 2: JOLTAGE COUNTER CONFIGURATION
// ================================================================================================

/**
 * Finds the minimum button presses to configure joltage counters.
 *
 * This is delegated to [GaussianEliminationSolver] which treats the problem as
 * a system of Linear Diophantine Equations.
 *
 * @param buttonSchemas Each button's list of counter indices it increments
 * @param targets Target values for each counter
 * @return Minimum button presses, or -1 if no valid solution exists
 */
private fun findMinPressesForJoltage(
    buttonSchemas: List<List<Int>>,
    targets: List<Int>,
): Int = GaussianEliminationSolver(buttonSchemas, targets).solve()

// ================================================================================================
// SOLVER CLASS
// ================================================================================================

/**
 * Encapsulates the Day 10 puzzle logic for machine initialization.
 *
 * Handles parsing of machine specifications and provides solutions for both parts:
 * - Part 1: Configure indicator lights (XOR toggle problem)
 * - Part 2: Configure joltage counters (additive Diophantine problem)
 *
 * @param input Raw input lines, one machine specification per line
 */
private class Day10Solver(
    input: List<String>,
) {
    private val machines: List<Machine> = input.map(Machine::parse)

    /**
     * **Part 1**: Find the total minimum button presses to configure all indicator lights.
     *
     * Uses BFS over bitmask states where each button press XORs the current state
     * with the button's effect mask.
     *
     * @return Sum of minimum presses for all machines
     */
    fun solvePart1(): Int =
        machines.sumOf { machine ->
            findMinPressesForLights(
                diagram = machine.indicatorLightDiagram,
                buttonSchemas = machine.buttonWiringSchemas,
            )
        }

    /**
     * **Part 2**: Find the total minimum button presses to configure all joltage counters.
     *
     * Uses Gaussian Elimination to solve the system of linear Diophantine equations
     * representing the button-counter relationships.
     *
     * @return Sum of minimum presses for all machines
     * @see GaussianEliminationSolver for algorithm details
     */
    fun solvePart2(): Int =
        machines.sumOf { machine ->
            findMinPressesForJoltage(
                buttonSchemas = machine.buttonWiringSchemas,
                targets = machine.joltageRequirements,
            )
        }
}

// ================================================================================================
// MAIN ENTRY POINT
// ================================================================================================

/**
 * Main entry point for Day 10 solution.
 */
fun main() {
    val input = readInput(10)
    val solver = Day10Solver(input)

    println("Part 1: ${solver.solvePart1()}")
    println("Part 2: ${solver.solvePart2()}")
}
