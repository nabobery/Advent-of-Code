package org.nabobery

import kotlin.io.path.Path
import kotlin.io.path.readLines
import kotlin.math.abs

/**
 * Reads the puzzle input for the specified day.
 *
 * @param day The day number (1-25)
 * @return List of lines from the input file
 */
fun readInput(day: Int): List<String> {
    val dayString = day.toString().padStart(2, '0')
    return Path("src/main/resources/day$dayString.txt").readLines()
}

/**
 * Represents a 2D point with Int coordinates.
 *
 * This class provides utility methods for both grid traversal operations
 * (used in Day04) and geometric calculations (used in Day09).
 *
 * @property x The X coordinate (column in grid context)
 * @property y The Y coordinate (row in grid context)
 */
data class Point(
    val x: Int,
    val y: Int,
) {
    /**
     * Returns all 8-directional neighbors (cardinal + diagonal).
     * Useful for grid-based problems requiring adjacency checks.
     *
     * @return List of 8 neighboring points
     */
    fun neighbors(): List<Point> =
        (-1..1).flatMap { dx ->
            (-1..1).mapNotNull { dy ->
                if (dx == 0 && dy == 0) null else Point(x + dx, y + dy)
            }
        }

    /**
     * Calculates the area of a rectangle with this point and another as opposite corners.
     * Uses inclusive bounds (both corner points are part of the rectangle).
     *
     * @param other The opposite corner of the rectangle
     * @return The area as Long (to handle large coordinate differences)
     */
    fun rectangleAreaWith(other: Point): Long = (abs(x - other.x) + 1).toLong() * (abs(y - other.y) + 1)

    companion object {
        /**
         * Parses a comma-separated coordinate string into a [Point].
         *
         * @param input String in format "X,Y"
         * @return Parsed Point instance
         * @throws NumberFormatException if coordinates cannot be parsed
         */
        fun parse(input: String): Point =
            input
                .split(',')
                .map(String::toInt)
                .let { (x, y) -> Point(x, y) }
    }
}
