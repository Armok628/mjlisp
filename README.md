# mjlisp
Amateur LISP interpreter in pure C with no research

I am aware of the "Build Your Own Lisp" book, but I haven't read any part of it. I guess you'll just have to take my word for it, but if there's any similarity at all, it's purely by coincidence. At the start of this project, I was an incoming college freshman intent on majoring in Computer Engineering. The interpreter was fully functional by the start of school, but I have since begun improving it from time to time.

This Lisp is slightly different in that the backquote/unquote feature in Common Lisp exists, but in a different form. The apostrophe represents not fully evaluating (or "quoting") the following data, and applies only to cells and symbols. In short, unquoted symbols will be always replaced with their symbol-value, and unquoted cells will always represent function application, even inside quoted lists.

For example, if `a` is bound to `1` and `b` is bound to `2`, `'(a 'b)` would become `(1 b)`. (In Common Lisp, the same would be written as `` `(,a b) ``).

Another difference is that undefined variables simply evaluate to nil instead of throwing an error. As a result, comments can be written pretty much anywhere inside ``progn`` forms without the need for escaping them.

Some definitions for more "complicated" functions (using only primitives) are included as examples.

A concession:

I started learning how to use GDB, scan-build and Valgrind when trying to improve memory management and programming etiquette in mogue2. 
This project is older than mogue2, so it leaks memory and it's probably poorly-written compared to mogue2 (which has no errors of any kind in scan-build or Valgrind).

It works well, though, so I'm still very proud of it. This is my favorite project I've ever done, by far. Implementing a dialect of my favorite language with my second favorite language is my biggest accomplishment for sure, even if it isn't perfect.

I'll work on fixing technical problems in the future after I cross a few more things off my to-do list. This isn't an industrial-grade interpreter, and it isn't supposed to be. All this is for fun.
