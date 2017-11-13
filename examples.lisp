(progn
  | Functions defined in terms of primitives only: |
  (define 'repl (lambda () (progn (display (eval (read 512))) (terpri) (terpri) (repl))))
  (define 'apply (lambda (f x) (eval (cons f x))))
  (define 'mapcar (lambda (f x) (if x (cons (f (car x)) (mapcar f (cdr x))) nil)))
  (define 'reduce (lambda (f x) ((lambda (f x a) (if x (# f (cdr x) (f a (car x))) a)) f (cdr (cdr x)) (f (car x) (car (cdr x))))))
  (define 'zip (lambda (x y) (if (and x y) (cons (cons (car x) (car y)) (zip (cdr x) (cdr y))) nil)))
  (define 'append (lambda (l a) (if l (cons (car l) (append (cdr l) a)) a)))
  (define 'reverse (lambda (l) ((lambda (l a) (if l (# (cdr l) (cons (car l) a)) a)) l nil)))
  (define 'reval (lambda (f) (if (atom f) (eval f) (eval (mapcar reval f)))))
  (define 'cxr (lambda (c l) (if c (eval '((if (eq (car c) 'a) car cdr) (cxr (cdr c) l))) l)))
  (define 'range (lambda (x s y) (if (or (and (> s 0) (> x y)) (and (< s 0) (< x y))) nil (cons x (range (+ x s) s y)))))
  (define 'nth (lambda (n l) (if (> n 0) (nth (- n 1) (cdr l)) (car l))))
  (define '! (lambda (n) (if (> n 1) (* n (! (- n 1))) 1)))

  | Functions defined in terms of non-primitives: |
  (define 'print (lambda (l) (mapcar display (reverse l))))
  (define 'println (lambda (l) (progn (print l) (terpri))))
  (define 'splice (lambda (l) (reduce append l)))
  (define 'space (lambda (l) (cdr (splice (mapcar (lambda (x) '(\  x)) l)))))
  (define 'iota (lambda (n) (range 1 1 n)))
  )
