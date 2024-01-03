#Q = { A, B, C }
#S = {0, 1}
#G = {0, 1, _}
#q0 = A
#B = _
#F = { C }
#N = 1

A 0 1 r A
A _ * l B
B 1 0 l B
B _ * r C
