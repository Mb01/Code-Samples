#lang racket

(require "knight-moves.rkt")

;; a chess board of vectors of integers
;; initialized to zero.
(define dimension 8)

;; display formatted text
(define df (lambda args
  (displayln (apply format args))))

(define (make-chess-board)
  (for/vector ([i dimension]) (make-vector dimension 0)))

(define board (make-chess-board))

(define (update-board x y val)
  (vector-set! (vector-ref board x) y val)) 

(define (board-ref x y)
  (vector-ref (vector-ref board x) y))

(define (display-board)
  (for ([i board]) (displayln i)))

(define count 0)

(define (try move x y)
  (let ([current-move (board-ref x y)]
        [next-x (+ x (first move))]
        [next-y (+ y (second move))])
    (cond
      [(or (>= (max next-x next-y) dimension) ; off the board
           (< (min next-x next-y) 0) ;; off the board
           (not (zero? (board-ref next-x next-y)))) ; already used
       #f] ; then we can't go there
      [else
       (update-board next-x next-y (add1 current-move))
       (search next-x next-y)])))

(define (try-all moves x y)
  (cond
    [(null? moves) (void)]
    [else (try (car moves) x y) (try-all (cdr moves) x y)]))

;; REMINDER: refactor/readability when rewriting
(define (try-corner-then-all moves x y)
  (define (available-corner? x y)
    (let ([dim (sub1 dimension)])
      (and (or (= x 0) (= x dim))   (or (= y 0) (= y dim)))))
  (cond
    [(null? moves) (try-all knight-moves x y)] ; then try other moves
    [(available-corner? (first (car moves)) (second (car moves)))
     (try (car moves) x y) ; try the corner
     (try-all knight-moves x y)] ; and then try all knight moves
    [else (try-corner-then-all (cdr moves) x y)]))

(define (search x y)
  (set! count (add1 count))
  (let ([current-move (board-ref x y)])
    (cond
      [(= current-move (sqr dimension)) (display-board)]
      [else (try-corner-then-all knight-moves x y)])
    (update-board x y 0))) ; clean up mutable data

;; set the first move to 1
(update-board 0 0 1)

;; search
(search 0 0)

;; display final board
board
