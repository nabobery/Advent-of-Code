/**
 * # Advent of Code 2025 - Day 1: Secret Entrance Safe Dial
 *
 * This solution handles the problem of decoding the password to the secret entrance
 * by analyzing dial rotations on a combination safe.
 *
 * ## Problem Summary:
 * - **Part 1**: Count how many times the dial **ends** at position 0 after each rotation.
 * - **Part 2**: Count how many times the dial **passes through or ends** at position 0
 *   during all rotations (each "click" where the dial points at 0 counts).
 *
 * ## Safe Dial Mechanics:
 * - The dial has 100 positions numbered 0-99 in a circle
 * - Starting position is 50
 * - 'L' rotations move toward lower numbers (wrapping from 0 to 99)
 * - 'R' rotations move toward higher numbers (wrapping from 99 to 0)
 *
 * ## Key Algorithm: Modular Arithmetic with Zero-Crossing Detection
 *
 * ### Part 1 Algorithm:
 * Simply track the dial position after each rotation using modular arithmetic:
 * - For 'L': `newPosition = (position - clicks).mod(100)`
 * - For 'R': `newPosition = (position + clicks).mod(100)`
 * - Count when `newPosition == 0`
 *
 * ### Part 2 Algorithm - Zero-Crossing Count:
 * Count how many times the dial passes through 0 during a rotation:
 *
 * #### Left Rotation (toward lower numbers):
 * Starting at position `p` and rotating `c` clicks to the left:
 * - If `c <= p`: The dial moves from `p` to `p - c`, never crossing 0
 * - If `c > p`: The dial crosses 0 at least once:
 *   - First crossing: uses `p` clicks to reach 0
 *   - Remaining clicks: `c - p`
 *   - Full cycles past 0: `(c - p) / 100` (each full cycle crosses 0 once more)
 *   - Initial crossing: `+1` (if `p != 0`, we cross 0 after `p` clicks)
 *
 * Formula: `crossings = ((c - p) / 100) + (if p != 0 then 1 else 0)` when `c > p`
 *
 * #### Right Rotation (toward higher numbers):
 * Starting at position `p` and rotating `c` clicks to the right:
 * - The dial reaches position `(p + c)` in the linear sense
 * - Each time `(p + c)` passes a multiple of 100, we cross 0
 * - Crossings: `(p + c) / 100`
 *
 * ### Complexity Analysis:
 * - **Time Complexity**: O(n) where n is the number of rotations
 * - **Space Complexity**: O(n) for storing parsed rotations (O(1) for solving)
 *
 * ## Example Walkthrough:
 * Starting at 50:
 * - L68 → 82 (crosses 0 once: 50→0→82)
 * - L30 → 52 (no crossing)
 * - R48 → 0 (ends at 0 ✓)
 * - L5 → 95 (no crossing)
 * - R60 → 55 (crosses 0 once: 95→99→0→55)
 * - L55 → 0 (ends at 0 ✓)
 * - L1 → 99 (no crossing)
 * - L99 → 0 (ends at 0 ✓)
 * - R14 → 14 (no crossing)
 * - L82 → 32 (crosses 0 once: 14→0→32)
 *
 * Part 1: 3 (rotations ending at 0)
 * Part 2: 6 (all times dial points at 0)
 *
 * @see <a href="https://adventofcode.com/2025/day/1">Advent of Code 2025 Day 1</a>
 */

package org.nabobery

// ================================================================================================
// CONSTANTS
// ================================================================================================

/** Total number of dial positions (0-99) */
private const val DIAL_SIZE = 100

/** Initial position of the dial */
private const val START_POSITION = 50

// ================================================================================================
// DATA MODELS
// ================================================================================================

/**
 * Represents the direction of a dial rotation.
 *
 * Using a sealed class provides type safety and exhaustive when-expressions,
 * making the code more robust than using raw characters.
 */
private sealed class Direction {
    /** Rotation toward lower numbers (0 toward 99 when wrapping) */
    data object Left : Direction()

    /** Rotation toward higher numbers (99 toward 0 when wrapping) */
    data object Right : Direction()

    companion object {
        /**
         * Parses a direction character into a [Direction].
         *
         * @param char 'L' for Left, 'R' for Right
         * @return The corresponding Direction
         * @throws IllegalArgumentException for invalid direction characters
         */
        fun fromChar(char: Char): Direction =
            when (char) {
                'L' -> Left
                'R' -> Right
                else -> error("Invalid direction character: '$char'. Expected 'L' or 'R'.")
            }
    }
}

/**
 * Represents a single rotation command for the safe dial.
 *
 * @property direction The direction to rotate (Left or Right)
 * @property clicks The number of clicks to rotate
 */
private data class Rotation(
    val direction: Direction,
    val clicks: Int,
) {
    companion object {
        /**
         * Parses a rotation string into a [Rotation] command.
         *
         * @param input String in format "L42" or "R15" (direction + number of clicks)
         * @return Parsed Rotation instance
         * @throws IllegalArgumentException for malformed input
         */
        fun parse(input: String): Rotation {
            require(input.length >= 2) { "Rotation string too short: '$input'" }

            val direction = Direction.fromChar(input.first())
            val clicks =
                input.drop(1).toIntOrNull()
                    ?: error("Invalid click count in rotation: '$input'")

            return Rotation(direction, clicks)
        }
    }
}

/**
 * Represents the current state of the dial during rotation processing.
 *
 * @property position Current dial position (0-99)
 * @property zeroCount Accumulated count of times the dial has pointed at 0
 */
