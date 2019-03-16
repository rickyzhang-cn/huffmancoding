#lang racket
(define (make-leaf symbol weight)
  (list 'leaf symbol weight))
(define (leaf? object)
  (eq? (car object) 'leaf))
(define (symbol-leaf x) (cadr x))
(define (weight-leaf x) (caddr x))

(define (make-code-tree left right)
  (list left
        right
        (append (symbols left) (symbols right))
        (+ (weight left) (weight right))))
(define (left-branch tree) (car tree))
(define (right-branch tree) (cadr tree))
(define (symbols tree)
  (if (leaf? tree)
      (list (symbol-leaf tree))
      (caddr tree)))
(define (weight tree)
  (if (leaf? tree)
      (weight-leaf tree)
      (cadddr tree)))

; ex2.67
(define (decode bits tree)
  (define (decode-1 bits current-branch)
    (if (null? bits)
        '()
        (let ((next-branch (choose-branch (car bits) current-branch)))
          (if (leaf? next-branch)
              (cons (symbol-leaf next-branch) (decode-1 (cdr bits) tree))
          (decode-1 (cdr bits) next-branch)))))
  (decode-1 bits tree))

(define (choose-branch bit branch)
  (cond [(= bit 0) (left-branch branch)]
        [(= bit 1) (right-branch branch)]
        [else (error "bad bit")]))


(define sample-tree
  (make-code-tree (make-leaf 'A 4)
                  (make-code-tree
                   (make-leaf 'B 2)
                   (make-code-tree (make-leaf 'D 1)
                                   (make-leaf 'C 1)))))

(define sample-message '(0 1 1 0 0 1 0 1 0 1 1 1 0))

(decode sample-message sample-tree)

; ex2.68
(define (encode message tree)
  (if (null? message)
      '()
      (append (encode-symbol (car message) tree)
              (encode (cdr message) tree))))

(define (encode-symbol symbol tree)
  (cond [(null? tree) (error "no such char")]
        [(leaf? tree) '()]
        [else
         (let ((lb (left-branch tree))
               (rb (right-branch tree)))
               (let ((ls (symbols lb))
                     (rs (symbols rb)))
                 (cond [(exist? symbol ls) (cons 0 (encode-symbol symbol lb))]
                       [(exist? symbol rs) (cons 1 (encode-symbol symbol rb))]
                       [else (error "no such char")])))]))

(define (exist? ch symbols)
  (if (null? symbols)
      #f
      (or (eq? ch (car symbols)) (exist? ch (cdr symbols)))))

(encode '(A D A B B C A) sample-tree)

; ex2.69
(define (adjoin-set x set)
  (cond ((null? set) (list x))
        ((< (weight x) (weight (car set))) (cons x set))
        (else (cons (car set)
                    (adjoin-set x (cdr set))))))

(define (make-leaf-set pairs)
  (if (null? pairs)
      '()
      (let ((pair (car pairs)))
        (adjoin-set (make-leaf (car pair)
                               (cadr pair))
                    (make-leaf-set (cdr pairs))))))

(define (generate-huffman-tree pairs)
  (successive-merge (make-leaf-set pairs)))

(define (successive-merge s)
  (cond [(null? s) '()]
        [(null? (cdr s)) (car s)]
        [else (successive-merge (adjoin-set (make-code-tree (car s) (cadr s)) (cddr s)))]))

(define sp (list (list 'A 8) (list 'B 3) (list 'C 1) (list 'D 1) (list 'E 1) (list 'F 1) (list 'G 1) (list 'H 1)))

(make-leaf-set sp)
(generate-huffman-tree sp)
(encode '(B A C A D A E) (generate-huffman-tree sp))

; ex2.70
(define symbol-table (list (list 'A 2) (list 'NA 16) (list 'BOOM 1) (list 'SHA 3)
                            (list 'GET 2) (list 'YIP 9) (list 'JOB 2) (list 'WAH 1)))
(define ht (generate-huffman-tree symbol-table))
(define text '(GET A JOB
               SHA NA NA NA NA NA NA NA NA
               GET A JOB SHA NA NA NA NA NA NA NA NA
               WAH YIP YIP YIP YIP YIP YIP YIP YIP YIP
               SHA BOOM))
(encode text ht)
(decode (encode text ht) ht)
