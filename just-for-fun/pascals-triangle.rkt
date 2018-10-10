#lang racket

;; make pascals triangle (without using "for" for fun)

;; get value of pascal's triangle as turned on it's side
;; like this:
;; 1 1 1 1 1 1
;; 1 2 3 4 5
;; 1 3 6 10
;; 1 4 10
;; 1 5
;; 1

(define (p-triangle row column)
  (cond
    ((or (zero? column) (zero? row)) 1)
    (else (+
           (p-triangle (sub1 row) column)
           (p-triangle row (sub1 column))))))

(define (pascals-triangle n-rows acc)
  (define (nth-row n x)
    ;; we are going up diagonally from left side
    (cond
      [(< n 0) '()]
      [else (cons (p-triangle n x) (nth-row (sub1 n) (add1 x)))]))
  (cond
    [(< n-rows 0) acc]
    [else (pascals-triangle (sub1 n-rows) (cons (nth-row n-rows 0) acc))]))

;; (pascals-triangle 5 '()) ->
;; '((1) (1 1) (1 2 1) (1 3 3 1) (1 4 6 4 1) (1 5 10 10 5 1))

;; now we want
;;   ..........1 
;;   ........1...1
;;   ......1...2...1
;;   ....1...3...3...1
;;   ..1...4...6...4...1
;;   1...5...10..10..5...1

(define (my-pretty-triangle n-rows)
  
  (define (pad s n); not the quickest way to do this but should suffice
    (if (= n 0)
        s
        (string-append " " (pad s (sub1 n)))))
  
  (define (for-row pt)
    (cond
      [(null? pt) '()]
      [else (cons (for-col (car pt) (length pt)) (for-row (cdr pt)))]))
  
  (define (for-col row height); first? element for row
    (if
     (null? row)
     '()
     (let* ([s (number->string (car row))]
            [len (string-length s)]
            [initial-pad (* (sub1 height) 2)]
            [normal-pad 4])
       (if (zero? height)
           (cons (pad s (- normal-pad len)) (for-col (cdr row) 0))
           (cons (pad s initial-pad) (for-col (cdr row) 0))))))
  
  (for-row (pascals-triangle n-rows '())))

(define (display-triangle pt)
  (cond
    [(null? pt) (void)]
    [else
     (displayln (apply string-append (car pt)))
     (display-triangle (cdr pt))]))

(display-triangle (my-pretty-triangle 12))
;; ->
;;                     1
;;                   1   1
;;                 1   2   1
;;               1   3   3   1
;;             1   4   6   4   1
;;           1   5  10  10   5   1
;;         1   6  15  20  15   6   1
;;       1   7  21  35  35  21   7   1
;;     1   8  28  56  70  56  28   8   1
;;   1   9  36  84 126 126  84  36   9   1
;; 1  10  45 120 210 252 210 120  45  10   1
