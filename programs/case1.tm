#Q = { find_b, write, find_a, del_a, del_b, cut, check_a0, check_a, check_b, back, accept, reject, reject_clear, reject_msg1, reject_msg2, reject_msg3, reject_msg4, reject_msg5, reject_msg6, reject_msg7, reject_msg8, reject_msg9, reject_msg10, reject_msg11, reject_msg12, reject_halt }
#S = { a, b, c }
#G = { a, b, c, _, I, l, e, g, n, p, u, t }
#q0 = check_a0
#B = _
#F = { accept }
#N = 2

check_a0 a_ a_ r* check_a
check_a0 b_ b_ ** reject    ; ^b
check_a0 __ __ ** reject    ; ^_
check_a a_ a_ r* check_a
check_a b_ b_ r* check_b
check_a __ __ ** reject     ; aaaaa^_
check_b a_ a_ ** reject     ; aabbb^a
check_b b_ b_ r* check_b
check_b __ __ l* back

back *_ *_ l* back
back __ __ r* del_a
del_a a_ __ r* find_b
find_b a_ a_ r* find_b
find_b b_ b_ ** write
write b_ bc rr write
write __ __ l* find_a
find_a *_ *_ l* find_a
find_a __ __ r* del_a
del_a a_ __ r* find_b
del_a b_ b_ ** del_b
del_b b_ __ r* del_b
del_b __ __ *l cut
cut _c c_ rl cut
cut __ __ ** accept

reject *_ *_ r* reject
reject __ __ l* reject_clear
reject_clear *_ __ l* reject_clear
reject_clear __ I_ r* reject_msg1
reject_msg1 __ l_ r* reject_msg2
reject_msg2 __ l_ r* reject_msg3
reject_msg3 __ e_ r* reject_msg4
reject_msg4 __ g_ r* reject_msg5
reject_msg5 __ a_ r* reject_msg6
reject_msg6 __ l_ r* reject_msg7
reject_msg7 __ __ r* reject_msg8
reject_msg8 __ I_ r* reject_msg9
reject_msg9 __ n_ r* reject_msg10
reject_msg10 __ p_ r* reject_msg11
reject_msg11 __ u_ r* reject_msg12
reject_msg12 __ t_ r* reject_halt
