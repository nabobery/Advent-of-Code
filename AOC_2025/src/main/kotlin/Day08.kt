/**
 * # Advent of Code 2025 - Day 8: Junction Box Circuits
 *
 * This solution models the problem of connecting junction boxes to form electrical circuits.
 *
 * ## Problem Summary:
 * - **Part 1**: Connect the 1000 closest pairs of junction boxes and find the product of the
 *   three largest circuit sizes.
 * - **Part 2**: Continue connecting until all junction boxes form a single circuit. Return the
 *   product of the X coordinates of the last two junction boxes connected.
 *
 * ## Key Algorithms:
 * - **Union-Find (Disjoint Set Union)**: Efficiently tracks connected components with path
 *   compression and union-by-rank optimizations for near O(1) operations.
 * - **Prim's Minimum Spanning Tree**: Used in Part 2 to find the order of connections that
 *   minimizes total distance, identifying the last edge that unifies all components.
 */

package org.nabobery

import kotlin.math.sqrt

/**
 * Represents a 3D point in space with integer coordinates.
 *
 * @property x The X coordinate
 * @property y The Y coordinate
 * @property z The Z coordinate
 */
private data class Point3D(
    val x: Int,
    val y: Int,
    val z: Int,
) {
    /**
     * Calculates the Euclidean distance to another 3D point.
     *
     * @param other The target point to measure distance to
     * @return The straight-line distance as a Double
     */
    infix fun distanceTo(other: Point3D): Double {
        val dx = (x - other.x).toDouble()
        val dy = (y - other.y).toDouble()
        val dz = (z - other.z).toDouble()
        return sqrt(dx * dx + dy * dy + dz * dz)
    }

    companion object {
        /**
         * Parses a comma-separated coordinate string into a [Point3D].
         *
         * @param input String in format "X,Y,Z"
         * @return Parsed Point3D instance
         * @throws NumberFormatException if coordinates cannot be parsed
         */
        fun parse(input: String): Point3D =
            input
                .split(',')
                .map(String::toInt)
                .let { (x, y, z) -> Point3D(x, y, z) }
    }
}

/**
 * Represents a distance measurement between two junction boxes.
 *
 * @property first Index of the first junction box
 * @property second Index of the second junction box
 * @property distance Euclidean distance between the two boxes
 */
private data class JunctionPair(
    val first: Int,
    val second: Int,
    val distance: Double,
)

/**
 * Union-Find (Disjoint Set Union) data structure with path compression and union-by-rank.
 *
 * This implementation provides near O(1) amortized time complexity for both `find` and `union`
 * operations through two key optimizations:
 * - **Path Compression**: Flattens the tree during find operations
 * - **Union by Rank**: Always attaches the smaller tree under the root of the larger tree
 *
 * @property size The number of elements in the data structure
 */
private class UnionFind(
    private val size: Int,
) {
    /** Parent array where parent[i] is the parent of element i. Initially, each element is its own parent. */
    private val parent = IntArray(size) { it }

    /** Rank array for union-by-rank optimization. Represents the upper bound of tree height. */
    private val rank = IntArray(size) { 0 }

    /** Tracks the number of distinct components (circuits). */
    var componentCount: Int = size
        private set

    /**
     * Finds the root representative of the set containing [element].
     * Uses path compression to flatten the tree structure.
     *
     * @param element The element to find the root for
     * @return The root representative of the element's set
     */
    fun find(element: Int): Int {
        if (parent[element] != element) {
            parent[element] = find(parent[element]) // Path compression
        }
        return parent[element]
    }

    /**
     * Merges the sets containing [first] and [second] elements.
     * Uses union-by-rank to maintain balanced trees.
     *
     * @param first First element
     * @param second Second element
     * @return `true` if the elements were in different sets and are now merged,
     *         `false` if they were already in the same set
     */
    fun union(
        first: Int,
        second: Int,
    ): Boolean {
        val rootFirst = find(first)
        val rootSecond = find(second)

        if (rootFirst == rootSecond) return false // Already in the same set

        // Union by rank: attach smaller tree under root of larger tree
        when {
            rank[rootFirst] < rank[rootSecond] -> parent[rootFirst] = rootSecond
            rank[rootFirst] > rank[rootSecond] -> parent[rootSecond] = rootFirst
            else -> {
                parent[rootSecond] = rootFirst
                rank[rootFirst]++
            }
        }

        componentCount--
        return true
    }

    /**
     * Computes the size of each component (circuit).
     *
     * @return Map from component root to component size
     */
    fun getComponentSizes(): Map<Int, Int> =
        (0 until size)
            .groupingBy { find(it) }
            .eachCount()
}

/**
 * Prim's Minimum Spanning Tree algorithm implementation.
 *
 * This class builds an MST from a distance matrix and identifies the last edge
 * added, which represents the connection between the most distant junction boxes
 * needed to complete the spanning tree.
 *
 * @property distanceMatrix Square matrix of distances between all junction box pairs
 */
