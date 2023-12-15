import re

class Solution:
    def __init__(self):
        self.sensorstobeacons = {}

    def init(self, lines):
        pattern = re.compile(r'-?\d+')
        for line in lines:
            matches = re.findall(pattern, line)
            sensor_coords = (int(matches[0]), int(matches[1]))
            beacon_coords = (int(matches[2]), int(matches[3]))
            self.sensorstobeacons[sensor_coords] = beacon_coords

    def merge_intervals(self, intervals):
        intervals.sort(key=lambda x: x[0])
        index = 0
        for i in range(1, len(intervals)):
            if intervals[index][1] >= intervals[i][0]:
                intervals[index] = (intervals[index][0], max(intervals[index][1], intervals[i][1]))
            else:
                index += 1
                intervals[index] = intervals[i]
        return intervals[:index + 1]

    def part1(self):
        y = 2000000
        result = 0
        intervals = []
        for sensor, beacon in self.sensorstobeacons.items():
            manhattan = abs(sensor[0] - beacon[0]) + abs(sensor[1] - beacon[1])
            y_dist = abs(sensor[1] - y)
            if y_dist > manhattan:
                continue
            x_dist = manhattan - y_dist
            intervals.append((sensor[0] - x_dist, sensor[0] + x_dist))
        intervals = self.merge_intervals(intervals)
        beacon_y = next(beacon for _, beacon in self.sensorstobeacons.items() if beacon[1] == y)
        for interval in intervals:
            result += interval[1] - interval[0] + 1
            if beacon_y[1] == y and interval[0] <= beacon_y[0] <= interval[1]:
                result -= 1
        print(f"Part 1: {result}")

    def part2(self):
        result = 0
        mx = 4000000
        for i in range(mx + 1):
            intervals = []
            for sensor, beacon in self.sensorstobeacons.items():
                manhattan = abs(sensor[0] - beacon[0]) + abs(sensor[1] - beacon[1])
                y_dist = abs(sensor[1] - i)
                if y_dist > manhattan:
                    continue
                x_dist = manhattan - y_dist
                intervals.append((max(0, sensor[0] - x_dist), min(mx, sensor[0] + x_dist)))
            intervals = self.merge_intervals(intervals)
            if len(intervals) > 1:
                result = (intervals[0][1] + 1) * mx + i
                break
        print(f"Part 2: {result}")


if __name__ == "__main__":
    s = Solution()
    file = open("input.txt", "r")
    lines = file.read().splitlines()
    s.init(lines)
    s.part1()
    s.part2()