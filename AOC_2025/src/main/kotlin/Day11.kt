/**
 * # Advent of Code 2025 - Day 11: Reactor Device Network
 *
 * This solution handles the problem of finding paths through a network of devices
 * connecting a server rack to a reactor.
 *
 * ## Problem Summary:
 * - **Part 1**: Count all possible paths from device "you" to device "out" in the device network.
 *   Uses memoized DFS to efficiently count paths in a directed acyclic graph (DAG).
 *
 * - **Part 2**: Count all paths from "svr" to "out" that pass through both "dac" (digital-to-analog
 *   converter) and "fft" (fast Fourier transform) devices in any order. Uses state-based memoization
 *   with a bitmask to track which required devices have been visited.
 *
 * ## Key Algorithms:
 * - **Part 1 - Memoized DFS**: Standard path counting in a DAG with O(V + E) complexity.
 * - **Part 2 - State Bitmask DFS**: Extends memoization key with a 2-bit state tracking
 *   dac and fft visits, resulting in O(4 * (V + E)) complexity.
 *
 * ## Graph Representation:
 * The input describes a directed graph where each line specifies a device and its output connections.
 * Data flows only from a device through its outputs (forward direction only).
 */

package org.nabobery

// ================================================================================================
// DATA MODELS
// ================================================================================================

/**
 * Represents the device network graph as an adjacency list.
 *
 * Each device maps to the list of devices it outputs to.
 *
 * @property adjacencyList The underlying map from device name to list of output devices
 */
@JvmInline
private value class DeviceNetwork(
    private val adjacencyList: Map<String, List<String>>,
) {
    /**
     * Gets the list of output devices for a given device.
     *
     * @param device The device to query
     * @return List of devices that this device outputs to, or empty list if not found
     */
    operator fun get(device: String): List<String> = adjacencyList[device] ?: emptyList()

    companion object {
        /**
         * Parses the input lines into a device network graph.
         *
         * Input format: `device: output1 output2 output3`
         * - Each line specifies a device followed by a colon and space-separated outputs
         *
         * @param input Raw input lines
         * @return Parsed [DeviceNetwork] instance
         */
        fun parse(input: List<String>): DeviceNetwork {
            val adjacencyList =
                input.associate { line ->
                    val device = line.substringBefore(":")
                    val outputs = line.substringAfter(": ").split(" ")
                    device to outputs
                }
            return DeviceNetwork(adjacencyList)
        }
    }
}

// ================================================================================================
// PATH COUNTING ALGORITHMS
// ================================================================================================

/**
 * Counts all paths from a source device to a target device using memoized DFS.
 *
 * ## Algorithm
 * This implements a standard path counting algorithm for directed acyclic graphs (DAGs):
 * - Base case: If current device is the target, return 1 (found a valid path)
 * - Recursive case: Sum the path counts from all output devices
 * - Memoization: Cache results to avoid recomputing paths for previously visited devices
 *
 * Time Complexity: O(V + E) where V = vertices, E = edges
 * Space Complexity: O(V) for memoization + O(max path length) for recursion stack
 *
 * @param network The device network graph
 * @param current The current device being visited
 * @param target The target device to reach ("out")
 * @param memo Memoization cache mapping device -> path count
 * @return Number of distinct paths from current to target
 */
private fun countPaths(
    network: DeviceNetwork,
    current: String,
    target: String,
    memo: MutableMap<String, Int>,
): Int {
    // Base case: reached the target
    if (current == target) return 1

    // Return cached result if available
    memo[current]?.let { return it }

    // Recursive case: sum paths through all output devices
    val pathCount =
        network[current].sumOf { neighbor ->
            countPaths(network, neighbor, target, memo)
        }

    // Cache and return result
    return pathCount.also { memo[current] = it }
}

/**
 * Represents the bitmask state for tracking required device visits.
 *
 * Uses a 2-bit representation:
 * - Bit 0 (value 1): dac has been visited
 * - Bit 1 (value 2): fft has been visited
 * - State 3 (binary 11): both devices have been visited
 */
private object VisitState {
    /** Bitmask indicating dac has been visited */
    const val DAC_VISITED = 1

    /** Bitmask indicating fft has been visited */
    const val FFT_VISITED = 2

    /** Target state indicating both dac and fft have been visited */
    const val BOTH_VISITED = 3

    /** dac device identifier */
    private const val DAC = "dac"

    /** fft device identifier */
    private const val FFT = "fft"