private class PrimMST(
    private val distanceMatrix: Array<DoubleArray>,
) {
    private val numberOfPoints = distanceMatrix.size

    /**
     * Finds the indices of the last two junction boxes connected when building the MST.
     *
     * The MST is built by always selecting the minimum weight edge that connects
     * a vertex in the tree to a vertex outside the tree. The last edge added
     * (with maximum weight among MST edges) represents the final connection
     * needed to unify all junction boxes.
     *
     * @return Pair of indices (parent, child) representing the last edge added to the MST
     */
    fun findLastConnection(): Pair<Int, Int> {
        val key = DoubleArray(numberOfPoints) { Double.POSITIVE_INFINITY }
        val parent = IntArray(numberOfPoints) { -1 }
        val inMST = BooleanArray(numberOfPoints) { false }

        key[0] = 0.0

        // Build MST by adding n-1 edges
        repeat(numberOfPoints - 1) {
            // Find minimum key vertex not yet in MST
            val u =
                (0 until numberOfPoints)
                    .filter { !inMST[it] }
                    .minByOrNull { key[it] }
                    ?: return@repeat

            inMST[u] = true

            // Update keys of adjacent vertices
            (0 until numberOfPoints)
                .filter { v -> !inMST[v] && distanceMatrix[u][v] > 0 && distanceMatrix[u][v] < key[v] }
                .forEach { v ->
                    parent[v] = u
                    key[v] = distanceMatrix[u][v]
                }
        }

        // Find the edge with maximum weight in MST (last connection needed)
        return (1 until numberOfPoints)
            .filter { parent[it] != -1 }
            .maxByOrNull { distanceMatrix[parent[it]][it] }
            ?.let { v -> parent[v] to v }
            ?: (-1 to -1)
    }
}

/**
 * Encapsulates the Day 8 puzzle logic for junction box circuit analysis.
 *
 * The solver handles:
 * - Parsing 3D coordinates from input
 * - Computing all pairwise distances
 * - Managing circuit connectivity via Union-Find
 * - Building MST via Prim's algorithm for Part 2
 *
 * @param input Raw input lines containing junction box coordinates
 */
private class Day08Solver(
    input: List<String>,
) {
    /** Parsed junction box positions */
    private val junctionBoxes: List<Point3D> = input.map(Point3D::parse)

    /** Pre-computed sorted list of all junction box pairs by distance */
    private val sortedPairs: List<JunctionPair> by lazy { computeAllPairs() }

    /** Pre-computed distance matrix for Part 2 */
    private val distanceMatrix: Array<DoubleArray> by lazy { buildDistanceMatrix() }

    /**
     * Computes all pairwise distances between junction boxes and returns them sorted.
     * Uses sequence for lazy evaluation during the mapping phase.
     *
     * @return List of [JunctionPair] sorted by ascending distance
     */
    private fun computeAllPairs(): List<JunctionPair> =
        buildList {
            for (i in junctionBoxes.indices) {
                for (j in i + 1 until junctionBoxes.size) {
                    add(JunctionPair(i, j, junctionBoxes[i] distanceTo junctionBoxes[j]))
                }
            }
        }.sortedBy(JunctionPair::distance)

    /**
     * Builds a symmetric distance matrix for all junction box pairs.
     *
     * @return 2D array where [i][j] contains the distance between junction boxes i and j
     */
    private fun buildDistanceMatrix(): Array<DoubleArray> {
        val n = junctionBoxes.size
        return Array(n) { i ->
            DoubleArray(n) { j ->
                if (i == j) 0.0 else junctionBoxes[i] distanceTo junctionBoxes[j]
            }
        }
    }

    /**
     * **Part 1**: Connect the 1000 closest pairs and compute the product of the three
     * largest circuit sizes.
     *
     * Algorithm:
     * 1. Sort all junction box pairs by distance
     * 2. Connect the 1000 closest pairs using Union-Find
     * 3. Count the size of each resulting circuit
     * 4. Return the product of the three largest circuit sizes
     *
     * @return Product of the three largest circuit sizes
     */
    fun solvePart1(): Long {
        val unionFind = UnionFind(junctionBoxes.size)

        // Connect the 1000 closest pairs
        sortedPairs
            .take(1000)
            .forEach { (first, second, _) ->
                unionFind.union(first, second)
            }

        // Get the three largest circuit sizes and compute their product
        return unionFind
            .getComponentSizes()
            .values
            .sortedDescending()
            .take(3)
            .fold(1L) { acc, size -> acc * size }
    }

    /**
     * **Part 2**: Find the last two junction boxes that need to be connected to form
     * a single unified circuit.
     *
     * Uses Prim's MST algorithm to find the order in which connections should be made.
     * The last edge added to the MST represents the final connection needed.
     *
     * @return Product of the X coordinates of the last two connected junction boxes
     */
    fun solvePart2(): Long {
        val mst = PrimMST(distanceMatrix)
        val (i, j) = mst.findLastConnection()
        return junctionBoxes[i].x.toLong() * junctionBoxes[j].x
    }
}

/**
 * Main entry point for Day 8 solution.
 */
fun main() {
    val input = readInput(8)
    val solver = Day08Solver(input)

    println("Part 1: ${solver.solvePart1()}")
    println("Part 2: ${solver.solvePart2()}")
}
