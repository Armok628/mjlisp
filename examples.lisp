(progn
  | Functions defined in terms of primitives only: |
  (define 'repl (lambda () (progn (display (eval (read 512))) (terpri) (terpri) (repl))))
  (define 'apply (lambda (f x) (eval (cons f x))))
  (define 'mapcar (lambda (f x) (if x (cons (f (car x)) (mapcar f (cdr x))) nil)))
  (define 'reduce (lambda (f x) (progn (define 'r (lambda (f x a) (if x (r f (cdr x) (f a (car x))) a))) (r f (cdr (cdr x)) (f (car x) (car (cdr x)))))))
  (define 'zip (lambda (x y) (if (and x y) (cons (cons (car x) (car y)) (zip (cdr x) (cdr y))) nil)))
  (define 'append (lambda (l a) (if l (cons (car l) (append (cdr l) a)) a)))
  (define 'reverse (lambda (l) (progn (define 'r (lambda (l a) (if l (r (cdr l) (cons (car l) a)) a))) (r l nil))))
  (define 'reval (lambda (f) (if (atom f) (eval f) (eval (mapcar reval f)))))
  (define 'cxr (lambda (c l) (if c (eval '((if (eq (car c) 'a) car cdr) (cxr (cdr c) l))) l)))
  (define 'range (lambda (x s y) (if (or (and (> s 0) (> x y)) (and (< s 0) (< x y))) nil (cons x (range (+ x s) s y)))))
  (define 'nth (lambda (n l) (if (> n 0) (nth (- n 1) (cdr l)) (car l))))

  | Functions defined in terms of non-primitives: |
  (define 'print (lambda (l) (mapcar display (reverse l))))
  (define 'println (lambda (l) (progn (print l) (terpri))))
  (define 'splice (lambda (l) (reduce append l)))
  (define 'space (lambda (l) (cdr (splice (mapcar (lambda (x) '(\  x)) l)))))
  (define 'iota (n) (range 0 1 n))
  (repl)
  )
