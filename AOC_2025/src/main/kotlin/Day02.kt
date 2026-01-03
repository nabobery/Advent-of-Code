/**
 * # Advent of Code 2025 - Day 2: Gift Shop
 *
 * You've arrived at the North Pole gift shop via elevator, and a clerk needs help identifying
 * invalid product IDs that a young Elf accidentally added to the database while playing.
 *
 * ## Problem Summary:
 *
 * - **Part 1**: Find product IDs that are made of a sequence of digits repeated **exactly twice**.
 *   Examples: `55` (5×2), `6464` (64×2), `123123` (123×2), `1188511885` (11885×2)
 *
 * - **Part 2**: Find product IDs that are made of a sequence of digits repeated **at least twice**.
 *   Examples: `55` (5×2), `111` (1×3), `123123123` (123×3), `1212121212` (12×5)
 *
 * ## Input Format:
 * A single line containing comma-separated ID ranges in the format `start-end`.
 * ```
 * 11-22,95-115,998-1012,1188511880-1188511890,...
 * ```
 *
 * ## Key Algorithms:
 *
 * ### Part 1 - Double Concatenation Detection:
 * A number is a "double concatenation" if its string representation has even length and the
 * first half equals the second half.
 * - Check: `s.length % 2 == 0 && s.take(mid) == s.drop(mid)`
 * - Time Complexity: O(L) per number where L = number of digits
 *
 * ### Part 2 - Repeated Block Detection (Clever String Trick):
 * A string `s` is made of a repeated block if and only if `s` appears in `(s + s)` at some
 * index **between** 0 and `s.length`.
 *
 * **Why this works:**
 * Consider `s = "abcabc"`:
 * - `s + s = "abcabcabcabc"`
 * - Searching for `s` starting at index 1 finds it at index 3 (before position 6)
 * - This indicates `s` is periodic with period 3 ("abc" repeated twice)
 *
 * Consider `s = "abcdef"` (not periodic):
 * - `s + s = "abcdefabcdef"`
 * - Searching for `s` starting at index 1 only finds it at index 6 (exactly at `s.length`)
 * - This means `s` has no proper period
 *
 * Mathematical proof: If `s` has period `p`, then `s` aligns at index `p` in `s + s`,
 * where `0 < p < s.length`.
 *
 * - Check: `(s + s).indexOf(s, startIndex = 1) < s.length`
 * - Time Complexity: O(L) per number using efficient string search (KMP/Z-algorithm internally)
 *
 * ## Example Walkthrough:
 *
 * For range `1188511880-1188511890`:
 * - `1188511885` → String: "1188511885", length = 10 (even)
 * - First half: "11885", Second half: "11885" → Match! Invalid ID.
 *
 * For range `95-115` (Part 2):
 * - `99` → "99" + "99" = "9999", indexOf("99", 1) = 1 < 2 → Repeated block (9×2)
 * - `111` → "111" + "111" = "111111", indexOf("111", 1) = 1 < 3 → Repeated block (1×3)
 *
 * @see <a href="https://adventofcode.com/2025/day/2">Advent of Code 2025, Day 2</a>
 */

package org.nabobery

// ================================================================================================
// EXTENSION FUNCTIONS - STRING REPETITION DETECTION
// ================================================================================================

/**
 * Checks if this number is a "double concatenation" - a number whose string representation
 * consists of the same sequence of digits repeated exactly twice.
 *
 * ## Examples:
 * - `55L` → "55" = "5" + "5" → **true**
 * - `123123L` → "123123" = "123" + "123" → **true**
 * - `12345L` → "12345" has odd length → **false**
 * - `1234L` → "1234" = "12" + "34" → **false**
 *
 * ## Algorithm:
 * 1. Convert to string representation
 * 2. Check if length is even (odd-length strings cannot be double concatenations)
 * 3. Compare the first half with the second half
 *
 * @receiver The Long value to check
 * @return `true` if the number is a double concatenation, `false` otherwise
 */
private fun Long.isDoubleConcat(): Boolean {
    val digits = this.toString()
    if (digits.length % 2 != 0) return false

    val mid = digits.length / 2
    return digits.take(mid) == digits.drop(mid)
}

/**
 * Checks if this number is made of a repeated block - a number whose string representation
 * consists of some sequence of digits repeated two or more times.
 *
 * ## Examples:
 * - `55L` → "5" × 2 → **true**
 * - `111L` → "1" × 3 → **true**
 * - `123123L` → "123" × 2 → **true**
 * - `121212L` → "12" × 3 → **true**
 * - `12345L` → no repeating pattern → **false**
 *
 * ## Algorithm (String Rotation Property):
 * A string `s` is made of a repeated block if and only if `s` appears in `(s + s)` at
 * an index strictly between 0 and `s.length`.
 *
 * ### Mathematical Insight:
 * - If `s = p + p + ... + p` (p repeated k times), then in `s + s`, there's a copy of
 *   `s` starting at index `|p|` (the length of the period).
 * - Conversely, if `s` appears in `s + s` at index `i` where `0 < i < |s|`, then
 *   `s` has period `i` and is thus made of the prefix of length `i` repeated.
 *
 * ### Why `indexOf(s, startIndex = 1)`?
 * We skip index 0 because `s` trivially appears at the start of `s + s`.
 * If the next occurrence is at exactly `s.length`, then `s` has no proper period.
 *
 * @receiver The Long value to check
 * @return `true` if the number is made of a repeated block, `false` otherwise
 */
