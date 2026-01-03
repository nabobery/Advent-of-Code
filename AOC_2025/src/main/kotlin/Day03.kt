/**
 * # Advent of Code 2025 - Day 3: Lobby Battery Banks
 *
 * This solution handles the problem of maximizing joltage output from battery banks
 * to power the escalator in the building lobby.
 *
 * ## Problem Summary:
 * - **Part 1**: From each battery bank, select exactly **2 batteries** to maximize the joltage output.
 *   The joltage is the number formed by the digits of the selected batteries in order.
 *
 * - **Part 2**: From each battery bank, select exactly **12 batteries** to maximize the joltage output.
 *   The joltage is similarly the number formed by the 12 selected battery digits in order.
 *
 * ## Key Algorithm: Monotonic Stack Subsequence Selection
 *
 * The core problem is: **find the lexicographically largest subsequence of length `k` from a string**.
 * This is solved using a **greedy monotonic stack** approach:
 *
 * ### Algorithm Steps:
 * 1. Iterate through each digit from left to right
 * 2. Maintain a stack that holds the current "best" subsequence candidates
 * 3. For each new digit, while:
 *    - The stack is non-empty, AND
 *    - The new digit is larger than the top of the stack, AND
 *    - There are enough remaining digits to still form a subsequence of length `k`
 *    → Pop the stack (remove the smaller digit)
 * 4. Push the current digit onto the stack
 * 5. After processing all digits, truncate the stack to exactly `k` elements
 *
 * ### Why This Works:
 * - A larger leading digit always produces a larger number (e.g., 91 > 89)
 * - By greedily replacing smaller digits with larger ones (when possible),
 *   we ensure the lexicographically largest result
 * - The "remaining digits" check ensures we never pop too many elements
 *
 * ### Complexity Analysis:
 * - **Time Complexity**: O(n) where n is the length of the battery bank
 *   - Each digit is pushed and popped at most once
 * - **Space Complexity**: O(k) for the stack, where k is the number of batteries to select
 *
 * ## Example Walkthrough (Part 1, k=2):
 *
 * Consider bank "818181911112111":
 * ```
 * Processing: 8 → stack: [8]
 * Processing: 1 → stack: [8, 1] (1 < 8, just push)
 * Processing: 8 → pop 1 (8 > 1, enough remaining), stack: [8, 8]
 * Processing: 1 → stack: [8, 8, 1] (1 < 8, just push)
 * Processing: 8 → pop 1 (8 > 1, enough remaining), stack: [8, 8, 8]
 * Processing: 1 → stack: [8, 8, 8, 1]
 * Processing: 9 → pop 1, pop 8, pop 8, pop 8 (9 > all, enough remaining), push 9: [9]
 * Processing: 1 → stack: [9, 1]
 * Processing: 1 → stack: [9, 1, 1]
 * Processing: 1 → stack: [9, 1, 1, 1]
 * Processing: 1 → stack: [9, 1, 1, 1, 1]
 * Processing: 2 → pop 1 (2 > 1, enough remaining), pop 1, pop 1, pop 1, stack: [9, 2]
 * Processing: 1 → stack: [9, 2, 1] (1 < 2, just push)
 * Processing: 1 → stack: [9, 2, 1, 1]
 * Processing: 1 → stack: [9, 2, 1, 1, 1] - final
 * Truncate to k=2: [9, 2] → "92"
 * ```
 *
 * @see <a href="https://adventofcode.com/2025/day/3">Advent of Code 2025 Day 3</a>
 */

package org.nabobery

// ================================================================================================
// CORE ALGORITHM
// ================================================================================================