private data class DialState(
    val position: Int = START_POSITION,
    val zeroCount: Int = 0,
)

// ================================================================================================
// CORE ALGORITHM
// ================================================================================================

/**
 * Calculates the new dial position after applying a rotation.
 *
 * Uses Kotlin's `mod` function which always returns a non-negative result,
 * properly handling the circular nature of the dial.
 *
 * @param position Current position (0-99)
 * @param rotation The rotation to apply
 * @return New position after rotation (0-99)
 */
private fun calculateNewPosition(
    position: Int,
    rotation: Rotation,
): Int =
    when (rotation.direction) {
        Direction.Left -> (position - rotation.clicks).mod(DIAL_SIZE)
        Direction.Right -> (position + rotation.clicks).mod(DIAL_SIZE)
    }

/**
 * Counts how many times the dial passes through or ends at position 0 during a rotation.
 *
 * This is the key algorithm for Part 2. The calculation differs based on direction:
 *
 * **Left Rotation**: Moving from position `p` by `c` clicks toward lower numbers:
 * - The dial points at 0 when click number `k ≡ p (mod 100)` for `1 ≤ k ≤ c`
 * - If `p = 0`: We cross 0 at clicks 100, 200, 300, ... → `floor(c / 100)`
 * - If `p > 0` and `c < p`: We never reach 0 → `0`
 * - If `p > 0` and `c >= p`: We hit 0 at click `p`, then every 100 clicks after
 *   → `1 + floor((c - p) / 100)`
 *
 * **Right Rotation**: Moving from position `p` by `c` clicks toward higher numbers:
 * - We cross 0 each time we pass position 99 and wrap to 0
 * - `(p + c) / 100` gives the number of times we wrap around
 *
 * @param position Current position before rotation
 * @param rotation The rotation to apply
 * @return Number of times the dial points at 0 during this rotation
 */
private fun countZeroCrossings(
    position: Int,
    rotation: Rotation,
): Int {
    val clicks = rotation.clicks

    return when (rotation.direction) {
        Direction.Left -> {
            when {
                // Special case: starting at 0, we only cross 0 every 100 clicks
                // (we don't count the starting position)
                position == 0 -> clicks / DIAL_SIZE

                // Moving left but not far enough to reach 0
                clicks < position -> 0

                // We hit 0 at click 'position', then every 100 clicks thereafter
                else -> {
                    val clicksPastZero = clicks - position
                    1 + (clicksPastZero / DIAL_SIZE)
                }
            }
        }

        Direction.Right -> {
            // Each time position + clicks exceeds a multiple of DIAL_SIZE, we cross 0
            (position + clicks) / DIAL_SIZE
        }
    }
}

// ================================================================================================
// SOLVER CLASS
// ================================================================================================

/**
 * Encapsulates the Day 1 puzzle logic for safe dial password decryption.
 *
 * The solver parses rotation commands and applies two different counting strategies:
 * - Part 1: Count rotations that end at position 0
 * - Part 2: Count every click where the dial points at 0 (including during rotations)
 *
 * @param input Raw input lines, where each line is a rotation command (e.g., "L42", "R15")
 */
private class Day01Solver(
    input: List<String>,
) {
    /** Parsed rotation commands */
    private val rotations: List<Rotation> = input.map(Rotation::parse)

    /**
     * **Part 1**: Count how many rotations end with the dial pointing at 0.
     *
     * ## Algorithm:
     * 1. Start at position 50
     * 2. For each rotation, calculate the new position using modular arithmetic
     * 3. Increment counter if the new position is 0
     *
     * ## Example:
     * From the puzzle example:
     * - R48 ends at 0 ✓
     * - L55 ends at 0 ✓
     * - L99 ends at 0 ✓
     * - Total: 3
     *
     * @return The number of rotations ending at position 0
     */
    fun solvePart1(): Int =
        rotations
            .fold(DialState()) { state, rotation ->
                val newPosition = calculateNewPosition(state.position, rotation)
                val zeroIncrement = if (newPosition == 0) 1 else 0
                DialState(
                    position = newPosition,
                    zeroCount = state.zeroCount + zeroIncrement,
                )
            }.zeroCount

    /**
     * **Part 2**: Count every time the dial points at 0 during all rotations.
     *
     * This includes both:
     * - Rotations that end at 0 (same as Part 1)
     * - Rotations where the dial passes through 0 during the rotation
     *
     * ## Algorithm:
     * For each rotation, calculate how many times the dial crosses or lands on 0
     * using the zero-crossing detection algorithm.
     *
     * ## Example:
     * From the puzzle example with R1000 starting at 50:
     * - The dial rotates 1000 clicks to the right
     * - (50 + 1000) / 100 = 10 crossings through 0
     * - Final position: (50 + 1000) % 100 = 50
     *
     * @return The total count of all times the dial points at 0
     */
    fun solvePart2(): Int =
        rotations
            .fold(DialState()) { state, rotation ->
                val crossings = countZeroCrossings(state.position, rotation)
                val newPosition = calculateNewPosition(state.position, rotation)
                DialState(
                    position = newPosition,
                    zeroCount = state.zeroCount + crossings,
                )
            }.zeroCount
}

// ================================================================================================
// MAIN ENTRY POINT
// ================================================================================================

/**
 * Main entry point for Day 1 solution.
 *
 * Reads the puzzle input, instantiates the solver, and prints results for both parts.
 */
fun main() {
    val input = readInput(1)
    val solver = Day01Solver(input)

    println("Part 1: ${solver.solvePart1()}")
    println("Part 2: ${solver.solvePart2()}")
}
