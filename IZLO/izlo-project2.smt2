(set-logic NIA)

(set-option :produce-models true)
(set-option :incremental true)

; Deklarace promennych pro vstupy
; ===============================

; Parametry
(declare-fun A () Int)
(declare-fun B () Int)
(declare-fun C () Int)
(declare-fun D () Int)
(declare-fun E () Int)

(declare-fun x () Int)
(declare-fun y () Int)
(declare-fun z () Int)

(declare-fun D1 () Int)
(declare-fun E1 () Int)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;; START OF SOLUTION ;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; XLOGIN: xdrabbo00

; Zde doplnte vase reseni:

(assert (and (> D 0) (> E 0)))                                                      ; Overenie, že premenné D a E sú väčšie ako 0


(assert (= x (- (* A B) (* C 5))))                                                  ; Výpočet hodnoty x ako rozdielu medzi súčinom A a B a súčinom C a 5


(assert (or (and (< (+ x E) (+ D D)) (= y (+ x (* 3 B))))                           ; Overenie podmienky pre hodnotu y na základe hodnôt x, E, D, B a C
            (and (not (< (+ x E) (+ D D))) (= y (* x (* C 2))))))


(assert                                                                             ; Overenie podmienky pre hodnotu z na základe hodnôt y, C, x, A, B a D
    (ite (<= (- y 5) C)
        (= z (- (* x A) (* y B)))
        (ite (> x (+ x 2) D)
            (= z (* (+ x A) (+ y B)))
            (= z (+ (* x B) (* y A))))))


(assert (and (> z (+ D E)) (> z 0)))                                                ; Overenie, že hodnota z je väčšia ako súčet D a E a zároveň väčšia ako 0


(assert                                                                             ; Overenie, že neexistujú hodnoty D1, E1, x, y a z, ktoré by spĺňali nasledujúce podmienky
  (not 
    (exists ((D1 Int) (E1 Int) (x Int) (y Int) (z Int))
      (and 
        (> D1 0)
        (> E1 0)
        (= x (- (* A B) (* C 5)))
        (or                                                                          ; Ak je súčet x a E1 menší ako dvojnásobok D1, y sa rovná súčtu x a trojnásobku B
          (and (< (+ x E1) (* 2 D1)) (= y (+ x (* 3 B))))
          (and (>= (+ x E1) (* 2 D1)) (= y (* x (* 2 C)))))
        (or                                                                          ; Ak je rozdiel y a 5 menší alebo rovný C, z sa rovná rozdielu medzi súčinom x a A a súčinom y a B
          (and (<= (- y 5) C) (= z (- (* x A) (* y B))))
          (and (> (- y 5) C) 
               (or 
                 (and (> (+ x 2) D1) (= z (* (+ x A) (+ y B))))
                 (and (<= (+ x 2) D1) (= z (+ (* x B) (* y A)))))))
        (> z (+ D1 E1))
        (< (+ D1 E1) (+ D E))
      )
    )
  )
)


; bonus:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;; END OF SOLUTION ;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Testovaci vstupy
; ================

(echo "Test 1 - vstup A=1, B=1, C=3")
(echo "a) Ocekavany vystup je sat a D+E se rovna 17")
(check-sat-assuming (
  (= A 1) (= B 1) (= C 3)
))
(get-value (D E (+ D E)))
(echo "b) Neexistuje jine reseni nez 17, ocekavany vystup je unsat")
(check-sat-assuming (
  (= A 1) (= B 1) (= C 3) (distinct (+ D E) 17)
))

(echo "")
(echo "")

(echo "Test 2 - vstup A=5, B=2, C=5")
(echo "a) Ocekavany vystup je sat a D+E se rovna 18")
(check-sat-assuming (
  (= A 5) (= B 2) (= C 5)
))
(get-value (D E (+ D E)))
(echo "b) Neexistuje jine reseni nez 18, ocekavany vystup je unsat")
(check-sat-assuming (
  (= A 5) (= B 2) (= C 5) (distinct (+ D E) 18)
))

(echo "")
(echo "")

(echo "Test 3 - vstup A=100, B=15, C=1")
(echo "a) Ocekavany vystup je sat a D+E se rovna 2")
(check-sat-assuming (
  (= A 100) (= B 15) (= C 1)
))
(get-value (D E (+ D E)))
(echo "b) Neexistuje jine reseni nez 2, ocekavany vystup je unsat")
(check-sat-assuming (
  (= A 100) (= B 15) (= C 1) (distinct (+ D E) 2)
))

(echo "")
(echo "")

(echo "Test 4 - vstup A=5, B=5, C=3")
(echo "a) Ocekavany vystup je sat a D+E se rovna 2")
(check-sat-assuming (
  (= A 5) (= B 5) (= C 3)
))
(get-value (D E (+ D E)))
(echo "b) Neexistuje jine reseni nez 2, ocekavany vystup je unsat")
(check-sat-assuming (
  (= A 5) (= B 5) (= C 3) (distinct (+ D E) 2)
))

(echo "")
(echo "")

(echo "Test 5 - vstup A=1, B=1, C=1")
(echo "a) Ocekavany vystup je unsat")
(check-sat-assuming (
  (= A 1) (= B 1) (= C 1)
))

(echo "")
(echo "")

(echo "Test 6 - vstup A=4, B=5, C=3")
(echo "a) Ocekavany vystup je sat a D+E se rovna 2")
(check-sat-assuming (
  (= A 4) (= B 5) (= C 3)
))
(get-value (D E (+ D E)))
(echo "b) Neexistuje jine reseni nez 2, ocekavany vystup je unsat")
(check-sat-assuming (
  (= A 4) (= B 5) (= C 3) (distinct (+ D E) 2)
))

(echo "")
(echo "")

(echo "Test 7 - vstup A=0, B=0, C=0")
(echo "a) Ocekavany vystup je unsat")
(check-sat-assuming (
  (= A 0) (= B 0) (= C 0)
))

(echo "")
(echo "")

(echo "Test 8 - vstup A=100, B=-50, C=-75")
(echo "a) Ocekavany vystup je sat a D+E se rovna 4628")
(check-sat-assuming (
  (= A 100) (= B -50) (= C -75)
))
(get-value (D E (+ D E)))
(echo "b) Neexistuje jine reseni nez 4628, ocekavany vystup je unsat")
(check-sat-assuming (
  (= A 100) (= B -50) (= C -75) (distinct (+ D E) 4628)
))

(echo "")
(echo "")

(echo "Test 9 - vstup A=3, B=2, C=2")
(echo "a) Ocekavany vystup je sat a D+E se rovna 7")
(check-sat-assuming (
  (= A 3) (= B 2) (= C 2)
))
(get-value (D E (+ D E)))
(echo "b) Neexistuje jine reseni nez 7, ocekavany vystup je unsat")
(check-sat-assuming (
  (= A 3) (= B 2) (= C 2) (distinct (+ D E) 7)
))

(echo "")
(echo "")

(echo "Test 10 - vstup A=-100, B=-15, C=-90")
(echo "a) Ocekavany vystup je sat a D+E se rovna 977")
(check-sat-assuming (
  (= A -100) (= B -15) (= C -90)
))
(get-value (D E (+ D E)))
(echo "b) Neexistuje jine reseni nez 977, ocekavany vystup je unsat")
(check-sat-assuming (
  (= A -100) (= B -15) (= C -90) (distinct (+ D E) 977)
))
