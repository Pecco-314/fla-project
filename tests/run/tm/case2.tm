#Q = { copy, back, check, acc_clear, acc_msg1, acc_msg2, acc_msg3, acc_halt, rej, rej_clear, rej_msg1, rej_msg2, rej_msg3, rej_msg4, rej_halt }
#S = { a, b, c}
#G = { a, b, c, _, t, r, u, e, f, l, s }
#q0 = copy
#B = _
#F = { acc_halt }
#N = 2

copy a_ aa rr copy
copy b_ bb rr copy
copy c_ c_ *l back
copy __ __ l* rej         ; no 'c'
back c* c* *l back
back c_ c_ rr check
check aa aa rr check
check bb bb rr check
check __ __ l* acc_clear
check *_ *_ ** rej        ; too long
check _* __ ** rej        ; too short
check ba a_ ** rej        ; should be 'a', get 'b'
check ca a_ ** rej        ; should be 'a', get 'c'
check ab a_ ** rej        ; should be 'b', get 'a'
check cb b_ ** rej        ; should be 'b', get 'c'

acc_clear *_ __ l* acc_clear
acc_clear __ t_ r* acc_msg1
acc_msg1 __ r_ r* acc_msg2
acc_msg2 __ u_ r* acc_msg3
acc_msg3 __ e_ r* acc_halt

rej *_ *_ r* rej
rej __ __ l* rej_clear
rej_clear *_ __ l* rej_clear
rej_clear __ f_ r* rej_msg1
rej_msg1 __ a_ r* rej_msg2
rej_msg2 __ l_ r* rej_msg3
rej_msg3 __ s_ r* rej_msg4
rej_msg4 __ e_ r* rej_halt