    /**
     * Updates the visit state based on the current device.
     *
     * @param currentState The current bitmask state
     * @param device The device being visited
     * @return Updated bitmask state
     */
    fun update(
        currentState: Int,
        device: String,
    ): Int =
        when (device) {
            DAC -> currentState or DAC_VISITED
            FFT -> currentState or FFT_VISITED
            else -> currentState
        }
}

/**
 * Counts all paths from source to target that visit both "dac" and "fft" devices.
 *
 * ## Algorithm
 * Extends the basic path counting with a state bitmask that tracks which required
 * devices (dac, fft) have been visited along the current path:
 *
 * - State 0: Neither dac nor fft visited
 * - State 1: Only dac visited
 * - State 2: Only fft visited
 * - State 3: Both dac and fft visited
 *
 * The memoization key is (device, state) to cache results for each device in each
 * possible visit state.
 *
 * Time Complexity: O(4 * (V + E)) = O(V + E) with constant factor of 4 states
 * Space Complexity: O(4 * V) = O(V) for memoization
 *
 * @param network The device network graph
 * @param current The current device being visited
 * @param target The target device to reach ("out")
 * @param currentState Current bitmask tracking dac/fft visits
 * @param memo Memoization cache mapping (device, state) -> path count
 * @return Number of distinct paths from current to target that visit both required devices
 */
private fun countConstrainedPaths(
    network: DeviceNetwork,
    current: String,
    target: String,
    currentState: Int,
    memo: MutableMap<Pair<String, Int>, Long>,
): Long {
    // Update state if current device is dac or fft
    val updatedState = VisitState.update(currentState, current)

    // Base case: reached the target
    // Only count this path if both dac and fft were visited
    if (current == target) {
        return if (updatedState == VisitState.BOTH_VISITED) 1L else 0L
    }

    // Return cached result if available
    val cacheKey = current to updatedState
    memo[cacheKey]?.let { return it }

    // Recursive case: sum paths through all output devices
    val pathCount =
        network[current].sumOf { neighbor ->
            countConstrainedPaths(network, neighbor, target, updatedState, memo)
        }

    // Cache and return result
    return pathCount.also { memo[cacheKey] = it }
}

// ================================================================================================
// SOLVER CLASS
// ================================================================================================

/**
 * Encapsulates the Day 11 puzzle logic for reactor device network path counting.
 *
 * Handles parsing of device network specifications and provides solutions for both parts:
 * - Part 1: Count all paths from "you" to "out"
 * - Part 2: Count paths from "svr" to "out" that visit both "dac" and "fft"
 *
 * @param input Raw input lines, one device specification per line
 */
private class Day11Solver(
    input: List<String>,
) {
    /** The parsed device network graph */
    private val network: DeviceNetwork = DeviceNetwork.parse(input)

    companion object {
        // Device identifiers
        private const val PART1_START = "you"
        private const val PART2_START = "svr"
        private const val TARGET = "out"
    }

    /**
     * **Part 1**: Count all paths from "you" to "out".
     *
     * Uses memoized DFS to efficiently count all distinct paths in the device network.
     * The graph is assumed to be a DAG (directed acyclic graph) based on the problem
     * description stating data flows only forward through outputs.
     *
     * @return Total number of distinct paths from "you" to "out"
     */
    fun solvePart1(): Int {
        val memo = mutableMapOf<String, Int>()
        return countPaths(network, PART1_START, TARGET, memo)
    }

    /**
     * **Part 2**: Count all paths from "svr" to "out" that visit both "dac" and "fft".
     *
     * Uses state-based memoized DFS where the state tracks which required devices
     * (dac: digital-to-analog converter, fft: fast Fourier transform) have been
     * visited along the current path.
     *
     * Only paths that visit both required devices (in any order) are counted.
     *
     * @return Total number of valid paths from "svr" to "out" visiting both dac and fft
     */
    fun solvePart2(): Long {
        val memo = mutableMapOf<Pair<String, Int>, Long>()
        return countConstrainedPaths(network, PART2_START, TARGET, 0, memo)
    }
}

// ================================================================================================
// MAIN ENTRY POINT
// ================================================================================================

/**
 * Main entry point for Day 11 solution.
 */
fun main() {
    val input = readInput(11)
    val solver = Day11Solver(input)

    println("Part 1: ${solver.solvePart1()}")
    println("Part 2: ${solver.solvePart2()}")
}
