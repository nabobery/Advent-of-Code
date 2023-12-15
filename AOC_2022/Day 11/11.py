from collections import deque
from typing import List
import re,copy

mod = 1

class Monkey:
    def __init__(self):
        self.items = deque()
        self.divisibility_test = 0
        self.if_true = 0
        self.if_false = 0
        self.ops = ""

# def evaluate(operation: str, val: int) -> int:
#     operands = deque()
#     operators = deque()
#     for i in range(len(operation)):
#         if operation[i].isdigit():
#             number = 0
#             while i < len(operation) and operation[i].isdigit():
#                 number = number * 10 + (ord(operation[i]) - ord('0'))
#                 i += 1
#             i -= 1
#             operands.append(number)
#         elif operation[i] in ['+', '*']:
#             operators.append(operation[i])
#         elif operation[i:i+3] == "old":
#             operands.append(val)
#             i += 2

#     while len(operators) > 0:
#         operand2 = operands.pop()
#         operand1 = operands.pop()
#         op = operators.pop()

#         if op == '+':
#             operands.append(operand1 + operand2)
#         elif op == '*':
#             operands.append(operand1 * operand2)

#     return operands[-1] % mod

def simulate(Monkeys: List[Monkey], rounds: int, divide: bool) -> int:
    items_inspected = [0] * len(Monkeys)
    monkeys = copy.deepcopy(Monkeys)
    for _ in range(rounds):
        for i in range(len(monkeys)):
            for _ in monkeys[i].items.copy():
                old = monkeys[i].items.popleft()
                items_inspected[i] += 1
                new_item = (eval(monkeys[i].ops) % mod)
                if divide:
                    new_item //= 3
                if new_item % monkeys[i].divisibility_test == 0:
                    monkeys[monkeys[i].if_true].items.append(new_item)
                else:
                    monkeys[monkeys[i].if_false].items.append(new_item)
    items_inspected.sort()
    print(items_inspected)
    result = items_inspected[-1] * items_inspected[-2]
    #print(result)
    return result

def main():
    item_pattern = re.compile("Starting items: ([0-9, ]*)")
    operation_pattern = re.compile("Operation: new = ([^\n]*)")
    div_test_pattern = re.compile("Test: divisible by ([0-9]+)")
    ifTrue_pattern = re.compile("If true: throw to monkey ([0-9]+)")
    ifFalse_pattern = re.compile("If false: throw to monkey ([0-9]+)")

    monkeys = []
    file = open("input.txt", "r")
    lines = file.read().splitlines()
    for i in range(0, len(lines), 7):
        monkey = Monkey()
        monkey.items = deque(map(int, item_pattern.findall(lines[i+1])[0].split(',')))
        monkey.ops = operation_pattern.findall(lines[i+2])[0]
        monkey.divisibility_test = int(div_test_pattern.findall(lines[i+3])[0])
        monkey.if_true = int(ifTrue_pattern.findall(lines[i+4])[0])
        monkey.if_false = int(ifFalse_pattern.findall(lines[i+5])[0])
        monkeys.append(monkey)

    # for monkey in monkeys:
    #     print(monkey.items)
    #     print(monkey.ops)
    #     print(monkey.divisibility_test)
    #     print(monkey.if_true)
    #     print(monkey.if_false)
    #     print()

    global mod
    for monkey in monkeys:
        mod *= monkey.divisibility_test
    # part 1
    print("The result for the 1st star is " + str(simulate(monkeys, 20, True)))
    # part 2
    print("The result for the 2nd star is " + str(simulate(monkeys, 10000, False)))

if __name__ == "__main__":
    main()
        


