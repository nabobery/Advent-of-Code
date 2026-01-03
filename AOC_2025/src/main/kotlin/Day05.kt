package org.nabobery

/**
 * Represents an immutable range with start and end values.
 * Uses Kotlin's built-in LongRange for containment checks.
 */
private data class NumberRange(
    val start: Long,
    val end: Long,
) {
    val asLongRange: LongRange get() = start..end

    val size: Long get() = end - start + 1

    operator fun contains(number: Long): Boolean = number in asLongRange
}

/**
 * Parses a range string (e.g., "10-20") into a NumberRange.
 */
private fun String.toNumberRange(): NumberRange =
    split('-')
        .map(String::toLong)
        .let { (start, end) -> NumberRange(start, end) }

/**
 * Encapsulates the Day 5 puzzle logic with parsed and merged ranges.
 */
private class Day05Solver(
    input: List<String>,
) {
    private val ranges: List<NumberRange>
    private val numbers: List<Long>

    init {
        val (rangeLines, numberLines) =
            input
                .filter(String::isNotBlank)
                .partition { '-' in it }

        numbers = numberLines.map { it.trim().toLong() }.sorted()
        ranges =
            rangeLines
                .map(String::toNumberRange)
                .sortedBy { it.start }
                .mergeOverlapping()
    }

    /**
     * Merges overlapping ranges using fold for an immutable, functional approach.
     */
    private fun List<NumberRange>.mergeOverlapping(): List<NumberRange> =
        fold(mutableListOf()) { merged, range ->
            merged.apply {
                val last = lastOrNull()
                if (last == null || range.start > last.end) {
                    add(range)
                } else {
                    // Merge by creating a new range (immutability preserved at data level)
                    set(lastIndex, NumberRange(last.start, maxOf(last.end, range.end)))
                }
            }
        }

    /**
     * Part 1: Count how many numbers fall within ANY of the merged ranges.
     * Uses two-pointer technique with fold for a functional approach.
     */
    fun solvePart1(): Int {
        data class State(
            val rangeIndex: Int,
            val count: Int,
        )

        return numbers
            .fold(State(rangeIndex = 0, count = 0)) { state, number ->
                var idx = state.rangeIndex

                // Skip ranges that are entirely before this number
                while (idx < ranges.size && number > ranges[idx].end) {
                    idx++
                }

                val inRange = idx < ranges.size && number in ranges[idx]
                State(
                    rangeIndex = idx,
                    count = state.count + if (inRange) 1 else 0,
                )
            }.count
    }

    /**
     * Part 2: Sum the total count of all values covered by all merged ranges.
     */
    fun solvePart2(): Long = ranges.sumOf(NumberRange::size)
}

fun main() {
    val input = readInput(5)
    val solver = Day05Solver(input)

    println("part 1: ${solver.solvePart1()}")
    println("part 2: ${solver.solvePart2()}")
}
