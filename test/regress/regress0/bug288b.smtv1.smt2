(set-option :incremental false)
(set-info :status sat)
(set-logic QF_LIA)
(declare-fun x () Int)
(declare-fun y () Int)
(check-sat-assuming ( (>= (* 2 (- x y)) 1) ))
