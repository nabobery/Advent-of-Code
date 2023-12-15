from functools import cmp_to_key

class Solution:
    def __init__(self):
        self.packets = []

    def init(self, input_file):
        with open(input_file, 'r') as input:
            lines = input.readlines()
            for i in range(0, len(lines), 3):
                packet1 = lines[i].strip()
                packet2 = lines[i + 1].strip()
                self.packets.append((packet1, packet2))

    def helper(self, s):
        result = []
        temp = ""
        depth = 0
        for i in range(1, len(s)-1):
            if s[i] == '[':
                depth += 1
            elif s[i] == ']':
                depth -= 1
            elif s[i] == ',':
                if depth == 0:
                    result.append(temp)
                    temp = ""
            if s[i] != ',' or (s[i] == ',' and depth != 0):
                temp += s[i]
        if temp != "":
            result.append(temp)
        return result

    def compare(self, left, right):
        for l, r in zip(left, right):
            if l[0] != '[' and r[0] != '[':
                if int(l) < int(r):
                    return -1
                elif int(l) > int(r):
                    return 1
            elif l[0] == '[' and r[0] != '[':
                temp = self.compare(self.helper(l), self.helper("[" + r + "]"))
                if temp != 0:
                    return temp
            elif l[0] != '[' and r[0] == '[':
                temp = self.compare(self.helper("[" + l + "]"), self.helper(r))
                if temp != 0:
                    return temp
            else:
                temp = self.compare(self.helper(l), self.helper(r))
                if temp != 0:
                    return temp
        return len(left) - len(right)

    def part1(self):
        result = 0
        for i, packet in enumerate(self.packets):
            if self.compare(self.helper(packet[0]), self.helper(packet[1])) < 0:
                result += (i + 1)
        print("Part1:", result)

    def part2(self):
        all_packets = []
        for packet in self.packets:
            all_packets.extend([self.helper(packet[0]), self.helper(packet[1])])

        # adding divider packets
        all_packets.extend([self.helper("[[2]]"), self.helper("[[6]]")])

        # sorting all packets based on the compare function
        all_packets = sorted(all_packets, key=cmp_to_key(self.compare))

        #print(all_packets)

        result = 1
        for i, packet in enumerate(all_packets):
            if not packet:
                continue
            if (packet[0] == "[2]" or packet[0] == "[6]") and len(packet) == 1:
                result *= (i + 1)
        print("Part2:", result)


if __name__ == "__main__":
    input_file = "input.txt"
    s = Solution()
    s.init(input_file)
    s.part1()
    s.part2()
