(progn
  (define 'repl (lambda () (progn (display (eval (read 512))) (terpri) (terpri) (repl))))
  (define 'apply (lambda (f x) (eval (cons f x))))
  (define 'mapcar (lambda (f x) (if x (cons (f (car x)) (mapcar f (cdr x))) nil)))
  (define 'reduce (lambda (f x) (progn (define 'r (lambda (f x a) (if x (r f (cdr x) (f a (car x))) a))) (r f (cdr (cdr x)) (f (car x) (car (cdr x)))))))
  (define 'zip (lambda (x y) (if (and x y) (cons (cons (car x) (car y)) (zip (cdr x) (cdr y))) nil)))
  (define 'append (lambda (l a) (if l (cons (car l) (append (cdr l) a)) a)))
  (define 'reverse (lambda (l) (progn (define 'r (lambda (l a) (if l (r (cdr l) (cons (car l) a)) a))) (r l nil))))

  (define 'print (lambda (l) (progn (mapcar display (reverse l)) (terpri))))
  (define 'space (lambda (l) (cdr (reduce append (mapcar (lambda (x) '(\ x)) l)))))
  (repl)
  )
