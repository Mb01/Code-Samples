#lang racket

;; (solve n) -> list
;; n is natural number > 1
;; the list represents the continued fraction form of sqrt(n)


;; the following is adapted from my answer to
;; Project Euler problem 64:
;; https://projecteuler.net/problem=64

;; (note that I have redacted much of the solution)

;; Since I didn't read up on the material before attempting the problem, the terminology might be awkward. Many number theory books as well as The Art of Computer Programming vol. 2 (Knuth) cover this.

;; let's walk through the example
;; I retain three numbers representing this form "(sqrt(a) + b) / c"
;; that is
;;     sqrt(23) = (sq23 + 0) / 1 =

;; ** While positive, move ones out of the fraction **
;;     4 + (sq23 - 4) / 1  = 

;; ** then "continued-fractionify" **
;;     4 + 1 / (1 / (sq23 - 4)) 
;;     (the 4 on the left is part of the answer and we continue with)
;;     1 / (sq23 - 4)

;; ** to return to the appropriate form **
;; ** rationalize the denominator by multiplying with conjugate **
;;     1 / (sq23 - 4) * (sq23 + 4) / (sq23 + 4) => (sq23 + 4) / 7

;; ** repeat **

;; --- this last step is  ---
;; a / (sqrt(b) + c) =
;; a (sqrt(b) - c) / (b - c^2)

;; ====== BRIEF EXPLANATION ====== Todo: formal proof

;; I haven't proven it, but I guess this always simplifies to:

;; (sqrt(root-of) + add) / den -> (sqrt(root-of) - add') / den'

;; where
;; add' = add - ones * den
;; den' = (root-of - add'^2) / den
;; ones = factor-out-ones(root-of, add, den)

;; How many ones can we remove from the fraction
;; while the fraction is still positive? 
(define (factor-out-ones root-of addend denominator)
  (cond
    [(> (sqr addend) root-of) -1]
    [else (add1 (factor-out-ones root-of (- addend denominator) denominator))]))

(define (solve root-of)
  ;; record arguments to detect cycle
  (define cycle-set (mutable-set))

  ;; current state in the form: (sq(root-of) + add) / den
  ;; acc has "left-side addends" already found
  (define (solve add den acc)
    (cond
      ;; already seen arguments represents cycle -> finish
      [(set-member? cycle-set (list add den)) (reverse acc)]
      [else
       ;; record arguments
       (set-add! cycle-set (list add den))
       (let* (;; factor out ones
              [ones (factor-out-ones root-of add den)]
              ;; update add to reflect this
              [add (- add (* ones den))]
              ;; invert -> rationalize -> cancel multiplier
              [new-den (/ (- root-of (sqr add)) den)]
              [new-add (- add)])
         ;; recurse
         (solve new-add new-den (cons ones acc)))]))
  (solve 0 1 '()))

;; (solve 23); -> '(4 1 3 1 8) note the first number is not part of the period

;; The rest redacted to discourage would be cheaters

;; Unwrapping the solve function allows for solving (1 + sqr(5))/ 2, fun stuff.