private fun Long.isRepeatedBlock(): Boolean {
    val digits = this.toString()
    if (digits.length < 2) return false

    // A string is composed of a repeated block iff it reappears inside (s + s) starting after index 0
    // and before index s.length
    return (digits + digits).indexOf(digits, startIndex = 1) < digits.length
}

// ================================================================================================
// INPUT PARSING
// ================================================================================================

/**
 * Represents a range of product IDs to check for invalid entries.
 *
 * @property start The first ID in the range (inclusive)
 * @property end The last ID in the range (inclusive)
 */
private data class IdRange(
    val start: Long,
    val end: Long,
) {
    /**
     * Generates a lazy sequence of all IDs in this range.
     *
     * Using [Sequence] instead of [List] provides memory efficiency for large ranges,
     * as IDs are generated on-demand rather than stored in memory.
     *
     * @return A sequence of all Long values from [start] to [end] inclusive
     */
    fun asSequence(): Sequence<Long> = (start..end).asSequence()

    companion object {
        /**
         * Parses a range string in the format "start-end".
         *
         * @param input The range string (e.g., "11-22", "1188511880-1188511890")
         * @return Parsed [IdRange] instance
         * @throws NumberFormatException if the values cannot be parsed as Long
         */
        fun parse(input: String): IdRange {
            val (start, end) = input.split('-', limit = 2).map(String::toLong)
            return IdRange(start, end)
        }
    }
}

/**
 * Parses the input line into a sequence of ID ranges.
 *
 * The input is a comma-separated list of ranges, where each range is in the format "start-end".
 * Empty segments (from trailing commas or empty input) are filtered out.
 *
 * ## Example:
 * Input: `"11-22,95-115,998-1012,"`
 * Output: Sequence of IdRange(11, 22), IdRange(95, 115), IdRange(998, 1012)
 *
 * @param line The raw input line containing comma-separated ranges
 * @return A sequence of parsed [IdRange] objects
 */
private fun parseRanges(line: String): Sequence<IdRange> =
    line
        .splitToSequence(',')
        .filter { it.isNotBlank() }
        .map { IdRange.parse(it) }

// ================================================================================================
// SOLVER CLASS
// ================================================================================================

/**
 * Encapsulates the Day 2 puzzle logic for identifying invalid product IDs.
 *
 * The solver parses comma-separated ID ranges from the input and provides methods to:
 * - Part 1: Sum IDs that are double concatenations (e.g., "123123")
 * - Part 2: Sum IDs that are repeated blocks (e.g., "123123123")
 *
 * ## Design Decisions:
 * - Uses [Sequence] throughout for lazy evaluation, enabling efficient processing of large ranges
 * - Extension functions on [Long] encapsulate the pattern-detection logic
 * - ID ranges are modeled as a simple data class for clarity
 *
 * @param input Raw input lines (only the first line is used)
 */
private class Day02Solver(
    input: List<String>,
) {
    /** The raw input line containing comma-separated ID ranges */
    private val rangeLine: String = input.first()

    /**
     * **Part 1**: Sum all invalid product IDs that are double concatenations.
     *
     * A double concatenation is a number whose string representation consists of
     * the same digit sequence repeated exactly twice (e.g., `6464` = "64" + "64").
     *
     * The algorithm:
     * 1. Parse all ID ranges from the input
     * 2. Expand each range into a sequence of IDs
     * 3. Filter to keep only double concatenations
     * 4. Sum all matching IDs
     *
     * Time Complexity: O(N × L) where N = total IDs across all ranges, L = max digit length
     * Space Complexity: O(1) auxiliary space due to lazy sequence processing
     *
     * @return Sum of all invalid IDs that are double concatenations
     */
    fun solvePart1(): Long =
        parseRanges(rangeLine)
            .flatMap { it.asSequence() }
            .filter { it.isDoubleConcat() }
            .sum()

    /**
     * **Part 2**: Sum all invalid product IDs that are repeated blocks.
     *
     * A repeated block is a number whose string representation consists of some digit
     * sequence repeated two or more times (e.g., `123123` = "123" × 2, `1111` = "1" × 4).
     *
     * This is a superset of Part 1's condition - all double concatenations are also
     * repeated blocks, but repeated blocks also include patterns like `111` (1×3) or
     * `12121212` (12×4).
     *
     * The algorithm uses the string rotation property: a string `s` is a repeated block
     * if and only if `s` appears in `s + s` at an index strictly between 0 and `s.length`.
     *
     * Time Complexity: O(N × L) where N = total IDs, L = max digit length
     * Space Complexity: O(L) for the concatenated string during each check
     *
     * @return Sum of all invalid IDs that are repeated blocks
     */
    fun solvePart2(): Long =
        parseRanges(rangeLine)
            .flatMap { it.asSequence() }
            .filter { it.isRepeatedBlock() }
            .sum()
}

// ================================================================================================
// MAIN ENTRY POINT
// ================================================================================================

/**
 * Main entry point for Day 2 solution.
 *
 * Reads the puzzle input, creates a solver, and prints solutions for both parts.
 */
fun main() {
    val input = readInput(2)
    val solver = Day02Solver(input)

    println("Part 1: ${solver.solvePart1()}")
    println("Part 2: ${solver.solvePart2()}")
}
