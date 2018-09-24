#lang racket

;; 

(provide knight-moves)

;;;;;;;;;;;; generate knight moves
;; ways of generating knight moves


;; knight moves can be defined as
(define (combine)
  (define dimensions 2)
  
  ;; assuming a 3d knight move is
  ;; reflections of (1, 2, 3)
  ;; alternatively reflect/rotations of 2d move in 3d, 4d etc. (0, 1, 2), (0, 0, 1, 2)
  
  (define valid-components (range 1 (add1 dimensions)))
  (define component-reflections (list + -))
  
  (define (permutations li)
    (define (without el)
      (map (curry cons el)
           (permutations (remove el li))))
    (if (null? li) '(())
        (append-map without li)))
  
  (define (multi-perms li len)
    (define (with el)
      (map (curry cons el)
           (multi-perms li (sub1 len))))
  (if (= len 0) '(())
      (append-map with li)))
  (define reflections (multi-perms component-reflections dimensions))
  (define moves (permutations valid-components))
  ;; '(1 2) '(+ -) -> '(1 -2)
  (define (combine-one refs comps)
    (map (lambda (ref comp) (ref comp)) refs comps))
  
  (define (using move-order)
    (map (curryr combine-one move-order) reflections))
  
  (append-map using moves))
  
(define knight-moves (combine))



;; On the other hand,

;; Knight moves could be defined as combinations of components (x, y)
(define (combinations li)
  (if (null? li) '(())
      (let* ([without (combinations (cdr li))]
             [with (map (curry cons (car li)) without)])
        (append with without))))

;; which can be filtered by
(define (valid-knight-move move)
  (and (= (length move) 2)
       (not (= (abs (first move)) (abs (second move))))))

;; '((-1 -2) (-1 2) (-2 1) (1 2) (-2 -1) (2 -1) (1 -2) (2 1))
(define knight-moves-bad
  (let ([forwards (filter valid-knight-move (combinations '(-1 -2  1  2)))])
    (append forwards (map reverse forwards))))

;; but I prefer the first approach
