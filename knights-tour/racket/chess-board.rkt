#lang racket

(require "knight-moves.rkt")

;; a chess board of vectors of integers
;; initialized to zero.
(define dimension 8)

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
           (not (zero? (board-ref next-x next-y)))) ; already used`
       #f] ; then we can't go there
      [else
       ;(df "going from ~a, ~a to ~a, ~a using move ~a" x y next-x next-y move)
       ;(display-board)
       ;(newline) (newline)
       (update-board next-x next-y (add1 current-move))
       (search next-x next-y)])))

(define (try-all moves x y)
  (cond
    [(null? moves) (void)]
    [else (try (car moves) x y) (try-all (cdr moves) x y)]))

(define (search x y)
  (set! count (add1 count))
  (let ([current-move (board-ref x y)])
    (when (zero? (modulo count 1000000)) (displayln count) (display-board))
    (cond
      ;[(> current-move 60) (display-board)]
      ;[(> count 20000000) (void)] ;; run out of time, reject moves
      [(= current-move (- (sqr dimension)) 1) (display-board)]
      [else (try-all knight-moves x y)])
    (update-board x y 0))) ; clean up mutable data

;(search 0 0)

board