/**
 * Finds the lexicographically largest subsequence of a given length from a string of digits.
 *
 * This implements a **monotonic stack** approach that greedily selects the largest
 * possible digits while ensuring the required subsequence length can still be achieved.
 *
 * ## Algorithm Invariant:
 * At any point during iteration, the stack contains the lexicographically largest
 * subsequence possible from the digits processed so far, given the constraint that
 * enough positions remain to complete the subsequence.
 *
 * @param bank The input string representing a battery bank (digits 1-9)
 * @param pickCount The number of batteries (digits) to select
 * @return The subsequence forming the maximum possible joltage value as a string
 *
 * @throws IllegalArgumentException if pickCount > bank.length (implicit via stack truncation)
 */
private fun findMaxSubsequence(
    bank: String,
    pickCount: Int,
): String {
    // Use ArrayDeque as a stack - efficient O(1) add/remove from end
    val stack = ArrayDeque<Char>(bank.length)

    for ((index, digit) in bank.withIndex()) {
        val remainingDigits = bank.length - index

        // Greedily pop smaller digits while we can still form a valid subsequence
        // Condition: current digit is larger AND we have enough remaining digits
        while (
            stack.isNotEmpty() &&
            digit > stack.last() &&
            stack.size + remainingDigits - 1 >= pickCount
        ) {
            stack.removeLast()
        }

        stack.addLast(digit)
    }

    // Trim excess digits if we collected more than needed
    while (stack.size > pickCount) {
        stack.removeLast()
    }

    return stack.joinToString("")
}

// ================================================================================================
// SOLVER CLASS
// ================================================================================================

/**
 * Encapsulates the Day 3 puzzle logic for maximizing battery bank joltage output.
 *
 * The solver uses a monotonic stack algorithm to find the lexicographically largest
 * subsequence of digits from each battery bank, which corresponds to the maximum
 * joltage output when those batteries are selected.
 *
 * @param input Raw input lines, where each line represents a battery bank (string of digits 1-9)
 */
private class Day03Solver(
    private val input: List<String>,
) {
    companion object {
        /** Number of batteries to select in Part 1 */
        private const val PART1_PICK_COUNT = 2

        /** Number of batteries to select in Part 2 */
        private const val PART2_PICK_COUNT = 12
    }

    /**
     * **Part 1**: Find the total output joltage when selecting exactly 2 batteries per bank.
     *
     * For each bank:
     * 1. Find the 2-digit subsequence that forms the largest number
     * 2. Convert to integer and sum across all banks
     *
     * ## Example:
     * - "987654321111111" → "98" → 98
     * - "811111111111119" → "89" → 89
     * - "234234234234278" → "78" → 78
     * - "818181911112111" → "92" → 92
     * - Total: 98 + 89 + 78 + 92 = 357
     *
     * @return Sum of maximum 2-digit joltages from all battery banks
     */
    fun solvePart1(): Int =
        input.sumOf { bank ->
            findMaxSubsequence(bank, PART1_PICK_COUNT).toInt()
        }

    /**
     * **Part 2**: Find the total output joltage when selecting exactly 12 batteries per bank.
     *
     * For each bank:
     * 1. Find the 12-digit subsequence that forms the largest number
     * 2. Convert to Long (12 digits exceeds Int range) and sum across all banks
     *
     * ## Example:
     * - "987654321111111" → "987654321111" → 987654321111
     * - "811111111111119" → "811111111119" → 811111111119
     * - "234234234234278" → "434234234278" → 434234234278
     * - "818181911112111" → "888911112111" → 888911112111
     * - Total: 3121910778619
     *
     * @return Sum of maximum 12-digit joltages from all battery banks
     */
    fun solvePart2(): Long =
        input.sumOf { bank ->
            findMaxSubsequence(bank, PART2_PICK_COUNT).toLong()
        }
}

// ================================================================================================
// MAIN ENTRY POINT
// ================================================================================================

/**
 * Main entry point for Day 3 solution.
 *
 * Reads the puzzle input, instantiates the solver, and prints results for both parts.
 */
fun main() {
    val input = readInput(3)
    val solver = Day03Solver(input)

    println("Part 1: ${solver.solvePart1()}")
    println("Part 2: ${solver.solvePart2()}")
}
