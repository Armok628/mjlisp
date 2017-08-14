# mjlisp
Amateur LISP interpreter in pure C with no research

I am aware of the "Build Your Own Lisp" book, but I haven't read any part of it. I guess you'll just have to take my word for it, but if there's any similarity at all, it's purely by coincidence. At the time of this writing, I am an incoming college freshman intent on majoring in Computer Engineering.

This Lisp is going to be slightly different in that the backquote/unquote feature in Common Lisp will exist, but in a different form. The apostrophe represents not fully evaluating (or "quoting") the following data, and applies only to cells and symbols. In short, unquoted symbols will be replaced with their symbol-value, and unquoted cells will represent function application.

For example, if `a` is bound to `1` and `b` is bound to `2`, `'(a 'b)` would become `(1 b)`. (In Common Lisp, the same would be written as `` `(,a b) ``)

Some quick examples:

`mapcar` from Common Lisp could be defined as
`(define 'mapcar (lambda (f x) (if x (cons (f (car x)) (mapcar f (cdr x))) nil)))`

`reduce` from Common Lisp can be defined as
`(define 'reduce (lambda (f x) (progn (define 'r (lambda (f x a) (if x (r f (cdr x) (f a (car x))) a))) (r f (cdr (cdr x)) (f (car x) (car (cdr x)))))))`

`zip` from Haskell can be defined as
`(define 'zip (lambda (x y) (if (and x y) (cons (cons (car x) (car y)) (zip (cdr x) (cdr y))) nil)))`
