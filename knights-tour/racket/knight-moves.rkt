#lang racket

;; defines knight moves 

(provide knight-moves)

;; Besides simply (define knight-moves
;;                  '((1 2) (1 -2) (-1 2) (-1 -2) (2 1) (2 -1) (-2 1) (-2 -1)))

;; In anticipation of general dimensions,
(define knight-moves
  
  (letrec ([dimensions 2]
           ;; assuming a 3d knight move is
           ;; reflect/rotations of 2d move in 3d, 4d etc.
           ;; e.g. (0, 1, 2), (0, 0, 1, 2)
           ;; before moving to 3d, counting 0 and -0  as different moves needs
           ;; to be addressed. The obvious solution is to filter repeats.

           ;; '(1 2), '(1 2 0), '(1 2 0 0) ... depending on dimensions
           [valid-components (append (list 1 2) (make-list (- dimensions 2) 0))]
           
           [component-reflections (list + -)]

           ;; redefined for completeness
           [permutations
            (lambda (li)
              (define (without el)
                (map (curry cons el)
                     (permutations (remove el li))))
              (if (null? li) '(())
                  (append-map without li)))]

           ;; n-length permutations with repetition
           [multi-perms
            (lambda (li len)
              (define (with el)
                (map (curry cons el)
                     (multi-perms li (sub1 len))))
              (if (= len 0) '(())
                  (append-map with li)))]
           
           [reflections (multi-perms component-reflections dimensions)]

           ;; generate positive moves e.g. '((1 2) (2 1))
           [moves (permutations valid-components)]

           ;; reflects a "move" according to a list of signs {+,-}
           ;; '(1 2) '(+ -) -> '(1 -2)
           [combine-one
            (lambda (refs comps)
             (map (lambda (ref comp) (ref comp)) refs comps))]

           ;; move-order -> move-order + reflections(move-order)
           [using
            (lambda (move-order)
              (map (curryr combine-one move-order) reflections))]

           [knight-moves (append-map using moves)])
    
    knight-moves))


;; On the other hand,
;; Knight moves could be defined as combinations of components (x, y)
(define (combinations li)
  (if (null? li) '(())
      (let* ([without (combinations (cdr li))]
             [with (map (curry cons (car li)) without)])
        (append with without))))

;; filtered by
(define (valid-knight-move move)
  (and (= (length move) 2)
       (not (= (abs (first move)) (abs (second move))))))

(define other-way-knight-moves
  (let ([forwards (filter valid-knight-move (combinations '(-1 -2  1  2)))])
    (append forwards (map reverse forwards))))

;; to yield
;; '((-1 -2) (-1 2) (-2 1) (1 2) (-2 -1) (2 -1) (1 -2) (2 1))
;; But I prefer the first approach.
