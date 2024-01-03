#Q = { A, B, C }
#S = { 0, 1 }
#G = { 0, 1, _ }
#q0 = A
#B = _
#F = {}
#N = 2

A 0_ *1 rr A
A __ ** ll B
B ** ** ll B
B __ ** *r C
C _1 0_ lr C
