
Caffeine, C Application Framework
===

Caffeine is a C language based framework which uses C99, POSIX
and SUSv3 standards, and system specific system calls -- Linux
and FreeBSD for now -- to support the development of daemons and
services. The idea is to have predefined algorithms to help you
in some tasks for building your own daemons, command line
applications and complex tasks such as integrating plugin
interfaces to your applications. The concrete goal of this project
is to implement most common algorithms to develop service oriented
applications.


Features
---

* Process pool support
* Thread pool support
* Static state machine support
* Dynamic state machine support
* Loadable state machine support
* Dynamic shared object support
* Deque support
* Linked list support
* Circular list support
* Hash table support
* Asynchronous I/O support
* File descriptor events support
* Buffer management support
* SysV IPC support




STEPS FOR BUILDING CAFFEINE
===========================

Build Tools:

* CMake
* PCRE

CMake is a Makefile generator tool writen in C++. To build
Caffeine manually you must run:

```shell

cmake .
make

```

In the bin directory resides a script called cmk.sh, it's script that
helps in the compilation of caffeine, setting verbose output and debug
binaries generation, written to help in debugging tasks.

To build Caffeine with cmk.sh you must run:

```shell

cmk.sh -mcb

```

To clean the proyect output, usually to get a patch:

```shell

cmk.sh -mc

```

To import the proyect into kdevelop:

```shell

cmk.sh -kcb

```

To clean the project output made by kdevelop:

```shell

cmk.sh -kc

```

Caffeine Coding Style
===


Indenting and Formatting
---

The style in Caffeine is a mixture between ISO C 99 calling conventions,
K&R blocks breakings and KNF -- Kernel Normal Form -- indenting.

* Functions in ISO C 99 (ISO/IEC 9899:1999) calling convention.
* Block must be broken using K&R indenting style.
* The code must be indented with concrete tabs of width 4.
* The fill column must set to the column 78.

Concrete tabs are used to speed up the compile time of caffeine, concrete
tabs means less tokens to parse.


Naming Conventions
---

* Lowercase functions.
* Lowercase identifiers.
* Lowercase constants in files and static variables.
* Uppercase enums.
* Uppercase macros.
* Uppercase define constants (in headers).

For core functions, you must use:
	<module_abbreviation>_<function_name>
I.E.:
	cbuf_create

For caffeine functions, you must use:
	<caf>_<module_abbreviation>_<function_name>

For parameters, and local variables, abbreviations are permitted.

For typedefs, you must use:
	<module_abbreviation>_<typedef>_t

For structures, you must use:
	<module_abbreviation>_<structure>_s

For single variables and parameters, hungarian notation -- type prefix
or suffix -- isn't permitted.


Emacs Style
---

To use emacs during caffeine hacking you must use the next sentence
in your .emacs file:

```lisp

(c-add-style "caffeine"
         '((c-basic-offset . 4)
           (c-comment-only-line-offset . 0)
           (c-offsets-alist . ((statement-block-intro . +)
                   (knr-argdecl-intro . +)
                   (substatement-open . 0)
                   (label . 0)
                   (statement-cont . +)
                   (inline-open . 0)
                   (inexpr-class . 0)
                   ))))
```

This will ensure that the code is indented under the caffeine rules.


Vim Style
---

For VIM is needed the cino variable. I'm working on that, but isn't ready
yet.


Kate Style
---

On Kate you must turn off the mixed indenting style and set the tab key
to insert concrete tabs. As part of KNF indenting style.


Caffeine Hacking Guide
===


Before you start
---

You must read the STYLE guide.


2. What can I do?
---

* Write unitary tests for every algorithm in Caffeine. If you don't
have enough time, you can submit the test and we will debug the
tests for you.

* Contribute with modules. Write a module to extend caffeine, something
  that can be usefull.

* Wait for the caffeine design specs.


3. Centralized Development
---

* Contributions are received through the development mailing list.
* Module contributions retain the original copyleft notice (copyright
  under LGPL), if you write a module, you own the copyleft notice.
* Patch and patchset contributions that affects up to 50.000001% of a
  module, means the inclusion of the proper copyleft notice in the
  module file.
* Patch and patchset contributions that affects less to 49.00001% of a
  module, means the inclusion of the author name in the AUTHORS file.


4. Avoid Compile Warnings
---

Caffeine is build under strict compilation flags. Common flags to build
caffeine are:

```shell
-Wall -Wextra -Wshadow -pedantic -std=c99
```

This means that the code must be 100% C99 (ISO/IEC 9899:1990) compilant.
Every compile warning related to this standard, must be removed. To build
Caffeine under debugging configuration, see the BUILDING text file.


Related Webpages
---

The caffeine main web page is located
here [coder.cl](http://coder.cl/products/caffeine/)


Authors
---

* Copyright (c) 2009
  [Daniel Molina Wegener](https://github.com/dmw)
  [coder.cl](http://coder.cl)
