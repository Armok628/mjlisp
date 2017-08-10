# mjlisp
Amateur attempt at a LISP interpreter in pure C with no research

I am aware of the "Build Your Own Lisp" book, but I haven't read any part of it. I guess you'll just have to take my word for it, but if there's any similarity at all, it's purely by coincidence. At the time of this writing, I am an incoming college freshman intent on majoring in Computer Engineering.

This Lisp is going to be slightly different in that the backquote/unquote feature in Common Lisp will exist, but in a different form. The apostrophe represents not fully evaluating (or "quoting") the following data, and applies only to cells and symbols. In short, unquoted symbols will be replaced with their symbol-value, and unquoted cells will represent function application.

For example, if `a` is bound to `1` and `b` is bound to `2`, `'(a 'b)` would become `(1 b)`. (In Common Lisp, the same would be written as `` `(,a b) ``)
