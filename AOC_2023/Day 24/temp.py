from z3 import *
def parse(s):
    x,y,z = map(int, s.split(','))
    return (x,y,z)
hs = []
for L in open('input.txt').read().splitlines():
    hs.append(tuple(map(parse, L.split(' @ '))))
P = [Real(f'P{i}') for i in range(3)]
PV = [Real(f'PV{i}') for i in range(3)]
s = Solver()
for i in range(len(hs)):
    t = Real(f't{i}')
    p,v = hs[i]
    for c in range(3):
        s.add(P[c] + t*PV[c] == p[c] + t*v[c])
if s.check() == sat:
    m = s.model()
    print('Part 2:', sum(int(str(m.evaluate(v))) for v in P))
else:
    print('Unable to solve')
