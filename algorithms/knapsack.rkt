#lang racket

;; problem constraints:
;; http://www.rosettacode.org/wiki/Knapsack_problem/0-1#Racket
;; (note: the solution given on the website (as of present) is my solution)

;; choose items from
(define items '((map 9 150) (compass 13 35) (water 153 200) (sandwich 50 160)
      (glucose 15 60) (tin 68 45)(banana 27 60) (apple 39 40)
      (cheese 23 30) (beer 52 10) (cream 11 70) (camera 32 30)
      (T-shirt 24 15) (trousers 48 10) (umbrella 73 40)
      (trousers 42 70) (overclothes 43 75) (notecase 22 80)
      (glasses 7 20) (towel 18 12) (socks 4 50) (book 30 10)))

;; such that weight of items does not exceed

(define max-weight 400)

;; with helpers

(define (item-value item)
  (caddr item))

(define (item-weight item)
  (cadr item))

(define (pack-weight pack)
  (apply + (map item-weight pack)))

(define (pack-value pack)
  (apply + (map item-value pack)))

;; returns "best" of two packs, todo: readability
(define (max-pack-value pack-with pack-without max-weight)
  (if (and
       (not (> (pack-weight pack-with) max-weight))
       (> (pack-value pack-with) (pack-value pack-without)))
      pack-with pack-without))

;; output the asnwer
(define (display-solution pack)
    (displayln (list 'weight: (pack-weight pack)
                     'value:  (pack-value pack)
                     'items:  (map car pack))))

;; first, the brute force solution
;; actually, it works well enough in this case

(define (show-brute)
  ;; call the following with an
  (define empty-accumulator '())
  ;; and the items defined above
  
  (define (knapsack-brute included items)
    (cond
      ((null? items) included)
      (else
       (max-pack-value
        (knapsack-brute (cons (car items) included) (cdr items))
        (knapsack-brute included (cdr items))
        max-weight
        ))))
  
  (display-solution (reverse (knapsack-brute empty-accumulator items))))

; uncomment to run, this took around five seconds on my computer
;(show-brute)

;;  memoizing solutions improves times
(define (show-memoized); -> side effect (output)
  
  (define (memoize func) ; -> memoized procedure
    (let ([results (make-hash)])
      (lambda args ; this is the rest-id pattern
        (when (not (hash-has-key? results args))
          (hash-set! results args (apply func args)))
        (hash-ref results args))))
  
  (define knapsack 
    (memoize
     (lambda (max-weight items)
       (cond
         ;; no items to choose from leaves pack empty
         ((null? items) '())
         (else
          ;; take the first item, leave the rest
          (let ([item (car items)] [items (cdr items)])
            ;; take the "best" of
            (max-pack-value
             ;; using the item and
             (cons item (knapsack (- max-weight (item-weight item)) items))
             ;; not using the item 
             (knapsack max-weight items)
             ;; (where max-weight is needed to detrmine the best)
             max-weight)))))))
  
  (display-solution (knapsack max-weight items)))

(show-memoized)
