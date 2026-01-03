package org.nabobery

/**
 * Represents a mathematical operator for column-wise equations.
 */
private enum class Operator(
    val symbol: Char,
) {
    ADD('+'),
    MULTIPLY('*'),
    ;

    companion object {
        fun fromChar(char: Char): Operator =
            entries.find { it.symbol == char }
                ?: throw IllegalArgumentException("Unknown operator: $char")
    }
}

/**
 * Represents a single column equation with operands and an operator.
 * Operands are stored as strings to preserve formatting for column-wise parsing.
 */
private data class ColumnEquation(
    val operands: List<String>,
    val operator: Operator,
) {
    /**
     * Evaluates the equation by applying the operator to all operands.
     * Operands are trimmed and converted to Long before evaluation.
     */
    fun evaluate(): Long {
        val values = operands.map { it.trim().toLong() }
        return when (operator) {
            Operator.ADD -> values.sum()
            Operator.MULTIPLY -> values.reduce(Long::times)
        }
    }

    /**
     * Creates a transposed version of this equation.
     * Transforms column-wise operands to row-wise by reading characters vertically.
     */
    fun transpose(): ColumnEquation {
        val transposed =
            (operands[0].lastIndex downTo 0).map { col ->
                operands.map { row -> row[col] }.joinToString("")
            }
        return copy(operands = transposed)
    }
}

/**
 * Encapsulates the Day 6 puzzle logic for column-wise equation parsing and evaluation.
 */
private class Day06Solver(
    input: List<String>,
) {
    private val equations: List<ColumnEquation> = parseColumnWiseEquations(input)

    /**
     * Parses column-wise equations from the input.
     *
     * Input format:
     * - Multiple rows of operands (space-separated columns)
     * - Last row contains operators for each column
     *
     * @param input Raw input lines
     * @return List of parsed column equations
     */
    private fun parseColumnWiseEquations(input: List<String>): List<ColumnEquation> {
        val operandRows = input.dropLast(1)
        val operators =
            input
                .last()
                .trim()
                .split(Regex("\\s+"))
                .map { Operator.fromChar(it.first()) }

        val maxLength = operandRows.maxOf(String::length)
        val paddedRows = operandRows.map { it.padEnd(maxLength) }

        return extractColumns(paddedRows)
            .zip(operators)
            .map { (columnOperands, operator) ->
                ColumnEquation(operands = columnOperands, operator = operator)
            }
    }

    /**
     * Extracts column data from padded rows by finding separator positions.
     *
     * @param paddedRows Rows padded to equal length
     * @return List of column operand lists
     */
    private fun extractColumns(paddedRows: List<String>): List<List<String>> =
        buildList {
            val maxLength = paddedRows.firstOrNull()?.length ?: return@buildList
            var start = 0

            for (i in 0 until maxLength) {
                val isSeparator = paddedRows.all { it[i] == ' ' }
                if (isSeparator) {
                    add(paddedRows.map { it.substring(start, i) })
                    start = i + 1
                }
            }
            // Add the final column
            add(paddedRows.map { it.substring(start, maxLength) })
        }

    /**
     * Part 1: Evaluate all equations normally and sum the results.
     */
    fun solvePart1(): Long = equations.sumOf(ColumnEquation::evaluate)

    /**
     * Part 2: Transpose each equation (read columns as rows) and sum the results.
     */
    fun solvePart2(): Long =
        equations
            .map(ColumnEquation::transpose)
            .sumOf(ColumnEquation::evaluate)
}

fun main() {
    val input = readInput(6)
    val solver = Day06Solver(input)

    println("part 1: ${solver.solvePart1()}")
    println("part 2: ${solver.solvePart2()}")
}
